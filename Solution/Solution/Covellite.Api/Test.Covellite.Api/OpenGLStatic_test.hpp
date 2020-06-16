
#pragma once

namespace
{

// ************************************************************************** //
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Texture_Albedo)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & _Source, 
    const int _Width, const int _Height)
  {
    const ::mock::GLuint TextureId = 1812181809;

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
      _Width, _Height, 0,
      GL_RGBA, GL_UNSIGNED_BYTE, _Source))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  {
    const uint8_t * pSource = (uint8_t *)1812181806;
    const int Width = 1812181807;
    const int Height = 1812181808;

    const auto pTexture = Component_t::Make(
      {
        { uT("data"), pSource },
        { uT("width"), Width },
        { uT("height"), Height },
      });

    TestCall(pTexture, pSource, Width, Height);
  }

  {
    const uint8_t * pSource = (uint8_t *)1907251053;
    const int Width = 1907251054;
    const int Height = 1907251055;

    const auto pTexture = Component_t::Make(
      {
        { uT("data"), pSource },
        { uT("width"), Width },
        { uT("height"), Height },
        { uT("destination"), uT("albedo") },
      });

    TestCall(pTexture, pSource, Width, Height);
  }

  {
    const uint8_t * pSource = (uint8_t *)1907251056;
    const int Width = 1907251057;
    const int Height = 1907251058;

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Texture") },
        { uT("data"), pSource },
        { uT("width"), Width },
        { uT("height"), Height },
      });

    auto DataRender = itDataCreator->second(pData);
    EXPECT_EQ(nullptr, DataRender);

    TestCall(Component_t::Make({}), pSource, Width, Height);
  }

  {
    const uint8_t * pSource = (uint8_t *)1907251059;
    const int Width = 1907251100;
    const int Height = 1907251101;

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Texture") },
        { uT("data"), pSource },
        { uT("width"), Width },
        { uT("height"), Height },
        { uT("destination"), uT("albedo") },
      });

    auto DataRender = itDataCreator->second(pData);
    EXPECT_EQ(nullptr, DataRender);

    TestCall(Component_t::Make({}), pSource, Width, Height);
  }
}

// ************************************************************************** //
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Texture_PBR)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pTexture)
  {
    const ::mock::GLuint TextureId = 1812181809;

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(_))
      .Times(0);

    EXPECT_CALL(GLProxy, BindTexture(_, _))
      .Times(0);

    EXPECT_CALL(GLProxy, TexImage2D(_, _, _, _, _, _, _, _, _))
      .Times(0);

    EXPECT_CALL(GLProxy, GetError())
      .Times(0);

    const auto Render = itCreator->second(_pTexture);
    EXPECT_EQ(nullptr, Render);
  };

  const ::std::vector<String_t> IgnoreDestination =
  {
    uT("metalness"),
    uT("roughness"),
    uT("normal"),
    uT("occlusion"),
  };

  for (const auto & Dest : IgnoreDestination)
  {
    const auto pTexture = Component_t::Make(
      {
        { uT("destination"), Dest },
      });

    TestCall(pTexture);
  }

  for (const auto & Dest : IgnoreDestination)
  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Texture") },
        { uT("destination"), Dest },
      });

    auto DataRender = itDataCreator->second(pData);
    EXPECT_EQ(nullptr, DataRender);

    TestCall(Component_t::Make({}));
  }
}

// ************************************************************************** //
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_State_AlphaTest)
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
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Material_DefaultValue)
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
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Material)
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
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Buffer_Vertex_Polygon)
{
  using Vertex_t = ::covellite::api::vertex::Polygon;

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

    EXPECT_CALL(GLProxy, DisableClientState(GL_NORMAL_ARRAY))
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
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Buffer_Vertex_Polyhedron_Static)
{
  using Vertex_t = ::covellite::api::vertex::Polyhedron;

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

    EXPECT_CALL(GLProxy, DisableClientState(GL_COLOR_ARRAY))
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
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Buffer_Vertex_Polyhedron_Dynamic)
{
  using Vertex_t = ::covellite::api::vertex::Polyhedron;
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<Vertex_t>;

  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(Mapper, bool(Vertex_t *));
  };

  Proxy oProxy;
  Proxy::GetInstance() = &oProxy;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const ::std::vector<Vertex_t> Source1 =
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

  const ::mock::GLProxy::Floats_t ExpectedVertexData1 =
  {
    Source1[0].x, Source1[0].y, Source1[0].z,
    Source1[1].x, Source1[1].y, Source1[1].z,
  };

  const ::mock::GLProxy::Floats_t ExpectedNormalData1 =
  {
    Source1[0].nx, Source1[0].ny, Source1[0].nz,
    Source1[1].nx, Source1[1].ny, Source1[1].nz,
  };

  const ::mock::GLProxy::Floats_t ExpectedTexCoordData1 =
  {
    Source1[0].tu, Source1[0].tv,
    Source1[1].tu, Source1[1].tv,
  };

  const ::std::vector<Vertex_t> Source2 =
  {
    { 
      1908011341.0f, 1908261933.0f, 1912161256.0f,
      1908261934.0f, 1912161257.0f,
      1908261935.0f, 1908261936.0f, 1912161258.0f
    },
    {
      1908261932.0f, 1908261933.0f, 1912161259.0f,
      1908261934.0f, 1912161300.0f,
      1908261935.0f, 1908261936.0f, 1912161301.0f
    },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // Маркер конца данных
  };

  const ::mock::GLProxy::Floats_t ExpectedVertexData2 =
  {
    Source2[0].x, Source2[0].y, Source2[0].z,
    Source2[1].x, Source2[1].y, Source2[1].z,
  };

  const ::mock::GLProxy::Floats_t ExpectedNormalData2 =
  {
    Source2[0].nx, Source2[0].ny, Source2[0].nz,
    Source2[1].nx, Source2[1].ny, Source2[1].nz,
  };

  const ::mock::GLProxy::Floats_t ExpectedTexCoordData2 =
  {
    Source2[0].tu, Source2[0].tv,
    Source2[1].tu, Source2[1].tv,
  };

  const BufferMapper_t Mapper = [&](Vertex_t * _pData)
  {
    if (_pData != nullptr)
    {
      memcpy(_pData, Source2.data(), sizeof(Vertex_t) * (Source2.size() - 1));
    }

    return oProxy.Mapper(_pData);
  };

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto TestCallRender = [&](Render_t & _Render)
  {
    ASSERT_NE(nullptr, _Render);

    using namespace ::testing;

    InSequence Dummy;

    {
      EXPECT_CALL(oProxy, Mapper(nullptr))
        .Times(1)
        .WillOnce(Return(false));

      EXPECT_CALL(oProxy, Mapper(_))
        .Times(0);

      EXPECT_CALL(GLProxy, VertexPointer(3, GL_FLOAT,
        sizeof(Vertex_t), ExpectedVertexData1))
        .Times(1);

      EXPECT_CALL(GLProxy, EnableClientState(GL_VERTEX_ARRAY))
        .Times(1);

      EXPECT_CALL(GLProxy, NormalPointer(GL_FLOAT,
        sizeof(Vertex_t), ExpectedNormalData1))
        .Times(1);

      EXPECT_CALL(GLProxy, EnableClientState(GL_NORMAL_ARRAY))
        .Times(1);

      EXPECT_CALL(GLProxy, TexCoordPointer(2, GL_FLOAT,
        sizeof(Vertex_t), ExpectedTexCoordData1))
        .Times(1);

      EXPECT_CALL(GLProxy, EnableClientState(GL_TEXTURE_COORD_ARRAY))
        .Times(1);

      _Render();
    }

    {
      EXPECT_CALL(oProxy, Mapper(nullptr))
        .Times(1)
        .WillOnce(Return(true));

      EXPECT_CALL(oProxy, Mapper(_))
        .Times(1);

      EXPECT_CALL(GLProxy, VertexPointer(3, GL_FLOAT,
        sizeof(Vertex_t), ExpectedVertexData2))
        .Times(1);

      EXPECT_CALL(GLProxy, EnableClientState(GL_VERTEX_ARRAY))
        .Times(1);

      EXPECT_CALL(GLProxy, NormalPointer(GL_FLOAT,
        sizeof(Vertex_t), ExpectedNormalData2))
        .Times(1);

      EXPECT_CALL(GLProxy, EnableClientState(GL_NORMAL_ARRAY))
        .Times(1);

      EXPECT_CALL(GLProxy, TexCoordPointer(2, GL_FLOAT,
        sizeof(Vertex_t), ExpectedTexCoordData2))
        .Times(1);

      EXPECT_CALL(GLProxy, EnableClientState(GL_TEXTURE_COORD_ARRAY))
        .Times(1);

      _Render();
    }
  };

  auto itCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  Render_t Render;

  // ***************** Передача данных в объекте компонента ***************** //

  {
    // Передача данных через локальный объект, чтобы убедиться, что рендер будет
    // хранить копию данных.
    const auto Data = Source1;

    const auto pBuffer = Component_t::Make(
      {
        { uT("data"), Data.data() },
        { uT("count"), Data.size() },
        { uT("mapper"), Mapper },
      });

    Render = itCreator->second(pBuffer);
  }

  TestCallRender(Render);

  // ************** Передача данных в объекте компонента Data *************** //

  {
    // Передача данных через локальный объект, чтобы убедиться, что рендер будет
    // хранить копию данных.
    const auto Data = Source1;

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

    const auto pBuffer = Component_t::Make(
      { 
        { uT("mapper"), Mapper },
      });

    Render = itCreator->second(pBuffer);
  }

  TestCallRender(Render);
}

