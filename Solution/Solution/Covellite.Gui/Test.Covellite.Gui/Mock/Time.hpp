
#pragma once

namespace mock
{

struct timeval
{
  long tv_sec;
  long tv_usec;
};

class TimeProxy :
  public ::alicorn::extension::testing::Proxy<TimeProxy>
{
public:
  MOCK_METHOD1(Get, timeval(void *));
};

namespace
{

void gettimeofday(timeval * _pValue, void * _pTimeZone)
{
  *_pValue = TimeProxy::GetInstance()->Get(_pTimeZone);
}

} // unnamed namespace

} // namespace mock

namespace covellite
{

namespace gui
{

using ::mock::timeval;
using ::mock::gettimeofday;

} // namespace gui

} // namespace covellite
