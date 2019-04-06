
#pragma once
#include <Covellite/Covellite.hpp>

namespace layers
{

class Button_t
{
public:
  // �������������� ������� ������ ������� �������.
  enum Id
  {
    Back = 0,
    Help,
    Controls,
    Text,
    Draw3DObject,
    Simple2DGame,
    Demo,
  };
};

namespace { Button_t Button; }

/**
* \ingroup ExampleGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ������� ����� �����, ������������ � �������� ������� 'layer.template.rml'.
*  
* \version
*  1.0.0.0        \n
* \date
*  14 ������ 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
class Layer :
  public ::covellite::gui::Layer
{
  using String_t = ::alicorn::extension::std::String;

public:
  // ��������� IWindow:
  void Subscribe(const EventHandlerPtr_t &) override {}

protected:
  const float m_FontSize;
  const bool  m_IsStopBasements;

protected:
  Layer(IWindowGui_t &, const Path_t &, const String_t &, const String_t &,
    const bool = false);
};

} // namespace layers
