
#pragma once
#include <alicorn/patterns/factory.hpp>

namespace std { template<class> class shared_ptr; }

namespace covellite
{

namespace os { class IWindow; }

namespace api
{

using WindowOs_t = ::covellite::os::IWindow;
template<class T>
using Registator_t =
  ::alicorn::modules::patterns::factory::Registrator<T, const WindowOs_t &>;

} // namespace api

} // namespace covellite
