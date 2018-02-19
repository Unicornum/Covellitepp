
#pragma once
#include <chrono>

namespace mock2
{

namespace std
{

namespace chrono
{

class system_clock
{
  using time_point = ::std::chrono::system_clock::time_point;
  using Time_t = ::std::chrono::microseconds;

public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD0(Now, Time_t(void));
  };

public:
  static time_point now(void)
  {
    return time_point(Proxy::GetInstance()->Now());
  }
};

} // namespace chrono

} // namespace std

} // namespace mock2
