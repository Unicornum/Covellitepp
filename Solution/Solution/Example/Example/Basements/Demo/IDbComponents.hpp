
#pragma once
#include <boost/signals2.hpp>
#include <alicorn/std/string.forward.hpp>
#include "Defines.hpp"

namespace basement
{

namespace model
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ������������ ����� ���� ������ �����������.
*
* \version
*  1.0.0.0        \n
* \date
*  10 ���� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
class IDbComponents
{
protected:
  using ComponentId_t = ::alicorn::extension::std::String;

public:
  /**
  * \brief
  *  ������� ��������� �������� ������� �� ��� ��������������.
  */
  virtual const Object_t & GetObject(const Id_t) const = 0;

  /**
  * \brief
  *  ������� ��������� ������� ���������� �� ��� ��������������.
  */
  virtual ComponentPtr_t GetComponent(const ComponentId_t &) const = 0;

public:
  /**
  * \brief
  *  ������ �������, ������������ � ���������� �������� �������.
  */
  ::boost::signals2::signal<void(Id_t)> DoAddObject;

  /**
  * \brief
  *  ������ �������, ������������ �� �������� �������� �������.
  */
  ::boost::signals2::signal<void(Id_t)> DoRemoveObject;

public:
  virtual ~IDbComponents(void) = default;
};

} // namespace model

} // namespace basement
