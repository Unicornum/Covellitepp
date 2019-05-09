
#pragma once

inline static ::std::vector<float> ARGBtoFloat4(uint32_t _HexColor)
{
  return
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

  const Data_t oData;
  const ::covellite::api::renderer::SettingsData & Data = oData;

  auto pExample = factory::make_unique<ITested_t>(m_UsingApi, Data);
  EXPECT_NO_THROW(dynamic_cast<Tested_t &>(*pExample));
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

  auto itStateCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itStateCreator);

  auto itTextureCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itTextureCreator);

  auto SamplerRender = itStateCreator->second(Component_t::Make(
    { 
      { uT("kind"), uT("Sampler") }
    }));
  ASSERT_NE(nullptr, SamplerRender);

  auto TextureRender = itTextureCreator->second(Component_t::Make({ }));
  ASSERT_NE(nullptr, TextureRender);

  using namespace ::testing;

  EXPECT_CALL(GLProxy, TexParameteri(_, _, _))
    .Times(0);

  SamplerRender();

  InSequence Dummy;

  EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, _))
    .Times(1);

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

  TextureRender();
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
      { uT("enabled"), true },
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
      { uT("enabled"), false },
    });

  auto Render = itCreator->second(pComponent);
  ASSERT_NE(nullptr, Render);

  using namespace ::testing;

  EXPECT_CALL(GLProxy, Disable(GL_SCISSOR_TEST))
    .Times(1);

  Render();
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_State_Depth_Disabled)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  using namespace ::testing;

  const auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pState)
  {
    auto Render = itCreator->second(_pState);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, Disable(GL_DEPTH_TEST))
      .Times(1);

    EXPECT_CALL(GLProxy, Enable(GL_DEPTH_TEST))
      .Times(0);

    EXPECT_CALL(GLProxy, Clear(GL_DEPTH_BUFFER_BIT))
      .Times(0);

    Render();
  };

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Depth") },
    }));

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Depth") },
      { uT("enabled"), false },
    }));
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_State_Depth_Enabled)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  using namespace ::testing;

  const auto Render = itCreator->second(Component_t::Make(
    {
      { uT("kind"), uT("Depth") },
      { uT("enabled"), true },
      { uT("clear"), false },
    }));
  ASSERT_NE(nullptr, Render);

  EXPECT_CALL(GLProxy, Disable(GL_DEPTH_TEST))
    .Times(0);

  EXPECT_CALL(GLProxy, Enable(GL_DEPTH_TEST))
    .Times(1);

  EXPECT_CALL(GLProxy, Clear(GL_DEPTH_BUFFER_BIT))
    .Times(0);

  Render();
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_State_Depth_Clear)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  using namespace ::testing;

  const auto Render = itCreator->second(Component_t::Make(
    {
      { uT("kind"), uT("Depth") },
      { uT("enabled"), true },
      { uT("clear"), true },
    }));
  ASSERT_NE(nullptr, Render);

  EXPECT_CALL(GLProxy, Disable(GL_DEPTH_TEST))
    .Times(0);

  EXPECT_CALL(GLProxy, Enable(GL_DEPTH_TEST))
    .Times(1);

  EXPECT_CALL(GLProxy, Clear(GL_DEPTH_BUFFER_BIT))
    .Times(1);

  Render();
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_State_Clear)
{
  using Color_t = ::std::vector<float>;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  Tested_t Example{ Data_t{} };
  ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pState,
    const Color_t & _ExpectedColor)
  {
    const auto Render = itCreator->second(_pState);
    ASSERT_NE(nullptr, Render);

    using namespace ::testing;

    InSequence Dummy;

    ASSERT_EQ(4, _ExpectedColor.size());

    EXPECT_CALL(GLProxy, ClearColor(_ExpectedColor[0], _ExpectedColor[1],
      _ExpectedColor[2], _ExpectedColor[3]))
      .Times(1);

    EXPECT_CALL(GLProxy, Clear(GL_COLOR_BUFFER_BIT))
      .Times(1);

    Render();
  };

  {
    const ::std::vector<FLOAT> DefaultColor =
    {
      0.0f,
      0.0f,
      0.0f,
      1.0f,
    };

    TestCallRender(Component_t::Make(
      {
        { uT("kind"), uT("Clear") },
      }), DefaultColor);
  }

  {
    const ::std::vector<FLOAT> Color =
    {
      0.86274509803921568627450980392157f, // DC
      0.72941176470588235294117647058824f, // BA
      0.5960784313725490196078431372549f, // 98
      0.9960784313725490196078431372549f, // FE
    };

    TestCallRender(Component_t::Make(
      {
        { uT("kind"), uT("Clear") },
        { uT("color"), 0xFEDCBA98 },
      }), Color);
  }
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_State_AlphaTest)
{
  using Color_t = ::std::vector<float>;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  Tested_t Example{ Data_t{} };
  ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pState,
    const float & _ExpectedValue)
  {
    const auto Render = itCreator->second(_pState);
    ASSERT_NE(nullptr, Render);

    using namespace ::testing;

    EXPECT_CALL(GLProxy, Enable(GL_ALPHA_TEST))
      .Times(1);

    EXPECT_CALL(GLProxy, AlphaFunc(GL_GREATER, _ExpectedValue))
      .Times(1);

    Render();
  };

  {
    const auto pState = Component_t::Make(
      {
        { uT("kind"), uT("AlphaTest") },
      });

    TestCallRender(pState, 0.0f);
  }

  {
    const auto Value = 1904271913.0f;

    const auto pState = Component_t::Make(
      {
        { uT("kind"), uT("AlphaTest") },
        { uT("discard"), Value },
      });

    TestCallRender(pState, Value);
  }
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
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Buffer_UnknownType)
{
  const ::std::vector<float> Source = { 0.0f };

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  // ***************** Передача данных в объекте компонента ***************** //

  {
    const auto pBuffer = Component_t::Make(
      {
        { uT("id"), uT("id.1905081956") },
        { uT("type"), uT("type.1905081956") },
        { uT("kind"), uT("kind.1905081956") },
        { uT("data"), Source.data() },
        { uT("count"), Source.size() },
      });

    EXPECT_STDEXCEPTION(itCreator->second(pBuffer),
      ".+\\.cpp\\([0-9]+\\): Unexpected buffer format \\["
      "id: id.1905081956, "
      "type: type.1905081956, "
      "kind: kind.1905081956\\]\\.");
  }

  // ************** Передача данных в объекте компонента Data *************** //

  {
    auto itDataCreator = IExample.GetCreators().find(uT("Data"));
    ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("data"), Source.data() },
        { uT("count"), Source.size() },
      });

    auto DataRender = itDataCreator->second(pData);
    EXPECT_EQ(nullptr, DataRender);

    const auto pBuffer = Component_t::Make(
      { 
        { uT("id"), uT("id.1905082000") },
        { uT("type"), uT("type.1905082000") },
        { uT("kind"), uT("kind.1905082000") },
        { uT("data"), Source.data() },
        { uT("count"), Source.size() },
      });

    EXPECT_STDEXCEPTION(itCreator->second(pBuffer),
      ".+\\.cpp\\([0-9]+\\): Unexpected buffer format \\["
      "id: id.1905082000, "
      "type: type.1905082000, "
      "kind: kind.1905082000\\]\\.");
  }
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Buffer_Vertex_Polygon)
{
  using Vertex_t = ::covellite::api::Vertex::Polygon;

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

  class Normal
  {
  public:
    float nx, ny, nz;
  };

  const ::mock::GLProxy::Floats_t ExpectedNormalData =
  {
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, // Здесь 3 элемента, потому что в исходном массиве вершин
                      // 3 элемента.
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

    EXPECT_CALL(GLProxy, NormalPointer(GL_FLOAT,
      sizeof(Normal), ExpectedNormalData))
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

    const auto pBuffer = Component_t::Make(
      {
        { uT("data"), Data.data() },
        { uT("count"), Data.size() },
      });

    Render = itCreator->second(pBuffer);
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

    const auto pBuffer = Component_t::Make({ });

    Render = itCreator->second(pBuffer);
  }

  TestCallRender(Render);
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Buffer_Vertex_Polyhedron)
{
  using Vertex_t = ::covellite::api::Vertex::Polyhedron;

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

    const auto pBuffer = Component_t::Make(
      {
        { uT("data"), Data.data() },
        { uT("count"), Data.size() },
      });

    Render = itCreator->second(pBuffer);
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

    const auto pBuffer = Component_t::Make({ });

    Render = itCreator->second(pBuffer);
  }

  TestCallRender(Render);
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Camera_Orthographic_DefaultPosition)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  using namespace ::testing;

  auto TestCallRender = [&](Render_t & _Render)
  {
    ASSERT_NE(nullptr, _Render);

    InSequence Dummy;

    EXPECT_CALL(GLProxy, Disable(GL_BLEND))
      .Times(1);

    EXPECT_CALL(GLProxy, Disable(GL_ALPHA_TEST))
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

    const float Viewport[] =
    {
      1812271148.0f, 1812271149.0f, 1812181722.0f, 1812181723.0f
    };

    EXPECT_CALL(GLProxy, GetFloatv(GL_VIEWPORT))
      .Times(1)
      .WillOnce(Return(Viewport));

    EXPECT_CALL(GLProxy, MatrixMode(GL_PROJECTION))
      .Times(1);

    EXPECT_CALL(GLProxy, LoadIdentity())
      .Times(1);

    EXPECT_CALL(GLProxy, MatrixMode(GL_MODELVIEW))
      .Times(1);

    EXPECT_CALL(GLProxy, LoadIdentity())
      .Times(1);

    EXPECT_CALL(GLProxy, Ortho(Viewport[0], Viewport[0] + Viewport[2],
      Viewport[1] + Viewport[3], Viewport[1], -1, 1))
      .Times(1);

    EXPECT_CALL(GLProxy, Disable(_))
      .Times(AtLeast(1));

    _Render();
  };

  auto Render = itCreator->second(Component_t::Make(
    {
      { uT("kind"), uT("Orthographic") },
    }));

  EXPECT_CALL(GLProxy, Disable(GL_DEPTH_TEST))
    .Times(1);

  EXPECT_CALL(GLProxy, Enable(GL_DEPTH_TEST))
    .Times(0);

  EXPECT_CALL(GLProxy, Clear(GL_DEPTH_BUFFER_BIT))
    .Times(0);

  TestCallRender(Render);
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Camera_Orthographic)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const auto X = 11111.0f;
  const auto Y = 22222.0f;

  const float SourceViewport[] =
  {
    19022.82014f, 19022.82015f, 19022.82016f, 19022.82017f
  };

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  using namespace ::testing;

  auto TestCallRender = [&](Render_t & _Render)
  {
    ASSERT_NE(nullptr, _Render);

    InSequence Dummy;

    EXPECT_CALL(GLProxy, Disable(GL_DEPTH_TEST))
      .Times(1);

    EXPECT_CALL(GLProxy, Enable(GL_DEPTH_TEST))
      .Times(0);

    EXPECT_CALL(GLProxy, Clear(GL_DEPTH_BUFFER_BIT))
      .Times(0);

    EXPECT_CALL(GLProxy, Disable(GL_BLEND))
      .Times(1);

    EXPECT_CALL(GLProxy, Disable(GL_ALPHA_TEST))
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

    EXPECT_CALL(GLProxy, GetFloatv(GL_VIEWPORT))
      .Times(1)
      .WillOnce(Return(SourceViewport));

    EXPECT_CALL(GLProxy, MatrixMode(GL_PROJECTION))
      .Times(1);

    EXPECT_CALL(GLProxy, LoadIdentity())
      .Times(1);

    EXPECT_CALL(GLProxy, MatrixMode(GL_MODELVIEW))
      .Times(1);

    EXPECT_CALL(GLProxy, LoadIdentity())
      .Times(1);

    EXPECT_CALL(GLProxy, Ortho(
      SourceViewport[0] + X, SourceViewport[0] + SourceViewport[2] + X,
      SourceViewport[1] + SourceViewport[3] + Y, SourceViewport[1] + Y, 
      -1, 1))
      .Times(1);

    EXPECT_CALL(GLProxy, Disable(_))
      .Times(AtLeast(1));

    _Render();
  };

  auto PositionRender = itDataCreator->second(Component_t::Make(
    {
      { uT("kind"), uT("Position") },
      { uT("x"), X },
      { uT("y"), Y },
    }));
  EXPECT_EQ(nullptr, PositionRender);

  auto Render = itCreator->second(Component_t::Make(
    {
      { uT("kind"), uT("Orthographic") },
    }));

  TestCallRender(Render);
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Camera_Perspective)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const float Width = 1024.0f;
  const float Height = 768.0f;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  using namespace ::testing;

  auto TestCallRender = [&](Render_t & _Render, 
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
        ::DirectX::XMMatrixPerspectiveFovRH(
          AngleYRadians, Width / Height, 0.01f, _zFar));

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
        ::DirectX::XMMatrixLookAtRH(Eye, Look,
          ::DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)));

    ASSERT_NE(nullptr, _Render);

    InSequence Dummy;

    EXPECT_CALL(GLProxy, Disable(GL_DEPTH_TEST))
      .Times(1);

    EXPECT_CALL(GLProxy, Enable(GL_DEPTH_TEST))
      .Times(0);

    EXPECT_CALL(GLProxy, Clear(GL_DEPTH_BUFFER_BIT))
      .Times(0);

    EXPECT_CALL(GLProxy, Disable(GL_BLEND))
      .Times(1);

    EXPECT_CALL(GLProxy, Disable(GL_ALPHA_TEST))
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

    EXPECT_CALL(GLProxy, Disable(_))
      .Times(AtLeast(1));

    _Render();
  };

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itCreator = IExample.GetCreators().find(uT("Camera"));
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

  auto TestCamera = [&](const Component_t::ComponentPtr_t & _pCamera)
  {
    {
      // Default values + not using Position & Rotation

      auto Render = itCreator->second(_pCamera);

      auto Info = CameraInfos[0];

      TestCallRender(Render, 
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

      TestCallRender(Render, 
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

        _pCamera->SetValue(uT("fov"), Info.AngleY);
        _pCamera->SetValue(uT("distance"), Info.Distance);

        TestCallRender(Render, 
          Info.AngleY, zFar,
          Info.X, Info.Y, Info.Z,
          Info.A, Info.B, Info.C,
          Info.Distance);
      }
    }
  };

  TestCamera(Component_t::Make(
    {
      { uT("kind"), uT("Perspective") },
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

    auto GetRawMatrix = [](const ::DirectX::XMMATRIX & _Matrix)
    {
      ::DirectX::XMFLOAT4X4 Result;
      ::DirectX::XMStoreFloat4x4(&Result, _Matrix);
      return Result;
    };

    const auto ViewMatrix = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixLookAtRH(
        ::DirectX::XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f),
        ::DirectX::XMVectorSet(4.0f, 5.0f, 6.0f, 0.0f),
        ::DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)));

    const auto RawViewMatrix = GetRawMatrix(ViewMatrix);

    const auto RawWorldMatrix = GetRawMatrix(
      ::DirectX::XMMatrixTranslation(_X, _Y, _Z) *
      ::DirectX::XMMatrixRotationX(_A) *
      ::DirectX::XMMatrixRotationY(_B) *
      ::DirectX::XMMatrixRotationZ(_C) *
      ::DirectX::XMMatrixScaling(_Sx, _Sy, _Sz) *
      ViewMatrix);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, PushMatrix())
      .Times(1);

    EXPECT_CALL(GLProxy, GetFloatv(GL_MODELVIEW_MATRIX))
      .Times(1)
      .WillOnce(Return(&RawViewMatrix.m[0][0]));

    EXPECT_CALL(GLProxy, LoadMatrixf(RawWorldMatrix))
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
      1956.0f, 1957.0f, 1958.0f,
      1204.0f, 1205.0f, 1206.0f,
      1152.0f, 1153.0f, 1154.0f);

    TestCallRender(IndexBufferRender, Render,
      1959.0f, 1960.0f, 1961.0f,
      1145.0f, 1146.0f, 1147.0f,
      1155.0f, 1157.0f, 1158.0f);
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
      1956.0f, 1957.0f, 1958.0f,
      1204.0f, 1205.0f, 1206.0f,
      1152.0f, 1153.0f, 1154.0f);

    TestCallRender(IndexBufferRender, Render,
      1959.0f, 1960.0f, 1961.0f,
      1145.0f, 1146.0f, 1147.0f,
      1155.0f, 1157.0f, 1158.0f);
  }
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Present_Geometry_Static)
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

  auto itBufferCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itBufferCreator);

  auto itPresentCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itPresentCreator);

  auto pPosition = Component_t::Make({ { uT("kind"), uT("Position") } });
  auto pRotation = Component_t::Make({ { uT("kind"), uT("Rotation") } });
  auto pScale = Component_t::Make({ { uT("kind"), uT("Scale") } });

  Render_t Render;
  ::DirectX::XMFLOAT4X4 LastRawWorldMatrix;

  auto TestCallRender = [&](Render_t & _IndexBufferRender,
    float _X, float _Y, float _Z,
    float _A, float _B, float _C,
    float _Sx, float _Sy, float _Sz,
    const bool _IsExpectLastMatrix)
  {
    pPosition->SetValue(uT("x"), _X);
    pPosition->SetValue(uT("y"), _Y);
    pPosition->SetValue(uT("z"), _Z);

    pRotation->SetValue(uT("x"), _A);
    pRotation->SetValue(uT("y"), _B);
    pRotation->SetValue(uT("z"), _C);

    pScale->SetValue(uT("x"), _Sx);
    pScale->SetValue(uT("y"), _Sy);
    pScale->SetValue(uT("z"), _Sz);

    if (!_IsExpectLastMatrix)
    {
      auto PositionRender = itDataCreator->second(pPosition);
      EXPECT_EQ(nullptr, PositionRender);

      auto RotationRender = itDataCreator->second(pRotation);
      EXPECT_EQ(nullptr, RotationRender);

      auto ScaleRender = itDataCreator->second(pScale);
      EXPECT_EQ(nullptr, ScaleRender);

      Render = itPresentCreator->second(Component_t::Make(
        {
          { uT("kind"), uT("Geometry") },
          { uT("static"), true },
        }));
    }

    ASSERT_NE(nullptr, _IndexBufferRender);
    ASSERT_NE(nullptr, Render);

    auto GetRawMatrix = [](const ::DirectX::XMMATRIX & _Matrix)
    {
      ::DirectX::XMFLOAT4X4 Result;
      ::DirectX::XMStoreFloat4x4(&Result, _Matrix);
      return Result;
    };

    const auto ViewMatrix = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixLookAtRH(
        ::DirectX::XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f),
        ::DirectX::XMVectorSet(4.0f, 5.0f, 6.0f, 0.0f),
        ::DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)));

    const auto RawViewMatrix = GetRawMatrix(ViewMatrix);

    if (!_IsExpectLastMatrix)
    {
      LastRawWorldMatrix = GetRawMatrix(
        ::DirectX::XMMatrixTranslation(_X, _Y, _Z) *
        ::DirectX::XMMatrixRotationX(_A) *
        ::DirectX::XMMatrixRotationY(_B) *
        ::DirectX::XMMatrixRotationZ(_C) *
        ::DirectX::XMMatrixScaling(_Sx, _Sy, _Sz) *
        ViewMatrix);
    }

    const auto RawWorldMatrix = LastRawWorldMatrix;

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, PushMatrix())
      .Times(1);

    EXPECT_CALL(GLProxy, GetFloatv(GL_MODELVIEW_MATRIX))
      .Times(1)
      .WillOnce(Return(&RawViewMatrix.m[0][0]));

    EXPECT_CALL(GLProxy, LoadMatrixf(RawWorldMatrix))
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

    Render();
  };

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

    // Два вызова, чтобы убедиться, что изменение исходных данных не приводит 
    // к изменению результата рендеринга.

    TestCallRender(IndexBufferRender,
      1956.0f, 1957.0f, 1958.0f,
      1204.0f, 1205.0f, 1206.0f,
      1152.0f, 1153.0f, 1154.0f,
      false);

    TestCallRender(IndexBufferRender,
      1959.0f, 1960.0f, 1961.0f,
      1145.0f, 1146.0f, 1147.0f,
      1155.0f, 1157.0f, 1158.0f,
      true);
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

    // Два вызова, чтобы убедиться, что изменение исходных данных не приводит 
    // к изменению результата рендеринга.

    TestCallRender(IndexBufferRender,
      1956.0f, 1957.0f, 1958.0f,
      1204.0f, 1205.0f, 1206.0f,
      1152.0f, 1153.0f, 1154.0f,
      false);

    TestCallRender(IndexBufferRender,
      1959.0f, 1960.0f, 1961.0f,
      1145.0f, 1146.0f, 1147.0f,
      1155.0f, 1157.0f, 1158.0f,
      true);
  }
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Present_Geometry_DefaultTransformValues)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itPresentCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itPresentCreator);

  auto pPosition = Component_t::Make({ { uT("kind"), uT("Position") } });
  auto pRotation = Component_t::Make({ { uT("kind"), uT("Rotation") } });
  auto pScale = Component_t::Make({ { uT("kind"), uT("Scale") } });

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pComponent)
  {
    auto PositionRender = itDataCreator->second(pPosition);
    EXPECT_EQ(nullptr, PositionRender);

    auto RotationRender = itDataCreator->second(pRotation);
    EXPECT_EQ(nullptr, RotationRender);

    auto ScaleRender = itDataCreator->second(pScale);
    EXPECT_EQ(nullptr, ScaleRender);

    const auto Render = itPresentCreator->second(_pComponent);
    ASSERT_NE(nullptr, Render);

    auto GetRawMatrix = [](const ::DirectX::XMMATRIX & _Matrix)
    {
      ::DirectX::XMFLOAT4X4 Result;
      ::DirectX::XMStoreFloat4x4(&Result, _Matrix);
      return Result;
    };

    const auto ViewMatrix = ::DirectX::XMMatrixTranspose(
      ::DirectX::XMMatrixLookAtRH(
        ::DirectX::XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f),
        ::DirectX::XMVectorSet(4.0f, 5.0f, 6.0f, 0.0f),
        ::DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)));

    const auto RawViewMatrix = GetRawMatrix(ViewMatrix);

    const auto RawWorldMatrix = GetRawMatrix(
      ::DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f) *
      ::DirectX::XMMatrixRotationX(0.0f) *
      ::DirectX::XMMatrixRotationY(0.0f) *
      ::DirectX::XMMatrixRotationZ(0.0f) *
      ::DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *
      ViewMatrix);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GetFloatv(GL_MODELVIEW_MATRIX))
      .Times(1)
      .WillOnce(Return(&RawViewMatrix.m[0][0]));

    EXPECT_CALL(GLProxy, LoadMatrixf(RawWorldMatrix))
      .Times(1);

    Render();
  };

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Geometry") },
    }));

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Geometry") },
      { uT("static"), true },
    }));
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Present_Geometry_CombineTransform)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto GetRawMatrix = [](const ::DirectX::XMMATRIX & _Matrix)
  {
    ::DirectX::XMFLOAT4X4 Result;
    ::DirectX::XMStoreFloat4x4(&Result, _Matrix);
    return Result;
  };

  ::DirectX::XMMATRIX MatrixIdentity;

  const auto SetPosition = [&](float _X, float _Y, float _Z)
  {
    auto pPosition = Component_t::Make(
      {
        { uT("kind"), uT("Position") },
        { uT("x"), _X },
        { uT("y"), _Y },
        { uT("z"), _Z },
      });

    auto PositionRender = itDataCreator->second(pPosition);
    EXPECT_EQ(nullptr, PositionRender);

    MatrixIdentity *= ::DirectX::XMMatrixTranslation(_X, _Y, _Z);
  };

  const auto SetRotation = [&](float _X, float _Y, float _Z)
  {
    auto pRotation = Component_t::Make(
      {
        { uT("kind"), uT("Rotation") },
        { uT("x"), _X },
        { uT("y"), _Y },
        { uT("z"), _Z },
      });

    auto RotationRender = itDataCreator->second(pRotation);
    EXPECT_EQ(nullptr, RotationRender);

    MatrixIdentity *= ::DirectX::XMMatrixRotationX(_X);
    MatrixIdentity *= ::DirectX::XMMatrixRotationY(_Y);
    MatrixIdentity *= ::DirectX::XMMatrixRotationZ(_Z);
  };

  const auto SetScale = [&](float _X, float _Y, float _Z)
  {
    auto pScale = Component_t::Make(
      {
        { uT("kind"), uT("Scale") },
        { uT("x"), _X },
        { uT("y"), _Y },
        { uT("z"), _Z },
      });

    auto ScaleRender = itDataCreator->second(pScale);
    EXPECT_EQ(nullptr, ScaleRender);

    MatrixIdentity *= ::DirectX::XMMatrixScaling(_X, _Y, _Z);
  };

  {
    MatrixIdentity = ::DirectX::XMMatrixTranspose(::DirectX::XMMatrixIdentity());
    const auto RawViewMatrix = GetRawMatrix(MatrixIdentity);

    SetPosition(1.0f, 2.0f, 3.0f);
    SetRotation(4.0f, 5.0f, 6.0f);
    SetScale(7.0f, 8.0f, 9.0f);

    SetPosition(11.0f, 22.0f, 33.0f);
    SetRotation(44.0f, 55.0f, 66.0f);
    SetScale(77.0f, 88.0f, 99.0f);

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Geometry") }
      }));
    ASSERT_NE(nullptr, Render);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GetFloatv(GL_MODELVIEW_MATRIX))
      .Times(1)
      .WillOnce(Return(&RawViewMatrix.m[0][0]));

    EXPECT_CALL(GLProxy, LoadMatrixf(GetRawMatrix(MatrixIdentity)))
      .Times(1);

    Render();
  }

  {
    MatrixIdentity = ::DirectX::XMMatrixTranspose(::DirectX::XMMatrixIdentity());
    const auto RawViewMatrix = GetRawMatrix(MatrixIdentity);

    SetPosition(1.0f, 2.0f, 3.0f);
    SetRotation(4.0f, 5.0f, 6.0f);
    SetScale(7.0f, 8.0f, 9.0f);

    SetPosition(11.0f, 22.0f, 33.0f);
    SetRotation(44.0f, 55.0f, 66.0f);
    SetScale(77.0f, 88.0f, 99.0f);

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Geometry") },
        { uT("static"), true }
      }));
    ASSERT_NE(nullptr, Render);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GetFloatv(GL_MODELVIEW_MATRIX))
      .Times(1)
      .WillOnce(Return(&RawViewMatrix.m[0][0]));

    EXPECT_CALL(GLProxy, LoadMatrixf(GetRawMatrix(MatrixIdentity)))
      .Times(1);

    Render();
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

  auto itCameraCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

  using namespace ::testing;

  auto TestCallRenderCameraWithoutLights = [&](Render_t & _Render)
  {
    EXPECT_CALL(GLProxy, Disable(_))
      .Times(AtLeast(1));

    EXPECT_CALL(GLProxy, Enable(_))
      .Times(AtLeast(1));

    const float Viewport[] =
    {
      1812271148.0f, 1812271149.0f, 1812181722.0f, 1812181723.0f
    };

    EXPECT_CALL(GLProxy, GetFloatv(GL_VIEWPORT))
      .Times(AtLeast(1))
      .WillRepeatedly(Return(Viewport));

    EXPECT_CALL(GLProxy, MatrixMode(_))
      .Times(AtLeast(1));

    InSequence Dummy;

    EXPECT_CALL(GLProxy, MatrixMode(GL_MODELVIEW))
      .Times(1);

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
      ::std::function<void(int, uint32_t)> Test;
    };

  public:
    Component_t::ComponentPtr_t pLight;
    ::std::vector<Data> DataInfo;
    uint32_t Color;
  };

  auto TestUsingLights = [&](
    const Component_t::ComponentPtr_t & _pCamera1,
    const Component_t::ComponentPtr_t & _pCamera2,
    bool _IsDefault,
    const ::std::vector<Light> & _Lights, 
    bool _IsTestDeleteOverMax = false)
  {
    auto Camera1Render = itCameraCreator->second(_pCamera1);
    ASSERT_NE(nullptr, Camera1Render);

    auto Camera2Render = itCameraCreator->second(_pCamera2);
    ASSERT_NE(nullptr, Camera2Render);

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
        Light.pLight->SetValue(uT("color"), Light.Color);
      }

      Render();
    }

    TestCallRenderCameraWithoutLights(Camera1Render);

    EXPECT_CALL(GLProxy, Disable(_))
      .Times(AtLeast(1));

    EXPECT_CALL(GLProxy, Enable(_))
      .Times(AtLeast(1));

    const float Viewport[] =
    {
      1812271148.0f, 1812271149.0f, 1812181722.0f, 1812181723.0f
    };

    EXPECT_CALL(GLProxy, GetFloatv(GL_VIEWPORT))
      .Times(AtLeast(1))
      .WillRepeatedly(Return(Viewport));

    EXPECT_CALL(GLProxy, MatrixMode(_))
      .Times(AtLeast(1));

    InSequence Dummy;

    EXPECT_CALL(GLProxy, MatrixMode(GL_MODELVIEW))
      .Times(1);

    EXPECT_CALL(GLProxy, Disable(GL_LIGHTING))
      .Times(0);

    EXPECT_CALL(GLProxy, Enable(GL_LIGHTING))
      .Times(1);

    EXPECT_CALL(GLProxy, Enable(GL_NORMALIZE))
      .Times(1);

    EXPECT_CALL(GLProxy, LightModelfv(GL_LIGHT_MODEL_AMBIENT, ARGBtoFloat4(0)))
      .Times(1);

    int Index = GL_LIGHT0;

    for (size_t i = 0; i < _Lights.size(); i++)
    {
      EXPECT_CALL(GLProxy, Enable(Index))
        .Times(1);

      for (const auto & Data : _Lights[i].DataInfo)
      {
        Data.Test(Index, _Lights[i].Color);
      }

      Index++;
    }

    const auto TestDeleteLights = [&](size_t _Count, bool _LastIsEnabled)
    {
      for (size_t i = 0; i < _Count; i++)
      {
        EXPECT_CALL(GLProxy, IsEnabled(Index))
          .Times(1)
          .WillOnce(Return(true));

        EXPECT_CALL(GLProxy, Disable(Index))
          .Times(1);

        Index++;
      }

      EXPECT_CALL(GLProxy, IsEnabled(Index))
        .Times(1)
        .WillOnce(Return(_LastIsEnabled));

      EXPECT_CALL(GLProxy, Disable(Index))
        .Times(0);

      EXPECT_CALL(GLProxy, IsEnabled(_))
        .Times(0);
    };

    if (_IsTestDeleteOverMax)
    {
      TestDeleteLights(MAX_LIGHT_POINT_COUNT - _Lights.size(), true);
    }
    else
    {
      TestDeleteLights(MAX_LIGHT_POINT_COUNT / 2, false);
    }

    EXPECT_CALL(GLProxy, Enable(_))
      .Times(0);

    EXPECT_CALL(GLProxy, Lightfv(_, _, _))
      .Times(0);

    EXPECT_CALL(GLProxy, Lightf(_, _, _))
      .Times(0);

    Camera2Render();

    TestCallRenderCameraWithoutLights(Camera1Render);
    TestCallRenderCameraWithoutLights(Camera2Render);
  };

  const Light::Data DefaultData =
  {
    Component_t::Make({}),
    [&](int _Index, uint32_t /*_Color*/)
    {
      EXPECT_CALL(GLProxy,
        Lightfv(_Index, GL_AMBIENT, ARGBtoFloat4(0xFF000000)))
        .Times(1);

      EXPECT_CALL(GLProxy,
        Lightfv(_Index, GL_DIFFUSE, ARGBtoFloat4(0xFF000000)))
        .Times(1);

      EXPECT_CALL(GLProxy,
        Lightfv(_Index, GL_SPECULAR, ARGBtoFloat4(0xFF000000)))
        .Times(1);

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
      0xFF000000
    },
    {
      Component_t::Make({ { uT("kind"), uT("Direction") } }),
      { DefaultData },
      0xFF000000
    },
    {
      Component_t::Make({ { uT("kind"), uT("Point") } }),
      { DefaultData },
      0xFF000000
    },
  };

  auto GetAmbientData = [&](void)
  {
    const Light::Data Ambient =
    {
      Component_t::Make({}),
      [&](int _Index, uint32_t _Color)
        {
          EXPECT_CALL(GLProxy,
            Lightfv(_Index, GL_AMBIENT, ARGBtoFloat4(_Color)))
            .Times(1);

          EXPECT_CALL(GLProxy,
            Lightfv(_Index, GL_DIFFUSE, ARGBtoFloat4(0xFF000000)))
            .Times(1);

          EXPECT_CALL(GLProxy,
            Lightfv(_Index, GL_SPECULAR, ARGBtoFloat4(0xFF000000)))
            .Times(1);

          EXPECT_CALL(GLProxy, Lightfv(_Index, GL_POSITION,
            ::std::vector<float>{ 0.0f, 0.0f, 1.0f, 0.0f }))
            .Times(1);

          EXPECT_CALL(GLProxy, Lightf(_Index, GL_CONSTANT_ATTENUATION, 1.0f))
            .Times(1);

          EXPECT_CALL(GLProxy, Lightf(_Index, GL_LINEAR_ATTENUATION, 0.0f))
            .Times(1);

          EXPECT_CALL(GLProxy, Lightf(_Index, GL_QUADRATIC_ATTENUATION, 0.0f))
            .Times(1);
        }
    };

    return ::std::vector<Light::Data>{ Ambient };
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
      [&](int _Index, uint32_t _Color)
        {
          EXPECT_CALL(GLProxy,
            Lightfv(_Index, GL_AMBIENT, ARGBtoFloat4(0xFF000000)))
            .Times(1);

          EXPECT_CALL(GLProxy,
            Lightfv(_Index, GL_DIFFUSE, ARGBtoFloat4(_Color)))
            .Times(1);

          EXPECT_CALL(GLProxy,
            Lightfv(_Index, GL_SPECULAR, ARGBtoFloat4(_Color)))
            .Times(1);

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
    const Light::Data Color =
    {
      Component_t::Make({}),
      [&](int _Index, uint32_t _Color)
        {
          EXPECT_CALL(GLProxy,
            Lightfv(_Index, GL_AMBIENT, ARGBtoFloat4(0xFF000000)))
            .Times(1);

          EXPECT_CALL(GLProxy,
            Lightfv(_Index, GL_DIFFUSE, ARGBtoFloat4(_Color)))
            .Times(1);

          EXPECT_CALL(GLProxy,
            Lightfv(_Index, GL_SPECULAR, ARGBtoFloat4(_Color)))
            .Times(1);
        }
    };

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
      [&](int _Index, uint32_t /*_Color*/)
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
      [&](int _Index, uint32_t /*_Color*/)
        {
          EXPECT_CALL(GLProxy, Lightf(_Index, GL_CONSTANT_ATTENUATION, Const))
            .Times(1);

          EXPECT_CALL(GLProxy, Lightf(_Index, GL_LINEAR_ATTENUATION, Linear))
            .Times(1);

          EXPECT_CALL(GLProxy, Lightf(_Index, GL_QUADRATIC_ATTENUATION, Exponent))
            .Times(1);
        }
    };

    return ::std::vector<Light::Data>{ Color, Position, Attenuation };
  };

  auto pPoint = Component_t::Make({ { uT("kind"), uT("Point") } });

  const ::std::vector<Light> LightAmbient =
  {
    {
      Component_t::Make({ { uT("kind"), uT("Ambient") } }),
      GetAmbientData(),
      0x02212020,
    },
  };

  const ::std::vector<Light> LightDirection =
  {
    {
      Component_t::Make({ { uT("kind"), uT("Direction") } }),
      GetDirectionData(),
      0x02212021,
    },
  };

  const ::std::vector<Light> LightPoint =
  {
    {
      pPoint,
      GetPointData(),
      0x02212022,
    },
  };

  const ::std::vector<Light> Lights =
  {
    {
      Component_t::Make({ { uT("kind"), uT("Ambient") } }),
      GetAmbientData(),
      0x12251657,
    },
    {
      Component_t::Make({ { uT("kind"), uT("Direction") } }),
      GetDirectionData(),
      0x12251660,
    },
    {
      pPoint,
      { DefaultData },
      0xFF000000,
    },
    {
      pPoint,
      GetPointData(),
      0x12251666,
    },
  };

  {
    auto pCamera1 = Component_t::Make(
      {
        { uT("id"), uT("Camera.1") },
        { uT("kind"), uT("Orthographic") },
      });

    auto pCamera2 = Component_t::Make(
      {
        { uT("id"), uT("Camera.2") },
        { uT("kind"), uT("Orthographic") },
      });

    TestUsingLights(pCamera1, pCamera2, true, Default, true);
    TestUsingLights(pCamera1, pCamera2, false, LightAmbient);
    TestUsingLights(pCamera1, pCamera2, false, LightDirection);
    TestUsingLights(pCamera1, pCamera2, false, LightPoint);
    TestUsingLights(pCamera1, pCamera2, false, Lights);
  }

  {
    auto pCamera1 = Component_t::Make(
      {
        { uT("id"), uT("Camera.3") },
        { uT("kind"), uT("Perspective") },
      });

    auto pCamera2 = Component_t::Make(
      {
        { uT("id"), uT("Camera.4") },
        { uT("kind"), uT("Perspective") },
      });

    TestUsingLights(pCamera1, pCamera2, true, Default, true);
    TestUsingLights(pCamera1, pCamera2, false, LightAmbient);
    TestUsingLights(pCamera1, pCamera2, false, LightDirection);
    TestUsingLights(pCamera1, pCamera2, false, LightPoint);
    TestUsingLights(pCamera1, pCamera2, false, Lights);
  }
}

} // unnamed namespace
