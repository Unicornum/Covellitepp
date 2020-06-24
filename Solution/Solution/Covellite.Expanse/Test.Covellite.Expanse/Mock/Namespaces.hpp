
#pragma once

namespace mock
{

namespace alicorn {}
namespace covellite {}

} // namespace mock

namespace covellite
{

namespace alicorn = ::mock::alicorn;
namespace covellite = ::mock::covellite;

} // namespace covellite
