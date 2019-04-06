
#pragma once
#include "IBasement.hpp"
#include "Demo/Model.hpp"
#include "Demo/View.hpp"

/**
* \defgroup ExampleBasementDemoGroup Basement
* \ingroup ExampleBasementGroup
*  Группа классов отрисовки 3D сцены демонстрационного режима.
*/

namespace basement
{

/**
* \ingroup ExampleBasementGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс реализации отрисовки 3D сцены демонстрационного режима.
*  
* \version
*  1.0.0.0        \n
* \date
*  10 Март 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Demo final :
  public IBasement
{
  using Events_t = ::covellite::events::Events;
  using Component_t = ::covellite::api::Component;
  using RendersPtr_t = ::std::shared_ptr<Component_t::Renders>;

public:
  void Notify(int, const ::boost::any &) override;
  void Render(void) override;

private:
  view::View    m_View;
  model::Model  m_Model;

public:
  Demo(const Events_t &, const RendersPtr_t &);
  ~Demo(void);
};

} // namespace basement
