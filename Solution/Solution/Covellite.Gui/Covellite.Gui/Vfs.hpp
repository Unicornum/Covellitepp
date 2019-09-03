
#pragma once
#include <Covellite/App/Vfs.hpp>

namespace covellite
{

namespace gui
{

// ����� ������������ ��-�� ����, ��� �� using �� �������� [[deprecated]].
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
