
#pragma once
#include <functional>
#include "Events.hpp"
#include "Vertex.hpp"

namespace covellite
{

namespace api
{

using Updater_t = ::std::function<void(const float)>;

template<class T>
using cbBufferMap_t = ::std::function<bool(T *)>;

} // namespace api

} // namespace covellite
