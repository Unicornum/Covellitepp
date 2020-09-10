
#pragma once
#include <memory>
#include <alicorn/std/string.forward.hpp>
#include <Covellite/Events/IEvents.hpp>
#include <Covellite/Api/Renders.hpp>

namespace covellite
{

namespace api
{

/**
* \ingroup CovelliteApiGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ������������ ����� ��� ���� ����, ����������� ��������� ����������� API.
*  
* \version
*  1.0.0.0        \n
*  1.0.1.0        \n
*  1.1.0.0        \n
* \date
*  29 �������� 2017    \n
*  20 ������ 2018    \n
*  19 ������ 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017 - 2018
*/
class IWindow :
  public ::covellite::events::IEvents
{
protected:
  using RendersPtr_t = ::std::shared_ptr<Component::Renders>;

public:
  /// ������� ��������� �������� ���������� ������� ����.
  virtual Rect GetClientRect(void) const = 0;
  /// ������� ��������� ������� ��������/�������� �������� ��� �����������.
  virtual RendersPtr_t GetRenders(void) const = 0;

public:
  virtual ~IWindow(void) = default;
};

} // namespace api

} // namespace covellite
