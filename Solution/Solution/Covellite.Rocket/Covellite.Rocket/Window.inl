
#pragma once

namespace covellite
{

namespace rocket
{

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

} // namespace rocket

} // namespace covellite
