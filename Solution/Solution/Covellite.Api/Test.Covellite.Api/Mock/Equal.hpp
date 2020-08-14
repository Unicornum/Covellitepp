
#pragma once

namespace covellite
{

namespace api
{

namespace renderer
{

inline bool operator== (
  const ::covellite::Any_t & _Left,
  const ::covellite::Any_t & _Right)
{
  if (_Left.type() != _Right.type())
  {
    return false;
  }

  if (_Left.type() == typeid(HWND))
  {
    return ::covellite::any_cast<HWND>(_Left) == ::covellite::any_cast<HWND>(_Left);
  }

  if (_Left.type() == typeid(ANativeWindow *))
  {
    return ::covellite::any_cast<ANativeWindow *>(_Left) == ::covellite::any_cast<ANativeWindow *>(_Left);
  }

  throw ::std::runtime_error("Comparison of any unimplemented for type.");
}

inline bool operator== (
  const ::covellite::api::renderer::SettingsData & _Left,
  const ::covellite::api::renderer::SettingsData & _Right)
{
  static_assert(sizeof(_Left) ==
    (sizeof(_Left.Handle) + 
      sizeof(_Left.Top) +
      sizeof(_Left.Width) +
      sizeof(_Left.Height) +
      sizeof(_Left.IsFullScreen) + 
      3 // Выравнивание
      ),
    "Unexpected Data size.");

  if (!(_Left.Handle == _Right.Handle)) return false;
  if (_Left.Top != _Right.Top) return false;
  if (_Left.Width != _Right.Width) return false;
  if (_Left.Height != _Right.Height) return false;
  if (_Left.IsFullScreen != _Right.IsFullScreen) return false;
  return true;
}

} // namespace renderer

} // namespace api

} // namespace covellite
