
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

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_State_Sampler_Texture_Mipmapping)
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
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_State_Sampler_TextureArray_Mipmapping)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itStateCreator = IExample.GetCreators().find(uT("State"));
  ASSERT_NE(IExample.GetCreators().end(), itStateCreator);

  auto itTextureCreator = IExample.GetCreators().find(uT("TextureArray"));
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

  EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D_ARRAY, _))
    .Times(1);

  EXPECT_CALL(GLProxy, TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER,
    GL_LINEAR_MIPMAP_LINEAR))
    .Times(1);

  EXPECT_CALL(GLProxy, TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER,
    GL_LINEAR))
    .Times(1);

  EXPECT_CALL(GLProxy, TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT))
    .Times(1);

  EXPECT_CALL(GLProxy, TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT))
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
    SettingsData.ClientRect.Top = 34;

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

    EXPECT_CALL(GLProxy, Viewport(_, _, _, _))
      .Times(0);

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
      const Texture_t::Ptr_t pTexture =
        (*pTextureComponent)[uT("entity")].Default(Texture_t::Ptr_t{});
      ASSERT_NE(nullptr, pTexture);
      EXPECT_EQ(TextureId + i, pTexture->m_TextureId);
      EXPECT_EQ(_IsUseMapper ? 4 * _ExpectedWidth * _ExpectedHeight : 0,
        pTexture->m_ReadCopyData.size());
      EXPECT_EQ(_ExpectedWidth, (int)(*pTextureComponent)[uT("width")].Default(0));
      EXPECT_EQ(_ExpectedHeight, (int)(*pTextureComponent)[uT("height")].Default(0));
    }
  };

  TestCall(Component_t::Make({ }), 
    false, 12, 45, 12, 45);
  TestCall(Component_t::Make({ }), 
    true, 45, 12, 45, 12);
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
    SettingsData.ClientRect.Top = 45;

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
      EXPECT_EQ(_ExpectedWidth, (int)(*pTextureComponent)[uT("width")].Default(0));
      EXPECT_EQ(_ExpectedHeight, (int)(*pTextureComponent)[uT("height")].Default(0));

      const Texture_t::Ptr_t pTexture =
        (*pTextureComponent)[uT("entity")].Default(Texture_t::Ptr_t{});
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
      .Times(0);

    Render();

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    IExample.PresentFrame();

    const int Viewport2[] = { 0, 0, _Width * 2, _Height * 2 };

    IExample.ResizeWindow(::covellite::Rect{ 0, 0, _Width * 2, _Height * 2 });

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

    EXPECT_CALL(GLProxy, Viewport(_, _, _, _))
      .Times(0);

    Render();

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    IExample.PresentFrame();

    EXPECT_EQ(_ExpectedWidth2, (int)(*_pBkSurface)[uT("width")]);
    EXPECT_EQ(_ExpectedHeight2, (int)(*_pBkSurface)[uT("height")]);

    for (::std::size_t i = 0; i < TextureComponents.size(); i++)
    {
      const auto pTextureComponent = TextureComponents[i];
      EXPECT_EQ(_ExpectedWidth2, (int)(*pTextureComponent)[uT("width")].Default(0));
      EXPECT_EQ(_ExpectedHeight2, (int)(*pTextureComponent)[uT("height")].Default(0));

      const Texture_t::Ptr_t pTexture =
        (*pTextureComponent)[uT("entity")].Default(Texture_t::Ptr_t{});
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
      .Times(0);

    Render();

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    IExample.PresentFrame();
  };

  TestCall(Component_t::Make({ }), 
    false, 45, 67, 45, 67, 90, 134);
  TestCall(Component_t::Make({ }), 
    true, 45, 67, 45, 67, 90, 134);
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
      (".*Compile shader fail \\[header line: 173\\]: " + ErrorText).c_str());
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
        { uT("entry"), uT("psTextured") },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }), "psTextured");
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
  using namespace ::alicorn::extension::std;

  const ::std::string ShaderData =
    "Pixel vs(Vertex _Value)\r\n"
    "float4 ps(Pixel _Value)\r\n";

  const ::std::string ErrorText = "Error1908291952";
  const auto LinkFailLogText = uT("Warning: Link program fail: ") +
    string_cast<String, Encoding::UTF8>(ErrorText);

  using LoggerProxy_t = ::mock::alicorn::modules::logger::LoggerProxy;
  LoggerProxy_t LoggerProxy;
  LoggerProxy_t::GetInstance() = &LoggerProxy;

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

      EXPECT_CALL(LoggerProxy, ToLog(Eq(LinkFailLogText)))
        .Times(1);

      _Render();
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
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Texture_Destination_Capacity16)
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

    (*_pDataTexture)[uT("entity")] = pTexture;

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

    (*_pDataTexture)[uT("entity")] = pTexture;

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

    (*_pDataTexture)[uT("entity")] = pTexture;

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

    (*_pDataTexture)[uT("entity")] = pTexture;

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

    (*_pDataTexture)[uT("entity")] = pTexture;

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

    (*_pDataTexture)[uT("entity")] = pTexture;

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

    (*_pData)[uT("entity")] = ::std::make_shared<Texture_t>(TextureData);

    EXPECT_CALL(GLProxy, GenTextures(_))
      .Times(0);

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    const Texture_t::Ptr_t pEntity = (*_pData)[uT("entity")].Default(Texture_t::Ptr_t{});
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

    (*_pData)[uT("entity")] = ::std::make_shared<Texture_t>(TextureData);

    EXPECT_CALL(GLProxy, GenTextures(_))
      .Times(0);

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    const Texture_t::Ptr_t pEntity = (*_pData)[uT("entity")].Default(Texture_t::Ptr_t{});
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
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_TextureArray_Default)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("TextureArray"));
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

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D_ARRAY, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8,
      _Width, _Height, 1))
      .Times(1);

    EXPECT_CALL(GLProxy, TexSubImage3D_1(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0,
      _Width, _Height))
      .Times(1);

    EXPECT_CALL(GLProxy, TexSubImage3D_2(1, GL_RGBA,
      GL_UNSIGNED_BYTE, _Source))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D_ARRAY, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D_ARRAY, TextureId))
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
        { uT("content"), ::std::vector{ Source } },
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
      0x25, 0x04, 0x07, 0x21, 0x03,
      0x25, 0x04, 0x07, 0x21, 0x04,
      0x25, 0x04, 0x07, 0x21, 0x05,
      0x25, 0x04, 0x07, 0x21, 0x06
    };
    IntroduceBufferSize(Source);

    const int Width = 1907251057;
    const int Height = 1907251058;

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("TextureArray") },
        { uT("content"), ::std::vector{ Source } },
        { uT("width"), Width },
        { uT("height"), Height },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
      Source, Width, Height);
  }
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_TextureArray_NameAndIndex)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("TextureArray"));
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

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D_ARRAY, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8,
      _Width, _Height, 1))
      .Times(1);

    EXPECT_CALL(GLProxy, TexSubImage3D_1(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0,
      _Width, _Height))
      .Times(1);

    EXPECT_CALL(GLProxy, TexSubImage3D_2(1, GL_RGBA,
      GL_UNSIGNED_BYTE, _Source))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D_ARRAY, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D_ARRAY, TextureId))
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
          { uT("content"), ::std::vector{ Source } },
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
          { uT("kind"), uT("TextureArray") },
          { uT("content"), ::std::vector{ Source } },
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
          { uT("content"), ::std::vector{ Source } },
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
          { uT("kind"), uT("TextureArray") },
          { uT("content"), ::std::vector{ Source } },
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
            { uT("content"), ::std::vector{ Source } },
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
            { uT("kind"), uT("TextureArray") },
            { uT("content"), ::std::vector{ Source } },
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
            { uT("content"), ::std::vector{ Source } },
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
            { uT("kind"), uT("TextureArray") },
            { uT("content"), ::std::vector{ Source } },
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
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_TextureArray_Destination)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("TextureArray"));
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

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D_ARRAY, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8,
      _Width, _Height, 1))
      .Times(1);

    EXPECT_CALL(GLProxy, TexSubImage3D_1(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0,
      _Width, _Height))
      .Times(1);

    EXPECT_CALL(GLProxy, TexSubImage3D_2(1, GL_RGBA,
      GL_UNSIGNED_BYTE, _Source))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D_ARRAY, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D_ARRAY, TextureId))
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
          { uT("content"), ::std::vector{ Source } },
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
          { uT("kind"), uT("TextureArray") },
          { uT("content"), ::std::vector{ Source } },
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
          { uT("content"), ::std::vector{ Source } },
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
          { uT("kind"), uT("TextureArray") },
          { uT("content"), ::std::vector{ Source } },
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
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_TextureArray_Mipmapping_NameAndIndex)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("TextureArray"));
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

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D_ARRAY, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexStorage3D(GL_TEXTURE_2D_ARRAY, 8, GL_RGBA8,
      _Width, _Height, 1))
      .Times(1);

    EXPECT_CALL(GLProxy, TexSubImage3D_1(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0,
      _Width, _Height))
      .Times(1);

    EXPECT_CALL(GLProxy, TexSubImage3D_2(1, GL_RGBA,
      GL_UNSIGNED_BYTE, _Source))
      .Times(1);

    EXPECT_CALL(GLProxy, GenerateMipmap(GL_TEXTURE_2D_ARRAY))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D_ARRAY, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D_ARRAY, TextureId))
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
          { uT("content"), ::std::vector{ Source } },
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
          { uT("kind"), uT("TextureArray") },
          { uT("content"), ::std::vector{ Source } },
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
            { uT("content"), ::std::vector{ Source } },
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
            { uT("kind"), uT("TextureArray") },
            { uT("content"), ::std::vector{ Source } },
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
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_TextureArray_Mipmapping_Destination)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("TextureArray"));
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

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D_ARRAY, TextureId))
      .Times(1);

    EXPECT_CALL(GLProxy, TexStorage3D(GL_TEXTURE_2D_ARRAY, 8, GL_RGBA8,
      _Width, _Height, 1))
      .Times(1);

    EXPECT_CALL(GLProxy, TexSubImage3D_1(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0,
      _Width, _Height))
      .Times(1);

    EXPECT_CALL(GLProxy, TexSubImage3D_2(1, GL_RGBA,
      GL_UNSIGNED_BYTE, _Source))
      .Times(1);

    EXPECT_CALL(GLProxy, GenerateMipmap(GL_TEXTURE_2D_ARRAY))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D_ARRAY, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, ActiveTexture(GL_TEXTURE0 + Index))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(GL_TEXTURE_2D_ARRAY, TextureId))
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
          { uT("content"), ::std::vector{ Source } },
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
          { uT("kind"), uT("TextureArray") },
          { uT("content"), ::std::vector{ Source } },
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
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_TextureArray_NoDeclaredInShader)
{
  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("TextureArray"));
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
          { uT("content"), ::std::vector{ Source } },
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
          { uT("kind"), uT("TextureArray") },
          { uT("content"), ::std::vector{ Source } },
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
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_TextureArray_MultiTextureData)
{
  using namespace ::alicorn::extension::std;

  using GLProxy_t = ::mock::GLProxy;
  GLProxy_t GLProxy;
  GLProxy_t::GetInstance() = &GLProxy;

  const Tested_t Example{ Data_t{} };
  const ITested_t & IExample = Example;

  auto itCreator = IExample.GetCreators().find(uT("TextureArray"));
  ASSERT_NE(IExample.GetCreators().end(), itCreator);

  const ::mock::GLuint TextureId = 1812181809;
  const ::mock::GLint ProgramId = 1908221258;
  const ::mock::GLint LocationId = 1908221259;

  const auto TestCall = [&](const Component_t::ComponentPtr_t & _pTexture,
    const ::std::vector<uint8_t> & _Source, const size_t _ArrayCount,
    const int _Width, const int _Height)
  {
    using namespace ::testing;

    InSequence Dummy;

    EXPECT_CALL(GLProxy, GenTextures(1))
      .Times(1)
      .WillOnce(Return(TextureId));

    EXPECT_CALL(GLProxy, BindTexture(_, _))
      .Times(1);

    EXPECT_CALL(GLProxy, TexStorage3D(_, _, _, _, _, _ArrayCount))
      .Times(1);

    EXPECT_CALL(GLProxy, TexSubImage3D_1(_, 0, 0, 0, 0, _, _))
      .Times(1);

    EXPECT_CALL(GLProxy, TexSubImage3D_2(_ArrayCount, _, _, _Source))
      .Times(1);

    EXPECT_CALL(GLProxy, BindTexture(_, 0))
      .Times(1);

    EXPECT_CALL(GLProxy, GetError())
      .Times(1)
      .WillOnce(Return(GL_NO_ERROR));

    auto Render = itCreator->second(_pTexture);
    ASSERT_NE(nullptr, Render);
  };

  {
    ::std::vector<uint8_t> Source1 =
    {
      0x20, 0x06, 0x15, 0x12, 0x28,
      0x20, 0x06, 0x15, 0x12, 0x28,
      0x20, 0x06, 0x15, 0x12, 0x28,
      0x20, 0x06, 0x15, 0x12, 0x28,
      0x20, 0x06, 0x15, 0x12, 0x28,
    };
    IntroduceBufferSize(Source1, false);

    ::std::vector<uint8_t> Source2 =
    {
      0x20, 0x06, 0x15, 0x12, 0x28,
      0x20, 0x06, 0x15, 0x12, 0x28,
      0x20, 0x06, 0x15, 0x12, 0x28,
      0x20, 0x06, 0x15, 0x12, 0x28,
      0x20, 0x06, 0x15, 0x12, 0x28,
      0x20, 0x06, 0x15, 0x12, 0x28,
    };
    IntroduceBufferSize(Source2, false);

    ::std::vector<uint8_t> Source3 =
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
    IntroduceBufferSize(Source3);

    const int Width = 1812181807;
    const int Height = 1812181808;

    const auto pTexture = Component_t::Make(
      {
        { uT("content"), ::std::vector{ Source1, Source2, Source3 } },
        { uT("width"), Width },
        { uT("height"), Height },
      });

    TestCall(pTexture, Source1 + Source2 + Source3, 3, Width, Height);
  }

  {
    ::std::vector<uint8_t> Source1 =
    {
      0x20, 0x06, 0x15, 0x12, 0x29,
      0x20, 0x06, 0x15, 0x12, 0x29,
      0x20, 0x06, 0x15, 0x12, 0x29,
    };
    IntroduceBufferSize(Source1, false);

    ::std::vector<uint8_t> Source2 =
    {
      0x25, 0x04, 0x07, 0x21, 0x03,
      0x25, 0x04, 0x07, 0x21, 0x04,
      0x25, 0x04, 0x07, 0x21, 0x05,
      0x25, 0x04, 0x07, 0x21, 0x06
    };
    IntroduceBufferSize(Source2, true);

    const int Width = 1907251057;
    const int Height = 1907251058;

    const auto pData = Component_t::Make(
      {
        { uT("kind"), uT("TextureArray") },
        { uT("content"), ::std::vector{ Source1, Source2 } },
        { uT("width"), Width },
        { uT("height"), Height },
      });

    TestCall(Component_t::Make({ { uT("service"), Object_t{ pData } } }),
      Source1 + Source2, 2, Width, Height);
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
    (*pPosition)[uT("x")] = _X;
    (*pPosition)[uT("y")] = _Y;
    (*pPosition)[uT("z")] = _Z;

    (*pRotation)[uT("x")] = _A;
    (*pRotation)[uT("y")] = _B;
    (*pRotation)[uT("z")] = _C;

    (*pScale)[uT("x")] = _Sx;
    (*pScale)[uT("y")] = _Sy;
    (*pScale)[uT("z")] = _Sz;

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
      (*pPosition)[uT("x")] = _X;
      (*pPosition)[uT("y")] = _Y;
      (*pPosition)[uT("z")] = _Z;

      (*pRotation)[uT("x")] = _A;
      (*pRotation)[uT("y")] = _B;
      (*pRotation)[uT("z")] = _C;

      (*pScale)[uT("x")] = _Sx;
      (*pScale)[uT("y")] = _Sy;
      (*pScale)[uT("z")] = _Sz;

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
      (*pPosition)[uT("x")] = _X;
      (*pPosition)[uT("y")] = _Y;
      (*pPosition)[uT("z")] = _Z;

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

  EXPECT_CALL(GLProxy, GetProgramiv(_, _))
    .Times(1)
    .WillOnce(Return(GL_TRUE));

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

  EXPECT_EQ(View, (::glm::mat4)(*pCameraComponent)[uT("view")].Default(::glm::mat4{ 1.0f }));
  EXPECT_EQ(Projection, (::glm::mat4)(*pCameraComponent)[uT("projection")].Default(::glm::mat4{ 1.0f }));

  VsRender(); // Нужно, чтобы сформировалась шейдерная программа, иначе
              // не будет вызывана функция glUseProgram().

  EXPECT_CALL(GLProxy, GetProgramiv(_, _))
    .Times(1)
    .WillOnce(Return(GL_TRUE));

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
  ::testing::DefaultValue<int>::Set(GL_TRUE);

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
      ::alicorn::extension::cpp::math::Constant<float>::DegreeToRadian;

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

    EXPECT_EQ(View, (::glm::mat4)(*_pCameraComponent)[uT("view")].Default(::glm::mat4{ 1.0f }));
    EXPECT_EQ(Projection, (::glm::mat4)(*_pCameraComponent)[uT("projection")].Default(::glm::mat4{ 1.0f }));

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

    ::testing::DefaultValue<int>::Clear();
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

    EXPECT_CALL(GLProxy, Viewport(0, 0, _, _))
      .Times(1);

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

    EXPECT_CALL(GLProxy, Viewport(0, 0, _, _))
      .Times(1);

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

    EXPECT_CALL(GLProxy, Viewport(0, 0, _, _))
      .Times(1);

    auto Render = itCreator->second(pCamera);

    for (size_t i = 1; i < CameraInfos.size(); i++)
    {
      auto Info = CameraInfos[i];

      (*pPosition)[uT("x")] = Info.X;
      (*pPosition)[uT("y")] = Info.Y;
      (*pPosition)[uT("z")] = Info.Z;

      (*pRotation)[uT("x")] = Info.A;
      (*pRotation)[uT("y")] = Info.B;
      (*pRotation)[uT("z")] = Info.C;

      (*pCamera)[uT("fov")] = Info.AngleY;
      (*pCamera)[uT("znear")] = Info.zNear;
      (*pCamera)[uT("zfar")] = Info.zFar;
      (*pCamera)[uT("distance")] = Info.Distance;

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
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Camera_SetWindowViewport)
{
  const auto TestCall = [](const Component_t::ComponentPtr_t & _pCamera)
  {
    ::mock::WindowsProxy WindowsProxy;
    ::mock::GLProxy GLProxy;

    Data_t Data;
    Data.ClientRect.Top = 12345;
    Data.ClientRect.Width = 2008131258;
    Data.ClientRect.Height = 2008131259;

    using namespace ::testing;

    Tested_t Example{ Data };
    ITested_t & IExample = Example;

    auto itCreator = IExample.GetCreators().find(uT("Camera"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    EXPECT_CALL(GLProxy, Viewport(0, 0, Data.ClientRect.Width, Data.ClientRect.Height - Data.ClientRect.Top))
      .Times(1);

    auto Render = itCreator->second(_pCamera);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, Viewport(0, 0, Data.ClientRect.Width, Data.ClientRect.Height - Data.ClientRect.Top))
      .Times(1);

    Render();

    IExample.ResizeWindow(::covellite::Rect{ 0, 54321, 2008131300, 2008131301 });

    EXPECT_CALL(GLProxy, Viewport(0, 0, 2008131300, 2008131301 - 54321))
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
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Camera_SetScaleViewport)
{
  const auto TestCall = [](const float _Scale, 
    const Component_t::ComponentPtr_t & _pCamera)
  {
    ::mock::WindowsProxy WindowsProxy;
    ::mock::GLProxy GLProxy;

    Data_t Data;
    Data.ClientRect.Top = 123;
    Data.ClientRect.Width = 1024;
    Data.ClientRect.Height = 2048;

    const auto ExpectedWidth = static_cast<int>(_Scale * Data.ClientRect.Width);
    const auto ExpectedHeight = static_cast<int>(_Scale * (Data.ClientRect.Height - Data.ClientRect.Top));

    using namespace ::testing;

    Tested_t Example{ Data };
    ITested_t & IExample = Example;

    auto itCreator = IExample.GetCreators().find(uT("Camera"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    EXPECT_CALL(GLProxy, Viewport(0, 0, ExpectedWidth, ExpectedHeight))
      .Times(1);

    auto Render = itCreator->second(_pCamera);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, Viewport(0, 0, ExpectedWidth, ExpectedHeight))
      .Times(1);

    Render();

    IExample.ResizeWindow(::covellite::Rect{ 0, 321, 2048, 4096 });

    const auto ExpectedWidth2 = static_cast<int>(_Scale * 2048);
    const auto ExpectedHeight2 = static_cast<int>(_Scale * (4096 - 321));

    EXPECT_CALL(GLProxy, Viewport(0, 0, ExpectedWidth2, ExpectedHeight2))
      .Times(1);

    Render();
  };

  TestCall(0.5f, Component_t::Make(
    {
      { uT("kind"), uT("Orthographic") },
      { uT("scale"), 0.5f },
    }));

  TestCall(2.0f, Component_t::Make(
    {
      { uT("kind"), uT("Perspective") },
      { uT("scale"), 2.0f },
    }));

  TestCall(0.25f, Component_t::Make(
    {
      { uT("kind"), uT("Orthographic") },
      { uT("scale"), 0.25f },
      { uT("width"), 2009081959 },
      { uT("height"), 2009082000 },
   }));

  TestCall(4.0f, Component_t::Make(
    {
      { uT("kind"), uT("Perspective") },
      { uT("scale"), 4.0f },
      { uT("width"), 2009081959 },
      { uT("height"), 2009082000 },
    }));
}

// ************************************************************************** //
TEST_F(OpenGLShader_test, /*DISABLED_*/Test_Camera_SetUserViewport)
{
  const auto TestCall = [](const int _Width, const int _Height,
    const Component_t::ComponentPtr_t & _pCamera)
  {
    ::mock::WindowsProxy WindowsProxy;
    ::mock::GLProxy GLProxy;

    Data_t Data;
    Data.ClientRect.Top = 345;
    Data.ClientRect.Width = 2009082008;
    Data.ClientRect.Height = 2009082009;

    using namespace ::testing;

    Tested_t Example{ Data };
    ITested_t & IExample = Example;

    auto itCreator = IExample.GetCreators().find(uT("Camera"));
    ASSERT_NE(IExample.GetCreators().end(), itCreator);

    EXPECT_CALL(GLProxy, Viewport(0, 0, _Width, _Height))
      .Times(1);

    auto Render = itCreator->second(_pCamera);
    ASSERT_NE(nullptr, Render);

    EXPECT_CALL(GLProxy, Viewport(0, 0, _Width, _Height))
      .Times(1);

    Render();

    IExample.ResizeWindow(::covellite::Rect{ 0, 543, 2008131300, 2008131301 });

    EXPECT_CALL(GLProxy, Viewport(0, 0, _Width, _Height))
      .Times(1);

    Render();
  };

  TestCall(1024, 2048, Component_t::Make(
    {
      { uT("kind"), uT("Orthographic") },
      { uT("width"), 1024 },
      { uT("height"), 2048 },
    }));

  TestCall(2048, 4096, Component_t::Make(
    {
      { uT("kind"), uT("Perspective") },
      { uT("width"), 2048 },
      { uT("height"), 4096 },
    }));
}

} // unnamed namespace
