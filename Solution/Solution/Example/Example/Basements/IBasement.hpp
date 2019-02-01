
#pragma once

/**
* \defgroup ExampleBasementGroup Basement
* \ingroup ExampleGroup
*  √руппа классов отрисовки 3D сцены, поверх которой рисуетс€ GUI.
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
*   ласс входит в проект \ref ExamplePage \n
*  »нтерфейсный класс дл€ классов отрисовки 3D сцены.
*  
* \version
*  1.0.0.0        \n
* \date
*  28 январь 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class IBasement
{
public:
  virtual void Render(void) = 0;

public:
  virtual ~IBasement(void) {}
};

} // namespace basement
