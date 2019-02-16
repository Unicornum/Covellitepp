
#include "stdafx.h"
#include "ExampleWindow.hpp"
#include <boost/format.hpp>
#include <alicorn/logger.hpp>
#include <alicorn/version.hpp>
#include <alicorn/application/current-module.hpp>
#include "Layers/MainScreen.hpp"
#include "Layers/Description.hpp"
#include "Layers/Controls.hpp"
#include "Layers/Text.hpp"
#include "Layers/Draw3DObject.hpp"
#include "Layers/Example2DGame.hpp"
#include "Layers/Demo.hpp"

/// [Constructor main window]
ExampleWindow::ExampleWindow(WindowGui_t & _WindowGui) :
  m_WindowGui(_WindowGui),
  m_Events(_WindowGui)
{
  // Набор строк локализации приложения в данном случае используется для 
  // вывода различного текста на различных платформах).
  m_WindowGui.Set(
  {
# if BOOST_OS_WINDOWS
    { u8"[BACK]", u8"ALT + LEFT" },
    { u8"[EXIT]", u8"кнопку закрытия окна программы" },
# elif BOOST_OS_ANDROID
    { u8"[BACK]", u8"кнопку BACK устройства" },
    { u8"[EXIT]", u8"кнопку HOME устройства" },
# endif
  });
    
  // Экран, который будет отображаться при старте программы.
  m_WindowGui.PushLayer<::layers::MainScreen>();
    
  // Подписка на события перехода к другим слоям (события объекта Button
  // активируются в классе layers::MainScreen при нажатии на соответствующую
  // кнопку).
  m_Events[::layers::Button.Back]
    .Connect([&](void) { m_WindowGui.Back(); });
  m_Events[::layers::Button.Help]
    .Connect([&](void) { m_WindowGui.PushLayer<::layers::Description>(); });
  m_Events[::layers::Button.Controls]
    .Connect([&](void) { m_WindowGui.PushLayer<::layers::Controls>(); });
  m_Events[::layers::Button.Text]
    .Connect([&](void) { m_WindowGui.PushLayer<::layers::Text>(); });
  m_Events[::layers::Button.Draw3DObject]
    .Connect([&](void) { m_WindowGui.PushLayer<::layers::Draw3DObject>(); });
  m_Events[::layers::Button.Simple2DGame]
    .Connect([&](void) { m_WindowGui.PushLayer<::layers::Simple2DGame>(); });
  m_Events[::layers::Button.Demo]
    .Connect([&](void) { m_WindowGui.PushLayer<::layers::Demo>(); });
}
/// [Constructor main window]
