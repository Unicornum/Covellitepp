﻿
#include "stdafx.h"
#include <Covellite\Core\EventHandler.hpp>
#include <Covellite\Core\ClickEventListener.hpp>

#if BOOST_COMP_MSVC
# pragma warning(push)
# pragma warning(disable: 4996)
#endif

using namespace covellite::core;

#if BOOST_COMP_MSVC
# pragma warning(pop)
#endif

EventHandler::EventHandler(Dummy) :
  m_fnCleaner([](void) {})
{

}

EventHandler::~EventHandler(void) noexcept
{
  m_fnCleaner();
}

/**
* \brief
*  Функция вызова сигнала отрисовки окна.
* \details
*  - Если приложение находится в режиме паузы, функция не будет вызывать сигнал
*  отрисовки окна.
*/
void EventHandler::PostCommandDrawWindow(void)
{
  if (m_IsPaused) return;

  DoCommand(Event::StartDrawing, params::Empty{});
  DoCommand(Event::Drawing, params::Empty{});
  DoCommand(Event::FinishDrawing, params::Empty{});
}

/**
* \brief
*  Функция вызывается при необходимости уничтожения окна.
* \details
*  - Функция обработки события Event::Destroy.
*/
void EventHandler::OnDestroy(void)
{
  OnSystem.clear();
  OnWindow.clear();
  OnInput.clear();
  OnFramework.clear();
  OnRocket.clear();
}