// ************************************************************************** //
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Buffer_Vertex_2D)
{
  using Vertex_t = ::covellite::api::Vertex;

  const ::std::vector<Vertex_t> Source =
  {
    {
      1808261932.0f, 1808261933.0f, 1908201953.0f, 1908201954.0f,
      1808261934.0f, 1808261935.0f, 
      1808261936.0f, 1908201955.0f, 1908201956.0f, 1908201957.0f
    },
    {
      1808261932.0f, 1808261933.0f, 1808261934.0f, 1808261935.0f, 
      1808261936.0f, 1908201958.0f,
      1908201959.0f, 1908202000.0f, 1908202001.0f, 1908202002.0f
    },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // Маркер конца данных
  };

  const ::mock::GLProxy::Floats_t ExpectedVertexData =
  {
    Source[0].px, Source[0].py,
    Source[1].px, Source[1].py,
  };

  const ::mock::GLProxy::Floats_t ExpectedTexCoordData =
  {
    Source[0].tu, Source[0].tv,
    Source[1].tu, Source[1].tv,
  };

  const ::mock::GLProxy::Floats_t ExpectedColorData =
  {
    Source[0].ex, Source[0].ey, Source[0].ez, Source[0].ew,
    Source[1].ex, Source[1].ey, Source[1].ez, Source[1].ew,
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

    EXPECT_CALL(GLProxy, ColorPointer(4, GL_FLOAT,
      sizeof(Vertex_t), ExpectedColorData))
      .Times(1);

    EXPECT_CALL(GLProxy, EnableClientState(GL_COLOR_ARRAY))
      .Times(1);

    EXPECT_CALL(GLProxy, DisableClientState(GL_NORMAL_ARRAY))
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
        { uT("dimension"), 2 },
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
        { uT("dimension"), 2 },
      });

    auto DataRender = itDataCreator->second(pData);
    EXPECT_EQ(nullptr, DataRender);

    const auto pBuffer = Component_t::Make({ });

    Render = itCreator->second(pBuffer);
  }

  TestCallRender(Render);
}

