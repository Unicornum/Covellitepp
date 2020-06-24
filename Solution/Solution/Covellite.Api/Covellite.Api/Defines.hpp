
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
template<class T>
using Buffer_t = ::std::vector<T>;
using BinaryData_t = Buffer_t<uint8_t>;

} // namespace api

} // namespace covellite
