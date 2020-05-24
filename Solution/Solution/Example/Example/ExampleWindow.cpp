
#include "stdafx.h"
#include "ExampleWindow.hpp"
#include "Layers/MainScreen.hpp"
#include "Layers/Description.hpp"
#include "Layers/Controls.hpp"
#include "Layers/Text.hpp"
#include "Layers/Draw3DObject.hpp"
#include "Layers/Example2DGame.hpp"
#include "Layers/Demo.hpp"
#include "Basements/Simple2DGame.hpp"
#include <Rocket/Plugin.hpp>

/// [Constructor main window]
ExampleWindow::ExampleWindow(WindowGui_t & _WindowGui) :
  m_WindowGui(_WindowGui),
  m_Events(_WindowGui)
{
  ::Rocket::Plugin::Initialise();

  // Набор строк локализации приложения в данном случае используется для 
  // вывода различного текста на различных платформах).
  m_WindowGui.Set(
  {
# if BOOST_OS_WINDOWS
    { u8"[BACK]", u8"ALT + LEFT" },
    { u8"[EXIT]", u8"кнопку закрытия окна программы" },
# elif BOOST_PLAT_ANDROID
    { u8"[BACK]", u8"кнопку BACK устройства" },
    { u8"[EXIT]", u8"кнопку HOME устройства" },
# endif
  });
    
  // Экран, который будет отображаться при старте программы.
  m_WindowGui.PushLayer<::layers::MainScreen>();
  m_LayerCount++;
    
  // Подписка на события перехода к другим слоям (события объекта Button
  // активируются в классе layers::MainScreen при нажатии на соответствующую
  // кнопку).
  m_Events[::layers::Button.Help]
    .Connect([&](void) 
  { 
    m_WindowGui.PushLayer<::layers::Description>(); 
    m_LayerCount++;
  });
  m_Events[::layers::Button.Controls]
    .Connect([&](void) 
  { 
    m_WindowGui.PushLayer<::layers::Controls>(); 
    m_LayerCount++;
  });
  m_Events[::layers::Button.Text]
    .Connect([&](void) 
  { 
    m_WindowGui.PushLayer<::layers::Text>(); 
    m_LayerCount++;
  });

  m_Events[::layers::Button.Draw3DObject]
    .Connect([&](void) 
  { 
    m_WindowGui.PushLayer<::layers::Draw3DObject>();
    m_LayerCount++;

    // 17 Март 2019 10:57 (unicornum.verum@gmail.com)
    TODO("Событие создания basement::Simple3DObject должно генерироваться здесь.");
  });

  m_Events[::layers::Button.Simple2DGame]
    .Connect([&](void) 
  { 
    m_WindowGui.PushLayer<::layers::Simple2DGame>();
    m_LayerCount++;

    m_Events[::events::Simple2DGame.Start]();
  });

  m_Events[::layers::Button.Demo]
    .Connect([&](void) 
  { 
    m_WindowGui.PushLayer<::layers::Demo>();
    m_LayerCount++;

    m_Events[::events::Demo.Start]();
  });

  m_Events[::layers::Button.Back]
    .Connect([&](const bool & _IsStopBasement)
  {
    m_WindowGui.Back();
    m_LayerCount--;

    if (_IsStopBasement)
    {
      m_Events[::events::Basement.Stop]();
    }
  });

  m_Events[::covellite::events::Key.Up]
    .Connect([&](const ::covellite::events::Key_t::Code & _Code)
  {
    static const auto WindowsEscKeyCode = 0x1B;

    if (_Code == WindowsEscKeyCode && m_LayerCount == 1)
    {
      m_Events[::covellite::events::Application.Exit]();
    }
  });
}
/// [Constructor main window]
