
#pragma once

namespace mock
{

namespace Rocket {}
namespace covellite {}
namespace alicorn {}

} // namespace mock

namespace covellite
{

namespace Rocket = ::mock::Rocket;
namespace covellite = ::mock::covellite;
namespace alicorn = ::mock::alicorn;

} // namespace covellite
