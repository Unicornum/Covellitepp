
#pragma once

namespace mock
{

namespace std {}
namespace Rocket {}
namespace alicorn {}
namespace covellite {}

} // namespace mock

namespace covellite
{

namespace std = ::mock::std;
namespace Rocket = ::mock::Rocket;
namespace alicorn = ::mock::alicorn;
namespace covellite = ::mock::covellite;

} // namespace covellite
