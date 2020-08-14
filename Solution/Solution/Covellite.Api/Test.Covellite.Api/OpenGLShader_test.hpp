
#pragma once
#include <gmock/gmock.hpp>
#include <alicorn/std/vector.hpp>
#include <alicorn/std/string/encoding.hpp>
#include <boost/algorithm/string/replace.hpp>

#ifndef OpenGLShader_test

class OpenGLShader_test :
  public ::testing::Test
{

};

#endif

namespace
{

::glm::vec4 ARGBtoVec4(uint32_t _HexColor)
{
  return ::glm::vec4
  {
    ((_HexColor & 0x00FF0000) >> 16) / 255.0f,
    ((_HexColor & 0x0000FF00) >> 8) / 255.0f,
    ((_HexColor & 0x000000FF) >> 0) / 255.0f,
    ((_HexColor & 0xFF000000) >> 24) / 255.0f
  };
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_State_Sampler_Mipmapping)
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

  auto TextureRender = itTextureCreator->second(Component_t::Make(
    { 
      { uT("mipmapping"), true }
    }));
  ASSERT_NE(nullptr, TextureRender);

  using namespace ::testing;

  EXPECT_CALL(GLProxy, TexParameteri(_, _, _))
    .Times(0);

  SamplerRender();

  InSequence Dummy;

  EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, _))
    .Times(1);

  EXPECT_CALL(GLProxy, TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
    GL_LINEAR_MIPMAP_LINEAR))
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
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_BkSurface_Fail)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("BkSurface"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint FrameBufferId = 1910041904;

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, GenFramebuffers(1))
    .Times(1)
    .WillOnce(Return(FrameBufferId));

  EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, FrameBufferId))
    .Times(1);

  EXPECT_CALL(GLProxy, CheckFramebufferStatus(GL_FRAMEBUFFER))
    .Times(1)
    .WillOnce(Return(1910041903));

  EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, 0))
    .Times(1);

  EXPECT_CALL(GLProxy, DeleteFramebuffers(1, FrameBufferId))
    .Times(1);

  EXPECT_THROW(itCreator->second(Component_t::Make({})), ::std::exception);
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_BkSurface)
{
  const auto TestCall = [](
    const Component_t::ComponentPtr_t & _pBkSurface,
    const bool _IsUseMapper,
    const int _Width, const int _Height,
    const int _ExpectedWidth, const int _ExpectedHeight)
  {
    using BufferMapper_t = ::covellite::api::cbBufferMap_t<const void>;

    ::mock::GLProxy GLProxy;

    Data_t SettingsData;
    SettingsData.Top = 34;

    Tested_t Example{ SettingsData };
    ITested_t & IExample = Example;
    IExample.PresentFrame();

    auto itCreator = IExample.GetCreators().find(uT("BkSurface"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    constexpr ::mock::GLint TextureId = 1910041735;
    constexpr ::mock::GLint FrameBufferId = 1910041803;
    constexpr ::mock::GLint BindingFrameBufferId = 2006162207;

    const ::std::vector<String_t> Destinations =
    {
      uT("depth"),
      uT("albedo"),
      uT("metalness"),
      uT("roughness"),
      uT("depth"),
      uT("normal"),
      uT("occlusion"),
      uT("depth"),
    };

    Object_t TextureComponents;

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      const auto pTexture = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("destination"), Destinations[i] },
        });

      if (_IsUseMapper) (*pTexture)[uT("mapper")] = BufferMapper_t{};

      TextureComponents.push_back(pTexture);
    }

    using namespace ::testing;

    InSequence Dummy;

    const int Viewport[] = { 0, 0, _Width, _Height };

    EXPECT_CALL(GLProxy, GetIntegerv(GL_VIEWPORT))
      .Times(1)
      .WillOnce(Return(Viewport));

    EXPECT_CALL(GLProxy, GenFramebuffers(1))
      .Times(1)
      .WillOnce(Return(FrameBufferId));

    EXPECT_CALL(GLProxy, GetIntegerv(GL_FRAMEBUFFER_BINDING))
      .Times(1)
      .WillOnce(Return(&BindingFrameBufferId));

    EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, FrameBufferId))
      .Times(1);

    ::std::vector<::mock::GLenum> AttachmentIndexes;

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      auto Format = GL_DEPTH_COMPONENT;
      auto Attachment = GL_DEPTH_ATTACHMENT;

      if (Destinations[i] != uT("depth"))
      {
        Format = GL_RGBA;
        Attachment = GL_COLOR_ATTACHMENT0 +
          static_cast<int>(AttachmentIndexes.size());
        AttachmentIndexes.push_back(Attachment);
      }

      EXPECT_CALL(GLProxy, GenTextures(1))
        .Times(1)
        .WillOnce(Return(TextureId + i));

      EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId + i))
        .Times(1);

      EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, _,
        _ExpectedWidth, _ExpectedHeight, 0, Format, _, ::std::vector<uint8_t>{}))
        .Times(1);

      EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
        .Times(1);

      EXPECT_CALL(GLProxy, GetError())
        .Times(1)
        .WillOnce(Return(GL_NO_ERROR));

      EXPECT_CALL(GLProxy, FramebufferTexture2D(GL_FRAMEBUFFER, Attachment,
        GL_TEXTURE_2D, TextureId + i, 0))
        .Times(1);
    }

    EXPECT_CALL(GLProxy, CheckFramebufferStatus(GL_FRAMEBUFFER))
      .Times(1)
      .WillOnce(Return(GL_FRAMEBUFFER_COMPLETE));

    EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, BindingFrameBufferId))
      .Times(1);

    (*_pBkSurface)[uT("service")] = TextureComponents;

    auto Render = itCreator->second(_pBkSurface);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_VIEWPORT))
      .Times(1)
      .WillOnce(Return(Viewport));

    EXPECT_CALL(GLProxy, TexImage2D(_, _, _, _, _, _, _, _, _))
      .Times(0);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_FRAMEBUFFER_BINDING))
      .Times(1)
      .WillOnce(Return(&BindingFrameBufferId));

    EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, FrameBufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, DrawBuffers(AttachmentIndexes))
      .Times(1);

    EXPECT_CALL(GLProxy, Viewport(0, 0, _ExpectedWidth, _ExpectedHeight - SettingsData.Top))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteFramebuffers(1, FrameBufferId))
      .Times(1);

    ASSERT_EQ(TextureComponents.size(), Destinations.size());

    EXPECT_EQ(_ExpectedWidth, (int)(*_pBkSurface)[uT("width")]);
    EXPECT_EQ(_ExpectedHeight, (int)(*_pBkSurface)[uT("height")]);

    for (::std::size_t i = 0; i < TextureComponents.size(); i++)
    {
      using Texture_t = ::covellite::api::renderer::OpenGLCommon::Texture;

      const auto pTextureComponent = TextureComponents[i];
      const auto pTexture =
        pTextureComponent->GetValue(uT("entity"), Texture_t::Ptr_t{});
      ASSERT_NE(nullptr, pTexture);
      EXPECT_EQ(TextureId + i, pTexture->m_TextureId);
      EXPECT_EQ(_IsUseMapper ? 4 * _ExpectedWidth * _ExpectedHeight : 0,
        pTexture->m_ReadCopyData.size());
      EXPECT_EQ(_ExpectedWidth, pTextureComponent->GetValue(uT("width"), 0));
      EXPECT_EQ(_ExpectedHeight, pTextureComponent->GetValue(uT("height"), 0));
    }
  };

  TestCall(Component_t::Make({ }), 
    false, 12, 45, 12, 45);
  TestCall(Component_t::Make({ }), 
    true, 45, 12, 45, 12);

  TestCall(Component_t::Make(
    {
      { uT("scale"), 0.5f },
    }),
    false, 12, 45, 6, 22);
  TestCall(Component_t::Make(
    { 
      { uT("scale"), 2.0f },
    }),
    true, 45, 12, 90, 24);

  TestCall(Component_t::Make(
    {
      { uT("scale"), 0.5f },
      { uT("width"), 2008141652 },
      { uT("height"), 2008141653 },
   }),
    false, 12, 45, 6, 22);
  TestCall(Component_t::Make(
    {
      { uT("scale"), 2.0f },
      { uT("width"), 2008141652 },
      { uT("height"), 2008141653 },
    }),
    true, 45, 12, 90, 24);

  TestCall(Component_t::Make(
    {
      { uT("width"), 12 },
      { uT("height"), 45 },
    }),
    false, 21, 54, 12, 45);
  TestCall(Component_t::Make(
    {
      { uT("width"), 45 },
      { uT("height"), 12 },
    }),
    true, 54, 21, 45, 12);
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_BkSurface_RenderDepthOnly)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("BkSurface"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint TextureId = 1910041735;
  const ::mock::GLuint FrameBufferId = 1910041803;

  using namespace ::testing;

  InSequence Dummy;

  const auto pTexture = Component_t::Make(
    {
      { uT("kind"), uT("Texture") },
      { uT("destination"), uT("depth") },
    });

  EXPECT_CALL(GLProxy, CheckFramebufferStatus(GL_FRAMEBUFFER))
    .Times(1)
    .WillOnce(Return(GL_FRAMEBUFFER_COMPLETE));

  auto Render = itCreator->second(Component_t::Make(
    {
      { uT("service"), Object_t{ pTexture } }
    }));
  ASSERT_NE(nullptr, Render);

  EXPECT_CALL(GLProxy, DrawBuffers(::std::vector<::mock::GLenum>{}))
    .Times(1);

  Render();
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_BkSurface_ResizeWindow)
{
  const auto TestCall = [](
    const Component_t::ComponentPtr_t & _pBkSurface,
    const bool _IsUseMapper,
    const int _Width, const int _Height,
    const int _ExpectedWidth, const int _ExpectedHeight,
    const int _ExpectedWidth2, const int _ExpectedHeight2)
  {
    using Texture_t = ::covellite::api::renderer::OpenGLCommon::Texture;
    using BufferMapper_t = ::covellite::api::cbBufferMap_t<const void>;

    ::mock::GLProxy GLProxy;

    Data_t SettingsData;
    SettingsData.Top = 45;

    Tested_t Example{ SettingsData };
    ITested_t & IExample = Example;
    IExample.PresentFrame();

    auto itCreator = IExample.GetCreators().find(uT("BkSurface"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    constexpr ::mock::GLuint TextureId = 1910091302;

    const ::std::vector<String_t> Destinations =
    {
      uT("albedo"),
      uT("metalness"),
      uT("roughness"),
      uT("normal"),
      uT("occlusion"),
      uT("depth"),
    };

    Object_t TextureComponents;

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      const auto pTexture = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("destination"), Destinations[i] },
        });

      if (_IsUseMapper) (*pTexture)[uT("mapper")] = BufferMapper_t{};

      TextureComponents.push_back(pTexture);
    }

    using namespace ::testing;

    InSequence Dummy;

    const int Viewport[] = { 0, 0, _Width, _Height };

    EXPECT_CALL(GLProxy, GetIntegerv(GL_VIEWPORT))
      .Times(1)
      .WillOnce(Return(Viewport));

    EXPECT_CALL(GLProxy, GetIntegerv(_))
      .Times(1);

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      EXPECT_CALL(GLProxy, GenTextures(1))
        .Times(1)
        .WillOnce(Return(TextureId + i));

      EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId + i))
        .Times(1);

      EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, _, _,
        _ExpectedWidth, _ExpectedHeight, _, _, _, ::std::vector<uint8_t>{}))
        .Times(1);

      EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
        .Times(1);

      EXPECT_CALL(GLProxy, GetError())
        .Times(1)
        .WillOnce(Return(GL_NO_ERROR));
    }

    EXPECT_CALL(GLProxy, CheckFramebufferStatus(GL_FRAMEBUFFER))
      .Times(1)
      .WillOnce(Return(GL_FRAMEBUFFER_COMPLETE));

    (*_pBkSurface)[uT("service")] = TextureComponents;

    auto Render = itCreator->second(_pBkSurface);
    ASSERT_NE(nullptr, Render);

    EXPECT_EQ(_ExpectedWidth, (int)(*_pBkSurface)[uT("width")]);
    EXPECT_EQ(_ExpectedHeight, (int)(*_pBkSurface)[uT("height")]);

    for (::std::size_t i = 0; i < TextureComponents.size(); i++)
    {
      const auto pTextureComponent = TextureComponents[i];
      EXPECT_EQ(_ExpectedWidth, pTextureComponent->GetValue(uT("width"), 0));
      EXPECT_EQ(_ExpectedHeight, pTextureComponent->GetValue(uT("height"), 0));

      const auto pTexture =
        pTextureComponent->GetValue(uT("entity"), Texture_t::Ptr_t{});
      ASSERT_NE(nullptr, pTexture);
      EXPECT_EQ(_IsUseMapper ? _ExpectedWidth * _ExpectedHeight * 4 : 0,
        pTexture->m_ReadCopyData.size());
    }

    EXPECT_CALL(GLProxy, GetIntegerv(GL_VIEWPORT))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(_, _, _, _, _, _, _, _, _))
      .Times(0);

    EXPECT_CALL(GLProxy, GetIntegerv(_))
      .Times(1);

    EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, _))
      .Times(1);

    EXPECT_CALL(GLProxy, Viewport(_, _, _, _))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, GetIntegerv(GL_VIEWPORT))
      .Times(1);

    IExample.PresentFrame();

    const int Viewport2[] = { 0, 0, _Width * 2, _Height * 2 };

    IExample.ResizeWindow(_Width * 2, _Height * 2);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_VIEWPORT))
      .Times(1)
      .WillOnce(Return(Viewport2));

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId + i))
        .Times(1);

      EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, _, _,
        _ExpectedWidth2, _ExpectedHeight2, _, _, _, ::std::vector<uint8_t>{}))
        .Times(1);

      EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
        .Times(1);

      EXPECT_CALL(GLProxy, GetError())
        .Times(1)
        .WillOnce(Return(GL_NO_ERROR));
    }

    EXPECT_CALL(GLProxy, GetIntegerv(_))
      .Times(1);

    EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, _))
      .Times(1);

    EXPECT_CALL(GLProxy, Viewport(0, 0, _ExpectedWidth2, _ExpectedHeight2 - SettingsData.Top))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, GetIntegerv(GL_VIEWPORT))
      .Times(1);

    IExample.PresentFrame();

    EXPECT_EQ(_ExpectedWidth2, (int)(*_pBkSurface)[uT("width")]);
    EXPECT_EQ(_ExpectedHeight2, (int)(*_pBkSurface)[uT("height")]);

    for (::std::size_t i = 0; i < TextureComponents.size(); i++)
    {
      const auto pTextureComponent = TextureComponents[i];
      EXPECT_EQ(_ExpectedWidth2, pTextureComponent->GetValue(uT("width"), 0));
      EXPECT_EQ(_ExpectedHeight2, pTextureComponent->GetValue(uT("height"), 0));

      const auto pTexture =
        pTextureComponent->GetValue(uT("entity"), Texture_t::Ptr_t{});
      ASSERT_NE(nullptr, pTexture);
      EXPECT_EQ(_IsUseMapper ? _ExpectedWidth2 * _ExpectedHeight2 * 4 : 0,
        pTexture->m_ReadCopyData.size());
    }

    EXPECT_CALL(GLProxy, GetIntegerv(GL_VIEWPORT))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(_, _, _, _, _, _, _, _, _))
      .Times(0);

    EXPECT_CALL(GLProxy, GetIntegerv(_))
      .Times(1);

    EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, _))
      .Times(1);

    EXPECT_CALL(GLProxy, Viewport(_, _, _, _))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, GetIntegerv(GL_VIEWPORT))
      .Times(1);

    IExample.PresentFrame();
  };

  TestCall(Component_t::Make({ }), 
    false, 45, 67, 45, 67, 90, 134);
  TestCall(Component_t::Make({ }), 
    true, 45, 67, 45, 67, 90, 134);

  TestCall(Component_t::Make(
    {
      { uT("scale"), 0.5f },
    }),
    false, 12, 45, 6, 22, 12, 45);
  TestCall(Component_t::Make(
    {
      { uT("scale"), 2.0f },
    }),
    true, 45, 12, 90, 24, 180, 48);

  TestCall(Component_t::Make(
    {
      { uT("scale"), 0.5f },
      { uT("width"), 2008141652 },
      { uT("height"), 2008141653 },
    }),
    false, 12, 45, 6, 22, 12, 45);
  TestCall(Component_t::Make(
    {
      { uT("scale"), 2.0f },
      { uT("width"), 2008141652 },
      { uT("height"), 2008141653 },
    }),
    true, 45, 12, 90, 24, 180, 48);

  TestCall(Component_t::Make(
    {
      { uT("width"), 12 },
      { uT("height"), 45 },
    }),
    false, 21, 54, 12, 45, 12, 45);
  TestCall(Component_t::Make(
    {
      { uT("width"), 45 },
      { uT("height"), 12 },
    }),
    true, 54, 21, 45, 12, 45, 12);
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Shader_InvalidType)
{
  const ::std::string ShaderData =
    "Pixel vs1(Polygon _Value)\r\n"
    "Pixel vs2(Polyhedron _Value)\r\n";

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint ShaderId = 1908250832;

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pShader)
  {
    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, CreateShader(_))
      .Times(0);

    auto Render = itCreator->second(_pShader);
    EXPECT_EQ(nullptr, Render);
  };

  {
    const auto pShader = Component_t::Make(
      {
        { uT("entry"), uT("vs1") },
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
      });

    TestCall(pShader);
  }

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Shader") },
        { uT("entry"), uT("vs2") },
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }));
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Shader_NotExistsEntryPoint)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint ShaderId = 1908250832;

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pShader)
  {
    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, CreateShader(_))
      .Times(0);

    EXPECT_THROW(itCreator->second(_pShader), ::std::exception);
  };

  {
    const auto pShader = Component_t::Make(
      {
        { uT("entry"), uT("vsUnknown") },
      });

    TestCall(pShader);
  }

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Shader") },
        { uT("entry"), uT("vsUnknown") },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }));
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Shader_CompileFail)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint ShaderId = 1908251346;
  const ::std::string ErrorText = "Error1908251350";

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pShader)
  {
    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, CreateShader(_))
      .Times(1)
      .WillOnce(Return(ShaderId));

    EXPECT_CALL(GLProxy, ShaderSource(ShaderId, _, _, _))
      .Times(1);

    EXPECT_CALL(GLProxy, CompileShader(ShaderId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetShaderiv(ShaderId, GL_COMPILE_STATUS))
      .Times(1)
      .WillOnce(Return(GL_FALSE));

    EXPECT_CALL(GLProxy, GetShaderInfoLog(ShaderId, 512, nullptr))
      .Times(1)
      .WillOnce(Return(ErrorText.c_str()));

    EXPECT_CALL(GLProxy, DeleteShader(ShaderId))
      .Times(1);

    EXPECT_STDEXCEPTION(itCreator->second(_pShader),
      (".*Compile shader fail \\[header line: 266\\]: " + ErrorText).c_str());
  };

  {
    const auto pShader = Component_t::Make(
      {
        { uT("entry"), uT("vsFlat") },
      });

    TestCall(pShader);
  }

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Shader") },
        { uT("entry"), uT("vsVolume") },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }));
  }
}

