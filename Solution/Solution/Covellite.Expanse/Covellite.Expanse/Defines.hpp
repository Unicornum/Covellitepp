
#pragma once
#include <memory>
#include <vector>
#include <functional>

/**
* \file
*  ‘айл общих определений, используемых в проекте и доступных дл€ использовани€
*  в клиентском коде.
*/

namespace covellite
{

namespace api
{

class Component;

} // namespace api

namespace expanse
{

using ObjectId_t = size_t;
using ComponentPtr_t = ::std::shared_ptr<api::Component>;
using GameObject_t = ::std::vector<ComponentPtr_t>;
using Render_t = ::std::function<void(void)>;
using C3DObject_t = ::std::vector<Render_t>;

} // namespace expanse

} // namespace covellite
