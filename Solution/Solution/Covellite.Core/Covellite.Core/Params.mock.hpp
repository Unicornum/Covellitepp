
#pragma once
#include <Covellite\Core\Params.hpp>

namespace mock
{

namespace covellite
{

namespace core
{

class ParamsProxy :
  public ::alicorn::extension::testing::Proxy<ParamsProxy>
{
  using RocketKeyCode_t = ::mock::Rocket::Core::Input::KeyIdentifier;

public:
  MOCK_METHOD1(ToRocketKeyCode, RocketKeyCode_t(int32_t));
};

} // namespace core

} // namespace covellite

} // namespace mock

namespace covellite
{

namespace core
{

namespace params
{

inline auto KeyCode::ToRocketKeyCode(void) const -> RocketKeyCode_t
{
  using ::mock::covellite::core::ParamsProxy;
  return ParamsProxy::GetInstance()->ToRocketKeyCode(AsciiKeyCode);
}

} // namespace params

} // namespace core

} // namespace covellite