::std::string ConvertTextShader(const ::std::vector<uint8_t> & _ShaderText)
{
  return ::std::string{ _ShaderText.cbegin(), _ShaderText.cend() };
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Shader_CreateVertex_Default)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint ShaderId = 1908250832;

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pShader, 
    const ::std::string & _Entry)
  {
    using namespace ::alicorn::extension::std;

    const auto ExpectedShaderText =
      ShaderHeader +
      "#define COVELLITE_SHADER_GLSL\r\n"
      "#define COVELLITE_SHADER_VERTEX\r\n" +
      ConvertTextShader(::Predefined + ::Data + ::Input + ::Default) +
      "out Pixel PixelValue;\r\n"
      "void main()\r\n"
      "{\r\n"
      "  Vertex InputData;\r\n"
      "  InputData.Position = Covellite_VertexPosition;\r\n"
      "  InputData.TexCoord = Covellite_VertexTexCoord;\r\n"
      "  InputData.Extra = Covellite_VertexExtra;\r\n"
      "  PixelValue = " + _Entry + "(InputData);\r\n"
      "  gl_Position = PixelValue.ScreenPos;\r\n"
      "}\r\n";

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, CreateShader(GL_VERTEX_SHADER))
      .Times(1)
      .WillOnce(Return(ShaderId));

    EXPECT_CALL(GLProxy, ShaderSource(ShaderId, 1, ExpectedShaderText, nullptr))
      .Times(1);

    EXPECT_CALL(GLProxy, CompileShader(ShaderId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetShaderiv(ShaderId, GL_COMPILE_STATUS))
      .Times(1)
      .WillOnce(Return(GL_TRUE));

    auto Render = itCreator->second(_pShader);
    ASSERT_NE(nullptr, Render);

    Render();

    EXPECT_CALL(GLProxy, DeleteShader(ShaderId))
      .Times(1);
  };

  {
    const auto pShader = Component_t::Make(
      {
        { uT("entry"), uT("vsFlat") },
      });

    TestCall(pShader, "vsFlat");
  }

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Shader") },
        { uT("entry"), uT("vsVolume") },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }), "vsVolume");
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Shader_CreateVertex)
{
  const ::std::string ShaderData = 
    "Pixel vs1(Vertex _Value)\r\n"
    "Pixel vs2_Dummy(Vertex _Value)\r\n"
    "Pixel vs2(Vertex _Value)\r\n";

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint ShaderId = 1908250832;

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pShader,
    const ::std::string & _Entry)
  {
    using namespace ::alicorn::extension::std;

    const auto ExpectedShaderText =
      ShaderHeader +
      "#define COVELLITE_SHADER_GLSL\r\n"
      "#define COVELLITE_SHADER_VERTEX\r\n" +
      ConvertTextShader(::Predefined + ::Data + ::Input) + ShaderData +
      "out Pixel PixelValue;\r\n"
      "void main()\r\n"
      "{\r\n"
      "  Vertex InputData;\r\n"
      "  InputData.Position = Covellite_VertexPosition;\r\n"
      "  InputData.TexCoord = Covellite_VertexTexCoord;\r\n"
      "  InputData.Extra = Covellite_VertexExtra;\r\n"
      "  PixelValue = " + _Entry + "(InputData);\r\n"
      "  gl_Position = PixelValue.ScreenPos;\r\n"
      "}\r\n";

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, CreateShader(GL_VERTEX_SHADER))
      .Times(1)
      .WillOnce(Return(ShaderId));

    EXPECT_CALL(GLProxy, ShaderSource(ShaderId, 1, ExpectedShaderText, nullptr))
      .Times(1);

    EXPECT_CALL(GLProxy, CompileShader(ShaderId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetShaderiv(ShaderId, GL_COMPILE_STATUS))
      .Times(1)
      .WillOnce(Return(GL_TRUE));

    auto Render = itCreator->second(_pShader);
    ASSERT_NE(nullptr, Render);

    Render();

    EXPECT_CALL(GLProxy, DeleteShader(ShaderId))
      .Times(1);
  };

  {
    const auto pShader = Component_t::Make(
      {
        { uT("entry"), uT("vs1") },
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
      });

    TestCall(pShader, "vs1");
  }

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Shader") },
        { uT("entry"), uT("vs2") },
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }), "vs2");
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Shader_CreateVertex_Instance_InvalidValue)
{
  const ::std::string ShaderData =
    "Pixel vs1(Vertex _Value)\r\n"
    "Pixel vs2(Vertex _Value)\r\n";

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  using namespace ::testing;

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pShader)
  {
    EXPECT_CALL(GLProxy, CompileShader(_))
      .Times(0);

    EXPECT_THROW(itCreator->second(_pShader), ::std::exception);
  };

  {
    const auto pShader = Component_t::Make(
      {
        { uT("entry"), uT("vs1") },
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
        { uT("instance"), uT("invalid1909231417") },
      });

    TestCall(pShader);
  }

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Shader") },
        { uT("entry"), uT("vs2") },
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
        { uT("instance"), uT("invalid1909231418") },
     });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }));
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Shader_CreateVertex_Instance)
{
  const ::std::string ShaderData =
    "Pixel vs1(Vertex _Value)\r\n"
    "Pixel vs2(Vertex _Value)\r\n";

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint ShaderId = 1908250832;

  auto InputData = ConvertTextShader(::Input);

  ::boost::algorithm::replace_first(InputData,
    "/* place for instance variables */", ::std::string{} +
    "COVELLITE_IN float4 iValue1 COVELLITE_INPUT_SEMANTIC(TEXCOORD1);" + (char)0x5C + "\r\n"
    "COVELLITE_IN float4 iValue2 COVELLITE_INPUT_SEMANTIC(TEXCOORD2);" + (char)0x5C + "\r\n"
    "COVELLITE_IN int4 iValue3 COVELLITE_INPUT_SEMANTIC(TEXCOORD3);" + (char)0x5C + "\r\n"
    "COVELLITE_IN float4 iValue4 COVELLITE_INPUT_SEMANTIC(TEXCOORD4);" + (char)0x5C + "\r\n"
  );

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pShader,
    const ::std::string & _Entry)
  {
    using namespace ::alicorn::extension::std;

    const auto ExpectedShaderText =
      ShaderHeader +
      "#define COVELLITE_SHADER_GLSL\r\n"
      "#define COVELLITE_SHADER_VERTEX\r\n" +
      ConvertTextShader(::Predefined + ::Data) + InputData + ShaderData +
      "out Pixel PixelValue;\r\n"
      "void main()\r\n"
      "{\r\n"
      "  Vertex InputData;\r\n"
      "  InputData.Position = Covellite_VertexPosition;\r\n"
      "  InputData.TexCoord = Covellite_VertexTexCoord;\r\n"
      "  InputData.Extra = Covellite_VertexExtra;\r\n"
      "  InputData.iValue1 = iValue1;\r\n"
      "  InputData.iValue2 = iValue2;\r\n"
      "  InputData.iValue3 = iValue3;\r\n"
      "  InputData.iValue4 = iValue4;\r\n"
      "  PixelValue = " + _Entry + "(InputData);\r\n"
      "  gl_Position = PixelValue.ScreenPos;\r\n"
      "}\r\n";

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, CreateShader(GL_VERTEX_SHADER))
      .Times(1)
      .WillOnce(Return(ShaderId));

    EXPECT_CALL(GLProxy, ShaderSource(ShaderId, 1, ExpectedShaderText, nullptr))
      .Times(1);

    EXPECT_CALL(GLProxy, CompileShader(ShaderId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetShaderiv(ShaderId, GL_COMPILE_STATUS))
      .Times(1)
      .WillOnce(Return(GL_TRUE));

    auto Render = itCreator->second(_pShader);
    ASSERT_NE(nullptr, Render);

    Render();

    EXPECT_CALL(GLProxy, DeleteShader(ShaderId))
      .Times(1);
  };

  {
    const auto pShader = Component_t::Make(
      {
        { uT("entry"), uT("vs1") },
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
        { uT("instance"), uT("f4f4i4f4") },
      });

    TestCall(pShader, "vs1");
  }

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Shader") },
        { uT("entry"), uT("vs2") },
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
        { uT("instance"), uT("f4f4i4f4") },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }), "vs2");
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Shader_CreatePixel_Default)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint ShaderId = 1908250832;

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pShader,
    const ::std::string & _Entry)
  {
    using namespace ::alicorn::extension::std;

    const auto ExpectedShaderText =
      ShaderHeader +
      "#define COVELLITE_SHADER_GLSL\r\n"
      "#define COVELLITE_SHADER_PIXEL\r\n" +
      ConvertTextShader(::Predefined + ::Data + ::Input + ::Default) +
      "in Pixel PixelValue;\r\n"
      "out vec4 Covellite_OutPixelColor;\r\n"
      "void main()\r\n"
      "{\r\n"
      "  Covellite_OutPixelColor = " + _Entry + "(PixelValue);\r\n"
      "}\r\n";

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, CreateShader(GL_FRAGMENT_SHADER))
      .Times(1)
      .WillOnce(Return(ShaderId));

    EXPECT_CALL(GLProxy, ShaderSource(ShaderId, 1, ExpectedShaderText, nullptr))
      .Times(1);

    EXPECT_CALL(GLProxy, CompileShader(ShaderId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetShaderiv(ShaderId, GL_COMPILE_STATUS))
      .Times(1)
      .WillOnce(Return(GL_TRUE));

    auto Render = itCreator->second(_pShader);
    ASSERT_NE(nullptr, Render);

    Render();

    EXPECT_CALL(GLProxy, DeleteShader(ShaderId))
      .Times(1);
  };

  {
    const auto pShader = Component_t::Make(
      {
        { uT("entry"), uT("psColored") },
      });

    TestCall(pShader, "psColored");
  }

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Shader") },
        { uT("entry"), uT("psLightened") },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }), "psLightened");
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Shader_CreatePixel_SingleTarget)
{
  const ::std::string ShaderData =
    "float4 ps1(Pixel _Value)\r\n"
    "vec4 ps2_Dummy(Pixel _Value)\r\n"
    "vec4 ps2(Pixel _Value)\r\n";

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint ShaderId = 1908250832;

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pShader,
    const ::std::string & _Entry)
  {
    using namespace ::alicorn::extension::std;

    const auto ExpectedShaderText =
      ShaderHeader +
      "#define COVELLITE_SHADER_GLSL\r\n"
      "#define COVELLITE_SHADER_PIXEL\r\n" +
      ConvertTextShader(::Predefined + ::Data + ::Input) + ShaderData +
      "in Pixel PixelValue;\r\n"
      "out vec4 Covellite_OutPixelColor;\r\n"
      "void main()\r\n"
      "{\r\n"
      "  Covellite_OutPixelColor = " + _Entry + "(PixelValue);\r\n"
      "}\r\n";

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, CreateShader(GL_FRAGMENT_SHADER))
      .Times(1)
      .WillOnce(Return(ShaderId));

    EXPECT_CALL(GLProxy, ShaderSource(ShaderId, 1, ExpectedShaderText, nullptr))
      .Times(1);

    EXPECT_CALL(GLProxy, CompileShader(ShaderId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetShaderiv(ShaderId, GL_COMPILE_STATUS))
      .Times(1)
      .WillOnce(Return(GL_TRUE));

    auto Render = itCreator->second(_pShader);
    ASSERT_NE(nullptr, Render);

    Render();

    EXPECT_CALL(GLProxy, DeleteShader(ShaderId))
      .Times(1);
  };

  {
    const auto pShader = Component_t::Make(
      {
        { uT("entry"), uT("ps1") },
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
      });

    TestCall(pShader, "ps1");
  }

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Shader") },
        { uT("entry"), uT("ps2") },
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }), "ps2");
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Shader_CreatePixel_MultiTarget)
{
  const ::std::string ShaderData =
    "Multi1910031832 ps(Pixel _Value)\r\n";

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint ShaderId = 1908250832;

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pShader,
    const ::std::string & _Entry)
  {
    using namespace ::alicorn::extension::std;

    const auto ExpectedShaderText =
      ShaderHeader +
      "#define COVELLITE_SHADER_GLSL\r\n"
      "#define COVELLITE_SHADER_PIXEL\r\n" +
      ConvertTextShader(::Predefined + ::Data + ::Input) + ShaderData +
      "in Pixel PixelValue;\r\n"
      "void main()\r\n"
      "{\r\n"
      "  Covellite_MultiOutPixelColor = " + _Entry + "(PixelValue).Target;\r\n"
      "}\r\n";

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, CreateShader(GL_FRAGMENT_SHADER))
      .Times(1)
      .WillOnce(Return(ShaderId));

    EXPECT_CALL(GLProxy, ShaderSource(ShaderId, 1, ExpectedShaderText, nullptr))
      .Times(1);

    EXPECT_CALL(GLProxy, CompileShader(ShaderId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetShaderiv(ShaderId, GL_COMPILE_STATUS))
      .Times(1)
      .WillOnce(Return(GL_TRUE));

    auto Render = itCreator->second(_pShader);
    ASSERT_NE(nullptr, Render);

    Render();

    EXPECT_CALL(GLProxy, DeleteShader(ShaderId))
      .Times(1);
  };

  {
    const auto pShader = Component_t::Make(
      {
        { uT("entry"), uT("ps") },
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
      });

    TestCall(pShader, "ps");
  }

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Shader") },
        { uT("entry"), uT("ps") },
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }), "ps");
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Shader_CreatePixel_NoReturn)
{
  const ::std::string ShaderData =
    "void ps(Pixel _Value)\r\n";

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint ShaderId = 1908250832;

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pShader,
    const ::std::string & _Entry)
  {
    using namespace ::alicorn::extension::std;

    const auto ExpectedShaderText =
      ShaderHeader +
      "#define COVELLITE_SHADER_GLSL\r\n"
      "#define COVELLITE_SHADER_PIXEL\r\n" +
      ConvertTextShader(::Predefined + ::Data + ::Input) + ShaderData +
      "in Pixel PixelValue;\r\n"
      "void main()\r\n"
      "{\r\n"
      "  " + _Entry + "(PixelValue);\r\n"
      "}\r\n";

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, CreateShader(GL_FRAGMENT_SHADER))
      .Times(1)
      .WillOnce(Return(ShaderId));

    EXPECT_CALL(GLProxy, ShaderSource(ShaderId, 1, ExpectedShaderText, nullptr))
      .Times(1);

    EXPECT_CALL(GLProxy, CompileShader(ShaderId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetShaderiv(ShaderId, GL_COMPILE_STATUS))
      .Times(1)
      .WillOnce(Return(GL_TRUE));

    auto Render = itCreator->second(_pShader);
    ASSERT_NE(nullptr, Render);

    Render();

    EXPECT_CALL(GLProxy, DeleteShader(ShaderId))
      .Times(1);
  };

  {
    const auto pShader = Component_t::Make(
      {
        { uT("entry"), uT("ps") },
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
      });

    TestCall(pShader, "ps");
  }

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Shader") },
        { uT("entry"), uT("ps") },
        { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }), "ps");
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Shader_UsingProgram_DeleteVertexShader)
{
  const ::std::string ShaderData =
    "Pixel vs(Vertex _Value)\r\n"
    "float4 ps(Pixel _Value)\r\n";

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto pVertexShader0 = Component_t::Make(
    {
      { uT("entry"), uT("vs") },
      { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
    });

  const auto pVertexShader1 = Component_t::Make(
    {
      { uT("entry"), uT("vs") },
      { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
    });

  const auto pPixelShader0 = Component_t::Make(
    {
      { uT("entry"), uT("ps") },
      { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
    });

  const auto pPixelShader1 = Component_t::Make(
    {
      { uT("entry"), uT("ps") },
      { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
    });

  const ::mock::GLuint VertexShader0Id = 1908251613;
  const ::mock::GLuint VertexShader1Id = 1908251614;
  const ::mock::GLuint PixelShader0Id = 1908251615;
  const ::mock::GLuint PixelShader1Id = 1908251616;

  const ::mock::GLuint Program00Id = 1908251641;
  const ::mock::GLuint Program01Id = 1908251652;
  const ::mock::GLuint Program10Id = 1908251653;
  const ::mock::GLuint Program11Id = 1908251654;

  using namespace ::testing;

  EXPECT_CALL(GLProxy, CreateShader(GL_FRAGMENT_SHADER))
    .Times(2)
    .WillOnce(Return(PixelShader0Id))
    .WillOnce(Return(PixelShader1Id));

  EXPECT_CALL(GLProxy, CreateShader(GL_VERTEX_SHADER))
    .Times(2)
    .WillOnce(Return(VertexShader0Id))
    .WillOnce(Return(VertexShader1Id));

  auto PixelShaderRender0 = itCreator->second(pPixelShader0);
  ASSERT_NE(nullptr, PixelShaderRender0);

  auto PixelShaderRender1 = itCreator->second(pPixelShader1);
  ASSERT_NE(nullptr, PixelShaderRender1);

  auto VertexShaderRender0 = itCreator->second(pVertexShader0);
  ASSERT_NE(nullptr, VertexShaderRender0);

  InSequence Dummy;

  {
    auto VertexShaderRender1 = itCreator->second(pVertexShader1);
    ASSERT_NE(nullptr, VertexShaderRender1);

    const auto TestCreateProgram = [&](
      const ::mock::GLuint _ProgramId,
      const ::mock::GLuint _VertexShaderId,
      const ::mock::GLuint _PixelShaderId)
    {
      EXPECT_CALL(GLProxy, CreateProgram())
        .Times(1)
        .WillOnce(Return(_ProgramId));

      EXPECT_CALL(GLProxy, AttachShader(_ProgramId, _VertexShaderId))
        .Times(1);

      EXPECT_CALL(GLProxy, AttachShader(_ProgramId, _PixelShaderId))
        .Times(1);

      EXPECT_CALL(GLProxy, LinkProgram(_ProgramId))
        .Times(1);

      EXPECT_CALL(GLProxy, GetProgramiv(_ProgramId, GL_LINK_STATUS))
        .Times(1)
        .WillOnce(Return(GL_TRUE));
    };

    // **************** Cоздание объектов программ шейдеров ******************* //

    EXPECT_CALL(GLProxy, CreateProgram())
      .Times(0);

    VertexShaderRender0();

    TestCreateProgram(Program00Id, VertexShader0Id, PixelShader0Id);

    EXPECT_CALL(GLProxy, UseProgram(Program00Id))
      .Times(1);

    PixelShaderRender0();

    TestCreateProgram(Program10Id, VertexShader1Id, PixelShader0Id);

    EXPECT_CALL(GLProxy, UseProgram(Program10Id))
      .Times(1);

    VertexShaderRender1();

    TestCreateProgram(Program11Id, VertexShader1Id, PixelShader1Id);

    EXPECT_CALL(GLProxy, UseProgram(Program11Id))
      .Times(1);

    PixelShaderRender1();

    TestCreateProgram(Program01Id, VertexShader0Id, PixelShader1Id);

    EXPECT_CALL(GLProxy, UseProgram(Program01Id))
      .Times(1);

    VertexShaderRender0();

    // ********* Использование ранее созданных программ шейдеров ************ //

    EXPECT_CALL(GLProxy, CreateProgram())
      .Times(0);

    EXPECT_CALL(GLProxy, UseProgram(Program00Id))
      .Times(1);

    PixelShaderRender0();

    EXPECT_CALL(GLProxy, CreateProgram())
      .Times(0);

    EXPECT_CALL(GLProxy, UseProgram(Program10Id))
      .Times(1);

    VertexShaderRender1();

    EXPECT_CALL(GLProxy, CreateProgram())
      .Times(0);

    EXPECT_CALL(GLProxy, UseProgram(Program11Id))
      .Times(1);

    PixelShaderRender1();

    EXPECT_CALL(GLProxy, CreateProgram())
      .Times(0);

    EXPECT_CALL(GLProxy, UseProgram(Program01Id))
      .Times(1);

    VertexShaderRender0();

    EXPECT_CALL(GLProxy, DeleteProgram(Program10Id))
      .Times(1);

    EXPECT_CALL(GLProxy, DeleteProgram(Program11Id))
      .Times(1);

    EXPECT_CALL(GLProxy, DeleteShader(VertexShader1Id))
      .Times(1);
  }

  EXPECT_CALL(GLProxy, DeleteProgram(Program00Id))
    .Times(1);

  EXPECT_CALL(GLProxy, DeleteProgram(Program01Id))
    .Times(1);

  EXPECT_CALL(GLProxy, DeleteShader(VertexShader0Id))
    .Times(1);

  EXPECT_CALL(GLProxy, DeleteShader(PixelShader1Id))
    .Times(1);

  EXPECT_CALL(GLProxy, DeleteShader(PixelShader0Id))
    .Times(1);
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Shader_UsingProgram_DeletePixelShader)
{
  const ::std::string ShaderData =
    "Pixel vs(Vertex _Value)\r\n"
    "float4 ps(Pixel _Value)\r\n";

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto pVertexShader0 = Component_t::Make(
    {
      { uT("entry"), uT("vs") },
      { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
    });

  const auto pVertexShader1 = Component_t::Make(
    {
      { uT("entry"), uT("vs") },
      { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
    });

  const auto pPixelShader0 = Component_t::Make(
    {
      { uT("entry"), uT("ps") },
      { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
    });

  const auto pPixelShader1 = Component_t::Make(
    {
      { uT("entry"), uT("ps") },
      { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
    });

  const ::mock::GLuint VertexShader0Id = 1908251613;
  const ::mock::GLuint VertexShader1Id = 1908251614;
  const ::mock::GLuint PixelShader0Id = 1908251615;
  const ::mock::GLuint PixelShader1Id = 1908251616;

  const ::mock::GLuint Program00Id = 1908251641;
  const ::mock::GLuint Program01Id = 1908251652;
  const ::mock::GLuint Program10Id = 1908251653;
  const ::mock::GLuint Program11Id = 1908251654;

  using namespace ::testing;

  EXPECT_CALL(GLProxy, CreateShader(GL_VERTEX_SHADER))
    .Times(2)
    .WillOnce(Return(VertexShader0Id))
    .WillOnce(Return(VertexShader1Id));

  EXPECT_CALL(GLProxy, CreateShader(GL_FRAGMENT_SHADER))
    .Times(2)
    .WillOnce(Return(PixelShader0Id))
    .WillOnce(Return(PixelShader1Id));

  auto VertexShaderRender0 = itCreator->second(pVertexShader0);
  ASSERT_NE(nullptr, VertexShaderRender0);

  auto VertexShaderRender1 = itCreator->second(pVertexShader1);
  ASSERT_NE(nullptr, VertexShaderRender1);

  auto PixelShaderRender0 = itCreator->second(pPixelShader0);
  ASSERT_NE(nullptr, PixelShaderRender0);

  InSequence Dummy;

  {
    auto PixelShaderRender1 = itCreator->second(pPixelShader1);
    ASSERT_NE(nullptr, PixelShaderRender1);

    const auto TestCreateProgram = [&](
      const ::mock::GLuint _ProgramId,
      const ::mock::GLuint _VertexShaderId,
      const ::mock::GLuint _PixelShaderId)
    {
      EXPECT_CALL(GLProxy, CreateProgram())
        .Times(1)
        .WillOnce(Return(_ProgramId));

      EXPECT_CALL(GLProxy, AttachShader(_ProgramId, _VertexShaderId))
        .Times(1);

      EXPECT_CALL(GLProxy, AttachShader(_ProgramId, _PixelShaderId))
        .Times(1);

      EXPECT_CALL(GLProxy, LinkProgram(_ProgramId))
        .Times(1);

      EXPECT_CALL(GLProxy, GetProgramiv(_ProgramId, GL_LINK_STATUS))
        .Times(1)
        .WillOnce(Return(GL_TRUE));
    };

    // **************** Cоздание объектов программ шейдеров ******************* //

    EXPECT_CALL(GLProxy, CreateProgram())
      .Times(0);

    VertexShaderRender0();

    TestCreateProgram(Program00Id, VertexShader0Id, PixelShader0Id);

    EXPECT_CALL(GLProxy, UseProgram(Program00Id))
      .Times(1);

    PixelShaderRender0();

    TestCreateProgram(Program10Id, VertexShader1Id, PixelShader0Id);

    EXPECT_CALL(GLProxy, UseProgram(Program10Id))
      .Times(1);

    VertexShaderRender1();

    TestCreateProgram(Program11Id, VertexShader1Id, PixelShader1Id);

    EXPECT_CALL(GLProxy, UseProgram(Program11Id))
      .Times(1);

    PixelShaderRender1();

    TestCreateProgram(Program01Id, VertexShader0Id, PixelShader1Id);

    EXPECT_CALL(GLProxy, UseProgram(Program01Id))
      .Times(1);

    VertexShaderRender0();

    // ********* Использование ранее созданных программ шейдеров ************ //

    EXPECT_CALL(GLProxy, CreateProgram())
      .Times(0);

    EXPECT_CALL(GLProxy, UseProgram(Program00Id))
      .Times(1);

    PixelShaderRender0();

    EXPECT_CALL(GLProxy, CreateProgram())
      .Times(0);

    EXPECT_CALL(GLProxy, UseProgram(Program10Id))
      .Times(1);

    VertexShaderRender1();

    EXPECT_CALL(GLProxy, CreateProgram())
      .Times(0);

    EXPECT_CALL(GLProxy, UseProgram(Program11Id))
      .Times(1);

    PixelShaderRender1();

    EXPECT_CALL(GLProxy, CreateProgram())
      .Times(0);

    EXPECT_CALL(GLProxy, UseProgram(Program01Id))
      .Times(1);

    VertexShaderRender0();

    EXPECT_CALL(GLProxy, DeleteProgram(Program01Id))
      .Times(1);

    EXPECT_CALL(GLProxy, DeleteProgram(Program11Id))
      .Times(1);

    EXPECT_CALL(GLProxy, DeleteShader(PixelShader1Id))
      .Times(1);
  }

  EXPECT_CALL(GLProxy, DeleteProgram(Program00Id))
    .Times(1);

  EXPECT_CALL(GLProxy, DeleteProgram(Program10Id))
    .Times(1);

  EXPECT_CALL(GLProxy, DeleteShader(PixelShader0Id))
    .Times(1);

  EXPECT_CALL(GLProxy, DeleteShader(VertexShader1Id))
    .Times(1);

  EXPECT_CALL(GLProxy, DeleteShader(VertexShader0Id))
    .Times(1);
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Shader_UsingProgram_LinkProgramFail)
{
  const ::std::string ShaderData =
    "Pixel vs(Vertex _Value)\r\n"
    "float4 ps(Pixel _Value)\r\n";

  const ::std::string ErrorText = "Error1908291952";

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto pVertexShader0 = Component_t::Make(
    {
      { uT("entry"), uT("vs") },
      { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
    });

  const auto pVertexShader1 = Component_t::Make(
    {
      { uT("entry"), uT("vs") },
      { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
    });

  const auto pPixelShader0 = Component_t::Make(
    {
      { uT("entry"), uT("ps") },
      { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
    });

  const auto pPixelShader1 = Component_t::Make(
    {
      { uT("entry"), uT("ps") },
      { uT("content"), ::std::vector<uint8_t>{ ShaderData.begin(), ShaderData.end() } },
    });

  const ::mock::GLuint VertexShader0Id = 1908251613;
  const ::mock::GLuint VertexShader1Id = 1908251614;
  const ::mock::GLuint PixelShader0Id = 1908251615;
  const ::mock::GLuint PixelShader1Id = 1908251616;

  const ::mock::GLuint Program00Id = 1908251641;
  const ::mock::GLuint Program01Id = 1908251652;
  const ::mock::GLuint Program10Id = 1908251653;
  const ::mock::GLuint Program11Id = 1908251654;

  using namespace ::testing;

  EXPECT_CALL(GLProxy, CreateShader(GL_VERTEX_SHADER))
    .Times(2)
    .WillOnce(Return(VertexShader0Id))
    .WillOnce(Return(VertexShader1Id));

  EXPECT_CALL(GLProxy, CreateShader(GL_FRAGMENT_SHADER))
    .Times(2)
    .WillOnce(Return(PixelShader0Id))
    .WillOnce(Return(PixelShader1Id));

  auto VertexShaderRender0 = itCreator->second(pVertexShader0);
  ASSERT_NE(nullptr, VertexShaderRender0);

  auto VertexShaderRender1 = itCreator->second(pVertexShader1);
  ASSERT_NE(nullptr, VertexShaderRender1);

  auto PixelShaderRender0 = itCreator->second(pPixelShader0);
  ASSERT_NE(nullptr, PixelShaderRender0);

  InSequence Dummy;

  {
    auto PixelShaderRender1 = itCreator->second(pPixelShader1);
    ASSERT_NE(nullptr, PixelShaderRender1);

    const auto TestCreateProgram = [&](
      const ::mock::GLuint _ProgramId,
      const ::mock::GLuint _VertexShaderId,
      const ::mock::GLuint _PixelShaderId,
      const Render_t & _Render)
    {
      EXPECT_CALL(GLProxy, CreateProgram())
        .Times(1)
        .WillOnce(Return(_ProgramId));

      EXPECT_CALL(GLProxy, AttachShader(_ProgramId, _VertexShaderId))
        .Times(1);

      EXPECT_CALL(GLProxy, AttachShader(_ProgramId, _PixelShaderId))
        .Times(1);

      EXPECT_CALL(GLProxy, LinkProgram(_ProgramId))
        .Times(1);

      EXPECT_CALL(GLProxy, GetProgramiv(_ProgramId, GL_LINK_STATUS))
        .Times(1)
        .WillOnce(Return(GL_FALSE));

      EXPECT_CALL(GLProxy, GetProgramInfoLog(_ProgramId, 512, nullptr))
        .Times(1)
        .WillOnce(Return(ErrorText.c_str()));

      EXPECT_STDEXCEPTION(_Render(),
        (".*Link program fail: " + ErrorText).c_str());
    };

    EXPECT_CALL(GLProxy, CreateProgram())
      .Times(0);

    EXPECT_CALL(GLProxy, UseProgram(_))
      .Times(0);

    VertexShaderRender0();

    TestCreateProgram(Program00Id, VertexShader0Id, PixelShader0Id,
      PixelShaderRender0);

    TestCreateProgram(Program10Id, VertexShader1Id, PixelShader0Id,
      VertexShaderRender1);

    TestCreateProgram(Program11Id, VertexShader1Id, PixelShader1Id,
      PixelShaderRender1);

    TestCreateProgram(Program01Id, VertexShader0Id, PixelShader1Id,
      VertexShaderRender0);

    EXPECT_CALL(GLProxy, DeleteProgram(Program01Id))
      .Times(1);

    EXPECT_CALL(GLProxy, DeleteProgram(Program11Id))
      .Times(1);
  }

  EXPECT_CALL(GLProxy, DeleteProgram(Program00Id))
    .Times(1);

  EXPECT_CALL(GLProxy, DeleteProgram(Program10Id))
    .Times(1);
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_Default)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint TextureId = 1812181809;
  const ::mock::GLint ProgramId = 1908221258;
  const ::mock::GLint LocationId = 1908221259;

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & _Source, const int _Width, const int _Height)
  {
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

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("TexDiffuse")))
      .Times(1)
      .WillOnce(Return(LocationId));

    EXPECT_CALL(GLProxy, Uniform1i(LocationId, 0))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  {
    ::std::vector<uint8_t> Source = 
    { 
      0x20, 0x06, 0x15, 0x12, 0x28,
      0x20, 0x06, 0x15, 0x12, 0x28,
      0x20, 0x06, 0x15, 0x12, 0x28,
      0x20, 0x06, 0x15, 0x12, 0x28,
      0x20, 0x06, 0x15, 0x12, 0x28,
      0x20, 0x06, 0x15, 0x12, 0x28,
      0x20, 0x06, 0x15, 0x12, 0x28,
      0x20, 0x06, 0x15, 0x12, 0x28,
    };
    IntroduceBufferSize(Source);

    const int Width = 1812181807;
    const int Height = 1812181808;

    const auto pTexture = Component_t::Make(
      {
        { uT("content"), Source },
        { uT("width"), Width },
        { uT("height"), Height },
      });

    TestCall(pTexture, Source, Width, Height);
  }

  {
    ::std::vector<uint8_t> Source = 
    { 
      0x20, 0x06, 0x15, 0x12, 0x29,
      0x20, 0x06, 0x15, 0x12, 0x29,
      0x20, 0x06, 0x15, 0x12, 0x29,
      0x20, 0x06, 0x15, 0x12, 0x29,
      0x20, 0x06, 0x15, 0x12, 0x29,
    };
    IntroduceBufferSize(Source);

    const int Width = 1907251057;
    const int Height = 1907251058;

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Texture") },
        { uT("content"), Source },
        { uT("width"), Width },
        { uT("height"), Height },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }), Source, Width, Height);
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_NameAndIndex)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint TextureId = 1908221839;
  const ::mock::GLint ProgramId = 1908221840;
  const ::mock::GLint LocationId = 1908221841;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & _Source,
    const int _Width, const int _Height,
    const ::std::size_t _Index,
    const ::std::string & _TexName)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

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

    EXPECT_CALL(GLProxy, GenFramebuffers(1))
      .Times(0);

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
      .Times(1)
      .WillOnce(Return(LocationId));

    EXPECT_CALL(GLProxy, GetUniformLocation(_, Eq("TexDiffuse")))
      .Times(0);

    EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Names =
  {
    { uT("TexEnvironment"), "TexEnvironment" },
    { uT("TexReflection"), "TexReflection" },
    { uT("TexBaseColor"), "TexBaseColor" },
  };

  for (int Index = 0; Index < Names.size(); Index++)
  {
    ::std::vector<uint8_t> Source =
    {
      0x20, 0x06, 0x15, 0x12, 0x30,
      0x20, 0x06, 0x15, 0x12, 0x30,
      0x20, 0x06, 0x15, 0x12, 0x30,
      0x20, 0x06, 0x15, 0x12, 0x30,
      0x20, 0x06, 0x15, 0x12, 0x30,
    };
    IntroduceBufferSize(Source);

    {
      const int Width = 1908221843;
      const int Height = 1908221844;

      const auto pTexture = Component_t::Make(
        {
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
        });

      TestCall(pTexture,
        Source, Width, Height, Index, Names[Index].second);
    }

    {
      const int Width = 1908221845;
      const int Height = 1908221847;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
        });

      TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, Index, Names[Index].second);
    }

    {
      const int Width = 1908221843;
      const int Height = 1908221844;

      const auto pTexture = Component_t::Make(
        {
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("mipmapping"), false },
        });

      TestCall(pTexture,
        Source, Width, Height, Index, Names[Index].second);
    }

    {
      const int Width = 1908221845;
      const int Height = 1908221847;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("mipmapping"), false },
        });

      TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, Index, Names[Index].second);
    }

    const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
    {
      { uT("albedo"),    "TexAlbedo" },
      { uT("metalness"), "TexMetalness" },
      { uT("roughness"), "TexRoughness" },
      { uT("normal"),    "TexNormal" },
      { uT("occlusion"), "TexOcclusion" },
      //{ uT("depth"),     "TexDepth" }, // другой формат
    };

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      {
        const int Width = 1908221843;
        const int Height = 1908221844;

        const auto pTexture = Component_t::Make(
          {
            { uT("content"), Source },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("name"), Names[Index].first },
            { uT("index"), Index },
            { uT("destination"), Destinations[i].first },
          });

        TestCall(pTexture,
          Source, Width, Height, Index, Names[Index].second);
      }

      {
        const int Width = 1908221845;
        const int Height = 1908221847;

        const auto pData = Component_t::Make(
          {
            { uT("kind"), uT("Texture") },
            { uT("content"), Source },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("name"), Names[Index].first },
            { uT("index"), Index },
            { uT("destination"), Destinations[i].first },
          });

        TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
          Source, Width, Height, Index, Names[Index].second);
      }

      {
        const int Width = 1908221843;
        const int Height = 1908221844;

        const auto pTexture = Component_t::Make(
          {
            { uT("content"), Source },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("name"), Names[Index].first },
            { uT("index"), Index },
            { uT("destination"), Destinations[i].first },
            { uT("mipmapping"), false },
          });

        TestCall(pTexture,
          Source, Width, Height, Index, Names[Index].second);
      }

      {
        const int Width = 1908221845;
        const int Height = 1908221847;

        const auto pData = Component_t::Make(
          {
            { uT("kind"), uT("Texture") },
            { uT("content"), Source },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("name"), Names[Index].first },
            { uT("index"), Index },
            { uT("destination"), Destinations[i].first },
            { uT("mipmapping"), false },
          });

        TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
          Source, Width, Height, Index, Names[Index].second);
      }
    }
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_NameAndIndex_Capacity16)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint TextureId = 1908221839;
  const ::mock::GLint ProgramId = 1908221840;
  const ::mock::GLint LocationId = 1908221841;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & _Source,
    const int _Width, const int _Height,
    const ::std::size_t _Index,
    const ::std::string & _TexName)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
      _Width, _Height, 0,
      GL_RGBA, GL_HALF_FLOAT, _Source))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    EXPECT_CALL(GLProxy, GenFramebuffers(1))
      .Times(0);

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
      .Times(1)
      .WillOnce(Return(LocationId));

    EXPECT_CALL(GLProxy, GetUniformLocation(_, Eq("TexDiffuse")))
      .Times(0);

    EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Names =
  {
    { uT("TexEnvironment"), "TexEnvironment" },
    { uT("TexReflection"), "TexReflection" },
    { uT("TexBaseColor"), "TexBaseColor" },
  };

  for (int Index = 0; Index < Names.size(); Index++)
  {
    ::std::vector<uint8_t> Source =
    {
      0x20, 0x06, 0x15, 0x12, 0x30,
      0x20, 0x06, 0x15, 0x12, 0x30,
      0x20, 0x06, 0x15, 0x12, 0x30,
      0x20, 0x06, 0x15, 0x12, 0x30,
      0x20, 0x06, 0x15, 0x12, 0x30,
    };
    IntroduceBufferSize(Source);

    {
      const int Width = 1908221843;
      const int Height = 1908221844;

      const auto pTexture = Component_t::Make(
        {
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("capacity"), 16 },
        });

      TestCall(pTexture,
        Source, Width, Height, Index, Names[Index].second);
    }

    {
      const int Width = 1908221845;
      const int Height = 1908221847;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("capacity"), 16 },
        });

      TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, Index, Names[Index].second);
    }

    {
      const int Width = 1908221843;
      const int Height = 1908221844;

      const auto pTexture = Component_t::Make(
        {
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("mipmapping"), false },
          { uT("capacity"), 16 },
        });

      TestCall(pTexture,
        Source, Width, Height, Index, Names[Index].second);
    }

    {
      const int Width = 1908221845;
      const int Height = 1908221847;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("mipmapping"), false },
          { uT("capacity"), 16 },
        });

      TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, Index, Names[Index].second);
    }

    const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
    {
      { uT("albedo"),    "TexAlbedo" },
      { uT("metalness"), "TexMetalness" },
      { uT("roughness"), "TexRoughness" },
      { uT("normal"),    "TexNormal" },
      { uT("occlusion"), "TexOcclusion" },
      //{ uT("depth"),     "TexDepth" }, // другой формат
    };

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      {
        const int Width = 1908221843;
        const int Height = 1908221844;

        const auto pTexture = Component_t::Make(
          {
            { uT("content"), Source },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("name"), Names[Index].first },
            { uT("index"), Index },
            { uT("destination"), Destinations[i].first },
            { uT("capacity"), 16 },
          });

        TestCall(pTexture,
          Source, Width, Height, Index, Names[Index].second);
      }

      {
        const int Width = 1908221845;
        const int Height = 1908221847;

        const auto pData = Component_t::Make(
          {
            { uT("kind"), uT("Texture") },
            { uT("content"), Source },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("name"), Names[Index].first },
            { uT("index"), Index },
            { uT("destination"), Destinations[i].first },
            { uT("capacity"), 16 },
          });

        TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
          Source, Width, Height, Index, Names[Index].second);
      }

      {
        const int Width = 1908221843;
        const int Height = 1908221844;

        const auto pTexture = Component_t::Make(
          {
            { uT("content"), Source },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("name"), Names[Index].first },
            { uT("index"), Index },
            { uT("destination"), Destinations[i].first },
            { uT("mipmapping"), false },
            { uT("capacity"), 16 },
          });

        TestCall(pTexture,
          Source, Width, Height, Index, Names[Index].second);
      }

      {
        const int Width = 1908221845;
        const int Height = 1908221847;

        const auto pData = Component_t::Make(
          {
            { uT("kind"), uT("Texture") },
            { uT("content"), Source },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("name"), Names[Index].first },
            { uT("index"), Index },
            { uT("destination"), Destinations[i].first },
            { uT("mipmapping"), false },
            { uT("capacity"), 16 },
          });

        TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
          Source, Width, Height, Index, Names[Index].second);
      }
    }
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_NameAndIndex_Capacity32)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint TextureId = 1908221839;
  const ::mock::GLint ProgramId = 1908221840;
  const ::mock::GLint LocationId = 1908221841;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & _Source,
    const int _Width, const int _Height,
    const ::std::size_t _Index,
    const ::std::string & _TexName)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
      _Width, _Height, 0,
      GL_RGBA, GL_FLOAT, _Source))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    EXPECT_CALL(GLProxy, GenFramebuffers(1))
      .Times(0);

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
      .Times(1)
      .WillOnce(Return(LocationId));

    EXPECT_CALL(GLProxy, GetUniformLocation(_, Eq("TexDiffuse")))
      .Times(0);

    EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Names =
  {
    { uT("TexEnvironment"), "TexEnvironment" },
    { uT("TexReflection"), "TexReflection" },
    { uT("TexBaseColor"), "TexBaseColor" },
  };

  for (int Index = 0; Index < Names.size(); Index++)
  {
    ::std::vector<uint8_t> Source =
    {
      0x20, 0x06, 0x15, 0x12, 0x30,
      0x20, 0x06, 0x15, 0x12, 0x30,
      0x20, 0x06, 0x15, 0x12, 0x30,
      0x20, 0x06, 0x15, 0x12, 0x30,
      0x20, 0x06, 0x15, 0x12, 0x30,
    };
    IntroduceBufferSize(Source);

    {
      const int Width = 1908221843;
      const int Height = 1908221844;

      const auto pTexture = Component_t::Make(
        {
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("capacity"), 32 },
        });

      TestCall(pTexture,
        Source, Width, Height, Index, Names[Index].second);
    }

    {
      const int Width = 1908221845;
      const int Height = 1908221847;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("capacity"), 32 },
        });

      TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, Index, Names[Index].second);
    }

    {
      const int Width = 1908221843;
      const int Height = 1908221844;

      const auto pTexture = Component_t::Make(
        {
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("mipmapping"), false },
          { uT("capacity"), 32 },
        });

      TestCall(pTexture,
        Source, Width, Height, Index, Names[Index].second);
    }

    {
      const int Width = 1908221845;
      const int Height = 1908221847;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("mipmapping"), false },
          { uT("capacity"), 32 },
        });

      TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, Index, Names[Index].second);
    }

    const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
    {
      { uT("albedo"),    "TexAlbedo" },
      { uT("metalness"), "TexMetalness" },
      { uT("roughness"), "TexRoughness" },
      { uT("normal"),    "TexNormal" },
      { uT("occlusion"), "TexOcclusion" },
      //{ uT("depth"),     "TexDepth" }, // другой формат
    };

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      {
        const int Width = 1908221843;
        const int Height = 1908221844;

        const auto pTexture = Component_t::Make(
          {
            { uT("content"), Source },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("name"), Names[Index].first },
            { uT("index"), Index },
            { uT("destination"), Destinations[i].first },
            { uT("capacity"), 32 },
          });

        TestCall(pTexture,
          Source, Width, Height, Index, Names[Index].second);
      }

      {
        const int Width = 1908221845;
        const int Height = 1908221847;

        const auto pData = Component_t::Make(
          {
            { uT("kind"), uT("Texture") },
            { uT("content"), Source },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("name"), Names[Index].first },
            { uT("index"), Index },
            { uT("destination"), Destinations[i].first },
            { uT("capacity"), 32 },
          });

        TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
          Source, Width, Height, Index, Names[Index].second);
      }

      {
        const int Width = 1908221843;
        const int Height = 1908221844;

        const auto pTexture = Component_t::Make(
          {
            { uT("content"), Source },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("name"), Names[Index].first },
            { uT("index"), Index },
            { uT("destination"), Destinations[i].first },
            { uT("mipmapping"), false },
            { uT("capacity"), 32 },
          });

        TestCall(pTexture,
          Source, Width, Height, Index, Names[Index].second);
      }

      {
        const int Width = 1908221845;
        const int Height = 1908221847;

        const auto pData = Component_t::Make(
          {
            { uT("kind"), uT("Texture") },
            { uT("content"), Source },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("name"), Names[Index].first },
            { uT("index"), Index },
            { uT("destination"), Destinations[i].first },
            { uT("mipmapping"), false },
            { uT("capacity"), 32 },
          });

        TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
          Source, Width, Height, Index, Names[Index].second);
      }
    }
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_Destination)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint TextureId = 1908221839;
  const ::mock::GLint ProgramId = 1908221840;
  const ::mock::GLint LocationId = 1908221841;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & _Source,
    const int _Width, const int _Height,
    const ::std::size_t _Index,
    const ::std::string & _TexName)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

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

    EXPECT_CALL(GLProxy, GenFramebuffers(1))
      .Times(0);

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
      .Times(1)
      .WillOnce(Return(LocationId));

    EXPECT_CALL(GLProxy, GetUniformLocation(_, Eq("TexDiffuse")))
      .Times(0);

    EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
  {
    { uT("albedo"),    "TexAlbedo" },
    { uT("metalness"), "TexMetalness" },
    { uT("roughness"), "TexRoughness" },
    { uT("normal"),    "TexNormal" },
    { uT("occlusion"), "TexOcclusion" },
    //{ uT("depth"),     "TexDepth" }, // другой формат
  };

  ::std::vector<uint8_t> Source =
  {
    0x20, 0x06, 0x15, 0x12, 0x30,
    0x20, 0x06, 0x15, 0x12, 0x30,
    0x20, 0x06, 0x15, 0x12, 0x30,
    0x20, 0x06, 0x15, 0x12, 0x30,
    0x20, 0x06, 0x15, 0x12, 0x30,
  };
  IntroduceBufferSize(Source);

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    {
      const int Width = 1908221843;
      const int Height = 1908221844;

      const auto pTexture = Component_t::Make(
        {
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("destination"), Destinations[i].first },
        });

      TestCall(pTexture,
        Source, Width, Height, i, Destinations[i].second);
    }

    {
      const int Width = 1908221845;
      const int Height = 1908221847;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("destination"), Destinations[i].first },
        });

      TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, i, Destinations[i].second);
    }

    {
      const int Width = 1908221843;
      const int Height = 1908221844;

      const auto pTexture = Component_t::Make(
        {
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("destination"), Destinations[i].first },
          { uT("mipmapping"), false },
        });

      TestCall(pTexture,
        Source, Width, Height, i, Destinations[i].second);
    }

    {
      const int Width = 1908221845;
      const int Height = 1908221847;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("destination"), Destinations[i].first },
          { uT("mipmapping"), false },
        });

      TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, i, Destinations[i].second);
    }
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test,/* DISABLED_*/Test_Texture_Destination_Capacity16)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint TextureId = 1908221839;
  const ::mock::GLint ProgramId = 1908221840;
  const ::mock::GLint LocationId = 1908221841;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & _Source,
    const int _Width, const int _Height,
    const ::std::size_t _Index,
    const ::std::string & _TexName)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
      _Width, _Height, 0,
      GL_RGBA, GL_HALF_FLOAT, _Source))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    EXPECT_CALL(GLProxy, GenFramebuffers(1))
      .Times(0);

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
      .Times(1)
      .WillOnce(Return(LocationId));

    EXPECT_CALL(GLProxy, GetUniformLocation(_, Eq("TexDiffuse")))
      .Times(0);

    EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
  {
    { uT("albedo"),    "TexAlbedo" },
    { uT("metalness"), "TexMetalness" },
    { uT("roughness"), "TexRoughness" },
    { uT("normal"),    "TexNormal" },
    { uT("occlusion"), "TexOcclusion" },
    //{ uT("depth"),     "TexDepth" }, // другой формат
  };

  ::std::vector<uint8_t> Source =
  {
    0x20, 0x06, 0x15, 0x12, 0x30,
    0x20, 0x06, 0x15, 0x12, 0x30,
    0x20, 0x06, 0x15, 0x12, 0x30,
    0x20, 0x06, 0x15, 0x12, 0x30,
    0x20, 0x06, 0x15, 0x12, 0x30,
  };
  IntroduceBufferSize(Source);

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    {
      const int Width = 1908221843;
      const int Height = 1908221844;

      const auto pTexture = Component_t::Make(
        {
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("destination"), Destinations[i].first },
          { uT("capacity"), 16 },
        });

      TestCall(pTexture,
        Source, Width, Height, i, Destinations[i].second);
    }

    {
      const int Width = 1908221845;
      const int Height = 1908221847;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("destination"), Destinations[i].first },
          { uT("capacity"), 16 },
        });

      TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, i, Destinations[i].second);
    }

    {
      const int Width = 1908221843;
      const int Height = 1908221844;

      const auto pTexture = Component_t::Make(
        {
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("destination"), Destinations[i].first },
          { uT("mipmapping"), false },
          { uT("capacity"), 16 },
        });

      TestCall(pTexture,
        Source, Width, Height, i, Destinations[i].second);
    }

    {
      const int Width = 1908221845;
      const int Height = 1908221847;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("destination"), Destinations[i].first },
          { uT("mipmapping"), false },
          { uT("capacity"), 16 },
        });

      TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, i, Destinations[i].second);
    }
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_Destination_Capacity32)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint TextureId = 1908221839;
  const ::mock::GLint ProgramId = 1908221840;
  const ::mock::GLint LocationId = 1908221841;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & _Source,
    const int _Width, const int _Height,
    const ::std::size_t _Index,
    const ::std::string & _TexName)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
      _Width, _Height, 0,
      GL_RGBA, GL_FLOAT, _Source))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    EXPECT_CALL(GLProxy, GenFramebuffers(1))
      .Times(0);

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
      .Times(1)
      .WillOnce(Return(LocationId));

    EXPECT_CALL(GLProxy, GetUniformLocation(_, Eq("TexDiffuse")))
      .Times(0);

    EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
  {
    { uT("albedo"),    "TexAlbedo" },
    { uT("metalness"), "TexMetalness" },
    { uT("roughness"), "TexRoughness" },
    { uT("normal"),    "TexNormal" },
    { uT("occlusion"), "TexOcclusion" },
    //{ uT("depth"),     "TexDepth" }, // другой формат
  };

  ::std::vector<uint8_t> Source =
  {
    0x20, 0x06, 0x15, 0x12, 0x30,
    0x20, 0x06, 0x15, 0x12, 0x30,
    0x20, 0x06, 0x15, 0x12, 0x30,
    0x20, 0x06, 0x15, 0x12, 0x30,
    0x20, 0x06, 0x15, 0x12, 0x30,
  };
  IntroduceBufferSize(Source);

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    {
      const int Width = 1908221843;
      const int Height = 1908221844;

      const auto pTexture = Component_t::Make(
        {
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("destination"), Destinations[i].first },
          { uT("capacity"), 32 },
        });

      TestCall(pTexture,
        Source, Width, Height, i, Destinations[i].second);
    }

    {
      const int Width = 1908221845;
      const int Height = 1908221847;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("destination"), Destinations[i].first },
          { uT("capacity"), 32 },
        });

      TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, i, Destinations[i].second);
    }

    {
      const int Width = 1908221843;
      const int Height = 1908221844;

      const auto pTexture = Component_t::Make(
        {
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("destination"), Destinations[i].first },
          { uT("mipmapping"), false },
          { uT("capacity"), 32 },
        });

      TestCall(pTexture,
        Source, Width, Height, i, Destinations[i].second);
    }

    {
      const int Width = 1908221845;
      const int Height = 1908221847;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("destination"), Destinations[i].first },
          { uT("mipmapping"), false },
          { uT("capacity"), 32 },
        });

      TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, i, Destinations[i].second);
    }
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_Mipmapping_NameAndIndex)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint TextureId = 1908221839;
  const ::mock::GLint ProgramId = 1908221840;
  const ::mock::GLint LocationId = 1908221841;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & _Source,
    const int _Width, const int _Height,
    const ::std::size_t _Index,
    const ::std::string & _TexName)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

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

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GenerateMipmap(GL_TEXTURE_2D))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GenFramebuffers(1))
      .Times(0);

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
      .Times(1)
      .WillOnce(Return(LocationId));

    EXPECT_CALL(GLProxy, GetUniformLocation(_, Eq("TexDiffuse")))
      .Times(0);

    EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Names =
  {
    { uT("TexEnvironment"), "TexEnvironment" },
    { uT("TexReflection"), "TexReflection" },
    { uT("TexBaseColor"), "TexBaseColor" },
  };

  for (int Index = 0; Index < Names.size(); Index++)
  {
    ::std::vector<uint8_t> Source =
    {
      0x20, 0x06, 0x15, 0x12, 0x31,
      0x20, 0x06, 0x15, 0x12, 0x31,
      0x20, 0x06, 0x15, 0x12, 0x31,
      0x20, 0x06, 0x15, 0x12, 0x31,
      0x20, 0x06, 0x15, 0x12, 0x31,
    };
    IntroduceBufferSize(Source);

    {
      const int Width = 1908221843;
      const int Height = 1908221844;

      const auto pTexture = Component_t::Make(
        {
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("mipmapping"), true },
        });

      TestCall(pTexture,
        Source, Width, Height, Index, Names[Index].second);
    }

    {
      const int Width = 1908221845;
      const int Height = 1908221847;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("mipmapping"), true },
        });

      TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, Index, Names[Index].second);
    }

    const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
    {
      { uT("albedo"),    "TexAlbedo" },
      { uT("metalness"), "TexMetalness" },
      { uT("roughness"), "TexRoughness" },
      { uT("normal"),    "TexNormal" },
      { uT("occlusion"), "TexOcclusion" },
    };

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      {
        const int Width = 1908221843;
        const int Height = 1908221844;

        const auto pTexture = Component_t::Make(
          {
            { uT("content"), Source },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("name"), Names[Index].first },
            { uT("index"), Index },
            { uT("destination"), Destinations[i].first },
            { uT("mipmapping"), true },
          });

        TestCall(pTexture,
          Source, Width, Height, Index, Names[Index].second);
      }

      {
        const int Width = 1908221845;
        const int Height = 1908221847;

        const auto pData = Component_t::Make(
          {
            { uT("kind"), uT("Texture") },
            { uT("content"), Source },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("name"), Names[Index].first },
            { uT("index"), Index },
            { uT("destination"), Destinations[i].first },
            { uT("mipmapping"), true },
          });

        TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
          Source, Width, Height, Index, Names[Index].second);
      }
    }
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_Mipmapping_NameAndIndex_Capacity16)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint TextureId = 1908221839;
  const ::mock::GLint ProgramId = 1908221840;
  const ::mock::GLint LocationId = 1908221841;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & _Source,
    const int _Width, const int _Height,
    const ::std::size_t _Index,
    const ::std::string & _TexName)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
      _Width, _Height, 0,
      GL_RGBA, GL_HALF_FLOAT, _Source))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GenerateMipmap(GL_TEXTURE_2D))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GenFramebuffers(1))
      .Times(0);

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
      .Times(1)
      .WillOnce(Return(LocationId));

    EXPECT_CALL(GLProxy, GetUniformLocation(_, Eq("TexDiffuse")))
      .Times(0);

    EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Names =
  {
    { uT("TexEnvironment"), "TexEnvironment" },
    { uT("TexReflection"), "TexReflection" },
    { uT("TexBaseColor"), "TexBaseColor" },
  };

  for (int Index = 0; Index < Names.size(); Index++)
  {
    ::std::vector<uint8_t> Source =
    {
      0x20, 0x06, 0x15, 0x12, 0x31,
      0x20, 0x06, 0x15, 0x12, 0x31,
      0x20, 0x06, 0x15, 0x12, 0x31,
      0x20, 0x06, 0x15, 0x12, 0x31,
      0x20, 0x06, 0x15, 0x12, 0x31,
    };
    IntroduceBufferSize(Source);

    {
      const int Width = 1908221843;
      const int Height = 1908221844;

      const auto pTexture = Component_t::Make(
        {
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("mipmapping"), true },
          { uT("capacity"), 16 },
        });

      TestCall(pTexture,
        Source, Width, Height, Index, Names[Index].second);
    }

    {
      const int Width = 1908221845;
      const int Height = 1908221847;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("mipmapping"), true },
          { uT("capacity"), 16 },
        });

      TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, Index, Names[Index].second);
    }

    const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
    {
      { uT("albedo"),    "TexAlbedo" },
      { uT("metalness"), "TexMetalness" },
      { uT("roughness"), "TexRoughness" },
      { uT("normal"),    "TexNormal" },
      { uT("occlusion"), "TexOcclusion" },
    };

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      {
        const int Width = 1908221843;
        const int Height = 1908221844;

        const auto pTexture = Component_t::Make(
          {
            { uT("content"), Source },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("name"), Names[Index].first },
            { uT("index"), Index },
            { uT("destination"), Destinations[i].first },
            { uT("mipmapping"), true },
            { uT("capacity"), 16 },
          });

        TestCall(pTexture,
          Source, Width, Height, Index, Names[Index].second);
      }

      {
        const int Width = 1908221845;
        const int Height = 1908221847;

        const auto pData = Component_t::Make(
          {
            { uT("kind"), uT("Texture") },
            { uT("content"), Source },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("name"), Names[Index].first },
            { uT("index"), Index },
            { uT("destination"), Destinations[i].first },
            { uT("mipmapping"), true },
            { uT("capacity"), 16 },
          });

        TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
          Source, Width, Height, Index, Names[Index].second);
      }
    }
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_Mipmapping_NameAndIndex_Capacity32)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint TextureId = 1908221839;
  const ::mock::GLint ProgramId = 1908221840;
  const ::mock::GLint LocationId = 1908221841;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & _Source,
    const int _Width, const int _Height,
    const ::std::size_t _Index,
    const ::std::string & _TexName)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
      _Width, _Height, 0,
      GL_RGBA, GL_FLOAT, _Source))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GenerateMipmap(GL_TEXTURE_2D))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GenFramebuffers(1))
      .Times(0);

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
      .Times(1)
      .WillOnce(Return(LocationId));

    EXPECT_CALL(GLProxy, GetUniformLocation(_, Eq("TexDiffuse")))
      .Times(0);

    EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Names =
  {
    { uT("TexEnvironment"), "TexEnvironment" },
    { uT("TexReflection"), "TexReflection" },
    { uT("TexBaseColor"), "TexBaseColor" },
  };

  for (int Index = 0; Index < Names.size(); Index++)
  {
    ::std::vector<uint8_t> Source =
    {
      0x20, 0x06, 0x15, 0x12, 0x31,
      0x20, 0x06, 0x15, 0x12, 0x31,
      0x20, 0x06, 0x15, 0x12, 0x31,
      0x20, 0x06, 0x15, 0x12, 0x31,
      0x20, 0x06, 0x15, 0x12, 0x31,
    };
    IntroduceBufferSize(Source);

    {
      const int Width = 1908221843;
      const int Height = 1908221844;

      const auto pTexture = Component_t::Make(
        {
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("mipmapping"), true },
          { uT("capacity"), 32 },
        });

      TestCall(pTexture,
        Source, Width, Height, Index, Names[Index].second);
    }

    {
      const int Width = 1908221845;
      const int Height = 1908221847;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("mipmapping"), true },
          { uT("capacity"), 32 },
        });

      TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, Index, Names[Index].second);
    }

    const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
    {
      { uT("albedo"),    "TexAlbedo" },
      { uT("metalness"), "TexMetalness" },
      { uT("roughness"), "TexRoughness" },
      { uT("normal"),    "TexNormal" },
      { uT("occlusion"), "TexOcclusion" },
    };

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      {
        const int Width = 1908221843;
        const int Height = 1908221844;

        const auto pTexture = Component_t::Make(
          {
            { uT("content"), Source },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("name"), Names[Index].first },
            { uT("index"), Index },
            { uT("destination"), Destinations[i].first },
            { uT("mipmapping"), true },
            { uT("capacity"), 32 },
          });

        TestCall(pTexture,
          Source, Width, Height, Index, Names[Index].second);
      }

      {
        const int Width = 1908221845;
        const int Height = 1908221847;

        const auto pData = Component_t::Make(
          {
            { uT("kind"), uT("Texture") },
            { uT("content"), Source },
            { uT("width"), Width },
            { uT("height"), Height },
            { uT("name"), Names[Index].first },
            { uT("index"), Index },
            { uT("destination"), Destinations[i].first },
            { uT("mipmapping"), true },
            { uT("capacity"), 32 },
          });

        TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
          Source, Width, Height, Index, Names[Index].second);
      }
    }
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_Mipmapping_Destination)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint TextureId = 1908221839;
  const ::mock::GLint ProgramId = 1908221840;
  const ::mock::GLint LocationId = 1908221841;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & _Source,
    const int _Width, const int _Height,
    const ::std::size_t _Index,
    const ::std::string & _TexName)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

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

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GenerateMipmap(GL_TEXTURE_2D))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GenFramebuffers(1))
      .Times(0);

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
      .Times(1)
      .WillOnce(Return(LocationId));

    EXPECT_CALL(GLProxy, GetUniformLocation(_, Eq("TexDiffuse")))
      .Times(0);

    EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
  {
    { uT("albedo"),    "TexAlbedo" },
    { uT("metalness"), "TexMetalness" },
    { uT("roughness"), "TexRoughness" },
    { uT("normal"),    "TexNormal" },
    { uT("occlusion"), "TexOcclusion" },
  };

  ::std::vector<uint8_t> Source =
  {
    0x20, 0x06, 0x15, 0x12, 0x31,
    0x20, 0x06, 0x15, 0x12, 0x31,
    0x20, 0x06, 0x15, 0x12, 0x31,
    0x20, 0x06, 0x15, 0x12, 0x31,
    0x20, 0x06, 0x15, 0x12, 0x31,
  };
  IntroduceBufferSize(Source);

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    {
      const int Width = 1908221843;
      const int Height = 1908221844;

      const auto pTexture = Component_t::Make(
        {
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("destination"), Destinations[i].first },
          { uT("mipmapping"), true },
        });

      TestCall(pTexture,
        Source, Width, Height, i, Destinations[i].second);
    }

    {
      const int Width = 1908221845;
      const int Height = 1908221847;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("destination"), Destinations[i].first },
          { uT("mipmapping"), true },
        });

      TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, i, Destinations[i].second);
    }
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_Mipmapping_Destination_Capacity16)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint TextureId = 1908221839;
  const ::mock::GLint ProgramId = 1908221840;
  const ::mock::GLint LocationId = 1908221841;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & _Source,
    const int _Width, const int _Height,
    const ::std::size_t _Index,
    const ::std::string & _TexName)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
      _Width, _Height, 0,
      GL_RGBA, GL_HALF_FLOAT, _Source))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GenerateMipmap(GL_TEXTURE_2D))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GenFramebuffers(1))
      .Times(0);

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
      .Times(1)
      .WillOnce(Return(LocationId));

    EXPECT_CALL(GLProxy, GetUniformLocation(_, Eq("TexDiffuse")))
      .Times(0);

    EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
  {
    { uT("albedo"),    "TexAlbedo" },
    { uT("metalness"), "TexMetalness" },
    { uT("roughness"), "TexRoughness" },
    { uT("normal"),    "TexNormal" },
    { uT("occlusion"), "TexOcclusion" },
  };

  ::std::vector<uint8_t> Source =
  {
    0x20, 0x06, 0x15, 0x12, 0x31,
    0x20, 0x06, 0x15, 0x12, 0x31,
    0x20, 0x06, 0x15, 0x12, 0x31,
    0x20, 0x06, 0x15, 0x12, 0x31,
    0x20, 0x06, 0x15, 0x12, 0x31,
  };
  IntroduceBufferSize(Source);

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    {
      const int Width = 1908221843;
      const int Height = 1908221844;

      const auto pTexture = Component_t::Make(
        {
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("destination"), Destinations[i].first },
          { uT("mipmapping"), true },
          { uT("capacity"), 16 },
        });

      TestCall(pTexture,
        Source, Width, Height, i, Destinations[i].second);
    }

    {
      const int Width = 1908221845;
      const int Height = 1908221847;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("destination"), Destinations[i].first },
          { uT("mipmapping"), true },
          { uT("capacity"), 16 },
        });

      TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, i, Destinations[i].second);
    }
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_Mipmapping_Destination_Capacity32)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint TextureId = 1908221839;
  const ::mock::GLint ProgramId = 1908221840;
  const ::mock::GLint LocationId = 1908221841;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & _Source,
    const int _Width, const int _Height,
    const ::std::size_t _Index,
    const ::std::string & _TexName)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
      _Width, _Height, 0,
      GL_RGBA, GL_FLOAT, _Source))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GenerateMipmap(GL_TEXTURE_2D))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GenFramebuffers(1))
      .Times(0);

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
      .Times(1)
      .WillOnce(Return(LocationId));

    EXPECT_CALL(GLProxy, GetUniformLocation(_, Eq("TexDiffuse")))
      .Times(0);

    EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
  {
    { uT("albedo"),    "TexAlbedo" },
    { uT("metalness"), "TexMetalness" },
    { uT("roughness"), "TexRoughness" },
    { uT("normal"),    "TexNormal" },
    { uT("occlusion"), "TexOcclusion" },
  };

  ::std::vector<uint8_t> Source =
  {
    0x20, 0x06, 0x15, 0x12, 0x31,
    0x20, 0x06, 0x15, 0x12, 0x31,
    0x20, 0x06, 0x15, 0x12, 0x31,
    0x20, 0x06, 0x15, 0x12, 0x31,
    0x20, 0x06, 0x15, 0x12, 0x31,
  };
  IntroduceBufferSize(Source);

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    {
      const int Width = 1908221843;
      const int Height = 1908221844;

      const auto pTexture = Component_t::Make(
        {
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("destination"), Destinations[i].first },
          { uT("mipmapping"), true },
          { uT("capacity"), 32 },
        });

      TestCall(pTexture,
        Source, Width, Height, i, Destinations[i].second);
    }

    {
      const int Width = 1908221845;
      const int Height = 1908221847;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("destination"), Destinations[i].first },
          { uT("mipmapping"), true },
          { uT("capacity"), 32 },
        });

      TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, i, Destinations[i].second);
    }
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_ReadData_NameAndIndex)
{
  using Texture_t = ::covellite::api::renderer::OpenGLCommon::Texture;
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<const void>;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  class MapperProxy :
    public ::alicorn::extension::testing::Proxy<MapperProxy>
  {
  public:
    MOCK_METHOD1(Map, bool(const void *));
    MOCK_METHOD1(CheckData, void(::std::vector<uint32_t>));
  };

  MapperProxy oMapperProxy;
  MapperProxy::GetInstance() = &oMapperProxy;

  const int Width = 3;
  const int Height = 4;

  const BufferMapper_t Mapper = [&](const void * _pData)
  {
    if (_pData != nullptr)
    {
      ::std::vector<uint32_t> Data(Width * Height);
      memcpy(Data.data(), _pData, Width * Height * 4);
      oMapperProxy.CheckData(Data);
    }

    return oMapperProxy.Map(_pData);
  };

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLint TextureId = 1910221154;
  const ::mock::GLint ProgramId = 1910221155;
  const ::mock::GLint LocationId = 1910221156;
  const ::mock::GLint FrameBufferId = 1910221209;
  const ::mock::GLint DefaultFrameBufferId = 1910221236;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pDataTexture,
    const ::std::size_t _Index,
    const ::std::string & _TexName,
    const ::std::vector<uint32_t> & _RawData,
    const ::std::vector<uint32_t> & _ExpectData)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

    ::covellite::api::renderer::Component::Texture TextureData{ *_pDataTexture, uT("") };

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
      Width, Height, 0,
      GL_RGBA, GL_UNSIGNED_BYTE, ::std::vector<uint8_t>{}))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    const auto pTexture = ::std::make_shared<Texture_t>(TextureData);
    ASSERT_EQ(Width * Height * 4, pTexture->m_ReadCopyData.size());

    _pDataTexture->SetValue(uT("entity"), pTexture);

    EXPECT_CALL(GLProxy, GenFramebuffers(1))
      .Times(1)
      .WillOnce(Return(FrameBufferId));

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(0);

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("service"), Object_t{ _pDataTexture } }
      }));
    ASSERT_NE(nullptr, Render);

    const auto TestCallActivateTexture = [&](void)
    {
      EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
        .Times(1);

      EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
        .Times(1);

      EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
        .Times(1)
        .WillOnce(Return(&ProgramId));

      EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
        .Times(1)
        .WillOnce(Return(LocationId));

      EXPECT_CALL(GLProxy, GetUniformLocation(_, Eq("TexDiffuse")))
        .Times(0);

      EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
        .Times(1);
    };

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(oMapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(false));

    EXPECT_CALL(GLProxy, BindFramebuffer(_, _))
      .Times(0);

    TestCallActivateTexture();
    Render();

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(oMapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(true));

    EXPECT_CALL(GLProxy, GetIntegerv(GL_FRAMEBUFFER_BINDING))
      .Times(1)
      .WillOnce(Return(&DefaultFrameBufferId));

    EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, FrameBufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, FramebufferTexture2D(GL_FRAMEBUFFER,
      GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureId, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetReadPixelsRawData())
      .Times(1)
      .WillOnce(Return(_RawData));

    EXPECT_CALL(GLProxy, ReadPixels(0, 0, Width, Height, GL_RGBA, 
      GL_UNSIGNED_BYTE, pTexture->m_ReadCopyData.data()))
      .Times(1);

    EXPECT_CALL(oMapperProxy, CheckData(_ExpectData))
      .Times(1);

    EXPECT_CALL(oMapperProxy, Map(pTexture->m_ReadCopyData.data()))
      .Times(1);

    EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, DefaultFrameBufferId))
      .Times(1);

    TestCallActivateTexture();
    Render();

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(GLProxy, DeleteFramebuffers(1, FrameBufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Names =
  {
    { uT("TexEnvironment"), "TexEnvironment" },
    { uT("TexReflection"), "TexReflection" },
    { uT("TexBaseColor"), "TexBaseColor" },
  };

  for (int Index = 0; Index < Names.size(); Index++)
  {
    const ::std::vector<uint32_t> RawData =
    {
      0x10221334, 0x10221335, 0x10221336,
      0x10221337, 0x10221338, 0x10221339,
      0x10221340, 0x10221341, 0x10221342,
      0x10221343, 0x10221344, 0x10221345,
    };

    const ::std::vector<uint32_t> ExpectData =
    {
      0x10221343, 0x10221344, 0x10221345,
      0x10221340, 0x10221341, 0x10221342,
      0x10221337, 0x10221338, 0x10221339,
      0x10221334, 0x10221335, 0x10221336,
    };

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Texture") },
        { uT("width"), Width },
        { uT("height"), Height },
        { uT("mapper"), Mapper },
        { uT("name"), Names[Index].first },
        { uT("index"), Index },
      });

    TestCall(pData, Index, Names[Index].second, RawData, ExpectData);

    const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
    {
      { uT("albedo"),    "TexAlbedo" },
      { uT("metalness"), "TexMetalness" },
      { uT("roughness"), "TexRoughness" },
      { uT("normal"),    "TexNormal" },
      { uT("occlusion"), "TexOcclusion" },
    };

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("mapper"), Mapper },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("destination"), Destinations[i].first },
        });

      TestCall(pData, Index, Names[Index].second, RawData, ExpectData);
    }
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_ReadData_NameAndIndex_Capacity16)
{
  using Texture_t = ::covellite::api::renderer::OpenGLCommon::Texture;
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<const void>;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  class MapperProxy :
    public ::alicorn::extension::testing::Proxy<MapperProxy>
  {
  public:
    MOCK_METHOD1(Map, bool(const void *));
    MOCK_METHOD1(CheckData, void(::std::vector<uint32_t>));
  };

  MapperProxy oMapperProxy;
  MapperProxy::GetInstance() = &oMapperProxy;

  const int Width = 3;
  const int Height = 4;

  const BufferMapper_t Mapper = [&](const void * _pData)
  {
    if (_pData != nullptr)
    {
      ::std::vector<uint32_t> Data(Width * Height);
      memcpy(Data.data(), _pData, Width * Height * 4);
      oMapperProxy.CheckData(Data);
    }

    return oMapperProxy.Map(_pData);
  };

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLint TextureId = 1910221154;
  const ::mock::GLint ProgramId = 1910221155;
  const ::mock::GLint LocationId = 1910221156;
  const ::mock::GLint FrameBufferId = 1910221209;
  const ::mock::GLint DefaultFrameBufferId = 1910221236;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pDataTexture,
    const ::std::size_t _Index,
    const ::std::string & _TexName,
    const ::std::vector<uint32_t> & _RawData,
    const ::std::vector<uint32_t> & _ExpectData)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

    ::covellite::api::renderer::Component::Texture TextureData{ *_pDataTexture, uT("") };

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
      Width, Height, 0,
      GL_RGBA, GL_HALF_FLOAT, ::std::vector<uint8_t>{}))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    const auto pTexture = ::std::make_shared<Texture_t>(TextureData);
    ASSERT_EQ(Width * Height * 4, pTexture->m_ReadCopyData.size());

    _pDataTexture->SetValue(uT("entity"), pTexture);

    EXPECT_CALL(GLProxy, GenFramebuffers(1))
      .Times(1)
      .WillOnce(Return(FrameBufferId));

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(0);

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("service"), Object_t{ _pDataTexture } }
      }));
    ASSERT_NE(nullptr, Render);

    const auto TestCallActivateTexture = [&](void)
    {
      EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
        .Times(1);

      EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
        .Times(1);

      EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
        .Times(1)
        .WillOnce(Return(&ProgramId));

      EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
        .Times(1)
        .WillOnce(Return(LocationId));

      EXPECT_CALL(GLProxy, GetUniformLocation(_, Eq("TexDiffuse")))
        .Times(0);

      EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
        .Times(1);
    };

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(oMapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(false));

    EXPECT_CALL(GLProxy, BindFramebuffer(_, _))
      .Times(0);

    TestCallActivateTexture();
    Render();

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(oMapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(true));

    EXPECT_CALL(GLProxy, GetIntegerv(GL_FRAMEBUFFER_BINDING))
      .Times(1)
      .WillOnce(Return(&DefaultFrameBufferId));

    EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, FrameBufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, FramebufferTexture2D(GL_FRAMEBUFFER,
      GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureId, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetReadPixelsRawData())
      .Times(1)
      .WillOnce(Return(_RawData));

    EXPECT_CALL(GLProxy, ReadPixels(0, 0, Width, Height, GL_RGBA,
      GL_UNSIGNED_BYTE, pTexture->m_ReadCopyData.data()))
      .Times(1);

    EXPECT_CALL(oMapperProxy, CheckData(_ExpectData))
      .Times(1);

    EXPECT_CALL(oMapperProxy, Map(pTexture->m_ReadCopyData.data()))
      .Times(1);

    EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, DefaultFrameBufferId))
      .Times(1);

    TestCallActivateTexture();
    Render();

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(GLProxy, DeleteFramebuffers(1, FrameBufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Names =
  {
    { uT("TexEnvironment"), "TexEnvironment" },
    { uT("TexReflection"), "TexReflection" },
    { uT("TexBaseColor"), "TexBaseColor" },
  };

  for (int Index = 0; Index < Names.size(); Index++)
  {
    const ::std::vector<uint32_t> RawData =
    {
      0x10221334, 0x10221335, 0x10221336,
      0x10221337, 0x10221338, 0x10221339,
      0x10221340, 0x10221341, 0x10221342,
      0x10221343, 0x10221344, 0x10221345,
    };

    const ::std::vector<uint32_t> ExpectData =
    {
      0x10221343, 0x10221344, 0x10221345,
      0x10221340, 0x10221341, 0x10221342,
      0x10221337, 0x10221338, 0x10221339,
      0x10221334, 0x10221335, 0x10221336,
    };

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Texture") },
        { uT("width"), Width },
        { uT("height"), Height },
        { uT("mapper"), Mapper },
        { uT("name"), Names[Index].first },
        { uT("index"), Index },
        { uT("capacity"), 16 },
      });

    TestCall(pData, Index, Names[Index].second, RawData, ExpectData);

    const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
    {
      { uT("albedo"),    "TexAlbedo" },
      { uT("metalness"), "TexMetalness" },
      { uT("roughness"), "TexRoughness" },
      { uT("normal"),    "TexNormal" },
      { uT("occlusion"), "TexOcclusion" },
    };

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("mapper"), Mapper },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("destination"), Destinations[i].first },
          { uT("capacity"), 16 },
        });

      TestCall(pData, Index, Names[Index].second, RawData, ExpectData);
    }
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_ReadData_NameAndIndex_Capacity32)
{
  using Texture_t = ::covellite::api::renderer::OpenGLCommon::Texture;
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<const void>;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  class MapperProxy :
    public ::alicorn::extension::testing::Proxy<MapperProxy>
  {
  public:
    MOCK_METHOD1(Map, bool(const void *));
    MOCK_METHOD1(CheckData, void(::std::vector<uint32_t>));
  };

  MapperProxy oMapperProxy;
  MapperProxy::GetInstance() = &oMapperProxy;

  const int Width = 3;
  const int Height = 4;

  const BufferMapper_t Mapper = [&](const void * _pData)
  {
    if (_pData != nullptr)
    {
      ::std::vector<uint32_t> Data(Width * Height);
      memcpy(Data.data(), _pData, Width * Height * 4);
      oMapperProxy.CheckData(Data);
    }

    return oMapperProxy.Map(_pData);
  };

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLint TextureId = 1910221154;
  const ::mock::GLint ProgramId = 1910221155;
  const ::mock::GLint LocationId = 1910221156;
  const ::mock::GLint FrameBufferId = 1910221209;
  const ::mock::GLint DefaultFrameBufferId = 1910221236;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pDataTexture,
    const ::std::size_t _Index,
    const ::std::string & _TexName,
    const ::std::vector<uint32_t> & _RawData,
    const ::std::vector<uint32_t> & _ExpectData)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

    ::covellite::api::renderer::Component::Texture TextureData{ *_pDataTexture, uT("") };

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
      Width, Height, 0,
      GL_RGBA, GL_FLOAT, ::std::vector<uint8_t>{}))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    const auto pTexture = ::std::make_shared<Texture_t>(TextureData);
    ASSERT_EQ(Width * Height * 4, pTexture->m_ReadCopyData.size());

    _pDataTexture->SetValue(uT("entity"), pTexture);

    EXPECT_CALL(GLProxy, GenFramebuffers(1))
      .Times(1)
      .WillOnce(Return(FrameBufferId));

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(0);

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("service"), Object_t{ _pDataTexture } }
      }));
    ASSERT_NE(nullptr, Render);

    const auto TestCallActivateTexture = [&](void)
    {
      EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
        .Times(1);

      EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
        .Times(1);

      EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
        .Times(1)
        .WillOnce(Return(&ProgramId));

      EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
        .Times(1)
        .WillOnce(Return(LocationId));

      EXPECT_CALL(GLProxy, GetUniformLocation(_, Eq("TexDiffuse")))
        .Times(0);

      EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
        .Times(1);
    };

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(oMapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(false));

    EXPECT_CALL(GLProxy, BindFramebuffer(_, _))
      .Times(0);

    TestCallActivateTexture();
    Render();

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(oMapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(true));

    EXPECT_CALL(GLProxy, GetIntegerv(GL_FRAMEBUFFER_BINDING))
      .Times(1)
      .WillOnce(Return(&DefaultFrameBufferId));

    EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, FrameBufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, FramebufferTexture2D(GL_FRAMEBUFFER,
      GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureId, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetReadPixelsRawData())
      .Times(1)
      .WillOnce(Return(_RawData));

    EXPECT_CALL(GLProxy, ReadPixels(0, 0, Width, Height, GL_RGBA,
      GL_UNSIGNED_BYTE, pTexture->m_ReadCopyData.data()))
      .Times(1);

    EXPECT_CALL(oMapperProxy, CheckData(_ExpectData))
      .Times(1);

    EXPECT_CALL(oMapperProxy, Map(pTexture->m_ReadCopyData.data()))
      .Times(1);

    EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, DefaultFrameBufferId))
      .Times(1);

    TestCallActivateTexture();
    Render();

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(GLProxy, DeleteFramebuffers(1, FrameBufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Names =
  {
    { uT("TexEnvironment"), "TexEnvironment" },
    { uT("TexReflection"), "TexReflection" },
    { uT("TexBaseColor"), "TexBaseColor" },
  };

  for (int Index = 0; Index < Names.size(); Index++)
  {
    const ::std::vector<uint32_t> RawData =
    {
      0x10221334, 0x10221335, 0x10221336,
      0x10221337, 0x10221338, 0x10221339,
      0x10221340, 0x10221341, 0x10221342,
      0x10221343, 0x10221344, 0x10221345,
    };

    const ::std::vector<uint32_t> ExpectData =
    {
      0x10221343, 0x10221344, 0x10221345,
      0x10221340, 0x10221341, 0x10221342,
      0x10221337, 0x10221338, 0x10221339,
      0x10221334, 0x10221335, 0x10221336,
    };

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Texture") },
        { uT("width"), Width },
        { uT("height"), Height },
        { uT("mapper"), Mapper },
        { uT("name"), Names[Index].first },
        { uT("index"), Index },
        { uT("capacity"), 32 },
      });

    TestCall(pData, Index, Names[Index].second, RawData, ExpectData);

    const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
    {
      { uT("albedo"),    "TexAlbedo" },
      { uT("metalness"), "TexMetalness" },
      { uT("roughness"), "TexRoughness" },
      { uT("normal"),    "TexNormal" },
      { uT("occlusion"), "TexOcclusion" },
    };

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("mapper"), Mapper },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("destination"), Destinations[i].first },
          { uT("capacity"), 32 },
        });

      TestCall(pData, Index, Names[Index].second, RawData, ExpectData);
    }
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_ReadData_Destination)
{
  using Texture_t = ::covellite::api::renderer::OpenGLCommon::Texture;
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<const void>;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  class MapperProxy :
    public ::alicorn::extension::testing::Proxy<MapperProxy>
  {
  public:
    MOCK_METHOD1(Map, bool(const void *));
    MOCK_METHOD1(CheckData, void(::std::vector<uint32_t>));
  };

  MapperProxy oMapperProxy;
  MapperProxy::GetInstance() = &oMapperProxy;

  const int Width = 3;
  const int Height = 4;

  const BufferMapper_t Mapper = [&](const void * _pData)
  {
    if (_pData != nullptr)
    {
      ::std::vector<uint32_t> Data(Width * Height);
      memcpy(Data.data(), _pData, Width * Height * 4);
      oMapperProxy.CheckData(Data);
    }

    return oMapperProxy.Map(_pData);
  };

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLint TextureId = 1910221154;
  const ::mock::GLint ProgramId = 1910221155;
  const ::mock::GLint LocationId = 1910221156;
  const ::mock::GLint FrameBufferId = 1910221209;
  const ::mock::GLint DefaultFrameBufferId = 1910221236;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pDataTexture,
    const ::std::size_t _Index,
    const ::std::string & _TexName,
    const ::std::vector<uint32_t> & _RawData,
    const ::std::vector<uint32_t> & _ExpectData)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

    ::covellite::api::renderer::Component::Texture TextureData{ *_pDataTexture, uT("") };

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
      Width, Height, 0,
      GL_RGBA, GL_UNSIGNED_BYTE, ::std::vector<uint8_t>{}))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    const auto pTexture = ::std::make_shared<Texture_t>(TextureData);
    ASSERT_EQ(Width * Height * 4, pTexture->m_ReadCopyData.size());

    _pDataTexture->SetValue(uT("entity"), pTexture);

    EXPECT_CALL(GLProxy, GenFramebuffers(1))
      .Times(1)
      .WillOnce(Return(FrameBufferId));

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(0);

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("service"), Object_t{ _pDataTexture } }
      }));
    ASSERT_NE(nullptr, Render);

    const auto TestCallActivateTexture = [&](void)
    {
      EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
        .Times(1);

      EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
        .Times(1);

      EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
        .Times(1)
        .WillOnce(Return(&ProgramId));

      EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
        .Times(1)
        .WillOnce(Return(LocationId));

      EXPECT_CALL(GLProxy, GetUniformLocation(_, Eq("TexDiffuse")))
        .Times(0);

      EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
        .Times(1);
    };

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(oMapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(false));

    EXPECT_CALL(GLProxy, BindFramebuffer(_, _))
      .Times(0);

    TestCallActivateTexture();
    Render();

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(oMapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(true));

    EXPECT_CALL(GLProxy, GetIntegerv(GL_FRAMEBUFFER_BINDING))
      .Times(1)
      .WillOnce(Return(&DefaultFrameBufferId));

    EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, FrameBufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, FramebufferTexture2D(GL_FRAMEBUFFER,
      GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureId, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetReadPixelsRawData())
      .Times(1)
      .WillOnce(Return(_RawData));

    EXPECT_CALL(GLProxy, ReadPixels(0, 0, Width, Height, GL_RGBA,
      GL_UNSIGNED_BYTE, pTexture->m_ReadCopyData.data()))
      .Times(1);

    EXPECT_CALL(oMapperProxy, CheckData(_ExpectData))
      .Times(1);

    EXPECT_CALL(oMapperProxy, Map(pTexture->m_ReadCopyData.data()))
      .Times(1);

    EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, DefaultFrameBufferId))
      .Times(1);

    TestCallActivateTexture();
    Render();

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(GLProxy, DeleteFramebuffers(1, FrameBufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
  {
    { uT("albedo"),    "TexAlbedo" },
    { uT("metalness"), "TexMetalness" },
    { uT("roughness"), "TexRoughness" },
    { uT("normal"),    "TexNormal" },
    { uT("occlusion"), "TexOcclusion" },
  };

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    const ::std::vector<uint32_t> RawData =
    {
      0x10221334, 0x10221335, 0x10221336,
      0x10221337, 0x10221338, 0x10221339,
      0x10221340, 0x10221341, 0x10221342,
      0x10221343, 0x10221344, 0x10221345,
    };

    const ::std::vector<uint32_t> ExpectData =
    {
      0x10221343, 0x10221344, 0x10221345,
      0x10221340, 0x10221341, 0x10221342,
      0x10221337, 0x10221338, 0x10221339,
      0x10221334, 0x10221335, 0x10221336,
    };

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Texture") },
        { uT("width"), Width },
        { uT("height"), Height },
        { uT("mapper"), Mapper },
        { uT("destination"), Destinations[i].first },
      });

    TestCall(pData, i, Destinations[i].second, RawData, ExpectData);
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_ReadData_Destination_Capacity16)
{
  using Texture_t = ::covellite::api::renderer::OpenGLCommon::Texture;
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<const void>;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  class MapperProxy :
    public ::alicorn::extension::testing::Proxy<MapperProxy>
  {
  public:
    MOCK_METHOD1(Map, bool(const void *));
    MOCK_METHOD1(CheckData, void(::std::vector<uint32_t>));
  };

  MapperProxy oMapperProxy;
  MapperProxy::GetInstance() = &oMapperProxy;

  const int Width = 3;
  const int Height = 4;

  const BufferMapper_t Mapper = [&](const void * _pData)
  {
    if (_pData != nullptr)
    {
      ::std::vector<uint32_t> Data(Width * Height);
      memcpy(Data.data(), _pData, Width * Height * 4);
      oMapperProxy.CheckData(Data);
    }

    return oMapperProxy.Map(_pData);
  };

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLint TextureId = 1910221154;
  const ::mock::GLint ProgramId = 1910221155;
  const ::mock::GLint LocationId = 1910221156;
  const ::mock::GLint FrameBufferId = 1910221209;
  const ::mock::GLint DefaultFrameBufferId = 1910221236;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pDataTexture,
    const ::std::size_t _Index,
    const ::std::string & _TexName,
    const ::std::vector<uint32_t> & _RawData,
    const ::std::vector<uint32_t> & _ExpectData)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

    ::covellite::api::renderer::Component::Texture TextureData{ *_pDataTexture, uT("") };

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
      Width, Height, 0,
      GL_RGBA, GL_HALF_FLOAT, ::std::vector<uint8_t>{}))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    const auto pTexture = ::std::make_shared<Texture_t>(TextureData);
    ASSERT_EQ(Width * Height * 4, pTexture->m_ReadCopyData.size());

    _pDataTexture->SetValue(uT("entity"), pTexture);

    EXPECT_CALL(GLProxy, GenFramebuffers(1))
      .Times(1)
      .WillOnce(Return(FrameBufferId));

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(0);

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("service"), Object_t{ _pDataTexture } }
      }));
    ASSERT_NE(nullptr, Render);

    const auto TestCallActivateTexture = [&](void)
    {
      EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
        .Times(1);

      EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
        .Times(1);

      EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
        .Times(1)
        .WillOnce(Return(&ProgramId));

      EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
        .Times(1)
        .WillOnce(Return(LocationId));

      EXPECT_CALL(GLProxy, GetUniformLocation(_, Eq("TexDiffuse")))
        .Times(0);

      EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
        .Times(1);
    };

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(oMapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(false));

    EXPECT_CALL(GLProxy, BindFramebuffer(_, _))
      .Times(0);

    TestCallActivateTexture();
    Render();

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(oMapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(true));

    EXPECT_CALL(GLProxy, GetIntegerv(GL_FRAMEBUFFER_BINDING))
      .Times(1)
      .WillOnce(Return(&DefaultFrameBufferId));

    EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, FrameBufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, FramebufferTexture2D(GL_FRAMEBUFFER,
      GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureId, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetReadPixelsRawData())
      .Times(1)
      .WillOnce(Return(_RawData));

    EXPECT_CALL(GLProxy, ReadPixels(0, 0, Width, Height, GL_RGBA,
      GL_UNSIGNED_BYTE, pTexture->m_ReadCopyData.data()))
      .Times(1);

    EXPECT_CALL(oMapperProxy, CheckData(_ExpectData))
      .Times(1);

    EXPECT_CALL(oMapperProxy, Map(pTexture->m_ReadCopyData.data()))
      .Times(1);

    EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, DefaultFrameBufferId))
      .Times(1);

    TestCallActivateTexture();
    Render();

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(GLProxy, DeleteFramebuffers(1, FrameBufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
  {
    { uT("albedo"),    "TexAlbedo" },
    { uT("metalness"), "TexMetalness" },
    { uT("roughness"), "TexRoughness" },
    { uT("normal"),    "TexNormal" },
    { uT("occlusion"), "TexOcclusion" },
  };

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    const ::std::vector<uint32_t> RawData =
    {
      0x10221334, 0x10221335, 0x10221336,
      0x10221337, 0x10221338, 0x10221339,
      0x10221340, 0x10221341, 0x10221342,
      0x10221343, 0x10221344, 0x10221345,
    };

    const ::std::vector<uint32_t> ExpectData =
    {
      0x10221343, 0x10221344, 0x10221345,
      0x10221340, 0x10221341, 0x10221342,
      0x10221337, 0x10221338, 0x10221339,
      0x10221334, 0x10221335, 0x10221336,
    };

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Texture") },
        { uT("width"), Width },
        { uT("height"), Height },
        { uT("mapper"), Mapper },
        { uT("destination"), Destinations[i].first },
        { uT("capacity"), 16 },
      });

    TestCall(pData, i, Destinations[i].second, RawData, ExpectData);
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_ReadData_Destination_Capacity32)
{
  using Texture_t = ::covellite::api::renderer::OpenGLCommon::Texture;
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<const void>;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  class MapperProxy :
    public ::alicorn::extension::testing::Proxy<MapperProxy>
  {
  public:
    MOCK_METHOD1(Map, bool(const void *));
    MOCK_METHOD1(CheckData, void(::std::vector<uint32_t>));
  };

  MapperProxy oMapperProxy;
  MapperProxy::GetInstance() = &oMapperProxy;

  const int Width = 3;
  const int Height = 4;

  const BufferMapper_t Mapper = [&](const void * _pData)
  {
    if (_pData != nullptr)
    {
      ::std::vector<uint32_t> Data(Width * Height);
      memcpy(Data.data(), _pData, Width * Height * 4);
      oMapperProxy.CheckData(Data);
    }

    return oMapperProxy.Map(_pData);
  };

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLint TextureId = 1910221154;
  const ::mock::GLint ProgramId = 1910221155;
  const ::mock::GLint LocationId = 1910221156;
  const ::mock::GLint FrameBufferId = 1910221209;
  const ::mock::GLint DefaultFrameBufferId = 1910221236;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pDataTexture,
    const ::std::size_t _Index,
    const ::std::string & _TexName,
    const ::std::vector<uint32_t> & _RawData,
    const ::std::vector<uint32_t> & _ExpectData)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

    ::covellite::api::renderer::Component::Texture TextureData{ *_pDataTexture, uT("") };

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
      Width, Height, 0,
      GL_RGBA, GL_FLOAT, ::std::vector<uint8_t>{}))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    const auto pTexture = ::std::make_shared<Texture_t>(TextureData);
    ASSERT_EQ(Width * Height * 4, pTexture->m_ReadCopyData.size());

    _pDataTexture->SetValue(uT("entity"), pTexture);

    EXPECT_CALL(GLProxy, GenFramebuffers(1))
      .Times(1)
      .WillOnce(Return(FrameBufferId));

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(0);

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("service"), Object_t{ _pDataTexture } }
      }));
    ASSERT_NE(nullptr, Render);

    const auto TestCallActivateTexture = [&](void)
    {
      EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
        .Times(1);

      EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
        .Times(1);

      EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
        .Times(1)
        .WillOnce(Return(&ProgramId));

      EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
        .Times(1)
        .WillOnce(Return(LocationId));

      EXPECT_CALL(GLProxy, GetUniformLocation(_, Eq("TexDiffuse")))
        .Times(0);

      EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
        .Times(1);
    };

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(oMapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(false));

    EXPECT_CALL(GLProxy, BindFramebuffer(_, _))
      .Times(0);

    TestCallActivateTexture();
    Render();

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(oMapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(true));

    EXPECT_CALL(GLProxy, GetIntegerv(GL_FRAMEBUFFER_BINDING))
      .Times(1)
      .WillOnce(Return(&DefaultFrameBufferId));

    EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, FrameBufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, FramebufferTexture2D(GL_FRAMEBUFFER,
      GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureId, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetReadPixelsRawData())
      .Times(1)
      .WillOnce(Return(_RawData));

    EXPECT_CALL(GLProxy, ReadPixels(0, 0, Width, Height, GL_RGBA,
      GL_UNSIGNED_BYTE, pTexture->m_ReadCopyData.data()))
      .Times(1);

    EXPECT_CALL(oMapperProxy, CheckData(_ExpectData))
      .Times(1);

    EXPECT_CALL(oMapperProxy, Map(pTexture->m_ReadCopyData.data()))
      .Times(1);

    EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, DefaultFrameBufferId))
      .Times(1);

    TestCallActivateTexture();
    Render();

    ////////////////////////////////////////////////////////////////////////////

    EXPECT_CALL(GLProxy, DeleteFramebuffers(1, FrameBufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
  {
    { uT("albedo"),    "TexAlbedo" },
    { uT("metalness"), "TexMetalness" },
    { uT("roughness"), "TexRoughness" },
    { uT("normal"),    "TexNormal" },
    { uT("occlusion"), "TexOcclusion" },
  };

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    const ::std::vector<uint32_t> RawData =
    {
      0x10221334, 0x10221335, 0x10221336,
      0x10221337, 0x10221338, 0x10221339,
      0x10221340, 0x10221341, 0x10221342,
      0x10221343, 0x10221344, 0x10221345,
    };

    const ::std::vector<uint32_t> ExpectData =
    {
      0x10221343, 0x10221344, 0x10221345,
      0x10221340, 0x10221341, 0x10221342,
      0x10221337, 0x10221338, 0x10221339,
      0x10221334, 0x10221335, 0x10221336,
    };

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Texture") },
        { uT("width"), Width },
        { uT("height"), Height },
        { uT("mapper"), Mapper },
        { uT("destination"), Destinations[i].first },
        { uT("capacity"), 32 },
      });

    TestCall(pData, i, Destinations[i].second, RawData, ExpectData);
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_Depth_NameAndIndex)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint TextureId = 1910041336;
  const ::mock::GLint ProgramId = 1910041337;
  const ::mock::GLint LocationId = 1910041338;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & _Source,
    const int _Width, const int _Height,
    const ::std::size_t _Index,
    const ::std::string & _TexName)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
      _Width, _Height, 0,
      GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, _Source))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
      .Times(1)
      .WillOnce(Return(LocationId));

    EXPECT_CALL(GLProxy, GetUniformLocation(_, Eq("TexDiffuse")))
      .Times(0);

    EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Names =
  {
    { uT("TexEnvironment"), "TexEnvironment" },
    { uT("TexReflection"), "TexReflection" },
    { uT("TexBaseColor"), "TexBaseColor" },
  };

  for (int Index = 0; Index < Names.size(); Index++)
  {
    ::std::vector<uint8_t> Source =
    {
      0x20, 0x06, 0x15, 0x12, 0x32,
      0x20, 0x06, 0x15, 0x12, 0x32,
      0x20, 0x06, 0x15, 0x12, 0x32,
      0x20, 0x06, 0x15, 0x12, 0x32,
      0x20, 0x06, 0x15, 0x12, 0x32,
    };
    IntroduceBufferSize(Source);

    {
      const int Width = 1910041343;
      const int Height = 1910041344;

      const auto pTexture = Component_t::Make(
        {
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("destination"), uT("depth") },
        });

      TestCall(pTexture, Source, Width, Height, Index, Names[Index].second);
    }

    {
      const int Width = 1910041346;
      const int Height = 1910041347;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("destination"), uT("depth") },
        });

      TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, Index, Names[Index].second);
    }
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_Depth_Destination)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint TextureId = 1910041336;
  const ::mock::GLint ProgramId = 1910041337;
  const ::mock::GLint LocationId = 1910041338;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & _Source,
    const int _Width, const int _Height,
    const ::std::size_t _Index,
    const ::std::string & _TexName)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
      _Width, _Height, 0,
      GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, _Source))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
      .Times(1)
      .WillOnce(Return(LocationId));

    EXPECT_CALL(GLProxy, GetUniformLocation(_, Eq("TexDiffuse")))
      .Times(0);

    EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  ::std::vector<uint8_t> Source =
  {
    0x20, 0x06, 0x15, 0x12, 0x32,
    0x20, 0x06, 0x15, 0x12, 0x32,
    0x20, 0x06, 0x15, 0x12, 0x32,
    0x20, 0x06, 0x15, 0x12, 0x32,
    0x20, 0x06, 0x15, 0x12, 0x32,
  };
  IntroduceBufferSize(Source);

  {
    const int Width = 1910041343;
    const int Height = 1910041344;

    const auto pTexture = Component_t::Make(
      {
        { uT("content"), Source },
        { uT("width"), Width },
        { uT("height"), Height },
        { uT("destination"), uT("depth") },
      });

    TestCall(pTexture, Source, Width, Height, 5, "TexDepth");
  }

  {
    const int Width = 1910041346;
    const int Height = 1910041347;

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Texture") },
        { uT("content"), Source },
        { uT("width"), Width },
        { uT("height"), Height },
        { uT("destination"), uT("depth") },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
      Source, Width, Height, 5, "TexDepth");
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_UsingExists_NameAndIndex)
{
  using Texture_t = ::covellite::api::renderer::OpenGLCommon::Texture;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint TextureId = 1908221839;
  const ::mock::GLint ProgramId = 1908221840;
  const ::mock::GLint LocationId = 1908221841;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pData,
    const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & _Source,
    const int _Width, const int _Height,
    const ::std::size_t _Index,
    const ::std::string & _TexName)
  {
    const ::covellite::api::renderer::Component::Texture TextureData{ *_pData, uT("") };
    const auto Index = static_cast<::mock::GLint>(_Index);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, _,
      _Width, _Height, 0, _, _, _Source))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    _pData->SetValue(uT("entity"), 
      ::std::make_shared<Texture_t>(TextureData));

    EXPECT_CALL(GLProxy, GenTextures(_))
      .Times(0);

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    const auto pEntity = _pData->GetValue(uT("entity"), Texture_t::Ptr_t{});
    EXPECT_EQ(nullptr, pEntity);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
      .Times(1)
      .WillOnce(Return(LocationId));

    EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Names =
  {
    { uT("TexEnvironment"), "TexEnvironment" },
    { uT("TexReflection"), "TexReflection" },
    { uT("TexBaseColor"), "TexBaseColor" },
  };

  for (int Index = 0; Index < Names.size(); Index++)
  {
    const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
    {
      { uT("albedo"),    "TexAlbedo" },
      { uT("metalness"), "TexMetalness" },
      { uT("roughness"), "TexRoughness" },
      { uT("normal"),    "TexNormal" },
      { uT("occlusion"), "TexOcclusion" },
      { uT("depth"),     "TexDepth" },
    };

    ::std::vector<uint8_t> Source =
    {
      0x20, 0x06, 0x15, 0x12, 0x33,
      0x20, 0x06, 0x15, 0x12, 0x33,
      0x20, 0x06, 0x15, 0x12, 0x33,
      0x20, 0x06, 0x15, 0x12, 0x33,
      0x20, 0x06, 0x15, 0x12, 0x33,
    };
    IntroduceBufferSize(Source);

    for (::std::size_t i = 0; i < Destinations.size(); i++)
    {
      const int Width = 1908221845;
      const int Height = 1908221847;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("name"), Names[Index].first },
          { uT("index"), Index },
          { uT("destination"), Destinations[i].first },
        });

      TestCall(pData, Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, Index, Names[Index].second);
    }
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_UsingExists_Destination)
{
  using Texture_t = ::covellite::api::renderer::OpenGLCommon::Texture;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint TextureId = 1908221839;
  const ::mock::GLint ProgramId = 1908221840;
  const ::mock::GLint LocationId = 1908221841;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pData,
    const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & _Source,
    const int _Width, const int _Height,
    const ::std::size_t _Index,
    const ::std::string & _TexName)
  {
    const ::covellite::api::renderer::Component::Texture TextureData{ *_pData, uT("") };
    const auto Index = static_cast<::mock::GLint>(_Index);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexImage2D(GL_TEXTURE_2D, 0, _,
      _Width, _Height, 0, _, _, _Source))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    _pData->SetValue(uT("entity"),
      ::std::make_shared<Texture_t>(TextureData));

    EXPECT_CALL(GLProxy, GenTextures(_))
      .Times(0);

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    const auto pEntity = _pData->GetValue(uT("entity"), Texture_t::Ptr_t{});
    EXPECT_EQ(nullptr, pEntity);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, _TexName))
      .Times(1)
      .WillOnce(Return(LocationId));

    EXPECT_CALL(GLProxy, Uniform1i(LocationId, Index))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
  {
    { uT("albedo"),    "TexAlbedo" },
    { uT("metalness"), "TexMetalness" },
    { uT("roughness"), "TexRoughness" },
    { uT("normal"),    "TexNormal" },
    { uT("occlusion"), "TexOcclusion" },
    { uT("depth"),     "TexDepth" },
  };

  ::std::vector<uint8_t> Source =
  {
    0x20, 0x06, 0x15, 0x12, 0x33,
    0x20, 0x06, 0x15, 0x12, 0x33,
    0x20, 0x06, 0x15, 0x12, 0x33,
    0x20, 0x06, 0x15, 0x12, 0x33,
    0x20, 0x06, 0x15, 0x12, 0x33,
  };
  IntroduceBufferSize(Source);

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    const int Width = 1908221845;
    const int Height = 1908221847;

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Texture") },
        { uT("content"), Source },
        { uT("width"), Width },
        { uT("height"), Height },
        { uT("destination"), Destinations[i].first },
      });

    TestCall(pData, Component_t::Make({ { uT("service"), Object_t{ pData } } }),
      Source, Width, Height, i, Destinations[i].second);
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_NoDeclaredInShader)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Texture"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint TextureId = 1908221839;

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & /*_Source*/,
    const int /*_Width*/, const int /*_Height*/,
    const ::std::size_t _Index,
    const ::std::string & _TexName)
  {
    const auto Index = static_cast<::mock::GLint>(_Index);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, GetUniformLocation(_, _TexName))
      .Times(1)
      .WillOnce(Return(-1));

    EXPECT_CALL(GLProxy, Uniform1i(_, _))
      .Times(0);

    EXPECT_THROW(Render(), ::std::exception);

    EXPECT_CALL(GLProxy, DeleteTextures(1, TextureId))
      .Times(1);
  };

  const ::std::vector<::std::pair<String_t, ::std::string>> Destinations =
  {
    { uT("albedo"),    "TexAlbedo" },
    { uT("metalness"), "TexMetalness" },
    { uT("roughness"), "TexRoughness" },
    { uT("normal"),    "TexNormal" },
    { uT("occlusion"), "TexOcclusion" },
    { uT("depth"),     "TexDepth" },
  };

  ::std::vector<uint8_t> Source =
  {
    0x20, 0x06, 0x15, 0x12, 0x34,
    0x20, 0x06, 0x15, 0x12, 0x34,
    0x20, 0x06, 0x15, 0x12, 0x34,
    0x20, 0x06, 0x15, 0x12, 0x34,
    0x20, 0x06, 0x15, 0x12, 0x34,
  };
  IntroduceBufferSize(Source);

  for (::std::size_t i = 0; i < Destinations.size(); i++)
  {
    {
      const uint8_t * pSource = (uint8_t *)1908221842;
      const int Width = 1908221843;
      const int Height = 1908221844;

      const auto pTexture = Component_t::Make(
        {
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("destination"), Destinations[i].first },
        });

      TestCall(pTexture,
        Source, Width, Height, i, Destinations[i].second);
    }

    {
      const uint8_t * pSource = (uint8_t *)1908221846;
      const int Width = 1908221845;
      const int Height = 1908221847;

      const auto pData = Component_t::Make(
        {
          { uT("kind"), uT("Texture") },
          { uT("content"), Source },
          { uT("width"), Width },
          { uT("height"), Height },
          { uT("destination"), Destinations[i].first },
        });

      TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
        Source, Width, Height, i, Destinations[i].second);
    }
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Buffer_Fail)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pComponent)
  {
    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenBuffers(_))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(1908232144));

    EXPECT_THROW(itCreator->second(_pComponent), ::std::exception);
  };

  {
    const ::std::vector<::covellite::api::Vertex> Source;

    const auto pBuffer = Component_t::Make(
      {
        { uT("content"), Source },
      });

    TestCallRender(pBuffer);

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("content"), Source },
      });

    TestCallRender(Component_t::Make({ { uT("service"), Object_t{ pData } } }));
  }

  {
    using BufferMapper_t =
      ::covellite::api::cbBufferMap_t<::covellite::api::Vertex>;
    const ::std::vector<::covellite::api::Vertex> Source;

    BufferMapper_t Mapper = [](::covellite::api::Vertex *) { return false; };

    const auto pBuffer = Component_t::Make(
      {
        { uT("content"), Source },
        { uT("mapper"), Mapper },
      });

    TestCallRender(pBuffer);

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("content"), Source },
        { uT("mapper"), Mapper },
      });

    TestCallRender(Component_t::Make({ { uT("service"), Object_t{ pData } } }));
  }

  {
    const ::std::vector<int> Source;

    const auto pBuffer = Component_t::Make(
      {
        { uT("content"), Source },
      });

    TestCallRender(pBuffer);

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("content"), Source },
      });

    TestCallRender(Component_t::Make({ { uT("service"), Object_t{ pData } } }));
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Buffer_Vertex_Static)
{
  using Vertex_t = ::covellite::api::Vertex;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const ::mock::GLuint BufferId = 1908231206;
  const ::mock::GLint ProgramId = 1908231246;
  const ::mock::GLint PositionId = 1908231301;
  const ::mock::GLint TexCoordId = 1908231302;
  const ::mock::GLint ExtraId = 1908231304;

  ::std::vector<Vertex_t> Source =
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

  IntroduceBufferSize(Source);

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

  auto itCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pComponent)
  {
    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenBuffers(1))
      .Times(1)
      .WillOnce(Return(BufferId));

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, BufferId))
      .Times(1);

    const auto Size =
      static_cast<::mock::GLsizeiptr>(Source.size() * sizeof(Vertex_t));

    EXPECT_CALL(GLProxy, BufferData(GL_ARRAY_BUFFER, Size, 
      GetExpected(Source), GL_STATIC_DRAW))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    const auto Render = itCreator->second(_pComponent);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, BufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy,
      GetAttribLocation(ProgramId, Eq("Covellite_VertexPosition")))
      .Times(1)
      .WillOnce(Return(PositionId));

    EXPECT_CALL(GLProxy, EnableVertexAttribArray(PositionId))
      .Times(1);

    EXPECT_CALL(GLProxy, VertexAttribPointer(PositionId, 4, GL_FLOAT, GL_FALSE,
      sizeof(Vertex_t), (void*)0))
      .Times(1);

    EXPECT_CALL(GLProxy, VertexAttribDivisor(PositionId, 0))
      .Times(1);

    EXPECT_CALL(GLProxy,
      GetAttribLocation(ProgramId, Eq("Covellite_VertexTexCoord")))
      .Times(1)
      .WillOnce(Return(TexCoordId));

    EXPECT_CALL(GLProxy, EnableVertexAttribArray(TexCoordId))
      .Times(1);

    EXPECT_CALL(GLProxy, VertexAttribPointer(TexCoordId, 2, GL_FLOAT, GL_FALSE,
      sizeof(Vertex_t), (void*)(sizeof(float) * 4)))
      .Times(1);

    EXPECT_CALL(GLProxy, VertexAttribDivisor(TexCoordId, 0))
      .Times(1);

    EXPECT_CALL(GLProxy,
      GetAttribLocation(ProgramId, Eq("Covellite_VertexExtra")))
      .Times(1)
      .WillOnce(Return(ExtraId));

    EXPECT_CALL(GLProxy, EnableVertexAttribArray(ExtraId))
      .Times(1);

    EXPECT_CALL(GLProxy, VertexAttribPointer(ExtraId, 4, GL_FLOAT, GL_FALSE,
      sizeof(Vertex_t), (void*)(sizeof(float) * 6)))
      .Times(1);

    EXPECT_CALL(GLProxy, VertexAttribDivisor(ExtraId, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, 0))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteBuffers(1, BufferId))
      .Times(1);
  };

  // ***************** Передача данных в объекте компонента ***************** //

  {
    const auto pBuffer = Component_t::Make(
      {
        { uT("content"), Source },
      });

    TestCallRender(pBuffer);
  }

  // ************** Передача данных в объекте компонента Data *************** //

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("content"), Source },
      });

    TestCallRender(Component_t::Make({ { uT("service"), Object_t{ pData } } }));
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Buffer_Vertex_Dynamic)
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

  const ::mock::GLuint BufferId = 1908232202;
  const ::mock::GLint ProgramId = 1908232203;
  const ::mock::GLint PositionId = 1908232204;
  const ::mock::GLint TexCoordId = 1908232205;
  const ::mock::GLint ExtraId = 1908232206;

  ::std::vector<Vertex_t> Source1 =
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

  IntroduceBufferSize(Source1);

  const ::std::vector<::mock::GLfloat> Source2 =
  {
    1908011341.0f, 1908261933.0f, 1912161256.0f, 1908261934.0f,
    1912161257.0f, 1908261935.0f,
    1908261936.0f, 1912161258.0f, 1908201949.0f, 1908201950.0f,
    1908261932.0f, 1908261933.0f, 1912161259.0f, 1908261934.0f,
    1912161300.0f, 1908261935.0f,
    1908261936.0f, 1912161301.0f, 1908201951.0f, 1908201952.0f,
  };

  const BufferMapper_t Mapper = [&](Vertex_t * _pData)
  {
    if (_pData != nullptr)
    {
      memcpy(_pData, Source2.data(), sizeof(::mock::GLfloat) * Source2.size());
    }

    return oProxy.Mapper(_pData);
  };

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pComponent)
  {
    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenBuffers(1))
      .Times(1)
      .WillOnce(Return(BufferId));

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, BufferId))
      .Times(1);

    const auto Size =
      static_cast<::mock::GLsizeiptr>(Source1.size() * sizeof(Vertex_t));

    EXPECT_CALL(GLProxy, BufferData(GL_ARRAY_BUFFER, Size, 
      GetExpected(Source1), GL_DYNAMIC_DRAW))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    const auto Render = itCreator->second(_pComponent);
    ASSERT_NE(nullptr, Render);

    {
      EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, BufferId))
        .Times(1);

      EXPECT_CALL(oProxy, Mapper(nullptr))
        .Times(1)
        .WillOnce(Return(false));

      EXPECT_CALL(oProxy, Mapper(_))
        .Times(0);

      EXPECT_CALL(GLProxy, BufferSubData(_, _, _, _))
        .Times(0);

      EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
        .Times(1)
        .WillOnce(Return(&ProgramId));

      EXPECT_CALL(GLProxy,
        GetAttribLocation(ProgramId, Eq("Covellite_VertexPosition")))
        .Times(1)
        .WillOnce(Return(PositionId));

      EXPECT_CALL(GLProxy, EnableVertexAttribArray(PositionId))
        .Times(1);

      EXPECT_CALL(GLProxy, VertexAttribPointer(PositionId, 4, GL_FLOAT, GL_FALSE,
        sizeof(Vertex_t), (void*)0))
        .Times(1);

      EXPECT_CALL(GLProxy, VertexAttribDivisor(PositionId, 0))
        .Times(1);

      EXPECT_CALL(GLProxy,
        GetAttribLocation(ProgramId, Eq("Covellite_VertexTexCoord")))
        .Times(1)
        .WillOnce(Return(TexCoordId));

      EXPECT_CALL(GLProxy, EnableVertexAttribArray(TexCoordId))
        .Times(1);

      EXPECT_CALL(GLProxy, VertexAttribPointer(TexCoordId, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex_t), (void*)(sizeof(float) * 4)))
        .Times(1);

      EXPECT_CALL(GLProxy, VertexAttribDivisor(TexCoordId, 0))
        .Times(1);

      EXPECT_CALL(GLProxy,
        GetAttribLocation(ProgramId, Eq("Covellite_VertexExtra")))
        .Times(1)
        .WillOnce(Return(ExtraId));

      EXPECT_CALL(GLProxy, EnableVertexAttribArray(ExtraId))
        .Times(1);

      EXPECT_CALL(GLProxy, VertexAttribPointer(ExtraId, 4, GL_FLOAT, GL_FALSE,
        sizeof(Vertex_t), (void*)(sizeof(float) * 6)))
        .Times(1);

      EXPECT_CALL(GLProxy, VertexAttribDivisor(ExtraId, 0))
        .Times(1);

      EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, 0))
        .Times(1);

      Render();
    }

    {
      EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, BufferId))
        .Times(1);

      EXPECT_CALL(oProxy, Mapper(nullptr))
        .Times(1)
        .WillOnce(Return(true));

      EXPECT_CALL(oProxy, Mapper(_))
        .Times(1);

      EXPECT_CALL(GLProxy, BufferSubData(GL_ARRAY_BUFFER, 0, Size, Source2))
        .Times(1);

      EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
        .Times(1)
        .WillOnce(Return(&ProgramId));

      EXPECT_CALL(GLProxy,
        GetAttribLocation(ProgramId, Eq("Covellite_VertexPosition")))
        .Times(1)
        .WillOnce(Return(PositionId));

      EXPECT_CALL(GLProxy, EnableVertexAttribArray(PositionId))
        .Times(1);

      EXPECT_CALL(GLProxy, VertexAttribPointer(PositionId, 4, GL_FLOAT, GL_FALSE,
        sizeof(Vertex_t), (void*)0))
        .Times(1);

      EXPECT_CALL(GLProxy, VertexAttribDivisor(PositionId, 0))
        .Times(1);

      EXPECT_CALL(GLProxy,
        GetAttribLocation(ProgramId, Eq("Covellite_VertexTexCoord")))
        .Times(1)
        .WillOnce(Return(TexCoordId));

      EXPECT_CALL(GLProxy, EnableVertexAttribArray(TexCoordId))
        .Times(1);

      EXPECT_CALL(GLProxy, VertexAttribPointer(TexCoordId, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex_t), (void*)(sizeof(float) * 4)))
        .Times(1);

      EXPECT_CALL(GLProxy, VertexAttribDivisor(TexCoordId, 0))
        .Times(1);

      EXPECT_CALL(GLProxy,
        GetAttribLocation(ProgramId, Eq("Covellite_VertexExtra")))
        .Times(1)
        .WillOnce(Return(ExtraId));

      EXPECT_CALL(GLProxy, EnableVertexAttribArray(ExtraId))
        .Times(1);

      EXPECT_CALL(GLProxy, VertexAttribPointer(ExtraId, 4, GL_FLOAT, GL_FALSE,
        sizeof(Vertex_t), (void*)(sizeof(float) * 6)))
        .Times(1);

      EXPECT_CALL(GLProxy, VertexAttribDivisor(ExtraId, 0))
        .Times(1);

      EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, 0))
        .Times(1);

      Render();
    }

    EXPECT_CALL(GLProxy, DeleteBuffers(1, BufferId))
      .Times(1);
  };

  // ***************** Передача данных в объекте компонента ***************** //

  {
    const auto pBuffer = Component_t::Make(
      {
        { uT("content"), Source1 },
        { uT("mapper"), Mapper },
      });

    TestCallRender(pBuffer);
  }

  // ************** Передача данных в объекте компонента Data *************** //

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("content"), Source1 },
      });

    const auto pBuffer = Component_t::Make(
      {
        { uT("mapper"), Mapper },
        { uT("service"), Object_t{ pData } }
      });

    TestCallRender(pBuffer);
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Buffer_Vertex_IgnoreNotExistsVariables)
{
  using Vertex_t = ::covellite::api::Vertex;
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<Vertex_t>;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const ::mock::GLuint BufferId = 1908231206;
  const ::mock::GLint ProgramId = 1908231246;

  ::std::vector<Vertex_t> Source =
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

  IntroduceBufferSize(Source);

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pComponent)
  {
    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenBuffers(1))
      .Times(1)
      .WillOnce(Return(BufferId));

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, BufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, BufferData(GL_ARRAY_BUFFER, _, _, _))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    const auto Render = itCreator->second(_pComponent);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, BufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy,
      GetAttribLocation(ProgramId, Eq("Covellite_VertexPosition")))
      .Times(1)
      .WillOnce(Return(-1));

    EXPECT_CALL(GLProxy, EnableVertexAttribArray(_))
      .Times(0);

    EXPECT_CALL(GLProxy, VertexAttribPointer(_, _, _, _, _, _))
      .Times(0);

    EXPECT_CALL(GLProxy, VertexAttribDivisor(_, _))
      .Times(0);

    EXPECT_CALL(GLProxy,
      GetAttribLocation(ProgramId, Eq("Covellite_VertexTexCoord")))
      .Times(1)
      .WillOnce(Return(-1));

    EXPECT_CALL(GLProxy, EnableVertexAttribArray(_))
      .Times(0);

    EXPECT_CALL(GLProxy, VertexAttribPointer(_, _, _, _, _, _))
      .Times(0);

    EXPECT_CALL(GLProxy, VertexAttribDivisor(_, _))
      .Times(0);

    EXPECT_CALL(GLProxy,
      GetAttribLocation(ProgramId, Eq("Covellite_VertexExtra")))
      .Times(1)
      .WillOnce(Return(-1));

    EXPECT_CALL(GLProxy, EnableVertexAttribArray(_))
      .Times(0);

    EXPECT_CALL(GLProxy, VertexAttribPointer(_, _, _, _, _, _))
      .Times(0);

    EXPECT_CALL(GLProxy, VertexAttribDivisor(_, _))
      .Times(0);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, 0))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteBuffers(1, BufferId))
      .Times(1);
  };

  // ***************** Передача данных в объекте компонента ***************** //

  {
    const auto pBuffer = Component_t::Make(
      {
        { uT("content"), Source },
      });

    TestCallRender(pBuffer);
  }

  // ************** Передача данных в объекте компонента Data *************** //

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("content"), Source },
      });

    TestCallRender(Component_t::Make({ { uT("service"), Object_t{ pData } } }));
  }

  //////////////////////////////////////////////////////////////////////////////

  // ***************** Передача данных в объекте компонента ***************** //

  {
    const auto pBuffer = Component_t::Make(
      {
        { uT("content"), Source },
        { uT("mapper"), BufferMapper_t{} },
      });

    TestCallRender(pBuffer);
  }

  // ************** Передача данных в объекте компонента Data *************** //

  {
    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("content"), Source },
      });

    const auto pBuffer = Component_t::Make(
      {
        { uT("mapper"), BufferMapper_t{} },
        { uT("service"), Object_t{ pData } }
      });

    TestCallRender(pBuffer);
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Transform_UnknownKind)
{
  // 17 Сентябрь 2019 12:54 (unicornum.verum@gmail.com)
  TODO("Одинаковый тест во всех реализациях");

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
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Transform_Default)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  const ::mock::GLint ProgramId = 1908241203;
  const ::mock::GLint MatrixWorldLocationId = 1908241207;

  auto itCreator = IExample.GetCreators().find(uT("Transform"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto pPosition = Component_t::Make({ { uT("kind"), uT("Position") } });
  auto pRotation = Component_t::Make({ { uT("kind"), uT("Rotation") } });
  auto pScale = Component_t::Make({ { uT("kind"), uT("Scale") } });
  const auto pTransform = Component_t::Make(
    {
      { uT("service"), Object_t{ pPosition, pRotation, pScale } }
    });

  const auto Render = itCreator->second(pTransform);
  ASSERT_NE(nullptr, Render);

  auto WorldMatrix = ::glm::identity<::glm::mat4>();

  auto TestCallRender = [&](
    float _X, float _Y, float _Z,
    float _A, float _B, float _C,
    float _Sx, float _Sy, float _Sz)
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

    WorldMatrix = ::glm::identity<::glm::mat4>();
    WorldMatrix = ::glm::scale(WorldMatrix, { _Sx, _Sy, _Sz });
    WorldMatrix = ::glm::rotate(WorldMatrix, _C, { 0.0f, 0.0f, 1.0f });
    WorldMatrix = ::glm::rotate(WorldMatrix, _B, { 0.0f, 1.0f, 0.0f });
    WorldMatrix = ::glm::rotate(WorldMatrix, _A, { 1.0f, 0.0f, 0.0f });
    WorldMatrix = ::glm::translate(WorldMatrix, { _X, _Y, _Z });
    WorldMatrix = ::glm::transpose(WorldMatrix);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("ObjectData.World")))
      .Times(1)
      .WillOnce(Return(MatrixWorldLocationId));

    EXPECT_CALL(GLProxy,
      UniformMatrix4fv(MatrixWorldLocationId, 1, GL_FALSE, WorldMatrix))
      .Times(1);

    // *************** Передача других данных шейдеру ********************* //

    EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
      .Times(AtLeast(1));

    // ******************************************************************** //

    Render();
  };

  // Два вызова, чтобы убедиться, что изменение исходных данных приводит 
  // к изменению результата рендеринга.

  TestCallRender(
    1956.0f, 1957.0f, 1958.0f,
    1204.0f, 1205.0f, 1206.0f,
    1152.0f, 1153.0f, 1154.0f);

  TestCallRender(
    1959.0f, 1960.0f, 1961.0f,
    1145.0f, 1146.0f, 1147.0f,
    1155.0f, 1157.0f, 1158.0f);
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Transform_Static)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  const ::mock::GLint ProgramId = 1908241203;
  const ::mock::GLint MatrixWorldLocationId = 1908241207;

  auto itCreator = IExample.GetCreators().find(uT("Transform"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto pPosition = Component_t::Make({ { uT("kind"), uT("Position") } });
  auto pRotation = Component_t::Make({ { uT("kind"), uT("Rotation") } });
  auto pScale = Component_t::Make({ { uT("kind"), uT("Scale") } });

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pTransform)
  {
    auto WorldMatrix = ::glm::identity<::glm::mat4>();

    auto SetValues = [&](const bool _IsBuildMatrix,
      float _X, float _Y, float _Z,
      float _A, float _B, float _C,
      float _Sx, float _Sy, float _Sz)
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

      if (_IsBuildMatrix)
      {
        WorldMatrix = ::glm::identity<::glm::mat4>();
        WorldMatrix = ::glm::scale(WorldMatrix, { _Sx, _Sy, _Sz });
        WorldMatrix = ::glm::rotate(WorldMatrix, _C, { 0.0f, 0.0f, 1.0f });
        WorldMatrix = ::glm::rotate(WorldMatrix, _B, { 0.0f, 1.0f, 0.0f });
        WorldMatrix = ::glm::rotate(WorldMatrix, _A, { 1.0f, 0.0f, 0.0f });
        WorldMatrix = ::glm::translate(WorldMatrix, { _X, _Y, _Z });
        WorldMatrix = ::glm::transpose(WorldMatrix);
      }
    };

    SetValues(true,
      1956.0f, 1957.0f, 1958.0f,
      1204.0f, 1205.0f, 1206.0f,
      1152.0f, 1153.0f, 1154.0f);

    const auto Render = itCreator->second(_pTransform);
    ASSERT_NE(nullptr, Render);

    auto TestCallRender = [&](void)
    {
      using namespace ::testing;

      InSequence Dummy;

      EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
        .Times(1)
        .WillOnce(Return(&ProgramId));

      EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("ObjectData.World")))
        .Times(1)
        .WillOnce(Return(MatrixWorldLocationId));

      EXPECT_CALL(GLProxy,
        UniformMatrix4fv(MatrixWorldLocationId, 1, GL_FALSE, WorldMatrix))
        .Times(1);

      // *************** Передача других данных шейдеру ********************* //

      EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
        .Times(AtLeast(1));

      // ******************************************************************** //

      Render();
    };

    // Два вызова, чтобы убедиться, что изменение исходных данных не приводит 
    // к изменению результата рендеринга.

    TestCallRender();

    SetValues(false,
      1959.0f, 1960.0f, 1961.0f,
      1145.0f, 1146.0f, 1147.0f,
      1155.0f, 1157.0f, 1158.0f);

    TestCallRender();
  };

  const auto pTransform = Component_t::Make(
    {
      { uT("kind"), uT("Static") },
      { uT("service"), Object_t{ pPosition, pRotation, pScale } }
    });

  TestCallRender(pTransform);
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Transform_Billboard)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  const ::mock::GLint ProgramId = 1908241203;
  const ::mock::GLint MatrixWorldLocationId = 1908241207;

  auto itCreator = IExample.GetCreators().find(uT("Transform"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto pPosition = Component_t::Make({ { uT("kind"), uT("Position") } });

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pTransform)
  {
    const auto Render = itCreator->second(_pTransform);
    ASSERT_NE(nullptr, Render);

    auto WorldMatrix = ::glm::identity<::glm::mat4>();

    auto TestCallRender = [&](float _X, float _Y, float _Z)
    {
      pPosition->SetValue(uT("x"), _X);
      pPosition->SetValue(uT("y"), _Y);
      pPosition->SetValue(uT("z"), _Z);

      WorldMatrix = ::glm::identity<::glm::mat4>();
      WorldMatrix = ::glm::translate(WorldMatrix, { _X, _Y, _Z });

      const auto ViewMatrix = ::glm::mat4
      {
        { 1.0f, 2.0f, 3.0f, 4.0f },
        { 5.0f, 6.0f, 7.0f, 8.0f },
        { 9.0f, 10.0f, 11.0f, 12.0f },
        { 13.0f, 14.0f, 15.0f, 16.0f },
      };

      GetCameraMartix(Example).View = ViewMatrix;

      auto TransposeViewMatrix = ViewMatrix;
      TransposeViewMatrix[0][3] = 0.0f;
      TransposeViewMatrix[1][3] = 0.0f;
      TransposeViewMatrix[2][3] = 0.0f;
      TransposeViewMatrix[3][0] = 0.0f;
      TransposeViewMatrix[3][1] = 0.0f;
      TransposeViewMatrix[3][2] = 0.0f;
      TransposeViewMatrix[3][3] = 1.0f;

      WorldMatrix = TransposeViewMatrix *
        ::glm::translate(::glm::identity<::glm::mat4>(), { _X, _Y, _Z });
      WorldMatrix = ::glm::transpose(WorldMatrix);

      using namespace ::testing;

      InSequence Dummy;

      EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
        .Times(1)
        .WillOnce(Return(&ProgramId));

      EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("ObjectData.World")))
        .Times(1)
        .WillOnce(Return(MatrixWorldLocationId));

      EXPECT_CALL(GLProxy,
        UniformMatrix4fv(MatrixWorldLocationId, 1, GL_FALSE, WorldMatrix))
        .Times(1);

      // *************** Передача других данных шейдеру ********************* //

      EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
        .Times(AtLeast(1));

      // ******************************************************************** //

      Render();
    };

    // Два вызова, чтобы убедиться, что изменение исходных данных приводит 
    // к изменению результата рендеринга.

    TestCallRender(1956.0f, 1957.0f, 1958.0f);
    TestCallRender(1959.0f, 1960.0f, 1961.0f);
  };

  const auto pTransform = Component_t::Make(
    {
      { uT("kind"), uT("Billboard") },
      { uT("service"), Object_t{ pPosition } }
    });

  TestCallRender(pTransform);
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Transform_DefaultValues)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Transform"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto pPosition = Component_t::Make({ { uT("kind"), uT("Position") } });
  auto pRotation = Component_t::Make({ { uT("kind"), uT("Rotation") } });
  auto pScale = Component_t::Make({ { uT("kind"), uT("Scale") } });

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pComponent,
    const bool _IsFullTransform = true)
  {
    const auto Render = itCreator->second(_pComponent);
    ASSERT_NE(nullptr, Render);

    const auto ViewMatrix = ::glm::mat4
    {
      { 1.0f, 2.0f, 3.0f, 4.0f },
      { 5.0f, 6.0f, 7.0f, 8.0f },
      { 9.0f, 10.0f, 11.0f, 12.0f },
      { 13.0f, 14.0f, 15.0f, 16.0f },
    };

    GetCameraMartix(Example).View = ViewMatrix;

    auto WorldMatrix = ::glm::identity<::glm::mat4>();

    if (_IsFullTransform)
    {
      WorldMatrix = ::glm::scale(WorldMatrix, { 1.0f, 1.0f, 1.0f });
      WorldMatrix = ::glm::transpose(WorldMatrix);
    }
    else
    {
      auto TransposeViewMatrix = ViewMatrix;
      TransposeViewMatrix[0][3] = 0.0f;
      TransposeViewMatrix[1][3] = 0.0f;
      TransposeViewMatrix[2][3] = 0.0f;
      TransposeViewMatrix[3][0] = 0.0f;
      TransposeViewMatrix[3][1] = 0.0f;
      TransposeViewMatrix[3][2] = 0.0f;
      TransposeViewMatrix[3][3] = 1.0f;

      WorldMatrix = TransposeViewMatrix;
      WorldMatrix = ::glm::transpose(WorldMatrix);
    }

    using namespace ::testing;

    EXPECT_CALL(GLProxy, UniformMatrix4fv(_, _, _, WorldMatrix))
      .Times(1);

    Render();
  };

  TestCallRender(Component_t::Make(
    {
      { uT("service"), Object_t{ pPosition, pRotation, pScale } }
    }));

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Static") },
      { uT("service"), Object_t{ pPosition, pRotation, pScale } }
    }));

  TestCallRender(Component_t::Make(
    {
      { uT("kind"), uT("Billboard") },
      { uT("service"), Object_t{ pPosition, pRotation, pScale } }
    }), false);
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Transform_Combine)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

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
    Components.clear();

    SetPosition(1.0f, 2.0f, 3.0f);
    SetRotation(4.0f, 5.0f, 6.0f);
    SetScale(7.0f, 8.0f, 9.0f);

    SetPosition(11.0f, 22.0f, 33.0f);
    SetRotation(44.0f, 55.0f, 66.0f);
    SetScale(77.0f, 88.0f, 99.0f);

    MatrixIdentity = ::glm::transpose(MatrixIdentity);

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("service"), Object_t{ Components.begin(), Components.end() } }
      }));
    ASSERT_NE(nullptr, Render);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, UniformMatrix4fv(_, _, _, MatrixIdentity))
      .Times(1);

    Render();
  }

  {
    MatrixIdentity = ::glm::identity<::glm::mat4>();
    Components.clear();

    SetPosition(1.0f, 2.0f, 3.0f);
    SetRotation(4.0f, 5.0f, 6.0f);
    SetScale(7.0f, 8.0f, 9.0f);

    SetPosition(11.0f, 22.0f, 33.0f);
    SetRotation(44.0f, 55.0f, 66.0f);
    SetScale(77.0f, 88.0f, 99.0f);

    MatrixIdentity = ::glm::transpose(MatrixIdentity);

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Static") },
        { uT("service"), Object_t{ Components.begin(), Components.end() } }
      }));
    ASSERT_NE(nullptr, Render);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, UniformMatrix4fv(_, _, _, MatrixIdentity))
      .Times(1);

    Render();
  }

  {
    const auto ViewMatrix = ::glm::mat4
    {
      { 1.0f, 2.0f, 3.0f, 4.0f },
      { 5.0f, 6.0f, 7.0f, 8.0f },
      { 9.0f, 10.0f, 11.0f, 12.0f },
      { 13.0f, 14.0f, 15.0f, 16.0f },
    };

    GetCameraMartix(Example).View = ViewMatrix;

    auto TransposeViewMatrix = ViewMatrix;
    TransposeViewMatrix[0][3] = 0.0f;
    TransposeViewMatrix[1][3] = 0.0f;
    TransposeViewMatrix[2][3] = 0.0f;
    TransposeViewMatrix[3][0] = 0.0f;
    TransposeViewMatrix[3][1] = 0.0f;
    TransposeViewMatrix[3][2] = 0.0f;
    TransposeViewMatrix[3][3] = 1.0f;

    MatrixIdentity = TransposeViewMatrix;
    Components.clear();

    SetPosition(1.0f, 2.0f, 3.0f);
    SetPosition(11.0f, 22.0f, 33.0f);
    SetPosition(111.0f, 222.0f, 333.0f);

    MatrixIdentity = ::glm::transpose(MatrixIdentity);

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Billboard") },
        { uT("service"), Object_t{ Components.begin(), Components.end() } }
      }));
    ASSERT_NE(nullptr, Render);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, UniformMatrix4fv(_, _, _, MatrixIdentity))
      .Times(1);

    Render();
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Buffer_Constant_ObjectLights_EmptyMapper)
{
  // 17 Сентябрь 2019 13:32 (unicornum.verum@gmail.com)
  TODO("Одинаковый тест для всех реализаций");

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
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Buffer_Const_ObjectLights_Ambient)
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

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itConstBufferCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itConstBufferCreator);

  auto itTransformCreator = IExample.GetCreators().find(uT("Transform"));
  ASSERT_NE(IExample.GetCreators().end(), itTransformCreator);

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

  auto TransformRender = itTransformCreator->second(Component_t::Make({ }));
  ASSERT_NE(nullptr, TransformRender);

  ConstBufferRender(); // Вызывается BufferMapper

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
    .Times(1)
    .WillOnce(Return(&ProgramId));

  // *************** Передача других данных шейдеру ********************* //

  EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
    .Times(AtLeast(1));

  // ******************************************************************** //

  EXPECT_CALL(GLProxy,
    GetUniformLocation(ProgramId, Eq("ObjectData.Lights.Ambient.IsValid")))
    .Times(1)
    .WillOnce(Return(IsValidLocationId));

  EXPECT_CALL(GLProxy,
    GetUniformLocation(ProgramId, Eq("ObjectData.Lights.Ambient.Color")))
    .Times(1)
    .WillOnce(Return(ColorLocationId));

  EXPECT_CALL(GLProxy, Uniform1i(IsValidLocationId, Lights.Ambient.IsValid))
    .Times(1);

  EXPECT_CALL(GLProxy, Uniform4fv(ColorLocationId, 1, Lights.Ambient.Color))
    .Times(1);

  // *************** Передача других данных шейдеру ********************* //

  EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
    .Times(AtLeast(1));

  EXPECT_CALL(GLProxy, Uniform1i(_, _))
    .Times(1);

  EXPECT_CALL(GLProxy, Uniform4fv(_, _, _))
    .Times(AtLeast(1));

  EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
    .Times(AtLeast(1));

  EXPECT_CALL(GLProxy, Uniform1i(_, _))
    .Times(1);

  // ******************************************************************** //

  TransformRender(); // Заполненный константный буфер передается шейдеру
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Buffer_Const_ObjectLights_Direction)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<::Lights_t>;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const ::mock::GLint ProgramId = 1908262007;
  const ::mock::GLint IsValidLocationId = 1908262008;
  const ::mock::GLint ColorLocationId = 1908262009;
  const ::mock::GLint DirectionLocationId = 1908262010;

  Lights_t Lights;
  memset(&Lights, 0x00, sizeof(Lights));

  Lights.Direction.IsValid = 1909161809;
  Lights.Direction.Color = color_t{ 1.9f, 0.91f, 6.18f, 1.1f };
  Lights.Direction.Direction = float4{ 19.0f, 0.9f, 16.1f, 812.0f };

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itConstBufferCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itConstBufferCreator);

  auto itTransformCreator = IExample.GetCreators().find(uT("Transform"));
  ASSERT_NE(IExample.GetCreators().end(), itTransformCreator);

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

  auto TransformRender = itTransformCreator->second(Component_t::Make({ }));
  ASSERT_NE(nullptr, TransformRender);

  ConstBufferRender(); // Вызывается BufferMapper

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
    .Times(1)
    .WillOnce(Return(&ProgramId));

  // *************** Передача других данных шейдеру ********************* //

  EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
    .Times(AtLeast(1));

  EXPECT_CALL(GLProxy, Uniform1i(_, _))
    .Times(1);

  EXPECT_CALL(GLProxy, Uniform4fv(_, _, _))
    .Times(AtLeast(1));

  // ******************************************************************** //

  EXPECT_CALL(GLProxy,
    GetUniformLocation(ProgramId, Eq("ObjectData.Lights.Direction.IsValid")))
    .Times(1)
    .WillOnce(Return(IsValidLocationId));

  EXPECT_CALL(GLProxy,
    GetUniformLocation(ProgramId, Eq("ObjectData.Lights.Direction.Color")))
    .Times(1)
    .WillOnce(Return(ColorLocationId));

  EXPECT_CALL(GLProxy,
    GetUniformLocation(ProgramId, Eq("ObjectData.Lights.Direction.Direction")))
    .Times(1)
    .WillOnce(Return(DirectionLocationId));

  EXPECT_CALL(GLProxy, Uniform1i(IsValidLocationId, Lights.Direction.IsValid))
    .Times(1);

  EXPECT_CALL(GLProxy, Uniform4fv(ColorLocationId, 1, Lights.Direction.Color))
    .Times(1);

  EXPECT_CALL(GLProxy, Uniform4fv(DirectionLocationId, 1, Lights.Direction.Direction))
    .Times(1);

  // *************** Передача других данных шейдеру ********************* //

  EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
    .Times(AtLeast(1));

  EXPECT_CALL(GLProxy, Uniform1i(_, _))
    .Times(1);

  // ******************************************************************** //

  TransformRender(); // Заполненный константный буфер передается шейдеру
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Buffer_Const_ObjectLights_Points)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<::Lights_t>;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const ::mock::GLint ProgramId = 1908262028;
  const ::mock::GLint UsedSlodCountId = 1908271923;
  const ::mock::GLint ColorLocationId = 1908262029;
  const ::mock::GLint PositionLocationId = 1908262030;
  const ::mock::GLint AttenuationLocationId = 1908262031;

  Lights_t Lights;
  memset(&Lights, 0x00, sizeof(Lights));

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

  auto itTransformCreator = IExample.GetCreators().find(uT("Transform"));
  ASSERT_NE(IExample.GetCreators().end(), itTransformCreator);

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

  auto TransformRender = itTransformCreator->second(Component_t::Make({ }));
  ASSERT_NE(nullptr, TransformRender);

  ConstBufferRender(); // Вызывается BufferMapper

  using namespace ::testing;

  InSequence Dummy;

  EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
    .Times(1)
    .WillOnce(Return(&ProgramId));

  // *************** Передача других данных шейдеру ********************* //

  EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
    .Times(AtLeast(1));

  EXPECT_CALL(GLProxy, Uniform1i(_, _))
    .Times(1);

  EXPECT_CALL(GLProxy, Uniform4fv(_, _, _))
    .Times(AtLeast(1));

  EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
    .Times(AtLeast(1));

  EXPECT_CALL(GLProxy, Uniform1i(_, _))
    .Times(1);

  EXPECT_CALL(GLProxy, Uniform4fv(_, _, _))
    .Times(AtLeast(1));

  // ******************************************************************** //

  EXPECT_CALL(GLProxy,
    GetUniformLocation(ProgramId, Eq("ObjectData.Lights.Points.UsedSlotCount")))
    .Times(1)
    .WillOnce(Return(UsedSlodCountId));

  EXPECT_CALL(GLProxy, Uniform1i(UsedSlodCountId, Lights.Points.UsedSlotCount))
    .Times(1);

  for (int i = 0; i < Lights.Points.UsedSlotCount; i++)
  {
    auto & Light = Lights.Points.Lights[i];

    using namespace ::alicorn::extension::std;

    const auto Point = string_cast<::std::string, Encoding::Ascii128>(
      uT("ObjectData.Lights.Points.Lights[<index>]").Replace(uT("<index>"), i));

    EXPECT_CALL(GLProxy,
      GetUniformLocation(ProgramId, Point + ".Color"))
      .Times(1)
      .WillOnce(Return(ColorLocationId + i));

    EXPECT_CALL(GLProxy,
      Uniform4fv(ColorLocationId + i, 1, Light.Color))
      .Times(1);

    EXPECT_CALL(GLProxy,
      GetUniformLocation(ProgramId, Point + ".Position"))
      .Times(1)
      .WillOnce(Return(PositionLocationId + i));

    EXPECT_CALL(GLProxy,
      Uniform4fv(PositionLocationId + i, 1, Light.Position))
      .Times(1);

    EXPECT_CALL(GLProxy,
      GetUniformLocation(ProgramId, Point + ".Attenuation"))
      .Times(1)
      .WillOnce(Return(AttenuationLocationId + i));

    EXPECT_CALL(GLProxy,
      Uniform4fv(AttenuationLocationId + i, 1, Light.Attenuation))
      .Times(1);
  }

  TransformRender(); // Заполненный константный буфер передается шейдеру
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Buffer_Constant_User_EmptyMapper)
{
  // 17 Сентябрь 2019 13:32 (unicornum.verum@gmail.com)
  TODO("Одинаковый тест для всех реализаций");

  using BufferMapper_t = ::covellite::api::cbBufferMap_t<void>;

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
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Buffer_Constant_User_EmptySize)
{
  // 17 Сентябрь 2019 13:32 (unicornum.verum@gmail.com)
  TODO("Одинаковый тест для всех реализаций");

  using BufferMapper_t = ::covellite::api::cbBufferMap_t<void>;

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const auto pComponent = Component_t::Make(
    {
      { uT("mapper"), BufferMapper_t{ [](void *) { return false; } } },
    });

  EXPECT_THROW(itCreator->second(pComponent), ::std::exception);
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Buffer_Constant_User_CreateBuffer_Error)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<void>;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  struct Data { };
  const BufferMapper_t BufferMapper = [&](void *) { return false; };

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itConstBufferCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itConstBufferCreator);

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pComponent)
  {
    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenBuffers(_))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(1909211243));

    EXPECT_THROW(itConstBufferCreator->second(_pComponent), ::std::exception);
  };

  const auto pBuffer = Component_t::Make(
    {
      { uT("mapper"), BufferMapper },
      { uT("size"), sizeof(Data) },
    });
  TestCallRender(pBuffer);
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Buffer_Constant_User_UpdateBuffer_NotExistsName)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<void>;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const BufferMapper_t BufferMapper = [&](void *) { return false; };

  using namespace ::testing;

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itConstBufferCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itConstBufferCreator);

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pComponent)
  {
    using namespace ::testing;

    const auto Render = itConstBufferCreator->second(_pComponent);
    ASSERT_NE(nullptr, Render);

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GetUniformBlockIndex(_, _))
      .Times(1)
      .WillOnce(Return(GL_INVALID_INDEX));

    EXPECT_CALL(GLProxy, BindBuffer(_, _))
      .Times(0);

    EXPECT_CALL(GLProxy, UniformBufferSubData(_, _, _))
      .Times(0);

    EXPECT_CALL(GLProxy, BindBufferBase(_, _, _))
      .Times(0);

    EXPECT_CALL(GLProxy, UniformBlockBinding(_, _, _))
      .Times(0);

    Render();
  };

  const auto pBuffer = Component_t::Make(
    {
      { uT("mapper"), BufferMapper },
      { uT("size"), ::std::size_t{ 4 } },
    });

  TestCallRender(pBuffer);
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Buffer_Constant_User)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<void>;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  class MapperProxy :
    public ::alicorn::extension::testing::Proxy<MapperProxy>
  {
  public:
    MOCK_METHOD1(DoMapper, void(const void *));
  };

  MapperProxy oMapperProxy;
  MapperProxy::GetInstance() = &oMapperProxy;

  const ::mock::GLuint BufferId = 1909211301;
  const ::mock::GLint ProgramId = 1909211302;
  const ::mock::GLint BlockIndex = 1909211332;
  const ::std::size_t Size = 313;

  void * pData = nullptr;

  const BufferMapper_t BufferMapper = [&](void * _pData)
  {
    pData = _pData;
    oMapperProxy.DoMapper(_pData);
    return false;
  };

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itConstBufferCreator = IExample.GetCreators().find(uT("Buffer"));
  ASSERT_NE(IExample.GetCreators().end(), itConstBufferCreator);

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pComponent,
    const ::std::string & _Name)
  {
    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenBuffers(1))
      .Times(1)
      .WillOnce(Return(BufferId));

    EXPECT_CALL(GLProxy, BindBuffer(GL_UNIFORM_BUFFER, BufferId))
      .Times(1);

    EXPECT_CALL(GLProxy,
      BufferData(GL_UNIFORM_BUFFER, Size, _, GL_DYNAMIC_DRAW))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_UNIFORM_BUFFER, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    const auto Render = itConstBufferCreator->second(_pComponent);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(oMapperProxy, DoMapper(_))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(_))
      .Times(1);

    EXPECT_CALL(GLProxy, GetUniformBlockIndex(_, _))
      .Times(1)
      .WillOnce(Return(BlockIndex));

    EXPECT_CALL(GLProxy, BindBuffer(_, _))
      .Times(1);

    EXPECT_CALL(GLProxy, UniformBufferSubData(_, _, _))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(_, _))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBufferBase(_, _, _))
      .Times(1);

    EXPECT_CALL(GLProxy, UniformBlockBinding(_, _, _))
      .Times(1);

    Render(); // Первый вызов, чтобы запомнить указатель на локальный буфер данных

    EXPECT_CALL(oMapperProxy, DoMapper(pData))
      .Times(1);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillOnce(Return(&ProgramId));

    EXPECT_CALL(GLProxy, GetUniformBlockIndex(ProgramId, _Name))
      .Times(1)
      .WillOnce(Return(BlockIndex));

    EXPECT_CALL(GLProxy, BindBuffer(GL_UNIFORM_BUFFER, BufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, UniformBufferSubData(0, Size, pData))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_UNIFORM_BUFFER, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, 
      BindBufferBase(GL_UNIFORM_BUFFER, COVELLITE_BUFFER_INDEX_USER, BufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, 
      UniformBlockBinding(ProgramId, BlockIndex, COVELLITE_BUFFER_INDEX_USER))
      .Times(1);

    Render();

    EXPECT_CALL(GLProxy, DeleteBuffers(1, BufferId))
      .Times(1);
  };

  // Default constant buffer name
  {
    const auto pBuffer = Component_t::Make(
      {
        { uT("mapper"), BufferMapper },
        { uT("size"), Size },
      });

    TestCallRender(pBuffer, "cbUserData");
  }

  {
    const ::std::string Name = "Name1909211251";

    const auto pBuffer = Component_t::Make(
      {
        { uT("mapper"), BufferMapper },
        { uT("size"), Size },
        { uT("name"), Name },
      });

    TestCallRender(pBuffer, Name);
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Camera_Orthographic_DefaultPosition)
{
  Tested::GetValue() = 1;

  ::mock::GLProxy GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto Render = itCreator->second(Component_t::Make(
    {
      { uT("kind"), uT("Orthographic") },
    }));
  ASSERT_NE(nullptr, Render);

  auto itShaderCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itShaderCreator);

  auto VsRender = itShaderCreator->second(Component_t::Make(
    {
      { uT("entry"), uT("vsFlat") },
    }));
  ASSERT_NE(nullptr, VsRender);

  auto PsRender = itShaderCreator->second(Component_t::Make(
    {
      { uT("entry"), uT("psColored") },
    }));
  ASSERT_NE(nullptr, PsRender);

  const float Viewport[] =
  {
    1812271148.0f, 1812271149.0f, 1812181722.0f, 1812181723.0f
  };

  using namespace ::testing;

  EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, 0))
    .Times(1);

  EXPECT_CALL(GLProxy, Disable(GL_DEPTH_TEST))
    .Times(1);

  EXPECT_CALL(GLProxy, Enable(GL_DEPTH_TEST))
    .Times(0);

  EXPECT_CALL(GLProxy, Clear(GL_DEPTH_BUFFER_BIT))
    .Times(0);

  EXPECT_CALL(GLProxy, Disable(GL_BLEND))
    .Times(1);

  EXPECT_CALL(GLProxy, Disable(GL_DITHER))
    .Times(1);

  EXPECT_CALL(GLProxy, Enable(GL_CULL_FACE))
    .Times(1);

  EXPECT_CALL(GLProxy, CullFace(GL_BACK))
    .Times(1);

  EXPECT_CALL(GLProxy, FrontFace(GL_CCW))
    .Times(1);

  EXPECT_CALL(GLProxy, Viewport(0, 0, _, _))
    .Times(1);

  EXPECT_CALL(GLProxy, GetFloatv(GL_VIEWPORT))
    .Times(1)
    .WillOnce(Return(Viewport));

  Render(); // Формирует матрицы вида и проекции

  VsRender(); // Нужно, чтобы сформировалась шейдерная программа, иначе
              // не будет вызывана функция glUseProgram().

  const ::mock::GLint ProgramId = 1908251932;
  const ::mock::GLint ProjectionLocationId = 1908251933;
  const ::mock::GLint ViewLocationId = 1908252005;
  const ::mock::GLint ViewInverseLocationId = 1908252013;

  const auto Projection = ::glm::transpose(::glm::ortho(
    Viewport[0], Viewport[0] + Viewport[2],
    Viewport[1] + Viewport[3], Viewport[1],
    1.0f, -1.0f));
  auto View = ::glm::identity<::glm::mat4>();
  const auto ViewInverse = ::glm::transpose(::glm::inverse(View));
  View = ::glm::transpose(View);

  InSequence Dummy;

  EXPECT_CALL(GLProxy, UseProgram(_))
    .Times(1);

  EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
    .Times(1)
    .WillOnce(Return(&ProgramId));

  // *************** Передача других данных шейдеру ********************* //

  EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
    .Times(AtLeast(1));

  EXPECT_CALL(GLProxy, UniformMatrix4fv(_, _, _, _))
    .Times(AtLeast(1));

  // ******************************************************************** //

  EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("CameraData.Projection")))
    .Times(1)
    .WillOnce(Return(ProjectionLocationId));

  EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("CameraData.View")))
    .Times(1)
    .WillOnce(Return(ViewLocationId));

  EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("CameraData.ViewInverse")))
    .Times(1)
    .WillOnce(Return(ViewInverseLocationId));

  EXPECT_CALL(GLProxy,
    UniformMatrix4fv(ProjectionLocationId, 1, GL_FALSE, Projection))
    .Times(1);

  EXPECT_CALL(GLProxy,
    UniformMatrix4fv(ViewLocationId, 1, GL_FALSE, View))
    .Times(1);

  EXPECT_CALL(GLProxy,
    UniformMatrix4fv(ViewInverseLocationId, 1, GL_FALSE, ViewInverse))
    .Times(1);

  PsRender(); // Передача матриц вида и проекции шейдеру
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Camera_Orthographic)
{
  Tested::GetValue() = 1;

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

  auto itCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto itShaderCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itShaderCreator);

  auto VsRender = itShaderCreator->second(Component_t::Make(
    {
      { uT("entry"), uT("vsFlat") },
    }));
  ASSERT_NE(nullptr, VsRender);

  auto PsRender = itShaderCreator->second(Component_t::Make(
    {
      { uT("entry"), uT("psColored") },
    }));
  ASSERT_NE(nullptr, PsRender);

  using namespace ::testing;

  const auto pPosition = Component_t::Make(
    {
      { uT("kind"), uT("Position") },
      { uT("x"), X },
      { uT("y"), Y },
    });

  const auto pCameraComponent = Component_t::Make(
    {
      { uT("kind"), uT("Orthographic") },
      { uT("service"), Object_t{ pPosition } },
    });

  auto Render = itCreator->second(pCameraComponent);
  ASSERT_NE(nullptr, Render);

  InSequence Dummy;

  EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, 0))
    .Times(1);

  EXPECT_CALL(GLProxy, Disable(GL_DEPTH_TEST))
    .Times(1);

  EXPECT_CALL(GLProxy, Enable(GL_DEPTH_TEST))
    .Times(0);

  EXPECT_CALL(GLProxy, Clear(GL_DEPTH_BUFFER_BIT))
    .Times(0);

  EXPECT_CALL(GLProxy, Disable(GL_BLEND))
    .Times(1);

  EXPECT_CALL(GLProxy, Disable(GL_DITHER))
    .Times(1);

  EXPECT_CALL(GLProxy, Enable(GL_CULL_FACE))
    .Times(1);

  EXPECT_CALL(GLProxy, CullFace(GL_BACK))
    .Times(1);

  EXPECT_CALL(GLProxy, FrontFace(GL_CCW))
    .Times(1);

  EXPECT_CALL(GLProxy, Viewport(0, 0, _, _))
    .Times(1);

  EXPECT_CALL(GLProxy, GetFloatv(GL_VIEWPORT))
    .Times(1)
    .WillOnce(Return(SourceViewport));

  Render(); // Формирует матрицы вида и проекции

  const auto Projection = ::glm::transpose(::glm::ortho(
    SourceViewport[0] + X,
    SourceViewport[0] + SourceViewport[2] + X,
    SourceViewport[1] + SourceViewport[3] + Y,
    SourceViewport[1] + Y,
    1.0f, -1.0f));

  auto View = ::glm::identity<::glm::mat4>();
  const auto ViewInverse = ::glm::transpose(::glm::inverse(View));
  View = ::glm::transpose(View);

  EXPECT_EQ(View, pCameraComponent->GetValue(uT("view"), ::glm::mat4{ 1.0f }));
  EXPECT_EQ(Projection, pCameraComponent->GetValue(uT("projection"), ::glm::mat4{ 1.0f }));

  VsRender(); // Нужно, чтобы сформировалась шейдерная программа, иначе
              // не будет вызывана функция glUseProgram().

  EXPECT_CALL(GLProxy, UseProgram(_))
    .Times(1);

  const ::mock::GLint ProgramId = 1908251932;
  const ::mock::GLint ProjectionLocationId = 1908251933;
  const ::mock::GLint ViewLocationId = 1908252005;
  const ::mock::GLint ViewInverseLocationId = 1908252013;

  EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
    .Times(1)
    .WillRepeatedly(Return(&ProgramId));

  // *************** Передача других данных шейдеру ********************* //

  EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
    .Times(AtLeast(1));

  EXPECT_CALL(GLProxy, UniformMatrix4fv(_, _, _, _))
    .Times(AtLeast(1));

  // ******************************************************************** //

  EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("CameraData.Projection")))
    .Times(1)
    .WillOnce(Return(ProjectionLocationId));

  EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("CameraData.View")))
    .Times(1)
    .WillOnce(Return(ViewLocationId));

  EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("CameraData.ViewInverse")))
    .Times(1)
    .WillOnce(Return(ViewInverseLocationId));

  EXPECT_CALL(GLProxy,
    UniformMatrix4fv(ProjectionLocationId, 1, GL_FALSE, Projection))
    .Times(1);

  EXPECT_CALL(GLProxy,
    UniformMatrix4fv(ViewLocationId, 1, GL_FALSE, View))
    .Times(1);

  EXPECT_CALL(GLProxy,
    UniformMatrix4fv(ViewInverseLocationId, 1, GL_FALSE, ViewInverse))
    .Times(1);

  PsRender(); // Передача матриц вида и проекции шейдеру
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Camera_Perspective)
{
  Tested::GetValue() = 1;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const float Width = 1024.0f;
  const float Height = 768.0f;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itShaderCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itShaderCreator);

  auto VsRender = itShaderCreator->second(Component_t::Make(
    {
      { uT("entry"), uT("vsFlat") },
    }));
  ASSERT_NE(nullptr, VsRender);

  auto PsRender = itShaderCreator->second(Component_t::Make(
    {
      { uT("entry"), uT("psColored") },
    }));
  ASSERT_NE(nullptr, PsRender);

  using namespace ::testing;

  auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pCameraComponent,
    Render_t & _Render,
    float _AngleY, float _zNear, float _zFar,
    float _X, float _Y, float _Z,
    float _A, float _B, float _C,
    float _Distance)
  {
    ASSERT_NE(nullptr, _Render);

    const float AngleYRadians = _AngleY *
      static_cast<float>(::alicorn::extension::cpp::math::GreedToRadian);

    InSequence Dummy;

    EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, Disable(GL_DEPTH_TEST))
      .Times(1);

    EXPECT_CALL(GLProxy, Enable(GL_DEPTH_TEST))
      .Times(0);

    EXPECT_CALL(GLProxy, Clear(GL_DEPTH_BUFFER_BIT))
      .Times(0);

    EXPECT_CALL(GLProxy, Disable(GL_BLEND))
      .Times(1);

    EXPECT_CALL(GLProxy, Disable(GL_DITHER))
      .Times(1);

    EXPECT_CALL(GLProxy, Enable(GL_CULL_FACE))
      .Times(1);

    EXPECT_CALL(GLProxy, CullFace(GL_BACK))
      .Times(1);

    EXPECT_CALL(GLProxy, FrontFace(GL_CCW))
      .Times(1);

    EXPECT_CALL(GLProxy, Viewport(0, 0, _, _))
      .Times(1);

    const float Viewport[] = { 0, 0, Width, Height };

    EXPECT_CALL(GLProxy, GetFloatv(GL_VIEWPORT))
      .Times(1)
      .WillOnce(Return(Viewport));

    _Render(); // Формирует матрицы вида и проекции

    const auto Projection = ::glm::transpose(::glm::perspectiveFovRH(
      AngleYRadians, Viewport[2], Viewport[3], _zFar, _zNear));

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

    auto View = ::glm::lookAtRH(
      GetEye(),
      ::glm::vec3{ _X, _Y, _Z },
      ::glm::vec3{ 0.0f, 0.0f, 1.0f });
    const auto ViewInverse = ::glm::transpose(::glm::inverse(View));
    View = ::glm::transpose(View);

    EXPECT_EQ(View, _pCameraComponent->GetValue(uT("view"), ::glm::mat4{ 1.0f }));
    EXPECT_EQ(Projection, _pCameraComponent->GetValue(uT("projection"), ::glm::mat4{ 1.0f }));

    EXPECT_CALL(GLProxy, UseProgram(_))
      .Times(AtLeast(1));

    VsRender(); // Нужно, чтобы сформировалась шейдерная программа, иначе
                // не будет вызывана функция glUseProgram().

    const ::mock::GLint ProgramId = 1908251932;
    const ::mock::GLint ProjectionLocationId = 1908251933;
    const ::mock::GLint ViewLocationId = 1908252005;
    const ::mock::GLint ViewInverseLocationId = 1908252013;

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillRepeatedly(Return(&ProgramId));

    // *************** Передача других данных шейдеру ********************* //

    EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
      .Times(AtLeast(1));

    EXPECT_CALL(GLProxy, UniformMatrix4fv(_, _, _, _))
      .Times(AtLeast(1));

    // ******************************************************************** //

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("CameraData.Projection")))
      .Times(1)
      .WillOnce(Return(ProjectionLocationId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("CameraData.View")))
      .Times(1)
      .WillOnce(Return(ViewLocationId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("CameraData.ViewInverse")))
      .Times(1)
      .WillOnce(Return(ViewInverseLocationId));

    EXPECT_CALL(GLProxy,
      UniformMatrix4fv(ProjectionLocationId, 1, GL_FALSE, Projection))
      .Times(1);

    EXPECT_CALL(GLProxy,
      UniformMatrix4fv(ViewLocationId, 1, GL_FALSE, View))
      .Times(1);

    EXPECT_CALL(GLProxy,
      UniformMatrix4fv(ViewInverseLocationId, 1, GL_FALSE, ViewInverse))
      .Times(1);

    PsRender(); // Передача матриц вида и проекции шейдеру
  };

  auto itCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  class CameraInfo
  {
  public:
    float AngleY;
    float zNear, zFar;
    float X, Y, Z;
    float A, B, C;
    float Distance;
  };

  const ::std::vector<CameraInfo> CameraInfos =
  {
    { 90.0f, 0.01f, 200.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // Default values
    { 91.0f, 1.11f, 10.1f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f },
    { 92.0f, 2.22f, 20.2f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f },
  };

  {
    // Default values + not using Position & Rotation

    const auto pCamera = Component_t::Make(
      {
        { uT("kind"), uT("Perspective") },
      });

    auto Render = itCreator->second(pCamera);

    auto Info = CameraInfos[0];

    TestCallRender(
      pCamera, Render,
      Info.AngleY, Info.zNear, Info.zFar,
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

    const auto pCamera = Component_t::Make(
      {
        { uT("kind"), uT("Perspective") },
        { uT("service"), Object_t{ pPosition, pRotation } },
      });

    auto Render = itCreator->second(pCamera);

    auto Info = CameraInfos[0];

    TestCallRender(
      pCamera, Render,
      Info.AngleY, Info.zNear, Info.zFar,
      Info.X, Info.Y, Info.Z,
      Info.A, Info.B, Info.C,
      Info.Distance);
  }

  {
    // Change values from Position & Rotation

    const auto pCamera = Component_t::Make(
      {
        { uT("kind"), uT("Perspective") },
        { uT("service"), Object_t{ pPosition, pRotation } },
      });

    auto Render = itCreator->second(pCamera);

    for (size_t i = 1; i < CameraInfos.size(); i++)
    {
      auto Info = CameraInfos[i];

      pPosition->SetValue(uT("x"), Info.X);
      pPosition->SetValue(uT("y"), Info.Y);
      pPosition->SetValue(uT("z"), Info.Z);

      pRotation->SetValue(uT("x"), Info.A);
      pRotation->SetValue(uT("y"), Info.B);
      pRotation->SetValue(uT("z"), Info.C);

      pCamera->SetValue(uT("fov"), Info.AngleY);
      pCamera->SetValue(uT("znear"), Info.zNear);
      pCamera->SetValue(uT("zfar"), Info.zFar);
      pCamera->SetValue(uT("distance"), Info.Distance);

      TestCallRender(
        pCamera, Render,
        Info.AngleY, Info.zNear, Info.zFar,
        Info.X, Info.Y, Info.Z,
        Info.A, Info.B, Info.C,
        Info.Distance);
    }
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Camera_SetViewport)
{
  const auto TestCall = [](const Component_t::ComponentPtr_t & _pCamera)
  {
    ::mock::WindowsProxy WindowsProxy;
    ::mock::GLProxy GLProxy;

    Data_t Data;
    Data.Width = 2008131258;
    Data.Height = 2008131259;

    using namespace ::testing;

    Tested_t Example{ Data };
    ITested_t & IExample = Example;

    auto itCreator = IExample.GetCreators().find(uT("Camera"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    auto Render = itCreator->second(_pCamera);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, Viewport(0, 0, Data.Width, Data.Height))
      .Times(1);

    Render();

    IExample.ResizeWindow(2008131300, 2008131301);

    EXPECT_CALL(GLProxy, Viewport(0, 0, 2008131300, 2008131301))
      .Times(1);

    Render();
  };

  TestCall(Component_t::Make(
    {
      { uT("kind"), uT("Orthographic") },
    }));

  TestCall(Component_t::Make(
    {
      { uT("kind"), uT("Perspective") },
    }));
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Present_Index_CreateBuffer_Fail)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pComponent)
  {
    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenBuffers(_))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, _))
      .Times(AtLeast(1));

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(1908232144));

    EXPECT_THROW(itCreator->second(_pComponent), ::std::exception);
  };

  const ::std::vector<int> Source;

  const auto pBuffer = Component_t::Make(
    {
      { uT("content"), Source },
    });

  TestCallRender(pBuffer);

  const auto pData = Component_t::Make(
    {
      { uT("kind"), uT("Buffer") },
      { uT("content"), Source },
    });

  TestCallRender(Component_t::Make({ { uT("service"), Object_t{ pData } } }));
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Present_Index)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  const ::mock::GLuint BufferId = 1908241121;

  ::std::vector<int> Indices =
  {
    1808261927,
    1808261928,
    1808261929,
    1808261930,
    1808261931
  };

  IntroduceBufferSize(Indices);

  auto itPresentCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itPresentCreator);

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pPresent)
  {
    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenBuffers(1))
      .Times(1)
      .WillOnce(Return(BufferId));

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferId))
      .Times(1);

    const auto Size =
      static_cast<::mock::GLsizeiptr>(Indices.size() * sizeof(int));

    EXPECT_CALL(GLProxy, BufferData(GL_ELEMENT_ARRAY_BUFFER, Size, 
      GetExpected(Indices), GL_STATIC_DRAW))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    const auto Render = itPresentCreator->second(_pPresent);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, DrawElements(GL_TRIANGLES, (int)Indices.size(),
      GL_UNSIGNED_INT, nullptr))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
      .Times(0); // Нельзя!!!

    Render();

    EXPECT_CALL(GLProxy, DeleteBuffers(1, BufferId))
      .Times(1);
  };

  {
    const auto pPresent = Component_t::Make(
      {
        { uT("content"), Indices },
      });

    TestCallRender(pPresent);
  }

  {
    // Индексный буфер через компонент Data

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("content"), Indices },
      });

    TestCallRender(Component_t::Make({ { uT("service"), Object_t{ pData } } }));
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Present_Instance_CreateBuffer_Fail)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<void>;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pComponent)
  {
    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenBuffers(_))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, _))
      .Times(AtLeast(1));

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    EXPECT_CALL(GLProxy, GenBuffers(_))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, _))
      .Times(AtLeast(1));

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(1908232144));

    EXPECT_THROW(itCreator->second(_pComponent), ::std::exception);
  };

  const ::std::vector<int> Source;

  const auto pInstanceData = Component_t::Make(
    {
      { uT("kind"), uT("Buffer") },
      { uT("mapper"), BufferMapper_t{} },
    });

  const auto pPresent = Component_t::Make(
    {
      { uT("content"), Source },
      { uT("service"), Object_t{ pInstanceData } }
    });

  TestCallRender(pPresent);
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Present_Instance)
{
  using BufferMapper_t = ::covellite::api::cbBufferMap_t<void>;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  class MapperProxy_t
  {
  public:
    MOCK_METHOD1(Map, bool(void *));
  };

  MapperProxy_t MapperProxy;

  const ::mock::GLuint IndexBufferId = 1909231813;
  const ::mock::GLuint InstanceBufferId = 1909231820;
  const ::std::size_t InstanceCount = 120;
  const ::std::size_t InstanceStride = 48;
  const ::std::size_t InstanceBufferSize = InstanceCount * InstanceStride;
  const ::mock::GLint ProgramId = 1909231917;

  ::std::vector<int> Indices =
  {
    1808261927,
    1808261928,
    1808261929,
    1808261930,
    1808261931,
    1909231821,
    1909231822,
    1909231823,
  };

  IntroduceBufferSize(Indices);

  void * pLocalData = (void *)1909232013;

  const BufferMapper_t Mapper = [&](void * _pData)
  {
    pLocalData = _pData;
    return MapperProxy.Map(_pData);
  };

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Present"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto TestCallRender = [&](const Component_t::ComponentPtr_t & _pPresent)
  {
    using namespace ::testing;

    InSequence Dummy;

    // *************************** Индексный буфер ************************** //

    EXPECT_CALL(GLProxy, GenBuffers(1))
      .Times(1)
      .WillOnce(Return(IndexBufferId));

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId))
      .Times(1);

    const auto Size =
      static_cast<::mock::GLsizeiptr>(Indices.size() * sizeof(int));

    EXPECT_CALL(GLProxy, BufferData(GL_ELEMENT_ARRAY_BUFFER, Size, 
      GetExpected(Indices), GL_STATIC_DRAW))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    // ****************************** Инстанс-буфер ************************* //

    EXPECT_CALL(GLProxy, GenBuffers(1))
      .Times(1)
      .WillOnce(Return(InstanceBufferId));

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, InstanceBufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, BufferData(GL_ARRAY_BUFFER, InstanceBufferSize, 
      ::std::vector<uint8_t>{}, GL_DYNAMIC_DRAW))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    // ********************************************************************** //

    const auto Render = itCreator->second(_pPresent);
    ASSERT_NE(nullptr, Render);

    // ****************** Отрисовка без обновления ************************** //

    const auto TestCallDraw = [&](void)
    {
      EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId))
        .Times(1);

      EXPECT_CALL(GLProxy, DrawElementsInstanced(GL_TRIANGLES,
        (int)Indices.size(), GL_UNSIGNED_INT, nullptr, InstanceCount))
        .Times(1);

      EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
        .Times(1);

      EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
        .Times(0); // Нельзя!!!
    };

    EXPECT_CALL(MapperProxy, Map(nullptr))
      .Times(1)
      .WillOnce(Return(false));

    EXPECT_CALL(MapperProxy, Map(_))
      .Times(0);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, InstanceBufferId))
      .Times(0);

    TestCallDraw();
    Render();

    // ******************* Отрисовка с обновлением ************************** //

    const auto TestCallSetInstanceData = [&](
      const ::mock::GLenum _iType,
      const ::mock::GLenum _Type,
      const ::mock::GLint _LocationIndex0,
      auto pData)
    {
      EXPECT_CALL(MapperProxy, Map(nullptr))
        .Times(1)
        .WillOnce(Return(true));

      EXPECT_CALL(MapperProxy, Map(pData))
        .Times(1);

      EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, InstanceBufferId))
        .Times(1);

      EXPECT_CALL(GLProxy, 
        BufferSubDataRaw(GL_ARRAY_BUFFER, 0, InstanceBufferSize, pData))
        .Times(1);

      EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
        .Times(1)
        .WillOnce(Return(&ProgramId));

      constexpr auto BlockSize = sizeof(float) * 4;

      EXPECT_CALL(GLProxy, GetProgramiv(ProgramId, GL_ACTIVE_ATTRIBUTES))
        .Times(1)
        .WillOnce(Return(BlockSize));

      for (int i = 0; i < BlockSize; i++)
      {
        EXPECT_CALL(GLProxy, GetActiveAttribType(ProgramId, i)) 
          .Times(1)
          .WillOnce(Return(_iType));

        EXPECT_CALL(GLProxy, GetActiveAttribName(ProgramId, i, 255))
          .Times(1)
          .WillOnce(Return("iValue" + ::std::to_string(i + 1)));
      }

      for (int i = 0; i < InstanceStride / BlockSize; i++)
      {
        EXPECT_CALL(GLProxy,
          GetAttribLocation(ProgramId, "iValue" + ::std::to_string(i + 1)))
          .Times(1)
          .WillOnce(Return(_LocationIndex0 + i));

        if (_LocationIndex0 + i >= 0)
        {
          EXPECT_CALL(GLProxy, EnableVertexAttribArray(_LocationIndex0 + i))
            .Times(1);

          EXPECT_CALL(GLProxy, VertexAttribPointer(_LocationIndex0 + i, 4, 
            _Type, GL_FALSE, InstanceStride, (void*)(BlockSize * i)))
            .Times(1);

          EXPECT_CALL(GLProxy, VertexAttribDivisor(_LocationIndex0 + i, 1))
            .Times(1);
        }
        else
        {
          EXPECT_CALL(GLProxy, EnableVertexAttribArray(_))
            .Times(0);

          EXPECT_CALL(GLProxy, VertexAttribPointer(_, _, _, _, _, _))
            .Times(0);

          EXPECT_CALL(GLProxy, VertexAttribDivisor(_, _))
            .Times(0);
        }
      }

      EXPECT_CALL(GLProxy, BindBuffer(GL_ARRAY_BUFFER, 0))
        .Times(1);
    };

    // Первый вызов без значения, чтобы сохранился указатель на локальные
    // данные в pLocalData
    TestCallSetInstanceData(GL_FLOAT_VEC4, GL_FLOAT, 1909232004, _);
    TestCallDraw();
    Render();

    TestCallSetInstanceData(GL_FLOAT_VEC4, GL_FLOAT, 1909232004, pLocalData);
    TestCallDraw();
    Render();

    TestCallSetInstanceData(GL_INT_VEC4, GL_INT, 1909232005, pLocalData);
    TestCallDraw();
    Render();

    TestCallSetInstanceData(2005221201, 2005221201, 2005221202, pLocalData);
    TestCallDraw();
    Render();

    TestCallSetInstanceData(GL_FLOAT_VEC4, GL_FLOAT, -1, pLocalData);
    TestCallDraw();
    Render();

    TestCallSetInstanceData(GL_INT_VEC4, GL_INT, -1, pLocalData);
    TestCallDraw();
    Render();

    // ********************************************************************** //

    EXPECT_CALL(GLProxy, DeleteBuffers(1, IndexBufferId))
      .Times(1);

    EXPECT_CALL(GLProxy, DeleteBuffers(1, InstanceBufferId))
      .Times(1);
  };

  const auto pInstanceData = Component_t::Make(
    {
      { uT("kind"), uT("Buffer") },
      { uT("size"), InstanceBufferSize },
      { uT("mapper"), Mapper },
      { uT("count"), InstanceCount },
    });

  const auto pPresent = Component_t::Make(
    {
      { uT("content"), Indices },
      { uT("service"), Object_t{ pInstanceData } }
    });

  TestCallRender(pPresent);
}


