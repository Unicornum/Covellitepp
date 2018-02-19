
#pragma once

namespace mock
{

class EglProxy :
  public ::alicorn::extension::testing::Proxy<EglProxy>
{
public:
  MOCK_METHOD0(GetError, ::covellite::egl::EGLint(void));
  MOCK_METHOD1(GetDisplay, ::covellite::egl::EGLDisplay(size_t));
  MOCK_METHOD3(Initialize, ::covellite::egl::EGLBoolean(::covellite::egl::EGLDisplay, void *, void *));
  MOCK_METHOD4(MakeCurrent, ::covellite::egl::EGLBoolean(::covellite::egl::EGLDisplay, 
    ::covellite::egl::EGLSurface, ::covellite::egl::EGLSurface, ::covellite::egl::EGLContext));
  MOCK_METHOD1(Terminate, ::covellite::egl::EGLBoolean(::covellite::egl::EGLDisplay));
  MOCK_METHOD4(ChooseConfig, ::covellite::egl::EGLBoolean(::covellite::egl::EGLDisplay,
    const ::covellite::egl::EGLint *, ::covellite::egl::EGLint, ::covellite::egl::EGLint *));
  MOCK_METHOD3(GetConfigAttrib, ::covellite::egl::EGLBoolean(::covellite::egl::EGLDisplay,
    ::covellite::egl::EGLConfig, ::covellite::egl::EGLint));
  MOCK_METHOD4(SetBuffersGeometry, int32_t(::ANativeWindow *,
    int32_t, int32_t, int32_t));
  MOCK_METHOD4(CreateContext, ::covellite::egl::EGLContext(::covellite::egl::EGLDisplay,
    ::covellite::egl::EGLConfig, ::covellite::egl::EGLContext, const ::covellite::egl::EGLint *));
  MOCK_METHOD2(DestroyContext, ::covellite::egl::EGLBoolean(::covellite::egl::EGLDisplay, 
    ::covellite::egl::EGLContext));
  MOCK_METHOD4(CreateSurface, ::covellite::egl::EGLContext(::covellite::egl::EGLDisplay,
    ::covellite::egl::EGLConfig, ANativeWindow *, const ::covellite::egl::EGLint *));
  MOCK_METHOD2(DestroySurface, ::covellite::egl::EGLBoolean(::covellite::egl::EGLDisplay,
    ::covellite::egl::EGLSurface));
  MOCK_METHOD3(QuerySurface, ::covellite::egl::EGLBoolean(::covellite::egl::EGLDisplay,
    ::covellite::egl::EGLSurface, ::covellite::egl::EGLint));
  MOCK_METHOD2(SwapBuffers, ::covellite::egl::EGLBoolean(::covellite::egl::EGLDisplay,
    ::covellite::egl::EGLSurface));

public:
  ::covellite::egl::EGLConfig *    m_pConfig = nullptr;
  ::covellite::egl::EGLint         m_Value = 0;
  const ::covellite::egl::EGLint * m_pAttributes = nullptr;
};

} // namespace mock

inline ::covellite::egl::EGLint eglGetError(void)
{
  return ::mock::EglProxy::GetInstance()->GetError();
}

inline ::covellite::egl::EGLDisplay eglGetDisplay(size_t _DisplayId)
{
  return ::mock::EglProxy::GetInstance()->GetDisplay(_DisplayId);
}

inline ::covellite::egl::EGLBoolean eglInitialize(::covellite::egl::EGLDisplay _Display, 
  ::covellite::egl::EGLint * _pMajor, ::covellite::egl::EGLint * _pMinor)
{
  return ::mock::EglProxy::GetInstance()->Initialize(_Display, _pMajor, _pMinor);
}

inline ::covellite::egl::EGLBoolean eglMakeCurrent(::covellite::egl::EGLDisplay _Display,
  ::covellite::egl::EGLSurface _Draw, ::covellite::egl::EGLSurface _Read, ::covellite::egl::EGLContext _Contex)
{
  return ::mock::EglProxy::GetInstance()->MakeCurrent(_Display, _Draw, _Read, _Contex);
}

