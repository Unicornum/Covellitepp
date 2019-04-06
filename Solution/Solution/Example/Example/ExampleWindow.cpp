
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
#include <Rocket/ProgressBar.h>
#include <Rocket/CircularBar.h>

/// [Constructor main window]
ExampleWindow::ExampleWindow(WindowGui_t & _WindowGui) :
  m_WindowGui(_WindowGui),
  m_Events(_WindowGui)
{
  // 05 Март 2019 11:59 (unicornum.verum@gmail.com)
  TODO("Перенести в Externals, в библиотеку [Rocket::Plugins] с единой функцией инициализации.");
  ::Rocket::ProgressBar::Initialise();
  ::Rocket::CircularBar::Initialise();

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
  m_Events[::layers::Button.Help]
    .Connect([&](void) { m_WindowGui.PushLayer<::layers::Description>(); });
  m_Events[::layers::Button.Controls]
    .Connect([&](void) { m_WindowGui.PushLayer<::layers::Controls>(); });
  m_Events[::layers::Button.Text]
    .Connect([&](void) { m_WindowGui.PushLayer<::layers::Text>(); });

  m_Events[::layers::Button.Draw3DObject]
    .Connect([&](void) 
  { 
    m_WindowGui.PushLayer<::layers::Draw3DObject>();

    // 17 Март 2019 10:57 (unicornum.verum@gmail.com)
    TODO("Событие создания basement::Simple3DObject должно генерироваться здесь.");
  });

  m_Events[::layers::Button.Simple2DGame]
    .Connect([&](void) 
  { 
    m_WindowGui.PushLayer<::layers::Simple2DGame>();
    m_Events[::events::Simple2DGame.Start]();
  });

  m_Events[::layers::Button.Demo]
    .Connect([&](void) 
  { 
    m_WindowGui.PushLayer<::layers::Demo>();
    m_Events[::events::Demo.Start]();
  });

  m_Events[::layers::Button.Back]
    .Connect([&](const bool & _IsStopBasement)
  {
    m_WindowGui.Back();

    if (_IsStopBasement)
    {
      m_Events[::events::Basement.Stop]();
    }
  });
}
/// [Constructor main window]
