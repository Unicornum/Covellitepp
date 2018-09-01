
#include "stdafx.h"
#include <Covellite\Gui\Layers.hpp>
#include <Covellite\Gui\ILayer.hpp>
#include <alicorn\std\exception.hpp>

using namespace covellite::gui;

/**
* \brief
*  Функция вставки слоя на вершину стека.
* \details
*  - Деактивирует слой на вершине стека, добавляет указанный слой, после чего
*  активирует его.
*  
* \param [in] _pLayer
*  Добавляемый слой.
*/
void Layers::Push(const LayerPtr_t & _pLayer) noexcept
{
  if (!m_Layers.empty()) m_Layers.back()->Hide();

  m_Layers.push_back(_pLayer);
  m_Layers.back()->Show();
}

/**
* \brief
*  Функция удаления слоя с вершины стека.
* \details
*  - Деактивирует и удаляет слой на вершине стека, после чего активирует слой, 
*  оказавшийся на вершине стека.
*  
* \return \b true
*  - Операция выполнена успешно (в стеке еще есть хотя бы один слой).
* \return \b false
*  - Был удален последний слой, стек пуст.
*  
* \exception std::exception
*  - Попытка удалить слой из пустого стека.
*/
bool Layers::Pop(void)
{
  if (m_Layers.empty()) throw STD_EXCEPTION << "Empty stack.";

  m_Layers.back()->Hide();
  m_Layers.pop_back();

  if (m_Layers.empty()) return false;

  m_Layers.back()->Show();
  return true;
}