inline ::covellite::egl::EGLBoolean eglTerminate(::covellite::egl::EGLDisplay _Display)
{
  return ::mock::EglProxy::GetInstance()->Terminate(_Display);
}

inline ::covellite::egl::EGLBoolean eglChooseConfig(::covellite::egl::EGLDisplay _Display,
  const ::covellite::egl::EGLint * _pAttributes, ::covellite::egl::EGLConfig * _pConfigs,
  ::covellite::egl::EGLint _ConfigSize, ::covellite::egl::EGLint * _pConfigCount)
{
  ::mock::EglProxy & Proxy = *::mock::EglProxy::GetInstance();

  if (Proxy.m_pAttributes == nullptr)
  {
    throw ::std::exception("Invalid EglProxy attributes pointer.");
  }

  for (size_t i = 0; i < 100; i++)
  {
    if (_pAttributes[i] != Proxy.m_pAttributes[i])
    {
      throw ::std::exception("Unexpected attribute value.");
    }

    if (Proxy.m_pAttributes[i] == ::covellite::egl::EGL_NONE) break;
    if (i == 99) throw ::std::exception("Very big attributes list.");
  }

  *_pConfigs = *Proxy.m_pConfig;
  return ::mock::EglProxy::GetInstance()->ChooseConfig(_Display,
    _pAttributes, _ConfigSize, _pConfigCount);
}

inline ::covellite::egl::EGLBoolean eglGetConfigAttrib(::covellite::egl::EGLDisplay _Display,
  ::covellite::egl::EGLConfig _Config, ::covellite::egl::EGLint _Attribute, ::covellite::egl::EGLint * _pValue)
{
  *_pValue = ::mock::EglProxy::GetInstance()->m_Value;
  return ::mock::EglProxy::GetInstance()->GetConfigAttrib(_Display,
    _Config, _Attribute);
}

inline int32_t ANativeWindow_setBuffersGeometry(::ANativeWindow * _pWindow,
  int32_t _Width, int32_t _Height, int32_t _Format)
{
  return ::mock::EglProxy::GetInstance()->SetBuffersGeometry(_pWindow,
    _Width, _Height, _Format);
}

inline ::covellite::egl::EGLContext eglCreateContext(::covellite::egl::EGLDisplay _Display,
  ::covellite::egl::EGLConfig _Config, ::covellite::egl::EGLContext _ShareContext,
  const ::covellite::egl::EGLint * _Attributes)
{
  return ::mock::EglProxy::GetInstance()->CreateContext(_Display,
    _Config, _ShareContext, _Attributes);
}

inline ::covellite::egl::EGLBoolean eglDestroyContext(::covellite::egl::EGLDisplay _Display,
  ::covellite::egl::EGLContext _Context)
{
  return ::mock::EglProxy::GetInstance()->DestroyContext(_Display, _Context);
}

inline ::covellite::egl::EGLSurface eglCreateWindowSurface(::covellite::egl::EGLDisplay _Display,
  ::covellite::egl::EGLConfig _Config, ANativeWindow * _pNativeWindow,
  const ::covellite::egl::EGLint * _Attributes)
{
  return ::mock::EglProxy::GetInstance()->CreateSurface(_Display,
    _Config, _pNativeWindow, _Attributes);
}

inline ::covellite::egl::EGLBoolean eglDestroySurface(::covellite::egl::EGLDisplay _Display, 
  ::covellite::egl::EGLSurface _Surface)
{
  return ::mock::EglProxy::GetInstance()->DestroySurface(_Display, _Surface);
}

inline ::covellite::egl::EGLBoolean eglQuerySurface(::covellite::egl::EGLDisplay _Display,
  ::covellite::egl::EGLSurface _Surface, ::covellite::egl::EGLint _Attribute, ::covellite::egl::EGLint * _pValue)
{
  *_pValue = ::mock::EglProxy::GetInstance()->m_Value;
  return ::mock::EglProxy::GetInstance()->QuerySurface(_Display, _Surface,
    _Attribute);
}

inline ::covellite::egl::EGLBoolean eglSwapBuffers(::covellite::egl::EGLDisplay _Display,
  ::covellite::egl::EGLSurface _Surface)
{
  return ::mock::EglProxy::GetInstance()->SwapBuffers(_Display, _Surface);
}
