
#pragma once

namespace mock
{

class AndroidAppProxy :
  public ::alicorn::extension::testing::Proxy<AndroidAppProxy>
{
public:
  MOCK_METHOD1(EntryPoint, void (::std::string));
};

} // namespace mock

#define APPLICATION_ENTRY_POINT(MainApplicationClass) \
  void ApplicationEntryPoint(void) \
  { \
    ::mock::AndroidAppProxy::GetInstance()->EntryPoint(#MainApplicationClass); \
  }
