
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
      sizeof(_Left.ClientRect) +
      sizeof(_Left.IsFullScreen) + 
      7 // Выравнивание
      ), "Unexpected Data size.");

  if (!(_Left.Handle == _Right.Handle)) return false;
  if (_Left.ClientRect.Top != _Right.ClientRect.Top) return false;
  if (_Left.ClientRect.Width != _Right.ClientRect.Width) return false;
  if (_Left.ClientRect.Height != _Right.ClientRect.Height) return false;
  if (_Left.IsFullScreen != _Right.IsFullScreen) return false;
  return true;
}

} // namespace renderer

} // namespace api

} // namespace covellite
