
#pragma once
#include <Covellite/Rocket/Layer.hpp>

namespace covellite
{

namespace rocket
{

/**
* \deprecated
*  Функция устарела и будет удалена в следующей стабильной версии. \n
*  Вместо нее используйте функцию PushLayer().
* \brief
*  Функция вставки нового слоя.
* \details
*  - Функцию следует вызывать при нажатии кнопки перехода к другому слою
*  (возврат к предыдущему слою осуществляется автоматически при нажатии 
*  кнопки 'Назад').
*  
* \tparam TLayer
*  Класс слоя, объект которого должен быть создан. \n
*  Должен наследоваться от covellite::rocket::Layer.
*  
* \return
*  Созданный объект слоя.
*/
template<class TLayer>
::std::shared_ptr<TLayer> Window::AddLayer(void)
{
  auto pLayer = ::std::make_shared<TLayer>(m_pContext.get());
  m_Layers.Push(pLayer);
  pLayer->Subscribe(m_pEvents);
  return pLayer;
}

/**
* \brief
*  Функция вставки нового слоя.
* \details
*  - Функцию следует вызывать при нажатии кнопки перехода к другому слою
*  (возврат к предыдущему слою осуществляется автоматически при нажатии
*  кнопки 'Назад').
*
* \tparam TLayer
*  Класс слоя, объект которого должен быть создан. \n
*  Должен наследоваться от covellite::rocket::Layer.
*/
template<class TLayer>
void Window::PushLayer(void)
{
  static_assert(::std::is_convertible<TLayer &, covellite::rocket::Layer &>::value,
    "TLayer must inherit covellite::rocket::Layer.");

  m_Layers.Push(::std::make_shared<TLayer>(*this));
}

} // namespace rocket

} // namespace covellite
