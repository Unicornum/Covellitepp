
#pragma once
#include <Covellite/Gui/Layer.hpp>

namespace covellite
{

namespace gui
{

/**
* \brief
*  Функция вставки нового слоя.
* \details
*  - Функцию следует вызывать при нажатии кнопки перехода к другому слою
*  (возврат к предыдущему слою осуществляется автоматически при нажатии
*  кнопки 'Назад').
*  - Для реализации работы пограммы 'вокруг' основного окна следует вставить
*  слой главного окна программы, организовать по нажатию его кнопок вставку
*  дочерних слоев и вызов функции Back() по нажатию кнопки возврата каждого
*  дочернего слоя.
*  - Для реализации работы программы в виде мастера (когда новый слой заменяет
*  собой предыдущий) следует создать пустой базовый слой, вставить первый, а
*  затем при нажатии кнопки перехода к следующему слою вызывать функцию
*  Back() и функцию вставки нового слоя.
*
* \tparam TLayer
*  Класс слоя, объект которого должен быть создан. \n
*  Должен наследоваться от covellite::gui::Layer.
*/
template<class TLayer>
TLayer & Window::PushLayer(void)
{
  static_assert(::std::is_convertible<TLayer &, covellite::gui::Layer &>::value,
    "TLayer must inherit covellite::gui::Layer.");

  const auto pLayer = ::std::make_shared<TLayer>(*this);
  PushLayer(pLayer);

  // 16 Июль 2019 14:30 (unicornum.verum@gmail.com)
  TODO("Тест возврата ссылки на созданный объект слоя");
  return *pLayer;
}

} // namespace gui

} // namespace covellite
