
#pragma once
#include <Covellite\Rocket\Rocket.forward.hpp>

namespace covellite
{

namespace rocket
{

class SystemToRocketKeyCodeProxy :
  public ::alicorn::extension::testing::Proxy<SystemToRocketKeyCodeProxy>
{
public:
  MOCK_METHOD1(Convert, RocketKeyCode_t(int32_t));
};

inline RocketKeyCode_t SystemToRocketKeyCode(int32_t _Code)
{
  return SystemToRocketKeyCodeProxy::GetInstance()->Convert(_Code);
}

} // namespace rocket

} // namespace covellite