// ************************************************************************** //
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Buffer_Vertex_3D_Static)
{
  using Vertex_t = ::covellite::api::Vertex;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const ::std::vector<Vertex_t> Source =
  {
    {
      1808261932.0f, 1808261933.0f, 1812161256.0f, 1808261934.0f,
      1812161257.0f, 1808261935.0f,
      1808261936.0f, 1812161258.0f, 1908201939.0f, 1908201940.0f
    },
    {
      1808261932.0f, 1808261933.0f, 1812161259.0f, 1808261934.0f,
      1812161300.0f, 1808261935.0f,
      1808261936.0f, 1812161301.0f, 1908201941.0f, 1908201942.0f
    },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // Маркер конца данных
  };

  const ::mock::GLProxy::Floats_t ExpectedVertexData =
  {
    Source[0].px, Source[0].py, Source[0].pz,
    Source[1].px, Source[1].py, Source[1].pz,
  };

  const ::mock::GLProxy::Floats_t ExpectedTexCoordData =
  {
    Source[0].tu, Source[0].tv,
    Source[1].tu, Source[1].tv,
  };

  const ::mock::GLProxy::Floats_t ExpectedNormalData =
  {
    Source[0].ex, Source[0].ey, Source[0].ez,
    Source[1].ex, Source[1].ey, Source[1].ez,
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

    EXPECT_CALL(GLProxy, DisableClientState(GL_COLOR_ARRAY))
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
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Buffer_Vertex_3D_Dynamic)
{
  using Vertex_t = ::covellite::api::Vertex;
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<Vertex_t>;

  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(Mapper, bool(Vertex_t *));
  };

  Proxy oProxy;
  Proxy::GetInstance() = &oProxy;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const ::std::vector<Vertex_t> Source1 =
  {
    { 
      1808261932.0f, 1808261933.0f, 1812161256.0f, 1808261934.0f, 
      1812161257.0f, 1808261935.0f, 
      1808261936.0f, 1812161258.0f, 1908201945.0f, 1908201946.0f
    },
    {
      1808261932.0f, 1808261933.0f, 1812161259.0f, 1808261934.0f, 
      1812161300.0f, 1808261935.0f, 
      1808261936.0f, 1812161301.0f, 1908201947.0f, 1908201948.0f
    },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // Маркер конца данных
  };

  const ::mock::GLProxy::Floats_t ExpectedVertexData1 =
  {
    Source1[0].px, Source1[0].py, Source1[0].pz,
    Source1[1].px, Source1[1].py, Source1[1].pz,
  };

  const ::mock::GLProxy::Floats_t ExpectedTexCoordData1 =
  {
    Source1[0].tu, Source1[0].tv,
    Source1[1].tu, Source1[1].tv,
  };

  const ::mock::GLProxy::Floats_t ExpectedNormalData1 =
  {
    Source1[0].ex, Source1[0].ey, Source1[0].ez,
    Source1[1].ex, Source1[1].ey, Source1[1].ez,
  };

  const ::std::vector<Vertex_t> Source2 =
  {
    {
      1908011341.0f, 1908261933.0f, 1912161256.0f, 1908261934.0f, 
      1912161257.0f, 1908261935.0f, 
      1908261936.0f, 1912161258.0f, 1908201949.0f, 1908201950.0f
    },
    {
      1908261932.0f, 1908261933.0f, 1912161259.0f, 1908261934.0f, 
      1912161300.0f, 1908261935.0f, 
      1908261936.0f, 1912161301.0f, 1908201951.0f, 1908201952.0f
    },
    { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // Маркер конца данных
  };

  const ::mock::GLProxy::Floats_t ExpectedVertexData2 =
  {
    Source2[0].px, Source2[0].py, Source2[0].pz,
    Source2[1].px, Source2[1].py, Source2[1].pz,
  };

  const ::mock::GLProxy::Floats_t ExpectedTexCoordData2 =
  {
    Source2[0].tu, Source2[0].tv,
    Source2[1].tu, Source2[1].tv,
  };

  const ::mock::GLProxy::Floats_t ExpectedNormalData2 =
  {
    Source2[0].ex, Source2[0].ey, Source2[0].ez,
    Source2[1].ex, Source2[1].ey, Source2[1].ez,
  };

  const BufferMapper_t Mapper = [&](Vertex_t * _pData)
  {
    if (_pData != nullptr)
    {
      memcpy(_pData, Source2.data(), sizeof(Vertex_t) * (Source2.size() - 1));
    }

    return oProxy.Mapper(_pData);
  };

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto TestCallRender = [&](Render_t & _Render)
  {
    ASSERT_NE(nullptr, _Render);

    using namespace ::testing;

    InSequence Dummy;

    {
      EXPECT_CALL(oProxy, Mapper(nullptr))
        .Times(1)
        .WillOnce(Return(false));

      EXPECT_CALL(oProxy, Mapper(_))
        .Times(0);

      EXPECT_CALL(GLProxy, VertexPointer(3, GL_FLOAT,
        sizeof(Vertex_t), ExpectedVertexData1))
        .Times(1);

      EXPECT_CALL(GLProxy, EnableClientState(GL_VERTEX_ARRAY))
        .Times(1);

      EXPECT_CALL(GLProxy, NormalPointer(GL_FLOAT,
        sizeof(Vertex_t), ExpectedNormalData1))
        .Times(1);

      EXPECT_CALL(GLProxy, EnableClientState(GL_NORMAL_ARRAY))
        .Times(1);

      EXPECT_CALL(GLProxy, TexCoordPointer(2, GL_FLOAT,
        sizeof(Vertex_t), ExpectedTexCoordData1))
        .Times(1);

      EXPECT_CALL(GLProxy, EnableClientState(GL_TEXTURE_COORD_ARRAY))
        .Times(1);

      _Render();
    }

    {
      EXPECT_CALL(oProxy, Mapper(nullptr))
        .Times(1)
        .WillOnce(Return(true));

      EXPECT_CALL(oProxy, Mapper(_))
        .Times(1);

      EXPECT_CALL(GLProxy, VertexPointer(3, GL_FLOAT,
        sizeof(Vertex_t), ExpectedVertexData2))
        .Times(1);

      EXPECT_CALL(GLProxy, EnableClientState(GL_VERTEX_ARRAY))
        .Times(1);

      EXPECT_CALL(GLProxy, NormalPointer(GL_FLOAT,
        sizeof(Vertex_t), ExpectedNormalData2))
        .Times(1);

      EXPECT_CALL(GLProxy, EnableClientState(GL_NORMAL_ARRAY))
        .Times(1);

      EXPECT_CALL(GLProxy, TexCoordPointer(2, GL_FLOAT,
        sizeof(Vertex_t), ExpectedTexCoordData2))
        .Times(1);

      EXPECT_CALL(GLProxy, EnableClientState(GL_TEXTURE_COORD_ARRAY))
        .Times(1);

      _Render();
    }
  };

  auto itCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  Render_t Render;

  // ***************** Передача данных в объекте компонента ***************** //

  {
    // Передача данных через локальный объект, чтобы убедиться, что рендер будет
    // хранить копию данных.
    const auto Data = Source1;

    const auto pBuffer = Component_t::Make(
      {
        { uT("data"), Data.data() },
        { uT("count"), Data.size() },
        { uT("mapper"), Mapper },
      });

    Render = itCreator->second(pBuffer);
  }

  TestCallRender(Render);

  // ************** Передача данных в объекте компонента Data *************** //

  {
    // Передача данных через локальный объект, чтобы убедиться, что рендер будет
    // хранить копию данных.
    const auto Data = Source1;

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

    const auto pBuffer = Component_t::Make(
      {
        { uT("mapper"), Mapper },
      });

    Render = itCreator->second(pBuffer);
  }

  TestCallRender(Render);
}

// ************************************************************************** //
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Camera_Orthographic_DefaultPosition)
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

    EXPECT_CALL(GLProxy, Disable(GL_FOG))
      .Times(1);

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

    EXPECT_CALL(GLProxy, Enable(GL_TEXTURE_2D))
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
      Viewport[1] + Viewport[3], Viewport[1], 1, -1))
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
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Camera_Orthographic)
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

    EXPECT_CALL(GLProxy, Disable(GL_FOG))
      .Times(1);

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

    EXPECT_CALL(GLProxy, Enable(GL_TEXTURE_2D))
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
      1, -1))
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
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Camera_Perspective)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const float Width = 1024.0f;
  const float Height = 768.0f;
  const float Viewport[] = { 0, 0, Width, Height };

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

    const auto Projection = ::glm::perspectiveFovRH(AngleYRadians,
      Viewport[2], Viewport[3], _zFar, 0.01f);

    auto GetEye = [&](void) -> ::glm::vec3
    {
      const auto Distance = _Distance + 0.1f;

      ::glm::mat4 Transform = ::glm::identity<::glm::mat4>();

      Transform = ::glm::translate(Transform,
        ::glm::vec3{ _X, _Y, _Z });
      Transform = ::glm::rotate(Transform,
        _C, ::glm::vec3{ 0.0f, 0.0f, 1.0f });
      Transform = ::glm::rotate(Transform,
        _B, ::glm::vec3{ 0.0f, 1.0f, 0.0f });
      Transform = ::glm::rotate(Transform,
        _A, ::glm::vec3{ 1.0f, 0.0f, 0.0f });

      return Transform * ::glm::vec4{ Distance, 0.0f, 0.0f, 1.0f };
    };

    const auto View = ::glm::lookAtRH(
      GetEye(),
      ::glm::vec3{ _X, _Y, _Z },
      ::glm::vec3{ 0.0f, 0.0f, 1.0f });

    ASSERT_NE(nullptr, _Render);

    InSequence Dummy;

    EXPECT_CALL(GLProxy, Disable(GL_DEPTH_TEST))
      .Times(1);

    EXPECT_CALL(GLProxy, Enable(GL_DEPTH_TEST))
      .Times(0);

    EXPECT_CALL(GLProxy, Clear(GL_DEPTH_BUFFER_BIT))
      .Times(0);

    EXPECT_CALL(GLProxy, Disable(GL_FOG))
      .Times(1);

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

    EXPECT_CALL(GLProxy, Enable(GL_TEXTURE_2D))
      .Times(1);

    EXPECT_CALL(GLProxy, Enable(GL_CULL_FACE))
      .Times(1);

    EXPECT_CALL(GLProxy, CullFace(GL_BACK))
      .Times(1);

    EXPECT_CALL(GLProxy, FrontFace(GL_CCW))
      .Times(1);

    EXPECT_CALL(GLProxy, GetFloatv(GL_VIEWPORT))
      .Times(1)
      .WillOnce(Return(Viewport));

    EXPECT_CALL(GLProxy, MatrixMode(GL_PROJECTION))
      .Times(1);

    EXPECT_CALL(GLProxy, LoadMatrixf(Projection))
      .Times(1);

    EXPECT_CALL(GLProxy, MatrixMode(GL_MODELVIEW))
      .Times(1);

    EXPECT_CALL(GLProxy, LoadMatrixf(View))
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
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Fog)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itCreator = IExample.GetCreators().find(uT("Fog"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pData,
    const Component_t::ComponentPtr_t & _pFog,
    const int _Type, const uint32_t _Color, const float _Near, 
    const float _Far, const float _Density)
  {
    if (_pData != nullptr)
    {
      const auto Render = itDataCreator->second(_pData);
      EXPECT_EQ(nullptr, Render);
    }

    const auto Render = itCreator->second(_pFog);
    ASSERT_NE(nullptr, Render);

    for (int i = 0; i < 5; i++)
    {
      using namespace ::testing;

      InSequence Dummy;

      EXPECT_CALL(GLProxy, Enable(GL_FOG))
        .Times(1);

      EXPECT_CALL(GLProxy, Hint(GL_FOG_HINT, GL_NICEST))
        .Times(1);

      EXPECT_CALL(GLProxy, Fogfv(GL_FOG_COLOR, ARGBtoFloat4(_Color + i)))
        .Times(1);

      EXPECT_CALL(GLProxy, Fogi(GL_FOG_MODE, _Type))
        .Times(1);

      EXPECT_CALL(GLProxy, Fogf(GL_FOG_START, _Near + i))
        .Times(1);

      EXPECT_CALL(GLProxy, Fogf(GL_FOG_END, _Far + i))
        .Times(1);

      EXPECT_CALL(GLProxy, Fogf(GL_FOG_DENSITY, _Density + i))
        .Times(1);

      Render();

      const Component_t::ComponentPtr_t pData =
        (_pData != nullptr) ? _pData : _pFog;

      pData->SetValue(uT("color"), _Color + i + 1);
      pData->SetValue(uT("near"), _Near + i + 1);
      pData->SetValue(uT("far"), _Far + i + 1);
      pData->SetValue(uT("density"), _Density + i + 1);
    }
  };

  {
    const auto pFog = Component_t::Make(
      {
        { uT("style"), uT("unknown") },
      });

    EXPECT_THROW(itCreator->second(pFog), ::std::exception);
  }

  {
    const auto pDefaultFog = Component_t::Make({ });

    TestCall(nullptr, pDefaultFog, GL_LINEAR, 0xFFFFFFFF, 10.0f, 100.0f, 1.0f);
  }

  {
    const uint32_t Color = 0x19072916;
    const auto Near = 1907291728.0f;
    const auto Far = 1907291729.0f;

    const auto pFog = Component_t::Make(
      {
        { uT("style"), uT("linear") },
        { uT("color"), Color },
        { uT("near"), Near },
        { uT("far"), Far },
      });

    TestCall(nullptr, pFog, GL_LINEAR, Color, Near, Far, 1.0f);
  }

  {
    const uint32_t Color = 0x19073010;
    const auto Near = 1907301018.0f;
    const auto Far = 1907301019.0f;

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Fog") },
        { uT("color"), Color },
        { uT("near"), Near },
        { uT("far"), Far },
      });

    const auto pFog = Component_t::Make(
      {
        { uT("style"), uT("linear") },
      });

    TestCall(pData, pFog, GL_LINEAR, Color, Near, Far, 1.0f);
  }

  {
    const uint32_t Color = 0x19072917;
    const auto Density = 1907291800.0f;

    const auto pFog = Component_t::Make(
      {
        { uT("style"), uT("exp") },
        { uT("color"), Color },
        { uT("density"), Density },
      });

    TestCall(nullptr, pFog, GL_EXP, Color, 10.0f, 100.0f, Density);
  }

  {
    const uint32_t Color = 0x7301020;
    const auto Density = 1907301021.0f;

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Fog") },
        { uT("color"), Color },
        { uT("density"), Density },
      });

    const auto pFog = Component_t::Make(
      {
        { uT("style"), uT("exp") },
      });

    TestCall(pData, pFog, GL_EXP, Color, 10.0f, 100.0f, Density);
  }

  {
    const uint32_t Color = 0x19072918;
    const auto Density = 1907291802.0f;

    const auto pFog = Component_t::Make(
      {
        { uT("style"), uT("exp2") },
        { uT("color"), Color },
        { uT("density"), Density },
      });

    TestCall(nullptr, pFog, GL_EXP2, Color, 10.0f, 100.0f, Density);
  }

  {
    const uint32_t Color = 0x7301022;
    const auto Density = 1907301023.0f;

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Fog") },
        { uT("color"), Color },
        { uT("density"), Density },
      });

    const auto pFog = Component_t::Make(
      {
        { uT("style"), uT("exp2") },
      });

    TestCall(pData, pFog, GL_EXP2, Color, 10.0f, 100.0f, Density);
  }
}