////////////////////////////////////////////////////////////////////////////////

// ************************************************************************** //
TEST_F(OpenGLShader_test, DISABLED_Test_Fog)
{
  Tested::GetValue() = 2;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itCreator = IExample.GetCreators().find(uT("Fog"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto itShaderCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itShaderCreator);

  auto VsRender = itShaderCreator->second(Component_t::Make(
    {
      { uT("entry"), uT("vsFlat") },
    }));
  ASSERT_NE(nullptr, VsRender);

  auto PsRender = itShaderCreator->second(Component_t::Make(
    {
      { uT("entry"), uT("psColored") },
    }));
  ASSERT_NE(nullptr, PsRender);

  const auto TestCall = [&](
    const Component_t::ComponentPtr_t & _pData,
    const Component_t::ComponentPtr_t & _pFog,
    const uint32_t _Color,
    const float _Near, const float _Far,
    const float _Density)
  {
    if (_pData != nullptr)
    {
      const auto Render = itDataCreator->second(_pData);
      EXPECT_EQ(nullptr, Render);
    }

    const auto Render = itCreator->second(_pFog);
    ASSERT_NE(nullptr, Render);

    const ::mock::GLint ProgramId = 1908261142;
    const ::mock::GLint ColorLocationId = 1908261240;
    const ::mock::GLint NearLocationId = 1908261241;
    const ::mock::GLint FarLocationId = 1908261242;
    const ::mock::GLint DensityLocationId = 1908261243;

    for (int i = 0; i < 5; i++)
    {
      using namespace ::testing;

      InSequence Dummy;

      Render(); // Переписывает значения из компонента в структуру

      EXPECT_CALL(GLProxy, UseProgram(_))
        .Times(AtLeast(1));

      VsRender(); // Нужно, чтобы сформировалась шейдерная программа, иначе
                  // не будет вызывана функция glUseProgram().

      EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
        .Times(1)
        .WillOnce(Return(&ProgramId));

      EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("FogData.Color")))
        .Times(1)
        .WillOnce(Return(ColorLocationId));

      EXPECT_CALL(GLProxy,
        Uniform4fv(ColorLocationId, 1, ARGBtoVec4(_Color + i)))
        .Times(1);

      EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("FogData.Near")))
        .Times(1)
        .WillOnce(Return(NearLocationId));

      EXPECT_CALL(GLProxy, Uniform1f(NearLocationId, _Near + i))
        .Times(1);

      EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("FogData.Far")))
        .Times(1)
        .WillOnce(Return(FarLocationId));

      EXPECT_CALL(GLProxy, Uniform1f(FarLocationId, _Far + i))
        .Times(1);

      EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("FogData.Density")))
        .Times(1)
        .WillOnce(Return(DensityLocationId));

      EXPECT_CALL(GLProxy, Uniform1f(DensityLocationId, _Density + i))
        .Times(1);

      PsRender(); // Передача данных шейдеру

      const Component_t::ComponentPtr_t pData =
        (_pData != nullptr) ? _pData : _pFog;

      pData->SetValue(uT("color"), _Color + i + 1);
      pData->SetValue(uT("near"), _Near + i + 1);
      pData->SetValue(uT("far"), _Far + i + 1);
      pData->SetValue(uT("density"), _Density + i + 1);
    }
  };

  {
    const auto pDefaultFog = Component_t::Make({ });

    TestCall(nullptr, pDefaultFog, 0xFFFFFFFF, 10.0f, 100.0f, 1.0f);
  }

  {
    const uint32_t Color = 0x19072916;
    const auto Near = 1907291728.0f;
    const auto Far = 1907291729.0f;
    const auto Density = 1907291800.0f;

    const auto pFog = Component_t::Make(
      {
        { uT("color"), Color },
        { uT("near"), Near },
        { uT("far"), Far },
        { uT("density"), Density },
      });

    TestCall(nullptr, pFog, Color, Near, Far, Density);
  }

  {
    const uint32_t Color = 0x19073010;
    const auto Near = 1907301018.0f;
    const auto Far = 1907301019.0f;
    const auto Density = 1907291800.0f;

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("Fog") },
        { uT("color"), Color },
        { uT("near"), Near },
        { uT("far"), Far },
        { uT("density"), Density },
      });

    const auto pFog = Component_t::Make({ });

    TestCall(pData, pFog, Color, Near, Far, Density);
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, DISABLED_Test_Present_Geometry_Default_deprecated)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  const ::mock::GLuint BufferId = 1908241121;
  const ::mock::GLint ProgramId = 1908241203;
  const ::mock::GLint MatrixWorldLocationId = 1908241207;

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

  auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pIndexBuffer,
    const Component_t::ComponentPtr_t & _pPresent)
  {
    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenBuffers(1))
      .Times(1)
      .WillOnce(Return(BufferId));

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferId))
      .Times(1);

    const auto Size =
      static_cast<::mock::GLsizeiptr>(Indices.size() * sizeof(int));

    EXPECT_CALL(GLProxy, BufferData(GL_ELEMENT_ARRAY_BUFFER, Size, 
      GetExpected(Indices), GL_STATIC_DRAW))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    const auto IndexBufferRender = itBufferCreator->second(_pIndexBuffer);
    ASSERT_NE(nullptr, IndexBufferRender);

    auto PositionRender = itDataCreator->second(pPosition);
    EXPECT_EQ(nullptr, PositionRender);

    auto RotationRender = itDataCreator->second(pRotation);
    EXPECT_EQ(nullptr, RotationRender);

    auto ScaleRender = itDataCreator->second(pScale);
    EXPECT_EQ(nullptr, ScaleRender);

    const auto Render = itPresentCreator->second(_pPresent);
    ASSERT_NE(nullptr, Render);

    auto WorldMatrix = ::glm::identity<::glm::mat4>();

    auto TestCallRender = [&](
      float _X, float _Y, float _Z,
      float _A, float _B, float _C,
      float _Sx, float _Sy, float _Sz)
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

      WorldMatrix = ::glm::identity<::glm::mat4>();
      WorldMatrix = ::glm::scale(WorldMatrix, { _Sx, _Sy, _Sz });
      WorldMatrix = ::glm::rotate(WorldMatrix, _C, { 0.0f, 0.0f, 1.0f });
      WorldMatrix = ::glm::rotate(WorldMatrix, _B, { 0.0f, 1.0f, 0.0f });
      WorldMatrix = ::glm::rotate(WorldMatrix, _A, { 1.0f, 0.0f, 0.0f });
      WorldMatrix = ::glm::translate(WorldMatrix, { _X, _Y, _Z });
      WorldMatrix = ::glm::transpose(WorldMatrix);

      EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferId))
        .Times(1);

      IndexBufferRender();

      EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
        .Times(1)
        .WillOnce(Return(&ProgramId));

      EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("MatricesData.World")))
        .Times(1)
        .WillOnce(Return(MatrixWorldLocationId));

      EXPECT_CALL(GLProxy,
        UniformMatrix4fv(MatrixWorldLocationId, 1, GL_FALSE, WorldMatrix))
        .Times(1);

      EXPECT_CALL(GLProxy, DrawElements(GL_TRIANGLES, (int)Indices.size(),
        GL_UNSIGNED_INT, nullptr))
        .Times(1);

      EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
        .Times(1);

      Render();
    };

    // Два вызова, чтобы убедиться, что изменение исходных данных приводит 
    // к изменению результата рендеринга.

    TestCallRender(
      1956.0f, 1957.0f, 1958.0f,
      1204.0f, 1205.0f, 1206.0f,
      1152.0f, 1153.0f, 1154.0f);

    TestCallRender(
      1959.0f, 1960.0f, 1961.0f,
      1145.0f, 1146.0f, 1147.0f,
      1155.0f, 1157.0f, 1158.0f);

    EXPECT_CALL(GLProxy, DeleteBuffers(1, BufferId))
      .Times(1);
  };

  {
    // Индексный буфер в компоненте буфера

    const auto pIndexBuffer = Component_t::Make(
      {
        { uT("kind"), uT("Index") },
        { uT("data"), Indices.data() },
        { uT("count"), Indices.size() },
      });

    const auto pPresent = Component_t::Make(
      {
        { uT("kind"), uT("Geometry") },
      });

    TestCallRender(pIndexBuffer, pPresent);
  }

  {
    // Индексный буфер через компонент Data

    auto IndexBufferDataRender = itDataCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("data"), Indices.data() },
        { uT("count"), Indices.size() },
      }));
    EXPECT_EQ(nullptr, IndexBufferDataRender);

    const auto pIndexBuffer = Component_t::Make(
      {
        { uT("kind"), uT("Index") },
      });

    const auto pPresent = Component_t::Make(
      {
        { uT("kind"), uT("Geometry") },
      });

    TestCallRender(pIndexBuffer, pPresent);
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, DISABLED_Test_Present_Geometry_Static_deprecated)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  const ::mock::GLuint BufferId = 1908241121;
  const ::mock::GLint ProgramId = 1908241203;
  const ::mock::GLint MatrixWorldLocationId = 1908241207;

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

  auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pIndexBuffer,
    const Component_t::ComponentPtr_t & _pPresent)
  {
    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenBuffers(1))
      .Times(1)
      .WillOnce(Return(BufferId));

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferId))
      .Times(1);

    const auto Size =
      static_cast<::mock::GLsizeiptr>(Indices.size() * sizeof(int));

    EXPECT_CALL(GLProxy, BufferData(GL_ELEMENT_ARRAY_BUFFER, Size, 
      GetExpected(Indices), GL_STATIC_DRAW))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    const auto IndexBufferRender = itBufferCreator->second(_pIndexBuffer);
    ASSERT_NE(nullptr, IndexBufferRender);

    auto WorldMatrix = ::glm::identity<::glm::mat4>();

    auto SetValues = [&](const bool _IsBuildMatrix,
      float _X, float _Y, float _Z,
      float _A, float _B, float _C,
      float _Sx, float _Sy, float _Sz)
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

      if (_IsBuildMatrix)
      {
        WorldMatrix = ::glm::identity<::glm::mat4>();
        WorldMatrix = ::glm::scale(WorldMatrix, { _Sx, _Sy, _Sz });
        WorldMatrix = ::glm::rotate(WorldMatrix, _C, { 0.0f, 0.0f, 1.0f });
        WorldMatrix = ::glm::rotate(WorldMatrix, _B, { 0.0f, 1.0f, 0.0f });
        WorldMatrix = ::glm::rotate(WorldMatrix, _A, { 1.0f, 0.0f, 0.0f });
        WorldMatrix = ::glm::translate(WorldMatrix, { _X, _Y, _Z });
        WorldMatrix = ::glm::transpose(WorldMatrix);
      }
    };

    SetValues(true,
      1956.0f, 1957.0f, 1958.0f,
      1204.0f, 1205.0f, 1206.0f,
      1152.0f, 1153.0f, 1154.0f);

    auto PositionRender = itDataCreator->second(pPosition);
    EXPECT_EQ(nullptr, PositionRender);

    auto RotationRender = itDataCreator->second(pRotation);
    EXPECT_EQ(nullptr, RotationRender);

    auto ScaleRender = itDataCreator->second(pScale);
    EXPECT_EQ(nullptr, ScaleRender);

    const auto Render = itPresentCreator->second(_pPresent);
    ASSERT_NE(nullptr, Render);

    auto TestCallRender = [&](void)
    {
      EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferId))
        .Times(1);

      IndexBufferRender();

      EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
        .Times(1)
        .WillOnce(Return(&ProgramId));

      EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("MatricesData.World")))
        .Times(1)
        .WillOnce(Return(MatrixWorldLocationId));

      EXPECT_CALL(GLProxy,
        UniformMatrix4fv(MatrixWorldLocationId, 1, GL_FALSE, WorldMatrix))
        .Times(1);

      EXPECT_CALL(GLProxy, DrawElements(GL_TRIANGLES, (int)Indices.size(),
        GL_UNSIGNED_INT, nullptr))
        .Times(1);

      EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
        .Times(1);

      Render();
    };

    // Два вызова, чтобы убедиться, что изменение исходных данных не приводит 
    // к изменению результата рендеринга.

    TestCallRender();

    SetValues(false,
      1959.0f, 1960.0f, 1961.0f,
      1145.0f, 1146.0f, 1147.0f,
      1155.0f, 1157.0f, 1158.0f);

    TestCallRender();

    EXPECT_CALL(GLProxy, DeleteBuffers(1, BufferId))
      .Times(1);
  };

  {
    // Индексный буфер в компоненте буфера

    const auto pIndexBuffer = Component_t::Make(
      {
        { uT("kind"), uT("Index") },
        { uT("data"), Indices.data() },
        { uT("count"), Indices.size() },
      });

    const auto pPresent = Component_t::Make(
      {
        { uT("kind"), uT("Geometry") },
        { uT("variety"), uT("Static") },
      });

    TestCallRender(pIndexBuffer, pPresent);
  }

  {
    // Индексный буфер через компонент Data

    auto IndexBufferDataRender = itDataCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("data"), Indices.data() },
        { uT("count"), Indices.size() },
      }));
    EXPECT_EQ(nullptr, IndexBufferDataRender);

    const auto pIndexBuffer = Component_t::Make(
      {
        { uT("kind"), uT("Index") },
      });

    const auto pPresent = Component_t::Make(
      {
        { uT("kind"), uT("Geometry") },
        { uT("variety"), uT("Static") },
      });

    TestCallRender(pIndexBuffer, pPresent);
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, DISABLED_Test_Present_Geometry_Billboard_deprecated)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  const ::mock::GLuint BufferId = 1908241121;
  const ::mock::GLint ProgramId = 1908241203;
  const ::mock::GLint MatrixWorldLocationId = 1908241207;

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

  auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pIndexBuffer,
    const Component_t::ComponentPtr_t & _pPresent)
  {
    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenBuffers(1))
      .Times(1)
      .WillOnce(Return(BufferId));

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferId))
      .Times(1);

    const auto Size =
      static_cast<::mock::GLsizeiptr>(Indices.size() * sizeof(int));

    EXPECT_CALL(GLProxy, BufferData(GL_ELEMENT_ARRAY_BUFFER, Size, 
      GetExpected(Indices), GL_STATIC_DRAW))
      .Times(1);

    EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    const auto IndexBufferRender = itBufferCreator->second(_pIndexBuffer);
    ASSERT_NE(nullptr, IndexBufferRender);

    auto PositionRender = itDataCreator->second(pPosition);
    EXPECT_EQ(nullptr, PositionRender);

    const auto Render = itPresentCreator->second(_pPresent);
    ASSERT_NE(nullptr, Render);

    auto WorldMatrix = ::glm::identity<::glm::mat4>();

    auto TestCallRender = [&](float _X, float _Y, float _Z)
    {
      pPosition->SetValue(uT("x"), _X);
      pPosition->SetValue(uT("y"), _Y);
      pPosition->SetValue(uT("z"), _Z);

      WorldMatrix = ::glm::identity<::glm::mat4>();
      WorldMatrix = ::glm::translate(WorldMatrix, { _X, _Y, _Z });

      const auto ViewMatrix = ::glm::mat4
      {
        { 1.0f, 2.0f, 3.0f, 4.0f },
        { 5.0f, 6.0f, 7.0f, 8.0f },
        { 9.0f, 10.0f, 11.0f, 12.0f },
        { 13.0f, 14.0f, 15.0f, 16.0f },
      };

      GetCameraMartix(Example).View = ViewMatrix;

      auto TransposeViewMatrix = ViewMatrix;
      TransposeViewMatrix[0][3] = 0.0f;
      TransposeViewMatrix[1][3] = 0.0f;
      TransposeViewMatrix[2][3] = 0.0f;
      TransposeViewMatrix[3][0] = 0.0f;
      TransposeViewMatrix[3][1] = 0.0f;
      TransposeViewMatrix[3][2] = 0.0f;
      TransposeViewMatrix[3][3] = 1.0f;

      WorldMatrix = TransposeViewMatrix *
        ::glm::translate(::glm::identity<::glm::mat4>(), { _X, _Y, _Z });
      WorldMatrix = ::glm::transpose(WorldMatrix);

      EXPECT_CALL(GLProxy, BindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferId))
        .Times(1);

      IndexBufferRender();

      EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
        .Times(1)
        .WillOnce(Return(&ProgramId));

      EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("MatricesData.World")))
        .Times(1)
        .WillOnce(Return(MatrixWorldLocationId));

      EXPECT_CALL(GLProxy,
        UniformMatrix4fv(MatrixWorldLocationId, 1, GL_FALSE, WorldMatrix))
        .Times(1);

      EXPECT_CALL(GLProxy, DrawElements(GL_TRIANGLES, (int)Indices.size(),
        GL_UNSIGNED_INT, nullptr))
        .Times(1);

      EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D, 0))
        .Times(1);

      Render();
    };

    // Два вызова, чтобы убедиться, что изменение исходных данных приводит 
    // к изменению результата рендеринга.

    TestCallRender(1956.0f, 1957.0f, 1958.0f);

    TestCallRender(1959.0f, 1960.0f, 1961.0f);

    EXPECT_CALL(GLProxy, DeleteBuffers(1, BufferId))
      .Times(1);
  };

  {
    // Индексный буфер в компоненте буфера

    const auto pIndexBuffer = Component_t::Make(
      {
        { uT("kind"), uT("Index") },
        { uT("data"), Indices.data() },
        { uT("count"), Indices.size() },
      });

    const auto pPresent = Component_t::Make(
      {
        { uT("kind"), uT("Geometry") },
        { uT("variety"), uT("Billboard") },
      });

    TestCallRender(pIndexBuffer, pPresent);
  }

  {
    // Индексный буфер через компонент Data

    auto IndexBufferDataRender = itDataCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Buffer") },
        { uT("data"), Indices.data() },
        { uT("count"), Indices.size() },
      }));
    EXPECT_EQ(nullptr, IndexBufferDataRender);

    const auto pIndexBuffer = Component_t::Make(
      {
        { uT("kind"), uT("Index") },
      });

    const auto pPresent = Component_t::Make(
      {
        { uT("kind"), uT("Geometry") },
        { uT("variety"), uT("Billboard") },
      });

    TestCallRender(pIndexBuffer, pPresent);
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, DISABLED_Test_Present_Geometry_DefaultTransformValues_deprecated)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  Tested_t Example{ Data_t{} };
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

    GetCameraMartix(Example).View = ViewMatrix;

    auto WorldMatrix = ::glm::identity<::glm::mat4>();

    if (_IsFullTransform)
    {
      WorldMatrix = ::glm::scale(WorldMatrix, { 1.0f, 1.0f, 1.0f });
      WorldMatrix = ::glm::transpose(WorldMatrix);
    }
    else
    {
      auto TransposeViewMatrix = ViewMatrix;
      TransposeViewMatrix[0][3] = 0.0f;
      TransposeViewMatrix[1][3] = 0.0f;
      TransposeViewMatrix[2][3] = 0.0f;
      TransposeViewMatrix[3][0] = 0.0f;
      TransposeViewMatrix[3][1] = 0.0f;
      TransposeViewMatrix[3][2] = 0.0f;
      TransposeViewMatrix[3][3] = 1.0f;

      WorldMatrix = TransposeViewMatrix;
      WorldMatrix = ::glm::transpose(WorldMatrix);
    }

    using namespace ::testing;

    EXPECT_CALL(GLProxy, UniformMatrix4fv(_, _, _, WorldMatrix))
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
TEST_F(OpenGLShader_test, DISABLED_Test_Present_Geometry_CombineTransform_deprecated)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  Tested_t Example{ Data_t{} };
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
    Components.clear();

    SetPosition(1.0f, 2.0f, 3.0f);
    SetRotation(4.0f, 5.0f, 6.0f);
    SetScale(7.0f, 8.0f, 9.0f);

    SetPosition(11.0f, 22.0f, 33.0f);
    SetRotation(44.0f, 55.0f, 66.0f);
    SetScale(77.0f, 88.0f, 99.0f);

    MatrixIdentity = ::glm::transpose(MatrixIdentity);

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

    EXPECT_CALL(GLProxy, UniformMatrix4fv(_, _, _, MatrixIdentity))
      .Times(1);

    Render();
  }

  {
    MatrixIdentity = ::glm::identity<::glm::mat4>();
    Components.clear();

    SetPosition(1.0f, 2.0f, 3.0f);
    SetRotation(4.0f, 5.0f, 6.0f);
    SetScale(7.0f, 8.0f, 9.0f);

    SetPosition(11.0f, 22.0f, 33.0f);
    SetRotation(44.0f, 55.0f, 66.0f);
    SetScale(77.0f, 88.0f, 99.0f);

    MatrixIdentity = ::glm::transpose(MatrixIdentity);

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

    EXPECT_CALL(GLProxy, UniformMatrix4fv(_, _, _, MatrixIdentity))
      .Times(1);

    Render();
  }

  {
    const auto ViewMatrix = ::glm::mat4
    {
      { 1.0f, 2.0f, 3.0f, 4.0f },
      { 5.0f, 6.0f, 7.0f, 8.0f },
      { 9.0f, 10.0f, 11.0f, 12.0f },
      { 13.0f, 14.0f, 15.0f, 16.0f },
    };

    GetCameraMartix(Example).View = ViewMatrix;

    auto TransposeViewMatrix = ViewMatrix;
    TransposeViewMatrix[0][3] = 0.0f;
    TransposeViewMatrix[1][3] = 0.0f;
    TransposeViewMatrix[2][3] = 0.0f;
    TransposeViewMatrix[3][0] = 0.0f;
    TransposeViewMatrix[3][1] = 0.0f;
    TransposeViewMatrix[3][2] = 0.0f;
    TransposeViewMatrix[3][3] = 1.0f;

    MatrixIdentity = TransposeViewMatrix;
    Components.clear();

    SetPosition(1.0f, 2.0f, 3.0f);
    SetPosition(11.0f, 22.0f, 33.0f);
    SetPosition(111.0f, 222.0f, 333.0f);

    MatrixIdentity = ::glm::transpose(MatrixIdentity);

    for (const auto & pComponent : Components)
    {
      auto ScaleRender = itDataCreator->second(pComponent);
      EXPECT_EQ(nullptr, ScaleRender);
    }

    auto Render = itCreator->second(Component_t::Make(
      {
        { uT("kind"), uT("Geometry") },
        { uT("variety"), uT("Billboard") },
      }));
    ASSERT_NE(nullptr, Render);

    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, UniformMatrix4fv(_, _, _, MatrixIdentity))
      .Times(1);

    Render();
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Camera_Orthographic_DefaultPosition_deprecated)
{
  Tested::GetValue() = 1;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  auto Render = itCreator->second(Component_t::Make(
    {
      { uT("kind"), uT("Orthographic") },
    }));
  ASSERT_NE(nullptr, Render);

  auto itShaderCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itShaderCreator);

  auto VsRender = itShaderCreator->second(Component_t::Make(
    {
      { uT("entry"), uT("vsFlat") },
    }));
  ASSERT_NE(nullptr, VsRender);

  auto PsRender = itShaderCreator->second(Component_t::Make(
    {
      { uT("entry"), uT("psColored") },
    }));
  ASSERT_NE(nullptr, PsRender);

  const float Viewport[] =
  {
    1812271148.0f, 1812271149.0f, 1812181722.0f, 1812181723.0f
  };

  using namespace ::testing;

  EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, 0))
    .Times(1);

  EXPECT_CALL(GLProxy, Disable(GL_DEPTH_TEST))
    .Times(1);

  EXPECT_CALL(GLProxy, Enable(GL_DEPTH_TEST))
    .Times(0);

  EXPECT_CALL(GLProxy, Clear(GL_DEPTH_BUFFER_BIT))
    .Times(0);

  EXPECT_CALL(GLProxy, Disable(GL_BLEND))
    .Times(1);

  EXPECT_CALL(GLProxy, Disable(GL_DITHER))
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

  Render(); // Формирует матрицы вида и проекции

  VsRender(); // Нужно, чтобы сформировалась шейдерная программа, иначе
              // не будет вызывана функция glUseProgram().

  const ::mock::GLint ProgramId = 1908251932;
  const ::mock::GLint ProjectionLocationId = 1908251933;
  const ::mock::GLint ViewLocationId = 1908252005;
  const ::mock::GLint ViewInverseLocationId = 1908252013;

  const auto Projection = ::glm::transpose(::glm::ortho(
    Viewport[0], Viewport[0] + Viewport[2],
    Viewport[1] + Viewport[3], Viewport[1],
    1.0f, -1.0f));

  auto View = ::glm::identity<::glm::mat4>();
  const auto ViewInverse = ::glm::transpose(::glm::inverse(View));
  View = ::glm::transpose(View);

  InSequence Dummy;

  EXPECT_CALL(GLProxy, UseProgram(_))
    .Times(1);

  EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
    .Times(1)
    .WillOnce(Return(&ProgramId));

  EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("MatricesData.Projection")))
    .Times(1)
    .WillOnce(Return(ProjectionLocationId));

  EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("MatricesData.View")))
    .Times(1)
    .WillOnce(Return(ViewLocationId));

  EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("MatricesData.ViewInverse")))
    .Times(1)
    .WillOnce(Return(ViewInverseLocationId));

  EXPECT_CALL(GLProxy,
    UniformMatrix4fv(ProjectionLocationId, 1, GL_FALSE, Projection))
    .Times(1);

  EXPECT_CALL(GLProxy,
    UniformMatrix4fv(ViewLocationId, 1, GL_FALSE, View))
    .Times(1);

  EXPECT_CALL(GLProxy,
    UniformMatrix4fv(ViewInverseLocationId, 1, GL_FALSE, ViewInverse))
    .Times(1);

  // *************** Передача других данных шейдеру ********************* //

  EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
    .Times(AtLeast(1));

  EXPECT_CALL(GLProxy, UniformMatrix4fv(_, _, _, _))
    .Times(AtLeast(1));

  // ******************************************************************** //

  PsRender(); // Передача матриц вида и проекции шейдеру
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, DISABLED_Test_Camera_Orthographic_deprecated)
{
  Tested::GetValue() = 1;

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

  auto itShaderCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itShaderCreator);

  auto VsRender = itShaderCreator->second(Component_t::Make(
    {
      { uT("entry"), uT("vsFlat") },
    }));
  ASSERT_NE(nullptr, VsRender);

  auto PsRender = itShaderCreator->second(Component_t::Make(
    {
      { uT("entry"), uT("psColored") },
    }));
  ASSERT_NE(nullptr, PsRender);

  using namespace ::testing;

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
  ASSERT_NE(nullptr, Render);

  InSequence Dummy;

  EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, 0))
    .Times(1);

  EXPECT_CALL(GLProxy, Disable(GL_DEPTH_TEST))
    .Times(1);

  EXPECT_CALL(GLProxy, Enable(GL_DEPTH_TEST))
    .Times(0);

  EXPECT_CALL(GLProxy, Clear(GL_DEPTH_BUFFER_BIT))
    .Times(0);

  EXPECT_CALL(GLProxy, Disable(GL_BLEND))
    .Times(1);

  EXPECT_CALL(GLProxy, Disable(GL_DITHER))
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

  Render(); // Формирует матрицы вида и проекции

  VsRender(); // Нужно, чтобы сформировалась шейдерная программа, иначе
              // не будет вызывана функция glUseProgram().

  EXPECT_CALL(GLProxy, UseProgram(_))
    .Times(1);

  const ::mock::GLint ProgramId = 1908251932;
  const ::mock::GLint ProjectionLocationId = 1908251933;
  const ::mock::GLint ViewLocationId = 1908252005;
  const ::mock::GLint ViewInverseLocationId = 1908252013;

  const auto Projection = ::glm::transpose(::glm::ortho(
    SourceViewport[0] + X, SourceViewport[0] + SourceViewport[2] + X,
    SourceViewport[1] + SourceViewport[3] + Y, SourceViewport[1] + Y,
    1.0f, -1.0f));

  auto View = ::glm::identity<::glm::mat4>();
  const auto ViewInverse = ::glm::transpose(::glm::inverse(View));
  View = ::glm::transpose(View);

  EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
    .Times(1)
    .WillRepeatedly(Return(&ProgramId));

  EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("MatricesData.Projection")))
    .Times(1)
    .WillOnce(Return(ProjectionLocationId));

  EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("MatricesData.View")))
    .Times(1)
    .WillOnce(Return(ViewLocationId));

  EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("MatricesData.ViewInverse")))
    .Times(1)
    .WillOnce(Return(ViewInverseLocationId));

  EXPECT_CALL(GLProxy,
    UniformMatrix4fv(ProjectionLocationId, 1, GL_FALSE, Projection))
    .Times(1);

  EXPECT_CALL(GLProxy,
    UniformMatrix4fv(ViewLocationId, 1, GL_FALSE, View))
    .Times(1);

  EXPECT_CALL(GLProxy,
    UniformMatrix4fv(ViewInverseLocationId, 1, GL_FALSE, ViewInverse))
    .Times(1);

  // *************** Передача других данных шейдеру ********************* //

  EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
    .Times(AtLeast(1));

  EXPECT_CALL(GLProxy, UniformMatrix4fv(_, _, _, _))
    .Times(AtLeast(1));

  // ******************************************************************** //

  PsRender(); // Передача матриц вида и проекции шейдеру
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, DISABLED_Test_Camera_Perspective_deprecated)
{
  Tested::GetValue() = 1;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const float Width = 1024.0f;
  const float Height = 768.0f;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itShaderCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itShaderCreator);

  auto VsRender = itShaderCreator->second(Component_t::Make(
    {
      { uT("entry"), uT("vsFlat") },
    }));
  ASSERT_NE(nullptr, VsRender);

  auto PsRender = itShaderCreator->second(Component_t::Make(
    {
      { uT("entry"), uT("psColored") },
    }));
  ASSERT_NE(nullptr, PsRender);

  using namespace ::testing;

  auto TestCallRender = [&](Render_t & _Render,
    float _AngleY, float _zFar,
    float _X, float _Y, float _Z,
    float _A, float _B, float _C,
    float _Distance)
  {
    ASSERT_NE(nullptr, _Render);

    const float AngleYRadians = _AngleY *
      static_cast<float>(::alicorn::extension::cpp::math::GreedToRadian);

    InSequence Dummy;

    EXPECT_CALL(GLProxy, BindFramebuffer(GL_FRAMEBUFFER, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, Disable(GL_DEPTH_TEST))
      .Times(1);

    EXPECT_CALL(GLProxy, Enable(GL_DEPTH_TEST))
      .Times(0);

    EXPECT_CALL(GLProxy, Clear(GL_DEPTH_BUFFER_BIT))
      .Times(0);

    EXPECT_CALL(GLProxy, Disable(GL_BLEND))
      .Times(1);

    EXPECT_CALL(GLProxy, Disable(GL_DITHER))
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

    _Render(); // Формирует матрицы вида и проекции

    EXPECT_CALL(GLProxy, UseProgram(_))
      .Times(AtLeast(1));

    VsRender(); // Нужно, чтобы сформировалась шейдерная программа, иначе
                // не будет вызывана функция glUseProgram().

    const ::mock::GLint ProgramId = 1908251932;
    const ::mock::GLint ProjectionLocationId = 1908251933;
    const ::mock::GLint ViewLocationId = 1908252005;
    const ::mock::GLint ViewInverseLocationId = 1908252013;

    const auto Projection = ::glm::transpose(::glm::perspectiveFovRH(
      AngleYRadians, Viewport[2], Viewport[3], _zFar, 0.01f));

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

    auto View = ::glm::lookAtRH(
      GetEye(),
      ::glm::vec3{ _X, _Y, _Z },
      ::glm::vec3{ 0.0f, 0.0f, 1.0f });
    const auto ViewInverse = ::glm::transpose(::glm::inverse(View));
    View = ::glm::transpose(View);

    EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
      .Times(1)
      .WillRepeatedly(Return(&ProgramId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("MatricesData.Projection")))
      .Times(1)
      .WillOnce(Return(ProjectionLocationId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("MatricesData.View")))
      .Times(1)
      .WillOnce(Return(ViewLocationId));

    EXPECT_CALL(GLProxy, GetUniformLocation(ProgramId, Eq("MatricesData.ViewInverse")))
      .Times(1)
      .WillOnce(Return(ViewInverseLocationId));

    EXPECT_CALL(GLProxy,
      UniformMatrix4fv(ProjectionLocationId, 1, GL_FALSE, Projection))
      .Times(1);

    EXPECT_CALL(GLProxy,
      UniformMatrix4fv(ViewLocationId, 1, GL_FALSE, View))
      .Times(1);

    EXPECT_CALL(GLProxy,
      UniformMatrix4fv(ViewInverseLocationId, 1, GL_FALSE, ViewInverse))
      .Times(1);

    // *************** Передача других данных шейдеру ********************* //

    EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
      .Times(AtLeast(1));

    EXPECT_CALL(GLProxy, UniformMatrix4fv(_, _, _, _))
      .Times(AtLeast(1));

    // ******************************************************************** //

    PsRender(); // Передача матриц вида и проекции шейдеру
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
TEST_F(OpenGLShader_test, DISABLED_Test_Light_Ambient_deprecated)
{
  Tested::GetValue() = 3;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  using namespace ::testing;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itLightCreator = IExample.GetCreators().find(uT("Light"));
  ASSERT_NE(IExample.GetCreators().end(), itLightCreator);

  auto itShaderCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itShaderCreator);

  auto itCameraCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

  const ::std::string ShaderData =
    "Pixel vs(Vertex _Value)\r\n"
    "float4 ps(Pixel _Value)\r\n";

  auto pVertexShader = Component_t::Make(
    {
      { uT("data"), (const uint8_t *)ShaderData.data() },
      { uT("count"), ShaderData.size() },
      { uT("entry"), uT("vs") },
    });

  auto pPixelShader = Component_t::Make(
    {
      { uT("data"), (const uint8_t *)ShaderData.data() },
      { uT("count"), ShaderData.size() },
      { uT("entry"), uT("ps") },
    });

  ::SceneLights EmptyLights; // = { 0 } недостаточно!
  memset(&EmptyLights, 0, sizeof(EmptyLights));

  ::SceneLights DefaultLights; // = { 0 } недостаточно!
  memset(&DefaultLights, 0, sizeof(DefaultLights));
  DefaultLights.Ambient.Color = ARGBtoVec4(0xFF000000);
  DefaultLights.Ambient.IsValid = 1;

  const auto Ambient = 0xFF031717;

  ::SceneLights ExpectedLights; // = { 0 } недостаточно!
  memset(&ExpectedLights, 0, sizeof(ExpectedLights));
  ExpectedLights.Ambient.Color = ARGBtoVec4(Ambient);
  ExpectedLights.Ambient.IsValid = 1;

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pCamera1,
    const Component_t::ComponentPtr_t & _pCamera2)
  {
    auto pLight = Component_t::Make(
      {
        { uT("kind"), uT("Ambient") },
      });

    auto LightRender = itLightCreator->second(pLight);
    ASSERT_NE(nullptr, LightRender);

    auto VsRender = itShaderCreator->second(pVertexShader);
    ASSERT_NE(nullptr, VsRender);

    auto PsRender = itShaderCreator->second(pPixelShader);
    ASSERT_NE(nullptr, PsRender);

    auto Camera1Render = itCameraCreator->second(_pCamera1);
    ASSERT_NE(nullptr, Camera1Render);

    auto Camera2Render = itCameraCreator->second(_pCamera2);
    ASSERT_NE(nullptr, Camera2Render);

    const float Viewport[] = { 1, 2, 3, 4 };

    EXPECT_CALL(GLProxy, GetFloatv(GL_VIEWPORT))
      .Times(AtLeast(1))
      .WillRepeatedly(Return(Viewport));

    // На случай, если при первом рендеринге будет записан мусор.
    Camera1Render();
    Camera2Render();

    const auto TestUsingLight = [&](
      const Render_t & _Camera,
      const Render_t & _Light,
      const ::SceneLights & _ExpectedLights)
    {
      const ::mock::GLint ProgramId = 1908261929;
      const ::mock::GLint IsValidLocationId = 1908261935;
      const ::mock::GLint ColorLocationId = 1908261936;

      _Camera(); // Активация камеры

      _Light(); // Добавление источников света в список активной камеры

      EXPECT_CALL(GLProxy, UseProgram(_))
        .Times(AtLeast(1));

      VsRender(); // Чтобы создался объект программы шейдера

      EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
        .Times(1)
        .WillOnce(Return(&ProgramId));

      EXPECT_CALL(GLProxy,
        GetUniformLocation(ProgramId, Eq("LightsData.Ambient.IsValid")))
        .Times(1)
        .WillOnce(Return(IsValidLocationId));

      EXPECT_CALL(GLProxy,
        GetUniformLocation(ProgramId, Eq("LightsData.Ambient.Color")))
        .Times(1)
        .WillOnce(Return(ColorLocationId));

      EXPECT_CALL(GLProxy, Uniform1i(IsValidLocationId,
        _ExpectedLights.Ambient.IsValid))
        .Times(1);

      EXPECT_CALL(GLProxy, Uniform4fv(ColorLocationId, 1,
        _ExpectedLights.Ambient.Color))
        .Times(1);

      // *************** Передача других данных шейдеру ********************* //

      EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
        .Times(AtLeast(1));

      EXPECT_CALL(GLProxy, Uniform1i(_, _))
        .Times(1);

      EXPECT_CALL(GLProxy, Uniform4fv(_, _, _))
        .Times(AtLeast(1));

      EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
        .Times(AtLeast(1));

      EXPECT_CALL(GLProxy, Uniform1i(_, _))
        .Times(1);

      // ******************************************************************** //

      PsRender(); // Передача данных источников света шейдеру
    };

    InSequence Dummy;

    TestUsingLight(Camera1Render, [](void) {}, EmptyLights);
    TestUsingLight(Camera2Render, LightRender, EmptyLights);

    pLight->SetValue(uT("color"), Ambient);

    TestUsingLight(Camera1Render, LightRender, EmptyLights);
    TestUsingLight(Camera2Render, [](void) {}, DefaultLights);

    TestUsingLight(Camera1Render, [](void) {}, ExpectedLights);
    TestUsingLight(Camera2Render, [](void) {}, EmptyLights);

    TestUsingLight(Camera1Render, [](void) {}, EmptyLights);
    TestUsingLight(Camera2Render, [](void) {}, EmptyLights);
  };

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

  TestCallRender(pCamera1, pCamera2);

  auto pFocalCamera1 = Component_t::Make(
    {
      { uT("id"), uT("Camera.3") },
      { uT("kind"), uT("Perspective") },
    });

  auto pFocalCamera2 = Component_t::Make(
    {
      { uT("id"), uT("Camera.4") },
      { uT("kind"), uT("Perspective") },
    });

  TestCallRender(pFocalCamera1, pFocalCamera2);
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, DISABLED_Test_Light_Direction_deprecated)
{
  Tested::GetValue() = 3;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  using namespace ::testing;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itLightCreator = IExample.GetCreators().find(uT("Light"));
  ASSERT_NE(IExample.GetCreators().end(), itLightCreator);

  auto itShaderCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itShaderCreator);

  auto itCameraCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

  const ::std::string ShaderData =
    "Pixel vs(Vertex _Value)\r\n"
    "float4 ps(Pixel _Value)\r\n";

  auto pVertexShader = Component_t::Make(
    {
      { uT("data"), (const uint8_t *)ShaderData.data() },
      { uT("count"), ShaderData.size() },
      { uT("entry"), uT("vs") },
    });

  auto pPixelShader = Component_t::Make(
    {
      { uT("data"), (const uint8_t *)ShaderData.data() },
      { uT("count"), ShaderData.size() },
      { uT("entry"), uT("ps") },
    });

  ::SceneLights EmptyLights; // = { 0 } недостаточно!
  memset(&EmptyLights, 0, sizeof(EmptyLights));

  ::SceneLights DefaultLights; // = { 0 } недостаточно!
  memset(&DefaultLights, 0, sizeof(DefaultLights));
  DefaultLights.Direction.IsValid = 1;
  DefaultLights.Direction.Color = ARGBtoVec4(0xFF000000);
  DefaultLights.Direction.Direction = { 1.0f, 0.0f, 0.0f, 0.0f };

  const auto Diffuse = 0xFF031836;

  const auto X = 1901031851.0f;
  const auto Y = 1901031852.0f;
  const auto Z = 1901031854.0f;

  ::SceneLights ExpectedLights; // = { 0 } недостаточно!
  memset(&ExpectedLights, 0, sizeof(ExpectedLights));
  ExpectedLights.Direction.IsValid = 1;
  ExpectedLights.Direction.Color = ARGBtoVec4(Diffuse);
  ExpectedLights.Direction.Direction = { X, Y, Z, 0.0f };

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pCamera1,
    const Component_t::ComponentPtr_t & _pCamera2)
  {
    auto pLight = Component_t::Make(
      {
        { uT("kind"), uT("Direction") },
      });

    auto pDirection = Component_t::Make(
      {
        { uT("kind"), uT("Direction") },
      });

    auto LightRender = itLightCreator->second(pLight);
    ASSERT_NE(nullptr, LightRender);

    auto DirectionRender = itDataCreator->second(pDirection);
    EXPECT_EQ(nullptr, DirectionRender);

    auto LightDataRender = itLightCreator->second(pLight);
    ASSERT_NE(nullptr, LightDataRender);

    auto VsRender = itShaderCreator->second(pVertexShader);
    ASSERT_NE(nullptr, VsRender);

    auto PsRender = itShaderCreator->second(pPixelShader);
    ASSERT_NE(nullptr, PsRender);

    auto Camera1Render = itCameraCreator->second(_pCamera1);
    ASSERT_NE(nullptr, Camera1Render);

    auto Camera2Render = itCameraCreator->second(_pCamera2);
    ASSERT_NE(nullptr, Camera2Render);

    const float Viewport[] = { 1, 2, 3, 4 };

    EXPECT_CALL(GLProxy, GetFloatv(GL_VIEWPORT))
      .Times(AtLeast(1))
      .WillRepeatedly(Return(Viewport));

    // На случай, если при первом рендеринге будет записан мусор.
    Camera1Render();
    Camera2Render();

    const auto TestUsingLight = [&](
      const Render_t & _Camera,
      const Render_t & _Light,
      const ::SceneLights & _ExpectedLights)
    {
      const ::mock::GLint ProgramId = 1908262007;
      const ::mock::GLint IsValidLocationId = 1908262008;
      const ::mock::GLint ColorLocationId = 1908262009;
      const ::mock::GLint DirectionLocationId = 1908262010;

      _Camera(); // Активация камеры

      _Light(); // Добавление источников света в список активной камеры

      EXPECT_CALL(GLProxy, UseProgram(_))
        .Times(AtLeast(1));

      VsRender(); // Чтобы создался объект программы шейдера

      EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
        .Times(1)
        .WillOnce(Return(&ProgramId));

      // *************** Передача других данных шейдеру ********************* //

      EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
        .Times(AtLeast(1));

      EXPECT_CALL(GLProxy, Uniform1i(_, _))
        .Times(1);

      EXPECT_CALL(GLProxy, Uniform4fv(_, _, _))
        .Times(AtLeast(1));

      // ******************************************************************** //

      EXPECT_CALL(GLProxy,
        GetUniformLocation(ProgramId, Eq("LightsData.Direction.IsValid")))
        .Times(1)
        .WillOnce(Return(IsValidLocationId));

      EXPECT_CALL(GLProxy,
        GetUniformLocation(ProgramId, Eq("LightsData.Direction.Color")))
        .Times(1)
        .WillOnce(Return(ColorLocationId));

      EXPECT_CALL(GLProxy,
        GetUniformLocation(ProgramId, Eq("LightsData.Direction.Direction")))
        .Times(1)
        .WillOnce(Return(DirectionLocationId));

      EXPECT_CALL(GLProxy, Uniform1i(IsValidLocationId,
        _ExpectedLights.Direction.IsValid))
        .Times(1);

      EXPECT_CALL(GLProxy, Uniform4fv(ColorLocationId, 1,
        _ExpectedLights.Direction.Color))
        .Times(1);

      EXPECT_CALL(GLProxy, Uniform4fv(DirectionLocationId, 1,
        _ExpectedLights.Direction.Direction))
        .Times(1);

      // *************** Передача других данных шейдеру ********************* //

      EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
        .Times(AtLeast(1));

      EXPECT_CALL(GLProxy, Uniform1i(_, _))
        .Times(1);

      // ******************************************************************** //

      PsRender(); // Передача данных источников света шейдеру
    };

    InSequence Dummy;

    TestUsingLight(Camera1Render, [](void) {}, EmptyLights);
    TestUsingLight(Camera2Render, LightRender, EmptyLights);

    pDirection->SetValue(uT("x"), 1.0f);
    pDirection->SetValue(uT("y"), 0.0f);
    pDirection->SetValue(uT("z"), 0.0f);

    TestUsingLight(Camera1Render, [](void) {}, EmptyLights);
    TestUsingLight(Camera2Render, LightDataRender, DefaultLights);

    pDirection->SetValue(uT("x"), X);
    pDirection->SetValue(uT("y"), Y);
    pDirection->SetValue(uT("z"), Z);

    pLight->SetValue(uT("color"), Diffuse);

    TestUsingLight(Camera1Render, LightDataRender, EmptyLights);
    TestUsingLight(Camera2Render, [](void) {}, DefaultLights);

    TestUsingLight(Camera1Render, [](void) {}, ExpectedLights);
    TestUsingLight(Camera2Render, [](void) {}, EmptyLights);

    TestUsingLight(Camera1Render, [](void) {}, EmptyLights);
    TestUsingLight(Camera2Render, [](void) {}, EmptyLights);
  };

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

  TestCallRender(pCamera1, pCamera2);

  auto pFocalCamera1 = Component_t::Make(
    {
      { uT("id"), uT("Camera.3") },
      { uT("kind"), uT("Perspective") },
    });

  auto pFocalCamera2 = Component_t::Make(
    {
      { uT("id"), uT("Camera.4") },
      { uT("kind"), uT("Perspective") },
    });

  TestCallRender(pFocalCamera1, pFocalCamera2);
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, DISABLED_Test_Light_Points_deprecated)
{
  Tested::GetValue() = 3;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  using namespace ::testing;

  const Tested_t oExample{ Data_t{} };
  const ITested_t & IExample = oExample;

  auto itDataCreator = IExample.GetCreators().find(uT("Data"));
  ASSERT_NE(IExample.GetCreators().end(), itDataCreator);

  auto itLightCreator = IExample.GetCreators().find(uT("Light"));
  ASSERT_NE(IExample.GetCreators().end(), itLightCreator);

  auto itShaderCreator = IExample.GetCreators().find(uT("Shader"));
  ASSERT_NE(IExample.GetCreators().end(), itShaderCreator);

  auto itCameraCreator = IExample.GetCreators().find(uT("Camera"));
  ASSERT_NE(IExample.GetCreators().end(), itCameraCreator);

  const ::std::string ShaderData =
    "Pixel vs(Vertex _Value)\r\n"
    "float4 ps(Pixel _Value)\r\n";

  auto pVertexShader = Component_t::Make(
    {
      { uT("data"), (const uint8_t *)ShaderData.data() },
      { uT("count"), ShaderData.size() },
      { uT("entry"), uT("vs") },
    });

  auto pPixelShader = Component_t::Make(
    {
      { uT("data"), (const uint8_t *)ShaderData.data() },
      { uT("count"), ShaderData.size() },
      { uT("entry"), uT("ps") },
    });

  ::SceneLights EmptyLights; // = { 0 } недостаточно!
  memset(&EmptyLights, 0, sizeof(EmptyLights));

  ::SceneLights DefaultLight; // = { 0 } недостаточно!
  memset(&DefaultLight, 0, sizeof(DefaultLight));
  DefaultLight.Points.Lights[0].Color = ARGBtoVec4(0xFF000000);
  DefaultLight.Points.Lights[0].Position = { 0.0f, 0.0f, 0.0f, 1.0f };
  DefaultLight.Points.Lights[0].Attenuation = { 1.0f, 0.0f, 0.0f, 0.0f };
  DefaultLight.Points.UsedSlotCount = 1;

  class LightData
  {
  public:
    uint32_t Diffuse;
    float X, Y, Z;
    float Const, Linear, Exponent;
  };

  const ::std::vector<LightData> SourceData =
  {
    {
      0xFF031933,
      1901031927.0f, 1901031928.0f, 1901031929.0f,
      1901031930.0f, 1901031931.0f, 1901031932.0f,
    },
    {
      0xFF112233,
      1901032025.0f, 1901032026.0f, 1901032027.0f,
      1901032028.0f, 1901032029.0f, 1901032030.0f,
    },
    {
      0x11111111,
      4.0f, 5.0f, 6.0f,
      7.0f, 8.0f, 9.0f,
    },
    {
      0x11111111,
      4.0f, 5.0f, 6.0f,
      7.0f, 8.0f, 9.0f,
    },
    {
      0x11111111,
      4.0f, 5.0f, 6.0f,
      7.0f, 8.0f, 9.0f,
    },
    {
      0x11111111,
      4.0f, 5.0f, 6.0f,
      7.0f, 8.0f, 9.0f,
    },
    {
      0x11111111,
      4.0f, 5.0f, 6.0f,
      7.0f, 8.0f, 9.0f,
    },
    {
      0x11111111,
      4.0f, 5.0f, 6.0f,
      7.0f, 8.0f, 9.0f,
    },
    {
      0x11111111,
      4.0f, 5.0f, 6.0f,
      7.0f, 8.0f, 9.0f,
    },
    {
      0x11111111,
      4.0f, 5.0f, 6.0f,
      7.0f, 8.0f, 9.0f,
    },
    {
      0x11111111,
      4.0f, 5.0f, 6.0f,
      7.0f, 8.0f, 9.0f,
    },
    {
      0x11111111,
      4.0f, 5.0f, 6.0f,
      7.0f, 8.0f, 9.0f,
    },
    {
      0x11111111,
      4.0f, 5.0f, 6.0f,
      7.0f, 8.0f, 9.0f,
    },
    {
      0x11111111,
      4.0f, 5.0f, 6.0f,
      7.0f, 8.0f, 9.0f,
    },
    {
      0x11111111,
      4.0f, 5.0f, 6.0f,
      7.0f, 8.0f, 9.0f,
    },
    {
      0x11111111,
      4.0f, 5.0f, 6.0f,
      7.0f, 8.0f, 9.0f,
    },
  };

  ::SceneLights DefaultLights; // = { 0 } недостаточно!
  memset(&DefaultLights, 0, sizeof(DefaultLights));
  DefaultLights.Points.UsedSlotCount =
    static_cast<uint32_t>(SourceData.size());

  ::SceneLights ExpectedLights; // = { 0 } недостаточно!
  memset(&ExpectedLights, 0, sizeof(ExpectedLights));
  ExpectedLights.Points.UsedSlotCount =
    static_cast<uint32_t>(SourceData.size());

  for (size_t i = 0; i < SourceData.size(); i++)
  {
    DefaultLights.Points.Lights[i] = DefaultLight.Points.Lights[0];

    ExpectedLights.Points.Lights[i].Color =
      ARGBtoVec4(SourceData[i].Diffuse);
    ExpectedLights.Points.Lights[i].Position =
    { SourceData[i].X, SourceData[i].Y, SourceData[i].Z, 1.0f };
    ExpectedLights.Points.Lights[i].Attenuation =
    { SourceData[i].Const, SourceData[i].Linear, SourceData[i].Exponent, 0.0f };
  }

  const auto TestCallRender = [&](
    const Component_t::ComponentPtr_t & _pCamera1,
    const Component_t::ComponentPtr_t & _pCamera2)
  {
    auto pLight = Component_t::Make(
      {
        { uT("kind"), uT("Point") },
      });

    auto LightRender = itLightCreator->second(pLight);
    ASSERT_NE(nullptr, LightRender);

    ::std::vector<Component_t::ComponentPtr_t> Positions;
    ::std::vector<Component_t::ComponentPtr_t> Attenuations;
    ::std::vector<Component_t::ComponentPtr_t> Lights;
    ::std::vector<Render_t> LightRenders;

    for (auto & Value : SourceData)
    {
      ::boost::ignore_unused(Value);

      auto pPosition = Component_t::Make(
        {
          { uT("kind"), uT("Position") },
        });

      Positions.push_back(pPosition);

      auto PositionRender = itDataCreator->second(pPosition);
      EXPECT_EQ(nullptr, PositionRender);

      auto pAttenuation = Component_t::Make(
        {
          { uT("kind"), uT("Attenuation") },
        });

      auto AttenuationRender = itDataCreator->second(pAttenuation);
      EXPECT_EQ(nullptr, AttenuationRender);

      Attenuations.push_back(pAttenuation);

      auto pLocalLight = Component_t::Make(
        {
          { uT("kind"), uT("Point") },
        });

      auto LocalLightRender = itLightCreator->second(pLocalLight);
      ASSERT_NE(nullptr, LocalLightRender);

      Lights.push_back(pLocalLight);
      LightRenders.push_back(LocalLightRender);
    }

    const auto LightDataRender = [&](void)
    {
      for (auto & Render : LightRenders) Render();
    };

    auto VsRender = itShaderCreator->second(pVertexShader);
    ASSERT_NE(nullptr, VsRender);

    auto PsRender = itShaderCreator->second(pPixelShader);
    ASSERT_NE(nullptr, PsRender);

    auto Camera1Render = itCameraCreator->second(_pCamera1);
    ASSERT_NE(nullptr, Camera1Render);

    auto Camera2Render = itCameraCreator->second(_pCamera2);
    ASSERT_NE(nullptr, Camera2Render);

    const float Viewport[] = { 1, 2, 3, 4 };

    EXPECT_CALL(GLProxy, GetFloatv(GL_VIEWPORT))
      .Times(AtLeast(1))
      .WillRepeatedly(Return(Viewport));

    // На случай, если при первом рендеринге будет записан мусор.
    Camera1Render();
    Camera2Render();

    const auto TestUsingLight = [&](const Render_t & _Camera,
      const Render_t & _Light, const ::SceneLights & _ExpectedLights)
    {
      const ::mock::GLint ProgramId = 1908262028;
      const ::mock::GLint UsedSlodCountId = 1908271923;
      const ::mock::GLint ColorLocationId = 1908262029;
      const ::mock::GLint PositionLocationId = 1908262030;
      const ::mock::GLint AttenuationLocationId = 1908262031;

      _Camera(); // Активация камеры

      _Light(); // Добавление источников света в список активной камеры

      EXPECT_CALL(GLProxy, UseProgram(_))
        .Times(AtLeast(1));

      VsRender(); // Чтобы создался объект программы шейдера

      EXPECT_CALL(GLProxy, GetIntegerv(GL_CURRENT_PROGRAM))
        .Times(1)
        .WillOnce(Return(&ProgramId));

      // *************** Передача других данных шейдеру ********************* //

      EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
        .Times(AtLeast(1));

      EXPECT_CALL(GLProxy, Uniform1i(_, _))
        .Times(1);

      EXPECT_CALL(GLProxy, Uniform4fv(_, _, _))
        .Times(AtLeast(1));

      EXPECT_CALL(GLProxy, GetUniformLocation(_, _))
        .Times(AtLeast(1));

      EXPECT_CALL(GLProxy, Uniform1i(_, _))
        .Times(1);

      EXPECT_CALL(GLProxy, Uniform4fv(_, _, _))
        .Times(AtLeast(1));

      // ******************************************************************** //

      EXPECT_CALL(GLProxy,
        GetUniformLocation(ProgramId, Eq("LightsData.Points.UsedSlotCount")))
        .Times(1)
        .WillOnce(Return(UsedSlodCountId));

      EXPECT_CALL(GLProxy,
        Uniform1i(UsedSlodCountId, _ExpectedLights.Points.UsedSlotCount))
        .Times(1);

      for (int i = 0; i < _ExpectedLights.Points.UsedSlotCount; i++)
      {
        auto & Light = _ExpectedLights.Points.Lights[i];

        using namespace ::alicorn::extension::std;

        const auto Point = string_cast<::std::string, Encoding::Ascii128>(
          uT("LightsData.Points.Lights[<index>]").Replace(uT("<index>"), i));

        EXPECT_CALL(GLProxy,
          GetUniformLocation(ProgramId, Point + ".Color"))
          .Times(1)
          .WillOnce(Return(ColorLocationId + i));

        EXPECT_CALL(GLProxy,
          Uniform4fv(ColorLocationId + i, 1, Light.Color))
          .Times(1);

        EXPECT_CALL(GLProxy,
          GetUniformLocation(ProgramId, Point + ".Position"))
          .Times(1)
          .WillOnce(Return(PositionLocationId + i));

        EXPECT_CALL(GLProxy,
          Uniform4fv(PositionLocationId + i, 1, Light.Position))
          .Times(1);

        EXPECT_CALL(GLProxy,
          GetUniformLocation(ProgramId, Point + ".Attenuation"))
          .Times(1)
          .WillOnce(Return(AttenuationLocationId + i));

        EXPECT_CALL(GLProxy,
          Uniform4fv(AttenuationLocationId + i, 1, Light.Attenuation))
          .Times(1);
      }

      PsRender(); // Передача данных источников света шейдеру
    };

    InSequence Dummy;

    TestUsingLight(Camera1Render, [](void) {}, EmptyLights);
    TestUsingLight(Camera2Render, LightRender, EmptyLights);

    TestUsingLight(Camera1Render, [](void) {}, EmptyLights);
    TestUsingLight(Camera2Render, LightDataRender, DefaultLight);

    for (size_t i = 0; i < SourceData.size(); i++)
    {
      Positions[i]->SetValue(uT("x"), SourceData[i].X);
      Positions[i]->SetValue(uT("y"), SourceData[i].Y);
      Positions[i]->SetValue(uT("z"), SourceData[i].Z);

      Attenuations[i]->SetValue(uT("const"), SourceData[i].Const);
      Attenuations[i]->SetValue(uT("linear"), SourceData[i].Linear);
      Attenuations[i]->SetValue(uT("exponent"), SourceData[i].Exponent);

      Lights[i]->SetValue(uT("color"), SourceData[i].Diffuse);
    }

    TestUsingLight(Camera1Render, LightDataRender, EmptyLights);
    TestUsingLight(Camera2Render, [](void) {}, DefaultLights);

    TestUsingLight(Camera1Render, [](void) {}, ExpectedLights);
    TestUsingLight(Camera2Render, [](void) {}, EmptyLights);

    TestUsingLight(Camera1Render, [](void) {}, EmptyLights);
    TestUsingLight(Camera2Render, [](void) {}, EmptyLights);
  };

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

  TestCallRender(pCamera1, pCamera2);

  auto pFocalCamera1 = Component_t::Make(
    {
      { uT("id"), uT("Camera.3") },
      { uT("kind"), uT("Perspective") },
    });

  auto pFocalCamera2 = Component_t::Make(
    {
      { uT("id"), uT("Camera.4") },
      { uT("kind"), uT("Perspective") },
    });

  TestCallRender(pFocalCamera1, pFocalCamera2);
}

} // unnamed namespace
