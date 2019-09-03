
#pragma once
#include <Covellite/App/Vfs.hpp>

namespace covellite
{

namespace gui
{

// Класс потребовался из-за того, что на using не работает [[deprecated]].
class [[deprecated("Deprecated: use covellite::app::Vfs_t instead")]] Vfs_t
{
public:
  inline static auto GetInstance(void)
  {
    return app::Vfs_t::GetInstance();
  }
};

} // namespace gui

} // namespace covellite
