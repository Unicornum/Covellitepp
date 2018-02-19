
#pragma once
#include <cstdarg>

namespace mock
{

class AndroidLogProxy :
  public ::alicorn::extension::testing::Proxy<AndroidLogProxy>
{
public:
  MOCK_METHOD3(Print, int(int, ::std::string, ::std::string));
  MOCK_METHOD1(Arg, void(::std::string));
};

} // namespace mock

# pragma warning(push)
# pragma warning(disable: 4505)

static int __android_log_print(int _Priority, ::std::string _Name,
  const char * _Format, va_list _Args)
{
  const auto Result = 
    ::mock::AndroidLogProxy::GetInstance()->Print(_Priority, _Name, _Format);

  size_t Index = 0;
  size_t Offset = 0;

  while (true)
  {
    const auto ArgPosition = ::std::string{ _Format }.find('%', Offset);
    if (ArgPosition == ::std::string::npos) break;

    auto * Arg = va_arg(_Args, const char *);
    ::mock::AndroidLogProxy::GetInstance()->Arg(Arg);

    Offset = ArgPosition + 1;
    Index++;
  }

  return Result;
}

#pragma warning(pop)

enum android_LogPriority
{
  ANDROID_LOG_ERROR = 1710151250,
  ANDROID_LOG_INFO = 1710151251,
};
