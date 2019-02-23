
#pragma once

namespace covellite
{

namespace api
{

namespace renderer
{

inline bool operator!= (
  const ::covellite::api::renderer::Renderer::Data::Color & _Left,
  const ::covellite::api::renderer::Renderer::Data::Color & _Right)
{
  static_assert(sizeof(_Left) ==
    (sizeof(_Left.R) +
      sizeof(_Left.G) + 
      sizeof(_Left.B) + 
      sizeof(_Left.A)),
    "Unexpected Data::Color size.");

  if (_Left.R != _Right.R) return true;
  if (_Left.G != _Right.G) return true;
  if (_Left.B != _Right.B) return true;
  if (_Left.A != _Right.A) return true;
  return false;
}

inline bool operator== (
  const ::covellite::api::renderer::Renderer::Data & _Left,
  const ::covellite::api::renderer::Renderer::Data & _Right)
{
  static_assert(sizeof(_Left) ==
    (sizeof(_Left.Handle) + 
      sizeof(_Left.Top) +
      sizeof(_Left.BkColor) + 
      sizeof(_Left.IsFullScreen) + 3 // ������������
      ),
    "Unexpected Data size.");

  if (_Left.Handle != _Right.Handle) return false;
  if (_Left.Top != _Right.Top) return false;
  if (_Left.BkColor != _Right.BkColor) return false;
  if (_Left.IsFullScreen != _Right.IsFullScreen) return false;
  return true;
}

} // namespace renderer

} // namespace api

} // namespace covellite