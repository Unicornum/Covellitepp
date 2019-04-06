
#include "stdafx.h"
#include "Loader.hpp"

namespace basement
{

namespace model
{

Loader::Loader(void) :
  GameObject(Type::Loader)
{

}

Object_t Loader::GetObject(const Any_t &) const /*override*/
{
  return {};
}

} // namespace model

} // namespace basement
