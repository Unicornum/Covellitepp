
#pragma once
#include <alicorn/patterns/factory.hpp>

namespace covellite
{

namespace api
{

template<class T>
using Registator_t =
  ::alicorn::modules::patterns::factory::Registrator<T, const renderer::SettingsData &>;

} // namespace api

} // namespace covellite
