
#pragma once
#include "Layer.hpp"

namespace layers
{

/**
* \ingroup ExampleGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс главного экрана программы.
*  
* \version
*  1.0.0.0        \n
* \date
*  01 Февраль 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
/// [Layer example]
class MainScreen final :
  public ::covellite::gui::Layer
{
  /// [Layer example]

public:
  // Интерфейс IWindow:
  void Subscribe(const EventHandlerPtr_t &) override {}

public:
  explicit MainScreen(IWindowGui_t &);
};

} // namespace layers
