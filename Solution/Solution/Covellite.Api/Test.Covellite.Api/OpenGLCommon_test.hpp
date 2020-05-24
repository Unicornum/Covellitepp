
#pragma once
#include <Platform/Windows.mock.hpp>

inline static ::std::vector<float> ARGBtoFloat4(const uint32_t _HexColor)
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
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_NoEqMatrix)
{
  // При определении некоторых макросов glm оператор == возвращает true для
  // разных матриц, поэтому в тестовых проектах используется отдельный
  // заголовочный файл, проверяем это:
  EXPECT_FALSE(::glm::mat4{ 1.0f } == ::glm::mat4{ 2.0f });
  EXPECT_TRUE(::glm::mat4{ 1.0f } != ::glm::mat4{ 2.0f });
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_ResizeWindow)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const int Width = 1710291059;
  const int Height = 1710291100;
  const int Top = 1218;

  Tested_t Example{ Data_t{ Top } };
  ITested_t & IExample = Example;

  using namespace ::testing;

  EXPECT_CALL(GLProxy, Viewport(0, 0, Width, Height - Top))
    .Times(1);

  IExample.ResizeWindow(Width, Height);
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_State_Blend)
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
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_State_Sampler)
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
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_State_Scissor_Enable)
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
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_State_Scissor_Disable)
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

    EXPECT_CALL(GLProxy, DepthFunc(_))
      .Times(0);

    EXPECT_CALL(GLProxy, ClearDepth(_))
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
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_State_Depth_Enable_NoClear_Overwrite)
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

  EXPECT_CALL(GLProxy, DepthMask(GL_TRUE))
    .Times(1);

  EXPECT_CALL(GLProxy, DepthFunc(GL_GREATER))
    .Times(1);

  EXPECT_CALL(GLProxy, ClearDepth(_))
    .Times(0);

  EXPECT_CALL(GLProxy, Clear(GL_DEPTH_BUFFER_BIT))
    .Times(0);

  Render();
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_State_Depth_Enable_Clear_Overwrite)
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

  EXPECT_CALL(GLProxy, DepthMask(GL_TRUE))
    .Times(1);

  EXPECT_CALL(GLProxy, DepthFunc(GL_GREATER))
    .Times(1);

  EXPECT_CALL(GLProxy, ClearDepth(0.0f))
    .Times(1);

  EXPECT_CALL(GLProxy, Clear(GL_DEPTH_BUFFER_BIT))
    .Times(1);

  Render();
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_State_Depth_Enable_NoClear_NoOverwrite)
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
      { uT("overwrite"), false },
    }));
  ASSERT_NE(nullptr, Render);

  EXPECT_CALL(GLProxy, Disable(GL_DEPTH_TEST))
    .Times(0);

  EXPECT_CALL(GLProxy, Enable(GL_DEPTH_TEST))
    .Times(1);

  EXPECT_CALL(GLProxy, DepthMask(GL_FALSE))
    .Times(1);

  EXPECT_CALL(GLProxy, DepthFunc(GL_GREATER))
    .Times(1);

  EXPECT_CALL(GLProxy, ClearDepth(_))
    .Times(0);

  EXPECT_CALL(GLProxy, Clear(GL_DEPTH_BUFFER_BIT))
    .Times(0);

  Render();
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_State_Depth_Enable_Clear_NoOverwrite)
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
      { uT("overwrite"), false },
    }));
  ASSERT_NE(nullptr, Render);

  EXPECT_CALL(GLProxy, Disable(GL_DEPTH_TEST))
    .Times(0);

  EXPECT_CALL(GLProxy, Enable(GL_DEPTH_TEST))
    .Times(1);

  EXPECT_CALL(GLProxy, DepthMask(GL_FALSE))
    .Times(1);

  EXPECT_CALL(GLProxy, DepthFunc(GL_GREATER))
    .Times(1);

  EXPECT_CALL(GLProxy, ClearDepth(0.0f))
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

  Tested_t Example{ Data_t{ } };
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

  EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
    .Times(1);

  EXPECT_CALL(GLProxy, GetError())
    .Times(1)
    .WillOnce(Return(1808261916));

  EXPECT_THROW(itCreator->second(Component_t::Make({})), ::std::exception);
}

// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Texture_UnknownDestination)
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

    EXPECT_THROW(itCreator->second(_pTexture), ::std::exception);
  };

  {
    const auto pTexture = Component_t::Make(
      {
        { uT("destination"), uT("1907251102") },
     });

    TestCall(pTexture);
  }

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Texture") },
        { uT("destination"), uT("1907251103") },
     });

    auto DataRender = itDataCreator->second(pData);
    EXPECT_EQ(nullptr, DataRender);

    TestCall(Component_t::Make({}));
  }
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


////////////////////////////////////////////////////////////////////////////////


// ************************************************************************** //
TEST_F(OpenGLCommon_test, /*DISABLED_*/Test_Present_Geometry_UnknownVariety_deprecated)
{
  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itPresentCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itPresentCreator);

  const auto pComponent = Component_t::Make(
    {
      { uT("kind"), uT("Geometry") },
      { uT("variety"), uT("Unknown1908071150") },
    });

  EXPECT_THROW(itPresentCreator->second(pComponent), ::std::exception);
}

} // unnamed namespace
