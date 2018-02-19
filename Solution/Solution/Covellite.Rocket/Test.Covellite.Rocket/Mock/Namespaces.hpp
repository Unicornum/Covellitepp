
#pragma once

namespace mock
{

namespace Rocket {}
namespace alicorn {}
namespace covellite {}

} // namespace mock

namespace mock2
{

namespace std {}

} // namespace mock2

namespace covellite
{

namespace std = ::mock2::std;
namespace Rocket = ::mock::Rocket;
namespace alicorn = ::mock::alicorn;
namespace covellite = ::mock::covellite;

} // namespace covellite
