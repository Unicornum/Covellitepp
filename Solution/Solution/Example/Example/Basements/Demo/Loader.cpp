
#include "stdafx.h"
#include "Loader.hpp"

namespace basement
{

namespace model
{

Loader::Loader(void) :
  GameObject(Extra::Loader)
{

}

auto Loader::GetObject(const Any_t &) const /*override*/ -> Objects_t
{
  return { {} };
}

} // namespace model

} // namespace basement