// ************************************************************************** //
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Shader)
{
  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto Render = itCreator->second(Component_t::Make({ }));
  EXPECT_EQ(nullptr, Render);
}

// ************************************************************************** //
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Transform_UnknownKind)
{
  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Transform"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Unknow1908061947") },
    });

  EXPECT_THROW(itCreator->second(pComponent), ::std::exception);
}

// ************************************************************************** //
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Transform_Default)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itCreator = IExample.GetCreators().find(uT("Transform"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto pPosition = Component_t::Make({ { uT("kind"), uT("Position") } });
  auto pRotation = Component_t::Make({ { uT("kind"), uT("Rotation") } });
  auto pScale = Component_t::Make({ { uT("kind"), uT("Scale") } });

  auto TestCallRender = [&](Render_t & _Render,
    float _X, float _Y, float _Z,
    float _A, float _B, float _C,
    float _Sx, float _Sy, float _Sz)
  {
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

    const auto ViewMatrix = ::glm::lookAtRH(
      ::glm::vec3{ 1.0f, 2.0f, 3.0f },
      ::glm::vec3{ 4.0f, 5.0f, 6.0f },
      ::glm::vec3{ 0.0f, 0.0f, 1.0f });

    auto WorldMatrix = ViewMatrix;
    WorldMatrix = ::glm::scale(WorldMatrix, { _Sx, _Sy, _Sz });
    WorldMatrix = ::glm::rotate(WorldMatrix, _C, { 0.0f, 0.0f, 1.0f });
    WorldMatrix = ::glm::rotate(WorldMatrix, _B, { 0.0f, 1.0f, 0.0f });
    WorldMatrix = ::glm::rotate(WorldMatrix, _A, { 1.0f, 0.0f, 0.0f });
    WorldMatrix = ::glm::translate(WorldMatrix, { _X, _Y, _Z });

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, PushMatrix())
      .Times(1);

    EXPECT_CALL(GLProxy, GetFloatv(GL_MODELVIEW_MATRIX))
      .Times(1)
      .WillOnce(Return(::glm::value_ptr(ViewMatrix)));

    EXPECT_CALL(GLProxy, LoadMatrixf(WorldMatrix))
      .Times(1);

    _Render();
  };

  auto PositionRender = itDataCreator->second(pPosition);
  EXPECT_EQ(nullptr, PositionRender);

  auto RotationRender = itDataCreator->second(pRotation);
  EXPECT_EQ(nullptr, RotationRender);

  auto ScaleRender = itDataCreator->second(pScale);
  EXPECT_EQ(nullptr, ScaleRender);

  auto Render = itCreator->second(Component_t::Make({ }));

  // Два вызова, чтобы убедиться, что изменение исходных данных приводит 
  // к изменению результата рендеринга.

  TestCallRender(Render,
    1956.0f, 1957.0f, 1958.0f,
    1204.0f, 1205.0f, 1206.0f,
    1152.0f, 1153.0f, 1154.0f);

  TestCallRender(Render,
    1959.0f, 1960.0f, 1961.0f,
    1145.0f, 1146.0f, 1147.0f,
    1155.0f, 1157.0f, 1158.0f);
}

