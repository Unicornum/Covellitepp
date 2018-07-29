
#pragma once

namespace mock
{

namespace alicorn {}

namespace covellite
{

namespace app {}

} // namespace covellite

} // namespace mock

namespace covellite
{

namespace alicorn = ::mock::alicorn;

namespace os
{

namespace app = ::mock::covellite::app;

} // namespace os

} // namespace covellite
