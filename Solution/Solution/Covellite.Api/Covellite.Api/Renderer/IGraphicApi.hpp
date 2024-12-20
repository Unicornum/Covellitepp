
#pragma once
#include <functional>
#include <alicorn/std/string.forward.hpp>
#include <Covellite/Predefined.hpp>

/**
* \defgroup CovelliteApiRendererGroup Renderer
* \ingroup CovelliteApiGroup
*  ������ ������� ���������� ���������� ��� ������ ���������� ����������� Api.
*/

namespace covellite
{

namespace api
{

class Component;

namespace renderer
{

/**
* \ingroup CovelliteApiRenderererGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ������������ ����� ���������� ���������� GUI ��� ������ ����������� Api.
*  
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
* \date
*  24 ������ 2018    \n
*  18 ������ 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
class IGraphicApi
{
protected:
  using String_t = ::alicorn::extension::std::String;
  using Render_t = ::std::function<void(void)>;
  using ComponentPtr_t = ::std::shared_ptr<::covellite::api::Component>;
  using Creator_t = ::std::function<Render_t(const ComponentPtr_t &)>;
  using Creators_t = ::std::map<String_t, Creator_t>;

public:
  virtual String_t GetUsingApi(void) const = 0;
  virtual void PresentFrame(void) = 0;
  virtual void ResizeWindow(const Rect &) = 0;
  virtual const Creators_t & GetCreators(void) const = 0;

public:
  virtual ~IGraphicApi(void) = default;
};

/// ����� ��� �������� �������� �������� ��������� ���������� �������.
class SettingsData final
{
public:
  Any_t Handle;       ///< ���� ���� ������������ �������.
  Rect  ClientRect;   ///< ����������� ������� ���������� ������� ����.
  bool  IsFullScreen; ///< ������� ������ ��������� � ������������� ������.
  uint8_t Align[7] = { 0 };
};

} // namespace renderer

} // namespace api

} // namespace covellite