// ************************************************************************** //
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Transform_Static)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itCreator = IExample.GetCreators().find(uT("Transform"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto pPosition = Component_t::Make({ { uT("kind"), uT("Position") } });
  auto pRotation = Component_t::Make({ { uT("kind"), uT("Rotation") } });
  auto pScale = Component_t::Make({ { uT("kind"), uT("Scale") } });

  Render_t Render;
  ::glm::mat4 LastWorldMatrix;

  auto TestCallRender = [&](
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

      Render = itCreator->second(Component_t::Make(
        {
          { uT("kind"), uT("Static") },
        }));
    }

    ASSERT_NE(nullptr, Render);

    const auto ViewMatrix = ::glm::lookAtRH(
      ::glm::vec3{ 1.0f, 2.0f, 3.0f },
      ::glm::vec3{ 4.0f, 5.0f, 6.0f },
      ::glm::vec3{ 0.0f, 0.0f, 1.0f });

    if (!_IsExpectLastMatrix)
    {
      auto WorldMatrix = ::glm::identity<::glm::mat4>();
      WorldMatrix = ::glm::scale(WorldMatrix, { _Sx, _Sy, _Sz });
      WorldMatrix = ::glm::rotate(WorldMatrix, _C, { 0.0f, 0.0f, 1.0f });
      WorldMatrix = ::glm::rotate(WorldMatrix, _B, { 0.0f, 1.0f, 0.0f });
      WorldMatrix = ::glm::rotate(WorldMatrix, _A, { 1.0f, 0.0f, 0.0f });
      WorldMatrix = ::glm::translate(WorldMatrix, { _X, _Y, _Z });

      LastWorldMatrix = ViewMatrix * WorldMatrix;
    }

    const auto RawWorldMatrix = LastWorldMatrix;

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, PushMatrix())
      .Times(1);

    EXPECT_CALL(GLProxy, GetFloatv(GL_MODELVIEW_MATRIX))
      .Times(1)
      .WillOnce(Return(::glm::value_ptr(ViewMatrix)));

    EXPECT_CALL(GLProxy, LoadMatrixf(RawWorldMatrix))
      .Times(1);

    Render();
  };

  // Два вызова, чтобы убедиться, что изменение исходных данных не приводит 
  // к изменению результата рендеринга.

  TestCallRender(
    1956.0f, 1957.0f, 1958.0f,
    1204.0f, 1205.0f, 1206.0f,
    1152.0f, 1153.0f, 1154.0f,
    false);

  TestCallRender(
    1959.0f, 1960.0f, 1961.0f,
    1145.0f, 1146.0f, 1147.0f,
    1155.0f, 1157.0f, 1158.0f,
    true);
}

// ************************************************************************** //
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Transform_Billboard)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itCreator = IExample.GetCreators().find(uT("Transform"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto pPosition = Component_t::Make({ { uT("kind"), uT("Position") } });

  Render_t Render;

  auto WorldMatrix = ::glm::identity<::glm::mat4>();

  auto TestCallRender = [&](
    float _X, float _Y, float _Z, const bool _IsCreateRender)
  {
    pPosition->SetValue(uT("x"), _X);
    pPosition->SetValue(uT("y"), _Y);
    pPosition->SetValue(uT("z"), _Z);

    if (_IsCreateRender)
    {
      auto PositionRender = itDataCreator->second(pPosition);
      EXPECT_EQ(nullptr, PositionRender);

      Render = itCreator->second(Component_t::Make(
        {
          { uT("kind"), uT("Billboard") },
        }));
    }

    ASSERT_NE(nullptr, Render);

    WorldMatrix = ::glm::identity<::glm::mat4>();
    WorldMatrix = ::glm::translate(WorldMatrix, { _X, _Y, _Z });

    const auto ViewMatrix = ::glm::mat4
    {
      { 1.0f, 2.0f, 3.0f, 4.0f },
      { 5.0f, 6.0f, 7.0f, 8.0f },
      { 9.0f, 10.0f, 11.0f, 12.0f },
      { 13.0f, 14.0f, 15.0f, 16.0f },
    };

    auto TransposeViewMatrix = ::glm::transpose(ViewMatrix);
    TransposeViewMatrix[0][3] = 0.0f;
    TransposeViewMatrix[1][3] = 0.0f;
    TransposeViewMatrix[2][3] = 0.0f;
    TransposeViewMatrix[3][0] = 0.0f;
    TransposeViewMatrix[3][1] = 0.0f;
    TransposeViewMatrix[3][2] = 0.0f;
    TransposeViewMatrix[3][3] = 1.0f;

    WorldMatrix = ViewMatrix * TransposeViewMatrix *
      ::glm::translate(::glm::identity<::glm::mat4>(), { _X, _Y, _Z });

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, PushMatrix())
      .Times(1);

    EXPECT_CALL(GLProxy, GetFloatv(GL_MODELVIEW_MATRIX))
      .Times(1)
      .WillOnce(Return(::glm::value_ptr(ViewMatrix)));

    EXPECT_CALL(GLProxy, LoadMatrixf(WorldMatrix))
      .Times(1);

    Render();
  };

  // Два вызова, чтобы убедиться, что изменение исходных данных не приводит 
  // к изменению результата рендеринга.

  TestCallRender(1956.0f, 1957.0f, 1958.0f,
    true);

  TestCallRender(1959.0f, 1960.0f, 1961.0f,
    false);
}

// ************************************************************************** //
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Transform_DefaultValues)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itCreator = IExample.GetCreators().find(uT("Transform"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto pPosition = Component_t::Make({ { uT("kind"), uT("Position") } });
  auto pRotation = Component_t::Make({ { uT("kind"), uT("Rotation") } });
  auto pScale = Component_t::Make({ { uT("kind"), uT("Scale") } });

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pComponent,
    const bool _IsFullTransform = true)
  {
    auto PositionRender = itDataCreator->second(pPosition);
    EXPECT_EQ(nullptr, PositionRender);

    if (_IsFullTransform)
    {
      auto RotationRender = itDataCreator->second(pRotation);
      EXPECT_EQ(nullptr, RotationRender);

      auto ScaleRender = itDataCreator->second(pScale);
      EXPECT_EQ(nullptr, ScaleRender);
    }

    const auto Render = itCreator->second(_pComponent);
    ASSERT_NE(nullptr, Render);

    const auto ViewMatrix = ::glm::mat4
    {
      { 1.0f, 2.0f, 3.0f, 4.0f },
      { 5.0f, 6.0f, 7.0f, 8.0f },
      { 9.0f, 10.0f, 11.0f, 12.0f },
      { 13.0f, 14.0f, 15.0f, 16.0f },
    };

    auto WorldMatrix = ::glm::identity<::glm::mat4>();

    if (_IsFullTransform)
    {
      WorldMatrix = ::glm::scale(WorldMatrix, { 1.0f, 1.0f, 1.0f });
    }
    else
    {
      auto TransposeViewMatrix = ::glm::transpose(ViewMatrix);
      TransposeViewMatrix[0][3] = 0.0f;
      TransposeViewMatrix[1][3] = 0.0f;
      TransposeViewMatrix[2][3] = 0.0f;
      TransposeViewMatrix[3][0] = 0.0f;
      TransposeViewMatrix[3][1] = 0.0f;
      TransposeViewMatrix[3][2] = 0.0f;
      TransposeViewMatrix[3][3] = 1.0f;

      WorldMatrix = TransposeViewMatrix;
    }

    const auto RawWorldMatrix = ViewMatrix * WorldMatrix;

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GetFloatv(GL_MODELVIEW_MATRIX))
      .Times(1)
      .WillOnce(Return(::glm::value_ptr(ViewMatrix)));

    EXPECT_CALL(GLProxy, LoadMatrixf(RawWorldMatrix))
      .Times(1);

    Render();
  };

  TestCallRender(Component_t::Make({ }));

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Static") },
    }));

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Billboard") },
    }), false);
}

