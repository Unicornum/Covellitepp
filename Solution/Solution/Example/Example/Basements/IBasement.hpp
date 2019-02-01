
#pragma once

/**
* \defgroup ExampleBasementGroup Basement
* \ingroup ExampleGroup
*  ������ ������� ��������� 3D �����, ������ ������� �������� GUI.
*/

namespace events
{

class Basement_t
{
public:
  enum Id
  {
    Stop = 0,
  };
};

namespace { Basement_t Basement; }

} // namespace events

namespace basement
{

/**
* \ingroup ExampleBasementGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ������������ ����� ��� ������� ��������� 3D �����.
*  
* \version
*  1.0.0.0        \n
* \date
*  28 ������ 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
class IBasement
{
public:
  virtual void Render(void) = 0;

public:
  virtual ~IBasement(void) {}
};

} // namespace basement
