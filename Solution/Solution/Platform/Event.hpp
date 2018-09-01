
#pragma once

namespace mock
{

class AEventProxy :
  public ::alicorn::extension::testing::Proxy<AEventProxy>
{
public:
  MOCK_METHOD1(GetType, int32_t(const AInputEvent *));
  MOCK_METHOD1(GetAction, int32_t(const AInputEvent *));
  MOCK_METHOD2(GetX, float(const AInputEvent *, size_t));
  MOCK_METHOD2(GetY, float(const AInputEvent *, size_t));
  MOCK_METHOD1(GetKeyAction, int32_t(const AInputEvent *));
  MOCK_METHOD1(GetKeyCode, int32_t(const AInputEvent *));
  MOCK_METHOD3(PollAll, int(int, int *, int *));
  MOCK_METHOD1(GetLanguage, ::std::string(AConfiguration *));
  MOCK_METHOD2(Process, void(android_app *, android_poll_source *));
  MOCK_METHOD1(Finish, void(ANativeActivity *));
  MOCK_METHOD2(HideSoftInput, void(ANativeActivity *, uint32_t));
  MOCK_METHOD3(OnCreate, void(ANativeActivity *, void *, size_t));
  MOCK_METHOD1(GetWidth, int32_t(ANativeWindow *));
  MOCK_METHOD1(GetHeight, int32_t(ANativeWindow *));

public:
  void * m_pOutData = nullptr;
};

} // namespace mock

inline int32_t AInputEvent_getType(const AInputEvent * _Event)
{
  return ::mock::AEventProxy::GetInstance()->GetType(_Event);
}

inline int32_t AMotionEvent_getAction(const AInputEvent * _Event)
{
  return ::mock::AEventProxy::GetInstance()->GetAction(_Event);
}

inline float AMotionEvent_getX(const AInputEvent * _Event, size_t _PointerIndex)
{
  return ::mock::AEventProxy::GetInstance()->GetX(_Event, _PointerIndex);
}

inline float AMotionEvent_getY(const AInputEvent * _Event, size_t _PointerIndex)
{
  return ::mock::AEventProxy::GetInstance()->GetY(_Event, _PointerIndex);
}

inline int32_t AKeyEvent_getAction(const AInputEvent * _Event)
{
  return ::mock::AEventProxy::GetInstance()->GetKeyAction(_Event);
}

inline int32_t AKeyEvent_getKeyCode(const AInputEvent* _Event)
{
  return ::mock::AEventProxy::GetInstance()->GetKeyCode(_Event);
}

inline int ALooper_pollAll(int _TimeoutMillis, int * _pOutFd, int * _pOutEvents,
  void ** _ppOutData)
{
  auto pProxy = ::mock::AEventProxy::GetInstance();

  *_ppOutData = pProxy->m_pOutData;
  return pProxy->PollAll(_TimeoutMillis, _pOutFd, _pOutEvents);
}

inline void AConfiguration_getLanguage(AConfiguration * _pConfig,
  char * _pOutLanguage)
{
  const auto Result = ::mock::AEventProxy::GetInstance()->GetLanguage(_pConfig);
  memcpy(_pOutLanguage, Result.c_str(), Result.length());
}

inline void Process(android_app * _pApp, android_poll_source * _pSource)
{
  ::mock::AEventProxy::GetInstance()->Process(_pApp, _pSource);
}

inline void ANativeActivity_finish(ANativeActivity * _pActivity)
{
  ::mock::AEventProxy::GetInstance()->Finish(_pActivity);
}

inline void ANativeActivity_hideSoftInput(ANativeActivity * _pActivity,
  uint32_t _Flags)
{
  ::mock::AEventProxy::GetInstance()->HideSoftInput(_pActivity, _Flags);
}

inline void ANativeActivity_onCreate(ANativeActivity * _pActivity,
  void * _pSavedState, size_t _SavedStateSize)
{
  ::mock::AEventProxy::GetInstance()->OnCreate(_pActivity, _pSavedState,
    _SavedStateSize);
}

inline int32_t ANativeWindow_getWidth(ANativeWindow * _pWindow)
{
  return ::mock::AEventProxy::GetInstance()->GetWidth(_pWindow);
}

inline int32_t ANativeWindow_getHeight(ANativeWindow * _pWindow)
{
  return ::mock::AEventProxy::GetInstance()->GetHeight(_pWindow);
}