// ************************************************************************** //
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Transform_Combine)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itCreator = IExample.GetCreators().find(uT("Transform"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto MatrixIdentity = ::glm::identity<::glm::mat4>();

  ::std::deque<Component_t::ComponentPtr_t> Components;

  const auto SetPosition = [&](float _X, float _Y, float _Z)
  {
    Components.push_front(Component_t::Make(
      {
        { uT("kind"), uT("Position") },
        { uT("x"), _X },
        { uT("y"), _Y },
        { uT("z"), _Z },
      }));

    MatrixIdentity = ::glm::translate(MatrixIdentity, { _X, _Y, _Z });
  };

  const auto SetRotation = [&](float _X, float _Y, float _Z)
  {
    Components.push_front(Component_t::Make(
      {
        { uT("kind"), uT("Rotation") },
        { uT("x"), _X },
        { uT("y"), _Y },
        { uT("z"), _Z },
      }));

    MatrixIdentity = ::glm::rotate(MatrixIdentity, _Z, { 0.0f, 0.0f, 1.0f });
    MatrixIdentity = ::glm::rotate(MatrixIdentity, _Y, { 0.0f, 1.0f, 0.0f });
    MatrixIdentity = ::glm::rotate(MatrixIdentity, _X, { 1.0f, 0.0f, 0.0f });
  };

  const auto SetScale = [&](float _X, float _Y, float _Z)
  {
    Components.push_front(Component_t::Make(
      {
        { uT("kind"), uT("Scale") },
        { uT("x"), _X },
        { uT("y"), _Y },
        { uT("z"), _Z },
      }));

    MatrixIdentity = ::glm::scale(MatrixIdentity, { _X, _Y, _Z });
  };

  {
    MatrixIdentity = ::glm::identity<::glm::mat4>();
    const auto ViewMatrix = MatrixIdentity;
    Components.clear();

    SetPosition(1.0f, 2.0f, 3.0f);
    SetRotation(4.0f, 5.0f, 6.0f);
    SetScale(7.0f, 8.0f, 9.0f);

    SetPosition(11.0f, 22.0f, 33.0f);
    SetRotation(44.0f, 55.0f, 66.0f);
    SetScale(77.0f, 88.0f, 99.0f);

    for (const auto & pComponent : Components)
    {
      auto ScaleRender = itDataCreator->second(pComponent);
      EXPECT_EQ(nullptr, ScaleRender);
    }

    auto Render = itCreator->second(Component_t::Make({ }));
    ASSERT_NE(nullptr, Render);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GetFloatv(GL_MODELVIEW_MATRIX))
      .Times(1)
      .WillOnce(Return(::glm::value_ptr(ViewMatrix)));

    EXPECT_CALL(GLProxy, LoadMatrixf(MatrixIdentity))
      .Times(1);

    Render();
  }

  {
    Components.clear();
    MatrixIdentity = ::glm::identity<::glm::mat4>();
    const auto ViewMatrix = MatrixIdentity;

    SetPosition(1.0f, 2.0f, 3.0f);
    SetRotation(4.0f, 5.0f, 6.0f);
    SetScale(7.0f, 8.0f, 9.0f);

    SetPosition(11.0f, 22.0f, 33.0f);
    SetRotation(44.0f, 55.0f, 66.0f);
    SetScale(77.0f, 88.0f, 99.0f);

    for (const auto & pComponent : Components)
    {
      auto ScaleRender = itDataCreator->second(pComponent);
      EXPECT_EQ(nullptr, ScaleRender);
    }

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Static") },
      }));
    ASSERT_NE(nullptr, Render);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GetFloatv(GL_MODELVIEW_MATRIX))
      .Times(1)
      .WillOnce(Return(::glm::value_ptr(ViewMatrix)));

    EXPECT_CALL(GLProxy, LoadMatrixf(MatrixIdentity))
      .Times(1);

    Render();
  }

  {
    Components.clear();
    MatrixIdentity = ::glm::identity<::glm::mat4>();

    const auto ViewMatrix = ::glm::mat4
    {
      { 1.0f, 2.0f, 3.0f, 4.0f },
      { 5.0f, 6.0f, 7.0f, 8.0f },
      { 9.0f, 10.0f, 11.0f, 12.0f },
      { 13.0f, 14.0f, 15.0f, 16.0f },
    };

    auto TransposeViewMatrix = ::glm::transpose(ViewMatrix);
    TransposeViewMatrix[0][3] = 0.0f;
    TransposeViewMatrix[1][3] = 0.0f;
    TransposeViewMatrix[2][3] = 0.0f;
    TransposeViewMatrix[3][0] = 0.0f;
    TransposeViewMatrix[3][1] = 0.0f;
    TransposeViewMatrix[3][2] = 0.0f;
    TransposeViewMatrix[3][3] = 1.0f;

    SetPosition(1.0f, 2.0f, 3.0f);
    SetPosition(11.0f, 22.0f, 33.0f);

    for (const auto & pComponent : Components)
    {
      auto ScaleRender = itDataCreator->second(pComponent);
      EXPECT_EQ(nullptr, ScaleRender);
    }

    const auto RawWorldMatrix =
      ViewMatrix * TransposeViewMatrix * MatrixIdentity;

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Billboard") },
      }));
    ASSERT_NE(nullptr, Render);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GetFloatv(GL_MODELVIEW_MATRIX))
      .Times(1)
      .WillOnce(Return(::glm::value_ptr(ViewMatrix)));

    EXPECT_CALL(GLProxy, LoadMatrixf(RawWorldMatrix))
      .Times(1);

    Render();
  }
}

// ************************************************************************** //
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Buffer_Constant_ObjectLights_NoMapper)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<::Lights_t>;

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  // Тип буфера выводится из переданного mapper'a
  //EXPECT_THROW(itCreator->second(Component_t::Make({ })), ::std::exception);

  const auto pComponent1 = Component_t::Make(
    {
      { uT("mapper"), BufferMapper_t{ } },
    });

  EXPECT_THROW(itCreator->second(pComponent1), ::std::exception);

  const auto pComponent2 = Component_t::Make(
    {
      { uT("mapper"), BufferMapper_t{ nullptr } },
    });

  EXPECT_THROW(itCreator->second(pComponent2), ::std::exception);
}

// ************************************************************************** //
TEST_F(OpenGLStatic_test, DISABLED_Test_Buffer_Constant_ObjectLights)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<::Lights_t>;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const ::mock::GLint ProgramId = 1908261929;
  const ::mock::GLint IsValidLocationId = 1908261935;
  const ::mock::GLint ColorLocationId = 1908261936;

  Lights_t Lights;
  memset(&Lights, 0x00, sizeof(Lights));

  Lights.Ambient.IsValid = 1909161807;
  Lights.Ambient.Color = color_t{ 0.1f, 9.09f, 1.61f, 8.08f };

  Lights.Direction.IsValid = 1909161809;
  Lights.Direction.Color = color_t{ 1.9f, 0.91f, 6.18f, 1.1f };
  Lights.Direction.Direction = float4{ 19.0f, 0.9f, 16.1f, 812.0f };

  Lights.Points.UsedSlotCount = COVELLITE_MAX_LIGHT_POINT_OBJECT_COUNT;

  for (int i = 0; i < Lights.Points.UsedSlotCount; i++)
  {
    Lights.Points.Lights[i].Color = color_t{ 1.9f, 0.91f, 6.18f, 1.4f };
    Lights.Points.Lights[i].Position = float4{ 1.9f, 0.91f, 6.18f, 1.5f };
    Lights.Points.Lights[i].Attenuation = float4{ 1.9f, 0.91f, 6.18f, 1.6f };
  }

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itConstBufferCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itConstBufferCreator);

  const BufferMapper_t BufferMapper = [&](Lights_t * _pLights)
  {
    *_pLights = Lights;
    return false;
  };

  auto ConstBufferRender = itConstBufferCreator->second(Component_t::Make(
    {
      { uT("mapper"), BufferMapper },
    }));
  ASSERT_NE(nullptr, ConstBufferRender);

  using namespace ::testing;

  InSequence Dummy;

  // ...

  ConstBufferRender(); // Вызывается BufferMapper
}

// ************************************************************************** //
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Present_Index)
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

  auto TestCallRender = [&](Render_t & _Render)
  {
    ASSERT_NE(nullptr, _Render);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, DrawElements(GL_TRIANGLES, (int)Indices.size(),
      GL_UNSIGNED_INT, Indices))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, PopMatrix())
      .Times(1);

    _Render();
  };

  auto itPresentCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itPresentCreator);

  {
    // Индексный буфер в компоненте

    Render_t Render;

    {
      // Передача локальной копии, чтобы убедиться, что рендер копирует
      // переданные ему данные.
      const auto IndicesCopy = Indices;

      Render = itPresentCreator->second(Component_t::Make(
        {
            { uT("data"), IndicesCopy.data() },
            { uT("count"), IndicesCopy.size() },
        }));
    }

    TestCallRender(Render);
  }

  {
    // Индексный буфер через компонент Data

    Render_t Render;

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

      Render = itPresentCreator->second(Component_t::Make({ }));
    }

    TestCallRender(Render);
  }
}


