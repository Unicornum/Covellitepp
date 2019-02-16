
#pragma once

using Color_t = ::std::vector<float>;

inline static Color_t ARGBtoFloat4(uint32_t _HexColor)
{
  return Color_t
  {
    ((_HexColor & 0x00FF0000) >> 16) / 255.0f,
    ((_HexColor & 0x0000FF00) >> 8) / 255.0f,
    ((_HexColor & 0x000000FF) >> 0) / 255.0f,
    ((_HexColor & 0xFF000000) >> 24) / 255.0f,
  };
};

namespace
{

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
// cppcheck-suppress syntaxError
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_RegisterIntoFactory)
{
  using namespace ::alicorn::modules::patterns;

  const ::covellite::api::renderer::Renderer::Data Data = { 0 }; // != Data_t
  auto pExample = factory::make_unique<ITested_t>(m_UsingApi, Data);
  EXPECT_NO_THROW(dynamic_cast<Tested_t &>(*pExample));
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_ClearFrame)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  Data_t Data;
  Data.BkColor.R = 1808261814.0f;
  Data.BkColor.G = 1808261815.0f;
  Data.BkColor.B = 1808261816.0f;
  Data.BkColor.A = 1808261817.0f;

  Tested_t Example{ Data };
  ITested_t & IExample = Example;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, ClearColor(Data.BkColor.R, Data.BkColor.G,
    Data.BkColor.B, Data.BkColor.A))
    .Times(1);

  EXPECT_CALL(GLProxy, Clear(GL_COLOR_BUFFER_BIT))
    .Times(1);

  IExample.ClearFrame();
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_ResizeWindow)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const int Width = 1710291059;
  const int Height = 1710291100;

  Tested_t Example{ Data_t{} };
  ITested_t & IExample = Example;

  using namespace ::testing;

  EXPECT_CALL(GLProxy, Viewport(0, 0, Width, Height - m_Top))
    .Times(1);

  IExample.ResizeWindow(Width, Height);
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_BlendState)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto Render = itCreator->second(Component_t::Make(
    {
      { uT("kind"), uT("Blend") }
    }));
  ASSERT_NE(nullptr, Render);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, Enable(GL_BLEND))
    .Times(1);

  EXPECT_CALL(GLProxy, BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))
    .Times(1);

  Render();
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_SamplerState)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto Render = itCreator->second(Component_t::Make(
    {
      { uT("kind"), uT("Sampler") }
    }));
  ASSERT_NE(nullptr, Render);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
    GL_LINEAR))
    .Times(1);

  EXPECT_CALL(GLProxy, TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
    GL_LINEAR))
    .Times(1);

  EXPECT_CALL(GLProxy, TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT))
    .Times(1);

  EXPECT_CALL(GLProxy, TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT))
    .Times(1);

  Render();
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Scissor_Enable)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreatorData = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itCreatorData);

  const auto pScissorData = Component_t::Make(
    {
      { uT("kind"), uT("Rect") },
    });

  auto ScissorDataRender = itCreatorData->second(pScissorData);
  EXPECT_EQ(nullptr, ScissorDataRender);

  auto itCreatorState = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreatorState);

  const auto pScissor = Component_t::Make(
    {
      { uT("kind"), uT("Scissor") },
      { uT("is_enabled"), true },
    });

  auto Render = itCreatorState->second(pScissor);
  ASSERT_NE(nullptr, Render);

  auto TestCallRender =
    [&](int _X, int _Y, int _Width, int _Height, int _WindowHeight)
  {
    const int Yo = _WindowHeight - (_Y + _Height);

    pScissorData->SetValue(uT("left"), _X);
    pScissorData->SetValue(uT("top"), _Y);
    pScissorData->SetValue(uT("right"), _X + _Width);
    pScissorData->SetValue(uT("bottom"), _Y + _Height);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, Enable(GL_SCISSOR_TEST))
      .Times(1);

    const int Viewport[4] = { 0, 0, 0, _WindowHeight };

    EXPECT_CALL(GLProxy, GetIntegerv(GL_VIEWPORT))
      .Times(1)
      .WillOnce(Return(Viewport));

    EXPECT_CALL(GLProxy, Scissor(_X, Yo, _Width, _Height))
      .Times(1);

    Render();
  };

  // Два вызова, чтобы убедиться, что изменение исходных данных приводит 
  // к изменению результата рендеринга.
  TestCallRender(1509, 1510, 1511, 1512, 1513);
  TestCallRender(1514, 1515, 1516, 1517, 1518);
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Scissor_Disable)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Scissor") },
      { uT("is_enabled"), false },
    });

  auto Render = itCreator->second(pComponent);
  ASSERT_NE(nullptr, Render);

  using namespace ::testing;

  EXPECT_CALL(GLProxy, Disable(GL_SCISSOR_TEST))
    .Times(1);

  Render();
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Material_DefaultValue)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const ::std::vector<float> Black = { 0.0f, 0.0f, 0.0f, 1.0f };
  const ::std::vector<float> Zero = { 0.0f };

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Material"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto pComponent = Component_t::Make({});

  auto Render = itCreator->second(pComponent);
  ASSERT_NE(nullptr, Render);

  using namespace ::testing;

  EXPECT_CALL(GLProxy, Materialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Black))
    .Times(1);

  EXPECT_CALL(GLProxy, Materialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Black))
    .Times(1);

  EXPECT_CALL(GLProxy, Materialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Black))
    .Times(1);

  EXPECT_CALL(GLProxy, Materialfv(GL_FRONT_AND_BACK, GL_EMISSION, Black))
    .Times(1);

  EXPECT_CALL(GLProxy, Materialfv(GL_FRONT_AND_BACK, GL_SHININESS, Zero))
    .Times(1);

  Render();
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Material)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const auto Ambient = 0xF2231236;
  const auto Diffuse = 0xF2231237;
  const auto Specular = 0xF2231238;
  const auto Emission = 0xF2231239;
  const ::std::vector<float> Shininess = { 1812231240.0f };

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Material"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto pComponent = Component_t::Make(
    {
      { uT("ambient"), Ambient },
      { uT("diffuse"), Diffuse },
      { uT("specular"), Specular },
      { uT("emission"), Emission },
      { uT("shininess"), Shininess[0] },
    });

  auto Render = itCreator->second(pComponent);
  ASSERT_NE(nullptr, Render);

  using namespace ::testing;

  EXPECT_CALL(GLProxy, Materialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ARGBtoFloat4(Ambient)))
    .Times(1);

  EXPECT_CALL(GLProxy, Materialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ARGBtoFloat4(Diffuse)))
    .Times(1);

  EXPECT_CALL(GLProxy, Materialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ARGBtoFloat4(Specular)))
    .Times(1);

  EXPECT_CALL(GLProxy, Materialfv(GL_FRONT_AND_BACK, GL_EMISSION, ARGBtoFloat4(Emission)))
    .Times(1);

  EXPECT_CALL(GLProxy, Materialfv(GL_FRONT_AND_BACK, GL_SHININESS, Shininess))
    .Times(1);

  Render();
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Texture_glTexImage2D_Fail)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const ::mock::GLuint TextureId = 1612182301;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, GenTextures(1))
    .Times(1)
    .WillOnce(Return(TextureId));

  EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
    .Times(1);

  EXPECT_CALL(GLProxy, TexImage2D(_, _, _, _, _, _, _, _, _))
    .Times(1);

  EXPECT_CALL(GLProxy, GetError())
    .Times(1)
    .WillOnce(Return(1808261916));

  EXPECT_THROW(itCreator->second(Component_t::Make({})), ::std::exception);
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Texture)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  // glTexImage2D копирует полученные данные, поэтому достаточно проверить
  // передачу указателя.
  const uint8_t * pSource = (uint8_t *)1811222026;
  const int Width = 1808261902;
  const int Height = 1808261903;
  const ::mock::GLuint TextureId = 1612182301;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, GenTextures(1))
    .Times(1)
    .WillOnce(Return(TextureId));

  EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
    .Times(1);

  EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
    Width, Height, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, pSource))
    .Times(1);

  EXPECT_CALL(GLProxy, GetError())
    .Times(1)
    .WillOnce(Return(GL_NO_ERROR));

  const auto pComponent = Component_t::Make(
    {
      { uT("data"), pSource },
      { uT("width"), Width },
      { uT("height"), Height },
    });

  auto Render = itCreator->second(pComponent);
  ASSERT_NE(nullptr, Render);

  EXPECT_CALL(GLProxy, Enable(GL_TEXTURE_2D))
    .Times(1);

  EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
    .Times(1);

  Render();

  EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
    .Times(1);
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Texture_FromDataComponent)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  // glTexImage2D копирует полученные данные, поэтому копировать их не нужно.
  const uint8_t * pSource = (uint8_t *)1812181806;
  const int Width = 1812181807;
  const int Height = 1812181808;
  const ::mock::GLuint TextureId = 1812181809;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  const auto pData = Component_t::Make(
    {
      { uT("kind"), uT("Texture") },
      { uT("data"), pSource },
      { uT("width"), Width },
      { uT("height"), Height },
    });

  auto DataRender = itDataCreator->second(pData);
  EXPECT_EQ(nullptr, DataRender);

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto pTexture = Component_t::Make({});

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, GenTextures(1))
    .Times(1)
    .WillOnce(Return(TextureId));

  EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
    .Times(1);

  EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
    Width, Height, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, pSource))
    .Times(1);

  EXPECT_CALL(GLProxy, GetError())
    .Times(1)
    .WillOnce(Return(GL_NO_ERROR));

  auto Render = itCreator->second(pTexture);
  ASSERT_NE(nullptr, Render);

  EXPECT_CALL(GLProxy, Enable(GL_TEXTURE_2D))
    .Times(1);

  EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
    .Times(1);

  Render();

  EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
    .Times(1);
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Buffer_VertexGui)
{
  using Vertex_t = ::covellite::api::Vertex::Gui;

  const ::std::vector<Vertex_t> Source =
  {
    {
      1808261932.0f, 1808261933.0f,
      1808261934,
      1808261935.0f, 1808261936.0f
    },
    {
      1808261932.0f, 1808261933.0f,
      1808261934,
      1808261935.0f, 1808261936.0f
    },
    { 0.0f, 0.0f, 0, 0.0f, 0.0f }, // Маркер конца данных
  };

  const ::mock::GLProxy::Floats_t ExpectedVertexData =
  {
    Source[0].x, Source[0].y,
    Source[1].x, Source[1].y,
  };

  const ::std::vector<uint32_t> ExpectedColorData =
  {
    Source[0].ABGRColor,
    Source[1].ABGRColor,
  };

  const ::mock::GLProxy::Floats_t ExpectedTexCoordData =
  {
    Source[0].u, Source[0].v,
    Source[1].u, Source[1].v,
  };

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto TestCallRender = [&](Render_t & _Render)
  {
    using GLProxy_t = ::mock::GLProxy;
    GLProxy_t GLProxy;
    GLProxy_t::GetInstance() = &GLProxy;

    ASSERT_NE(nullptr, _Render);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, VertexPointer(2, GL_FLOAT,
      sizeof(Vertex_t), ExpectedVertexData))
      .Times(1);

    EXPECT_CALL(GLProxy, EnableClientState(GL_VERTEX_ARRAY))
      .Times(1);

    EXPECT_CALL(GLProxy, ColorPointer(4, GL_UNSIGNED_BYTE,
      sizeof(Vertex_t), ExpectedColorData))
      .Times(1);

    EXPECT_CALL(GLProxy, EnableClientState(GL_COLOR_ARRAY))
      .Times(1);

    EXPECT_CALL(GLProxy, TexCoordPointer(2, GL_FLOAT,
      sizeof(Vertex_t), ExpectedTexCoordData))
      .Times(1);

    EXPECT_CALL(GLProxy, EnableClientState(GL_TEXTURE_COORD_ARRAY))
      .Times(1);

    _Render();
  };

  auto itCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  Render_t Render;

  // ***************** Передача данных в объекте компонента ***************** //

  {
    // Передача данных через локальный объект, чтобы убедиться, что рендер будет
    // хранить копию данных.
    const auto Data = Source;

    const auto pComponent = Component_t::Make(
      {
        { uT("kind"), uT("Vertex.Gui") },
        { uT("data"), Data.data() },
        { uT("count"), Data.size() },
      });

    Render = itCreator->second(pComponent);
  }

  TestCallRender(Render);

  // ************** Передача данных в объекте компонента Data *************** //

  {
    // Передача данных через локальный объект, чтобы убедиться, что рендер будет
    // хранить копию данных.
    const auto Data = Source;

    auto itDataCreator = IExample.GetCreators().find(uT("Data"));
    ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("data"), Data.data() },
        { uT("count"), Data.size() },
      });

    auto DataRender = itDataCreator->second(pData);
    EXPECT_EQ(nullptr, DataRender);

    const auto pComponent = Component_t::Make(
      {
        { uT("kind"), uT("Vertex.Gui") },
      });

    Render = itCreator->second(pComponent);
  }

  TestCallRender(Render);
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Buffer_VertexTextured)
{
  using Vertex_t = ::covellite::api::Vertex::Textured;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const ::std::vector<Vertex_t> Source =
  {
    { 1808261932.0f, 1808261933.0f, 1812161256.0f,
      1808261934.0f, 1812161257.0f,
      1808261935.0f, 1808261936.0f, 1812161258.0f
    },
    {
      1808261932.0f, 1808261933.0f, 1812161259.0f,
      1808261934.0f, 1812161300.0f,
      1808261935.0f, 1808261936.0f, 1812161301.0f
    },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // Маркер конца данных
  };

  const ::mock::GLProxy::Floats_t ExpectedVertexData =
  {
    Source[0].x, Source[0].y, Source[0].z,
    Source[1].x, Source[1].y, Source[1].z,
  };

  const ::mock::GLProxy::Floats_t ExpectedNormalData =
  {
    Source[0].nx, Source[0].ny, Source[0].nz,
    Source[1].nx, Source[1].ny, Source[1].nz,
  };

  const ::mock::GLProxy::Floats_t ExpectedTexCoordData =
  {
    Source[0].tu, Source[0].tv,
    Source[1].tu, Source[1].tv,
  };

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto TestCallRender = [&](Render_t & _Render)
  {
    ASSERT_NE(nullptr, _Render);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, VertexPointer(3, GL_FLOAT,
      sizeof(Vertex_t), ExpectedVertexData))
      .Times(1);

    EXPECT_CALL(GLProxy, EnableClientState(GL_VERTEX_ARRAY))
      .Times(1);

    EXPECT_CALL(GLProxy, NormalPointer(GL_FLOAT,
      sizeof(Vertex_t), ExpectedNormalData))
      .Times(1);

    EXPECT_CALL(GLProxy, EnableClientState(GL_NORMAL_ARRAY))
      .Times(1);

    EXPECT_CALL(GLProxy, TexCoordPointer(2, GL_FLOAT,
      sizeof(Vertex_t), ExpectedTexCoordData))
      .Times(1);

    EXPECT_CALL(GLProxy, EnableClientState(GL_TEXTURE_COORD_ARRAY))
      .Times(1);

    _Render();
  };

  auto itCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  Render_t Render;

  // ***************** Передача данных в объекте компонента ***************** //

  {
    // Передача данных через локальный объект, чтобы убедиться, что рендер будет
    // хранить копию данных.
    const auto Data = Source;

    const auto pComponent = Component_t::Make(
      {
        { uT("kind"), uT("Vertex.Textured") },
        { uT("data"), Data.data() },
        { uT("count"), Data.size() },
      });

    Render = itCreator->second(pComponent);
  }

  TestCallRender(Render);

  // ************** Передача данных в объекте компонента Data *************** //

  {
    // Передача данных через локальный объект, чтобы убедиться, что рендер будет
    // хранить копию данных.
    const auto Data = Source;

    auto itDataCreator = IExample.GetCreators().find(uT("Data"));
    ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("data"), Data.data() },
        { uT("count"), Data.size() },
      });

    auto DataRender = itDataCreator->second(pData);
    EXPECT_EQ(nullptr, DataRender);

    const auto pComponent = Component_t::Make(
      {
        { uT("kind"), uT("Vertex.Textured") },
      });

    Render = itCreator->second(pComponent);
  }

  TestCallRender(Render);
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Present_Camera)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  using namespace ::testing;

  auto TestCallRender = [&](Render_t & _Render)
  {
    ASSERT_NE(nullptr, _Render);

    InSequence Dummy;

    EXPECT_CALL(GLProxy, Disable(GL_BLEND))
      .Times(1);

    EXPECT_CALL(GLProxy, Disable(GL_DITHER))
      .Times(1);

    EXPECT_CALL(GLProxy, ShadeModel(GL_SMOOTH))
      .Times(1);

    EXPECT_CALL(GLProxy, Hint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST))
      .Times(1);

    EXPECT_CALL(GLProxy, Enable(GL_CULL_FACE))
      .Times(1);

    EXPECT_CALL(GLProxy, CullFace(GL_BACK))
      .Times(1);

    EXPECT_CALL(GLProxy, FrontFace(GL_CCW))
      .Times(1);

    EXPECT_CALL(GLProxy, Disable(_))
      .Times(AtLeast(0));

    EXPECT_CALL(GLProxy, MatrixMode(GL_PROJECTION))
      .Times(1);

    EXPECT_CALL(GLProxy, LoadIdentity())
      .Times(1);

    EXPECT_CALL(GLProxy, MatrixMode(GL_MODELVIEW))
      .Times(1);

    EXPECT_CALL(GLProxy, LoadIdentity())
      .Times(1);

    const float Viewport[] = 
    { 
      1812271148.0f, 1812271149.0f, 1812181722.0f, 1812181723.0f 
    };

    EXPECT_CALL(GLProxy, GetFloatv(GL_VIEWPORT))
      .Times(1)
      .WillOnce(Return(Viewport));

    EXPECT_CALL(GLProxy, Ortho(Viewport[0], Viewport[0] + Viewport[2],
      Viewport[1] + Viewport[3], Viewport[1], -1, 1))
      .Times(1);

    _Render();
  };

  {
    auto itCreator = IExample.GetCreators().find(uT("Present"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Camera") },
      }));

    EXPECT_CALL(GLProxy, Disable(GL_DEPTH_TEST))
      .Times(1);

    TestCallRender(Render);
  }

  {
    auto itCreator = IExample.GetCreators().find(uT("Present"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Camera") },
        { uT("dept"), uT("Enabled") },
      }));

    EXPECT_CALL(GLProxy, Enable(GL_DEPTH_TEST))
      .Times(1);

    EXPECT_CALL(GLProxy, Clear(GL_DEPTH_BUFFER_BIT))
      .Times(1);

    TestCallRender(Render);
  }
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Present_FocalCamera)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const float Width = 1024.0f;
  const float Height = 768.0f;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  using namespace ::testing;

  auto TestCallRender = [&](Render_t & _Render, bool _IsDeptEnabled,
    float _AngleY, float _zFar,
    float _X, float _Y, float _Z,
    float _A, float _B, float _C,
    float _Distance)
  {
    const float AngleYRadians = _AngleY *
      static_cast<float>(::alicorn::extension::cpp::math::GreedToRadian);

    auto GetMatrixLineValues = [](const ::DirectX::XMMATRIX & _Matrix)
    {
      ::DirectX::XMFLOAT4X4 Result;
      ::DirectX::XMStoreFloat4x4(&Result, _Matrix);
      return Result;
    };

    const auto ProjectionMatrix = GetMatrixLineValues(
      ::DirectX::XMMatrixTranspose(
        ::DirectX::XMMatrixPerspectiveFovLH(
          AngleYRadians, Width / Height, 0.01f, _zFar)));

    const auto Look = ::DirectX::XMVectorSet(_X, _Y, _Z, 1.0f);

    auto Transform =
      ::DirectX::XMMatrixRotationX(_A) *
      ::DirectX::XMMatrixRotationY(_B) *
      ::DirectX::XMMatrixRotationZ(_C) *
      ::DirectX::XMMatrixTranslation(_X, _Y, _Z);
    auto Eye = ::DirectX::XMVector3TransformCoord(
      ::DirectX::XMVectorSet(_Distance + 0.1f, 0.0f, 0.0f, 1.0f),
      Transform);

    const auto ViewMatrix = GetMatrixLineValues(
      ::DirectX::XMMatrixTranspose(
        ::DirectX::XMMatrixLookAtLH(Eye, Look,
          ::DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f))));

    ASSERT_NE(nullptr, _Render);

    InSequence Dummy;

    if (_IsDeptEnabled)
    {
      EXPECT_CALL(GLProxy, Enable(GL_DEPTH_TEST))
        .Times(1);

      EXPECT_CALL(GLProxy, Clear(GL_DEPTH_BUFFER_BIT))
        .Times(1);
    }
    else
    {
      EXPECT_CALL(GLProxy, Disable(GL_DEPTH_TEST))
        .Times(1);
    }

    EXPECT_CALL(GLProxy, Disable(GL_BLEND))
      .Times(1);

    EXPECT_CALL(GLProxy, Disable(GL_DITHER))
      .Times(1);

    EXPECT_CALL(GLProxy, ShadeModel(GL_SMOOTH))
      .Times(1);

    EXPECT_CALL(GLProxy, Hint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST))
      .Times(1);

    EXPECT_CALL(GLProxy, Enable(GL_CULL_FACE))
      .Times(1);

    EXPECT_CALL(GLProxy, CullFace(GL_BACK))
      .Times(1);

    EXPECT_CALL(GLProxy, FrontFace(GL_CCW))
      .Times(1);

    EXPECT_CALL(GLProxy, Disable(_))
      .Times(AtLeast(0));

    const float Viewport[] = { 0, 0, Width, Height };

    EXPECT_CALL(GLProxy, GetFloatv(GL_VIEWPORT))
      .Times(1)
      .WillOnce(Return(Viewport));

    EXPECT_CALL(GLProxy, MatrixMode(GL_PROJECTION))
      .Times(1);

    EXPECT_CALL(GLProxy, LoadMatrixf(ProjectionMatrix))
      .Times(1);

    EXPECT_CALL(GLProxy, MatrixMode(GL_MODELVIEW))
      .Times(1);

    EXPECT_CALL(GLProxy, LoadMatrixf(ViewMatrix))
      .Times(1);

    _Render();
  };

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  class CameraInfo
  {
  public:
    float AngleY;
    float X, Y, Z;
    float A, B, C;
    float Distance;
  };

  const float zFar = 200.0f; // const

  const ::std::vector<CameraInfo> CameraInfos =
  {
    { 90.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // Default values
    { 91.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f },
    { 92.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f },
  };

  auto TestCamera = [&](bool _IsDeptEnabled,
    const Component_t::ComponentPtr_t & _pCamera)
  {
    {
      // Default values + not using Position & Rotation

      auto Render = itCreator->second(_pCamera);

      auto Info = CameraInfos[0];

      TestCallRender(Render, _IsDeptEnabled,
        Info.AngleY, zFar,
        Info.X, Info.Y, Info.Z,
        Info.A, Info.B, Info.C,
        Info.Distance);
    }

    const auto pPosition = Component_t::Make(
      {
        { uT("kind"), uT("Position") },
      });

    const auto pRotation = Component_t::Make(
      {
        { uT("kind"), uT("Rotation") },
      });

    {
      // Default values from Position & Rotation

      auto Position = itDataCreator->second(pPosition);
      EXPECT_EQ(nullptr, Position);

      auto Rotation = itDataCreator->second(pRotation);
      EXPECT_EQ(nullptr, Rotation);

      auto Render = itCreator->second(_pCamera);

      auto Info = CameraInfos[0];

      TestCallRender(Render, _IsDeptEnabled,
        Info.AngleY, zFar,
        Info.X, Info.Y, Info.Z,
        Info.A, Info.B, Info.C,
        Info.Distance);
    }

    {
      // Change values from Position & Rotation

      auto Position = itDataCreator->second(pPosition);
      EXPECT_EQ(nullptr, Position);

      auto Rotation = itDataCreator->second(pRotation);
      EXPECT_EQ(nullptr, Rotation);

      auto Render = itCreator->second(_pCamera);

      for (size_t i = 1; i < CameraInfos.size(); i++)
      {
        auto Info = CameraInfos[i];

        pPosition->SetValue(uT("x"), Info.X);
        pPosition->SetValue(uT("y"), Info.Y);
        pPosition->SetValue(uT("z"), Info.Z);

        pRotation->SetValue(uT("x"), Info.A);
        pRotation->SetValue(uT("y"), Info.B);
        pRotation->SetValue(uT("z"), Info.C);

        _pCamera->SetValue(uT("angle"), Info.AngleY);
        _pCamera->SetValue(uT("distance"), Info.Distance);

        TestCallRender(Render, _IsDeptEnabled,
          Info.AngleY, zFar,
          Info.X, Info.Y, Info.Z,
          Info.A, Info.B, Info.C,
          Info.Distance);
      }
    }
  };

  TestCamera(false, Component_t::Make(
    {
      { uT("kind"), uT("Camera") },
      { uT("focal"), uT("Enabled") },
    }));

  TestCamera(true, Component_t::Make(
    {
      { uT("kind"), uT("Camera") },
      { uT("dept"), uT("Enabled") },
      { uT("focal"), uT("Enabled") },
    }));
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Present_Geometry)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  const ::std::vector<int> Indices =
  {
    1808261927,
    1808261928,
    1808261929,
    1808261930,
    1808261931
  };

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto pPosition = Component_t::Make({ { uT("kind"), uT("Position") } });
  auto pRotation = Component_t::Make({ { uT("kind"), uT("Rotation") } });
  auto pScale = Component_t::Make({ { uT("kind"), uT("Scale") } });

  auto TestCallRender = [&](Render_t & _IndexBufferRender, Render_t & _Render,
    float _X, float _Y, float _Z,
    float _A, float _B, float _C,
    float _Sx, float _Sy, float _Sz)
  {
    ASSERT_NE(nullptr, _IndexBufferRender);
    ASSERT_NE(nullptr, _Render);

    pPosition->SetValue(uT("x"), _X);
    pPosition->SetValue(uT("y"), _Y);
    pPosition->SetValue(uT("z"), _Z);

    pRotation->SetValue(uT("x"), _A);
    pRotation->SetValue(uT("y"), _B);
    pRotation->SetValue(uT("z"), _C);

    pScale->SetValue(uT("x"), _Sx);
    pScale->SetValue(uT("y"), _Sy);
    pScale->SetValue(uT("z"), _Sz);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, PushMatrix())
      .Times(1);

    EXPECT_CALL(GLProxy, Scalef(_Sx, _Sy, _Sz))
      .Times(1);

    using namespace ::alicorn::extension::cpp::math;

    EXPECT_CALL(GLProxy, Rotatef(_A * (float)RadianToGreed, 1.0f, 0.0f, 0.0f))
      .Times(1);

    EXPECT_CALL(GLProxy, Rotatef(_B * (float)RadianToGreed, 0.0f, 1.0f, 0.0f))
      .Times(1);

    EXPECT_CALL(GLProxy, Rotatef(_C * (float)RadianToGreed, 0.0f, 0.0f, 1.0f))
      .Times(1);

    EXPECT_CALL(GLProxy, Translatef(_X, _Y, _Z))
      .Times(1);

    EXPECT_CALL(GLProxy, DrawElements(GL_TRIANGLES, (int)Indices.size(),
      GL_UNSIGNED_INT, Indices))
      .Times(1);

    EXPECT_CALL(GLProxy, DisableClientState(GL_VERTEX_ARRAY))
      .Times(1);

    EXPECT_CALL(GLProxy, DisableClientState(GL_COLOR_ARRAY))
      .Times(1);

    EXPECT_CALL(GLProxy, DisableClientState(GL_NORMAL_ARRAY))
      .Times(1);

    EXPECT_CALL(GLProxy, DisableClientState(GL_TEXTURE_COORD_ARRAY))
      .Times(1);

    EXPECT_CALL(GLProxy, Disable(GL_TEXTURE_2D))
      .Times(1);

    EXPECT_CALL(GLProxy, PopMatrix())
      .Times(1);

    _IndexBufferRender();

    _Render();
  };

  auto itBufferCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itBufferCreator);

  auto itPresentCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itPresentCreator);

  {
    // Индексный буфер в компоненте буфера

    Render_t IndexBufferRender;

    {
      // Передача локальной копии, чтобы убедиться, что рендер копирует
      // переданные ему данные.
      const auto IndicesCopy = Indices;

      IndexBufferRender = itBufferCreator->second(Component_t::Make(
        {
          { uT("kind"), uT("Index") },
          { uT("data"), IndicesCopy.data() },
          { uT("count"), IndicesCopy.size() },
        }));
    }

    auto PositionRender = itDataCreator->second(pPosition);
    EXPECT_EQ(nullptr, PositionRender);

    auto RotationRender = itDataCreator->second(pRotation);
    EXPECT_EQ(nullptr, RotationRender);

    auto ScaleRender = itDataCreator->second(pScale);
    EXPECT_EQ(nullptr, ScaleRender);

    auto Render = itPresentCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Geometry") },
      }));

    // Два вызова, чтобы убедиться, что изменение исходных данных приводит 
    // к изменению результата рендеринга.

    TestCallRender(IndexBufferRender, Render,
      1811221956.0f, 1811221957.0f, 1811221958.0f,
      1812171204.0f, 1812171205.0f, 1812171206.0f,
      1812181152.0f, 1812181153.0f, 1812181154.0f);

    TestCallRender(IndexBufferRender, Render,
      1811221959.0f, 1811221960.0f, 1811221961.0f,
      1812181145.0f, 1812181146.0f, 1812181147.0f,
      1812181155.0f, 1812181157.0f, 1812181158.0f);
  }

  {
    // Индексный буфер через компонент Data

    Render_t IndexBufferRender;

    {
      // Передача локальной копии, чтобы убедиться, что рендер копирует
      // переданные ему данные.
      const auto IndicesCopy = Indices;

      auto IndexBufferDataRender = itDataCreator->second(Component_t::Make(
        {
          { uT("kind"), uT("Buffer") },
          { uT("data"), IndicesCopy.data() },
          { uT("count"), IndicesCopy.size() },
        }));
      EXPECT_EQ(nullptr, IndexBufferDataRender);

      IndexBufferRender = itBufferCreator->second(Component_t::Make(
        {
          { uT("kind"), uT("Index") },
        }));
    }

    auto PositionRender = itDataCreator->second(pPosition);
    EXPECT_EQ(nullptr, PositionRender);

    auto RotationRender = itDataCreator->second(pRotation);
    EXPECT_EQ(nullptr, RotationRender);

    auto ScaleRender = itDataCreator->second(pScale);
    EXPECT_EQ(nullptr, ScaleRender);

    auto Render = itPresentCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Geometry") },
      }));

    // Два вызова, чтобы убедиться, что изменение исходных данных приводит 
    // к изменению результата рендеринга.

    TestCallRender(IndexBufferRender, Render,
      1811221956.0f, 1811221957.0f, 1811221958.0f,
      1812171204.0f, 1812171205.0f, 1812171206.0f,
      1812181152.0f, 1812181153.0f, 1812181154.0f);

    TestCallRender(IndexBufferRender, Render,
      1811221959.0f, 1811221960.0f, 1811221961.0f,
      1812181145.0f, 1812181146.0f, 1812181147.0f,
      1812181155.0f, 1812181157.0f, 1812181158.0f);
  }
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Light_ComplexUsing)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itLightCreator = IExample.GetCreators().find(uT("Light"));
  ASSERT_NE(IExample.GetCreators().end(), itLightCreator);

  auto itCameraCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

  auto pCamera1 = Component_t::Make(
    {
      { uT("id"), uT("Camera.1") },
      { uT("kind"), uT("Camera") },
    });

  auto pCamera2 = Component_t::Make(
    {
      { uT("id"), uT("Camera.2") },
      { uT("kind"), uT("Camera") },
    });

  auto Camera1Render = itCameraCreator->second(pCamera1);
  ASSERT_NE(nullptr, Camera1Render);

  auto Camera2Render = itCameraCreator->second(pCamera2);
  ASSERT_NE(nullptr, Camera2Render);

  using namespace ::testing;

  auto TestCallRenderCameraWithoutLights = [&](Render_t & _Render)
  {
    EXPECT_CALL(GLProxy, Disable(_))
      .Times(AtLeast(1));

    EXPECT_CALL(GLProxy, Enable(_))
      .Times(AtLeast(1));

    InSequence Dummy;

    EXPECT_CALL(GLProxy, Disable(GL_LIGHTING))
      .Times(1);

    EXPECT_CALL(GLProxy, Enable(_))
      .Times(0);

    _Render();
  };

  class Light
  {
  public:
    class Data
    {
    public:
      Component_t::ComponentPtr_t pData;
      ::std::function<void(int)> Test;
    };

  public:
    Component_t::ComponentPtr_t pLight;
    ::std::vector<Data> DataInfo;
    uint32_t Ambient;
    uint32_t Diffuse;
    uint32_t Specular;
  };

  auto TestUsingLights = [&](bool _IsDefault,
    const ::std::vector<Light> & _Lights)
  {
    TestCallRenderCameraWithoutLights(Camera1Render);
    TestCallRenderCameraWithoutLights(Camera2Render);

    for (const auto & Light : _Lights)
    {
      for (const auto & Data : Light.DataInfo)
      {
        auto DataRender = itDataCreator->second(Data.pData);
        EXPECT_EQ(nullptr, DataRender);
      }

      auto Render = itLightCreator->second(Light.pLight);
      ASSERT_NE(nullptr, Render);

      if (!_IsDefault)
      {
        Light.pLight->SetValue(uT("ambient"), Light.Ambient);
        Light.pLight->SetValue(uT("diffuse"), Light.Diffuse);
        Light.pLight->SetValue(uT("specular"), Light.Specular);
      }

      Render();
    }

    TestCallRenderCameraWithoutLights(Camera1Render);

    EXPECT_CALL(GLProxy, Disable(_))
      .Times(AtLeast(1));

    EXPECT_CALL(GLProxy, Enable(_))
      .Times(AtLeast(1));

    InSequence Dummy;

    EXPECT_CALL(GLProxy, Disable(GL_LIGHTING))
      .Times(0);

    EXPECT_CALL(GLProxy, Enable(GL_LIGHTING))
      .Times(1);

    for (size_t i = 0; i < _Lights.size(); i++)
    {
      const auto Index = static_cast<int>(GL_LIGHT0 + i);

      EXPECT_CALL(GLProxy, Enable(Index))
        .Times(1);

      auto ARGBtoFloat4 = [](uint32_t _HexColor)
      {
        return ::std::vector<float>
        {
          ((_HexColor & 0x00FF0000) >> 16) / 255.0f,
            ((_HexColor & 0x0000FF00) >> 8) / 255.0f,
            ((_HexColor & 0x000000FF) >> 0) / 255.0f,
            ((_HexColor & 0xFF000000) >> 24) / 255.0f,
        };
      };

      EXPECT_CALL(GLProxy,
        Lightfv(Index, GL_AMBIENT, ARGBtoFloat4(_Lights[i].Ambient)))
        .Times(1);

      EXPECT_CALL(GLProxy,
        Lightfv(Index, GL_DIFFUSE, ARGBtoFloat4(_Lights[i].Diffuse)))
        .Times(1);

      EXPECT_CALL(GLProxy,
        Lightfv(Index, GL_SPECULAR, ARGBtoFloat4(_Lights[i].Specular)))
        .Times(1);

      for (const auto & Data : _Lights[i].DataInfo)
      {
        Data.Test(Index);
      }
    }

    EXPECT_CALL(GLProxy, Enable(_))
      .Times(0);

    EXPECT_CALL(GLProxy, Lightfv(_, _, _))
      .Times(0);

    Camera2Render();

    TestCallRenderCameraWithoutLights(Camera1Render);
    TestCallRenderCameraWithoutLights(Camera2Render);
  };

  const Light::Data DefaultData =
  {
    Component_t::Make({}),
    [&](int _Index)
    {
      EXPECT_CALL(GLProxy, Lightfv(_Index, GL_POSITION,
        ::std::vector<float>{ 0.0f, 0.0f, 1.0, 0.0f }))
        .Times(1);

      EXPECT_CALL(GLProxy, Lightf(_Index, GL_CONSTANT_ATTENUATION, 1.0f))
        .Times(1);

      EXPECT_CALL(GLProxy, Lightf(_Index, GL_LINEAR_ATTENUATION, 0.0f))
        .Times(1);

      EXPECT_CALL(GLProxy, Lightf(_Index, GL_QUADRATIC_ATTENUATION, 0.0f))
        .Times(1);
    }
  };

  const ::std::vector<Light> Default =
  {
    {
      Component_t::Make({ { uT("kind"), uT("Ambient") } }),
      { DefaultData },
      0xFF000000,
      0xFF000000,
      0xFF000000,
    },
    {
      Component_t::Make({ { uT("kind"), uT("Direction") } }),
      { DefaultData },
      0xFF000000,
      0xFF000000,
      0xFF000000,
    },
    {
      Component_t::Make({ { uT("kind"), uT("Point") } }),
      { DefaultData },
      0xFF000000,
      0xFF000000,
      0xFF000000,
    },
  };

  auto GetDirectionData = [&](void)
  {
    static const float X = 1812251801.0f;
    static const float Y = 1812251802.0f;
    static const float Z = 1812251803.0f;

    const Light::Data Direction =
    {
      Component_t::Make(
        {
          { uT("kind"), uT("Direction") },
          { uT("x"), X },
          { uT("y"), Y },
          { uT("z"), Z },
        }),
      [&](int _Index)
        {
          EXPECT_CALL(GLProxy, Lightfv(_Index, GL_POSITION,
            ::std::vector<float>{ X, Y, Z, 0.0f }))
            .Times(1);

          EXPECT_CALL(GLProxy, Lightf(_Index, GL_CONSTANT_ATTENUATION, 1.0f))
            .Times(1);

          EXPECT_CALL(GLProxy, Lightf(_Index, GL_LINEAR_ATTENUATION, 0.0f))
            .Times(1);

          EXPECT_CALL(GLProxy, Lightf(_Index, GL_QUADRATIC_ATTENUATION, 0.0f))
            .Times(1);
        }
    };

    return ::std::vector<Light::Data>{ Direction };
  };

  auto GetPointData = [&](void)
  {
    static const float X = 1812251923.0f;
    static const float Y = 1812251924.0f;
    static const float Z = 1812251925.0f;

    const Light::Data Position =
    {
      Component_t::Make(
        {
          { uT("kind"), uT("Position") },
          { uT("x"), X },
          { uT("y"), Y },
          { uT("z"), Z },
        }),
      [&](int _Index)
        {
          EXPECT_CALL(GLProxy, Lightfv(_Index, GL_POSITION,
            ::std::vector<float>{ X, Y, Z, 1.0f }))
            .Times(1);
        }
    };

    static const float Const = 1812251934.0f;
    static const float Linear = 1812251935.0f;
    static const float Exponent = 1812251936.0f;

    const Light::Data Attenuation =
    {
      Component_t::Make(
        {
          { uT("kind"), uT("Attenuation") },
          { uT("const"), Const },
          { uT("linear"), Linear },
          { uT("exponent"), Exponent },
        }),
      [&](int _Index)
        {
          EXPECT_CALL(GLProxy, Lightf(_Index, GL_CONSTANT_ATTENUATION, Const))
            .Times(1);

          EXPECT_CALL(GLProxy, Lightf(_Index, GL_LINEAR_ATTENUATION, Linear))
            .Times(1);

          EXPECT_CALL(GLProxy, Lightf(_Index, GL_QUADRATIC_ATTENUATION, Exponent))
            .Times(1);
        }
    };

    return ::std::vector<Light::Data>{ Position, Attenuation };
  };

  auto pPoint = Component_t::Make({ { uT("kind"), uT("Point") } });

  const ::std::vector<Light> Lights =
  {
    {
      Component_t::Make({ { uT("kind"), uT("Ambient") } }),
      { DefaultData },
      0x12251657,
      0x12251658,
      0x12251659,
    },
    {
      Component_t::Make({ { uT("kind"), uT("Direction") } }),
      GetDirectionData(),
      0x12251660,
      0x12251661,
      0x12251662,
    },
    {
      pPoint,
      { DefaultData },
      0x12251663,
      0x12251664,
      0x12251665,
    },
    {
      pPoint,
      GetPointData(),
      0x12251666,
      0x12251667,
      0x12251668,
    },
  };

  TestUsingLights(true, Default);
  TestUsingLights(false, Lights);
}

} // unnamed namespace
