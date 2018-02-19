
#pragma once

namespace mock
{

namespace Rocket {}
namespace alicorn {}
namespace covellite {}

} // namespace mock

namespace covellite
{

namespace Rocket = ::mock::Rocket;
namespace alicorn = ::mock::alicorn;
namespace covellite = ::mock::covellite;

} // namespace covellite