////////////////////////////////////////////////////////////////////////////////


// ************************************************************************** //
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Present_Geometry_deprecated)
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

    const auto ViewMatrix = ::glm::lookAtRH(
      ::glm::vec3{ 1.0f, 2.0f, 3.0f },
      ::glm::vec3{ 4.0f, 5.0f, 6.0f },
      ::glm::vec3{ 0.0f, 0.0f, 1.0f });

    auto WorldMatrix = ViewMatrix;
    WorldMatrix = ::glm::scale(WorldMatrix, { _Sx, _Sy, _Sz });
    WorldMatrix = ::glm::rotate(WorldMatrix, _C, { 0.0f, 0.0f, 1.0f });
    WorldMatrix = ::glm::rotate(WorldMatrix, _B, { 0.0f, 1.0f, 0.0f });
    WorldMatrix = ::glm::rotate(WorldMatrix, _A, { 1.0f, 0.0f, 0.0f });
    WorldMatrix = ::glm::translate(WorldMatrix, { _X, _Y, _Z });

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, PushMatrix())
      .Times(1);

    EXPECT_CALL(GLProxy, GetFloatv(GL_MODELVIEW_MATRIX))
      .Times(1)
      .WillOnce(Return(::glm::value_ptr(ViewMatrix)));

    EXPECT_CALL(GLProxy, LoadMatrixf(WorldMatrix))
      .Times(1);

    EXPECT_CALL(GLProxy, DrawElements(GL_TRIANGLES, (int)Indices.size(),
      GL_UNSIGNED_INT, Indices))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
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
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Present_Geometry_Static_deprectaed)
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
  ::glm::mat4 LastWorldMatrix;

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
          { uT("variety"), uT("Static") },
        }));
    }

    ASSERT_NE(nullptr, _IndexBufferRender);
    ASSERT_NE(nullptr, Render);

    const auto ViewMatrix = ::glm::lookAtRH(
      ::glm::vec3{ 1.0f, 2.0f, 3.0f },
      ::glm::vec3{ 4.0f, 5.0f, 6.0f },
      ::glm::vec3{ 0.0f, 0.0f, 1.0f });

    if (!_IsExpectLastMatrix)
    {
      auto WorldMatrix = ::glm::identity<::glm::mat4>();
      WorldMatrix = ::glm::scale(WorldMatrix, { _Sx, _Sy, _Sz });
      WorldMatrix = ::glm::rotate(WorldMatrix, _C, { 0.0f, 0.0f, 1.0f });
      WorldMatrix = ::glm::rotate(WorldMatrix, _B, { 0.0f, 1.0f, 0.0f });
      WorldMatrix = ::glm::rotate(WorldMatrix, _A, { 1.0f, 0.0f, 0.0f });
      WorldMatrix = ::glm::translate(WorldMatrix, { _X, _Y, _Z });

      LastWorldMatrix = ViewMatrix * WorldMatrix;
    }

    const auto RawWorldMatrix = LastWorldMatrix;

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, PushMatrix())
      .Times(1);

    EXPECT_CALL(GLProxy, GetFloatv(GL_MODELVIEW_MATRIX))
      .Times(1)
      .WillOnce(Return(::glm::value_ptr(ViewMatrix)));

    EXPECT_CALL(GLProxy, LoadMatrixf(RawWorldMatrix))
      .Times(1);

    EXPECT_CALL(GLProxy, DrawElements(GL_TRIANGLES, (int)Indices.size(),
      GL_UNSIGNED_INT, Indices))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
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
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Present_Geometry_Billboard_deprecated)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  const ::std::vector<int> Indices =
  {
    1908071155,
    1908071156,
    1908071157,
    1908071158,
    1908071159
  };

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itBufferCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itBufferCreator);

  auto itPresentCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itPresentCreator);

  auto pPosition = Component_t::Make({ { uT("kind"), uT("Position") } });

  Render_t Render;

  auto WorldMatrix = ::glm::identity<::glm::mat4>();

  auto TestCallRender = [&](Render_t & _IndexBufferRender,
    float _X, float _Y, float _Z, const bool _IsCreateRender)
  {
    pPosition->SetValue(uT("x"), _X);
    pPosition->SetValue(uT("y"), _Y);
    pPosition->SetValue(uT("z"), _Z);

    if (_IsCreateRender)
    {
      auto PositionRender = itDataCreator->second(pPosition);
      EXPECT_EQ(nullptr, PositionRender);

      Render = itPresentCreator->second(Component_t::Make(
        {
          { uT("kind"), uT("Geometry") },
          { uT("variety"), uT("Billboard") },
        }));
    }

    ASSERT_NE(nullptr, _IndexBufferRender);
    ASSERT_NE(nullptr, Render);

    WorldMatrix = ::glm::identity<::glm::mat4>();
    WorldMatrix = ::glm::translate(WorldMatrix, { _X, _Y, _Z });

    const auto ViewMatrix = ::glm::mat4
    {
      { 1.0f, 2.0f, 3.0f, 4.0f },
      { 5.0f, 6.0f, 7.0f, 8.0f },
      { 9.0f, 10.0f, 11.0f, 12.0f },
      { 13.0f, 14.0f, 15.0f, 16.0f },
    };

    auto TransposeViewMatrix = ::glm::transpose(ViewMatrix);
    TransposeViewMatrix[0][3] = 0.0f;
    TransposeViewMatrix[1][3] = 0.0f;
    TransposeViewMatrix[2][3] = 0.0f;
    TransposeViewMatrix[3][0] = 0.0f;
    TransposeViewMatrix[3][1] = 0.0f;
    TransposeViewMatrix[3][2] = 0.0f;
    TransposeViewMatrix[3][3] = 1.0f;

    WorldMatrix = ViewMatrix * TransposeViewMatrix *
      ::glm::translate(::glm::identity<::glm::mat4>(), { _X, _Y, _Z });

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, PushMatrix())
      .Times(1);

    EXPECT_CALL(GLProxy, GetFloatv(GL_MODELVIEW_MATRIX))
      .Times(1)
      .WillOnce(Return(::glm::value_ptr(ViewMatrix)));

    EXPECT_CALL(GLProxy, LoadMatrixf(WorldMatrix))
      .Times(1);

    EXPECT_CALL(GLProxy, DrawElements(GL_TRIANGLES, (int)Indices.size(),
      GL_UNSIGNED_INT, Indices))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
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
      true);

    TestCallRender(IndexBufferRender,
      1959.0f, 1960.0f, 1961.0f,
      false);
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
      true);

    TestCallRender(IndexBufferRender,
      1959.0f, 1960.0f, 1961.0f,
      false);
  }
}

