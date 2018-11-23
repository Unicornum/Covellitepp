
#pragma once
#include <alicorn/patterns/factory.hpp>
#include "Renderer.hpp"

namespace covellite
{

namespace api
{

using Data_t = ::covellite::api::renderer::Renderer::Data;
template<class T>
using Registator_t =
  ::alicorn::modules::patterns::factory::Registrator<T, const Data_t &>;

} // namespace api

} // namespace covellite