// ************************************************************************** //
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Present_Geometry_DefaultTransformValues_deprecated)
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

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pComponent,
    const bool _IsFullTransform = true)
  {
    auto PositionRender = itDataCreator->second(pPosition);
    EXPECT_EQ(nullptr, PositionRender);

    if (_IsFullTransform)
    {
      auto RotationRender = itDataCreator->second(pRotation);
      EXPECT_EQ(nullptr, RotationRender);

      auto ScaleRender = itDataCreator->second(pScale);
      EXPECT_EQ(nullptr, ScaleRender);
    }

    const auto Render = itPresentCreator->second(_pComponent);
    ASSERT_NE(nullptr, Render);

    const auto ViewMatrix = ::glm::mat4
    {
      { 1.0f, 2.0f, 3.0f, 4.0f },
      { 5.0f, 6.0f, 7.0f, 8.0f },
      { 9.0f, 10.0f, 11.0f, 12.0f },
      { 13.0f, 14.0f, 15.0f, 16.0f },
    };

    auto WorldMatrix = ::glm::identity<::glm::mat4>();

    if (_IsFullTransform)
    {
      WorldMatrix = ::glm::scale(WorldMatrix, { 1.0f, 1.0f, 1.0f });
    }
    else
    {
      auto TransposeViewMatrix = ::glm::transpose(ViewMatrix);
      TransposeViewMatrix[0][3] = 0.0f;
      TransposeViewMatrix[1][3] = 0.0f;
      TransposeViewMatrix[2][3] = 0.0f;
      TransposeViewMatrix[3][0] = 0.0f;
      TransposeViewMatrix[3][1] = 0.0f;
      TransposeViewMatrix[3][2] = 0.0f;
      TransposeViewMatrix[3][3] = 1.0f;

      WorldMatrix = TransposeViewMatrix;
    }

    const auto RawWorldMatrix = ViewMatrix * WorldMatrix;

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GetFloatv(GL_MODELVIEW_MATRIX))
      .Times(1)
      .WillOnce(Return(::glm::value_ptr(ViewMatrix)));

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
      { uT("variety"), uT("Static") },
    }));

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Geometry") },
      { uT("variety"), uT("Billboard") },
    }), false);
}

// ************************************************************************** //
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Present_Geometry_CombineTransform_deprecated)
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

  auto MatrixIdentity = ::glm::identity<::glm::mat4>();

  ::std::deque<Component_t::ComponentPtr_t> Components;

  const auto SetPosition = [&](float _X, float _Y, float _Z)
  {
    Components.push_front(Component_t::Make(
      {
        { uT("kind"), uT("Position") },
        { uT("x"), _X },
        { uT("y"), _Y },
        { uT("z"), _Z },
      }));

    MatrixIdentity = ::glm::translate(MatrixIdentity, { _X, _Y, _Z });
  };

  const auto SetRotation = [&](float _X, float _Y, float _Z)
  {
    Components.push_front(Component_t::Make(
      {
        { uT("kind"), uT("Rotation") },
        { uT("x"), _X },
        { uT("y"), _Y },
        { uT("z"), _Z },
      }));

    MatrixIdentity = ::glm::rotate(MatrixIdentity, _Z, { 0.0f, 0.0f, 1.0f });
    MatrixIdentity = ::glm::rotate(MatrixIdentity, _Y, { 0.0f, 1.0f, 0.0f });
    MatrixIdentity = ::glm::rotate(MatrixIdentity, _X, { 1.0f, 0.0f, 0.0f });
  };

  const auto SetScale = [&](float _X, float _Y, float _Z)
  {
    Components.push_front(Component_t::Make(
      {
        { uT("kind"), uT("Scale") },
        { uT("x"), _X },
        { uT("y"), _Y },
        { uT("z"), _Z },
      }));

    MatrixIdentity = ::glm::scale(MatrixIdentity, { _X, _Y, _Z });
  };

  {
    MatrixIdentity = ::glm::identity<::glm::mat4>();
    const auto ViewMatrix = MatrixIdentity;
    Components.clear();

    SetPosition(1.0f, 2.0f, 3.0f);
    SetRotation(4.0f, 5.0f, 6.0f);
    SetScale(7.0f, 8.0f, 9.0f);

    SetPosition(11.0f, 22.0f, 33.0f);
    SetRotation(44.0f, 55.0f, 66.0f);
    SetScale(77.0f, 88.0f, 99.0f);

    for (const auto & pComponent : Components)
    {
      auto ScaleRender = itDataCreator->second(pComponent);
      EXPECT_EQ(nullptr, ScaleRender);
    }

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Geometry") }
      }));
    ASSERT_NE(nullptr, Render);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GetFloatv(GL_MODELVIEW_MATRIX))
      .Times(1)
      .WillOnce(Return(::glm::value_ptr(ViewMatrix)));

    EXPECT_CALL(GLProxy, LoadMatrixf(MatrixIdentity))
      .Times(1);

    Render();
  }

  {
    Components.clear();
    MatrixIdentity = ::glm::identity<::glm::mat4>();
    const auto ViewMatrix = MatrixIdentity;

    SetPosition(1.0f, 2.0f, 3.0f);
    SetRotation(4.0f, 5.0f, 6.0f);
    SetScale(7.0f, 8.0f, 9.0f);

    SetPosition(11.0f, 22.0f, 33.0f);
    SetRotation(44.0f, 55.0f, 66.0f);
    SetScale(77.0f, 88.0f, 99.0f);

    for (const auto & pComponent : Components)
    {
      auto ScaleRender = itDataCreator->second(pComponent);
      EXPECT_EQ(nullptr, ScaleRender);
    }

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Geometry") },
        { uT("variety"), uT("Static") },
      }));
    ASSERT_NE(nullptr, Render);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GetFloatv(GL_MODELVIEW_MATRIX))
      .Times(1)
      .WillOnce(Return(::glm::value_ptr(ViewMatrix)));

    EXPECT_CALL(GLProxy, LoadMatrixf(MatrixIdentity))
      .Times(1);

    Render();
  }

  {
    Components.clear();
    MatrixIdentity = ::glm::identity<::glm::mat4>();

    const auto ViewMatrix = ::glm::mat4
    {
      { 1.0f, 2.0f, 3.0f, 4.0f },
      { 5.0f, 6.0f, 7.0f, 8.0f },
      { 9.0f, 10.0f, 11.0f, 12.0f },
      { 13.0f, 14.0f, 15.0f, 16.0f },
    };

    auto TransposeViewMatrix = ::glm::transpose(ViewMatrix);
    TransposeViewMatrix[0][3] = 0.0f;
    TransposeViewMatrix[1][3] = 0.0f;
    TransposeViewMatrix[2][3] = 0.0f;
    TransposeViewMatrix[3][0] = 0.0f;
    TransposeViewMatrix[3][1] = 0.0f;
    TransposeViewMatrix[3][2] = 0.0f;
    TransposeViewMatrix[3][3] = 1.0f;

    SetPosition(1.0f, 2.0f, 3.0f);
    SetPosition(11.0f, 22.0f, 33.0f);

    for (const auto & pComponent : Components)
    {
      auto ScaleRender = itDataCreator->second(pComponent);
      EXPECT_EQ(nullptr, ScaleRender);
    }

    const auto RawWorldMatrix =
      ViewMatrix * TransposeViewMatrix * MatrixIdentity;

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Geometry") },
        { uT("variety"), uT("Billboard") },
      }));
    ASSERT_NE(nullptr, Render);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GetFloatv(GL_MODELVIEW_MATRIX))
      .Times(1)
      .WillOnce(Return(::glm::value_ptr(ViewMatrix)));

    EXPECT_CALL(GLProxy, LoadMatrixf(RawWorldMatrix))
      .Times(1);

    Render();
  }
}

// ************************************************************************** //
TEST_F(OpenGLStatic_test, /*DISABLED_*/Test_Light_ComplexUsing_deprecated)
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
      TestDeleteLights(COVELLITE_MAX_LIGHT_POINT_SCENE_COUNT - _Lights.size(), true);
    }
    else
    {
      TestDeleteLights(COVELLITE_MAX_LIGHT_POINT_SCENE_COUNT / 2, false);
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

  const ::std::vector<Light> DefaultLights =
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

    TestUsingLights(pCamera1, pCamera2, true, DefaultLights, true);
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

    TestUsingLights(pCamera1, pCamera2, true, DefaultLights, true);
    TestUsingLights(pCamera1, pCamera2, false, LightAmbient);
    TestUsingLights(pCamera1, pCamera2, false, LightDirection);
    TestUsingLights(pCamera1, pCamera2, false, LightPoint);
    TestUsingLights(pCamera1, pCamera2, false, Lights);
  }
}

} // unnamed namespace
