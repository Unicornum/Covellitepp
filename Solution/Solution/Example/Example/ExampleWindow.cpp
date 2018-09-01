
#include "stdafx.h"
#include "ExampleWindow.hpp"
#include <boost\format.hpp>
#include <alicorn\logger.hpp>
#include <alicorn\version.hpp>
#include <alicorn\application\current-module.hpp>

static const auto PathToDataDirectory =
  ::alicorn::system::application::CurrentModule::GetAppRootPath() / "data";

/// [Example events ids]
class Example_t
{
public:
  // Идентификаторы событий уровня проекта примера.
  enum Id
  {
    Button1 = 0,
    Button2,
    Button3,
  };
};
  
namespace { Example_t Example; }
/// [Example events ids]

/// [Title layer]
class ExampleWindow::DemoLayer1 final :
  public Layer_t
{
public:
  // Интерфейс IWindow:
  void Subscribe(const EventHandlerPtr_t &) override {}
    
public:
  explicit DemoLayer1(IWindowRocket_t & _Window) :
    Layer_t(_Window, PathToDataDirectory / "demo.rml", "title")
  {
    using namespace ::alicorn::extension::std;
      
    ::alicorn::system::version::Info Info;
      
    GetElement("id_header").SetText(
      "<h1>Covellite++ v" + string_cast<::std::string, Locale::UTF8>(
        Info.GetValue(uT("FileVersionShort"))) + "</h1>");
      
    using namespace ::covellite;
      
    // При нажатии кнопок слоя генерируем события уровня проекта примера,
    // которые обрабатываются в классе окна.
    m_Events[events::Click.DocumentId(GetId()).ElementId("button1")]
      .Connect([&]() { m_Events[Example.Button1](); });
    m_Events[events::Click.DocumentId(GetId()).ElementId("button2")]
      .Connect([&]() { m_Events[Example.Button2](); });
    m_Events[events::Click.DocumentId(GetId()).ElementId("button3")]
      .Connect([&]() { m_Events[Example.Button3](); });
    m_Events[events::Click.DocumentId(GetId()).ElementId("exit")]
      .Connect([&]() { m_Events[events::Application.Exit](); });
  }
};
/// [Title layer]

/// [Layer example]
class ExampleWindow::DemoLayer2 final :
  public Layer_t
{
public:
  // Интерфейс IWindow:
  void Subscribe(const EventHandlerPtr_t &) override {}
    
public:
  explicit DemoLayer2(IWindowRocket_t & _Window) :
    Layer_t(_Window, PathToDataDirectory / "demo2.rml")
  {
    using namespace ::covellite::events;
      
    // Подписываемся на событие нажатия кнопки с идентификатором "id_hello".
    m_Events[Click.DocumentId(GetId()).ElementId("id_hello")]
      .Connect([&](const Click_t::Info &) 
    {
      // Действие, выполняемое при нажатии на кнопку id_hello.
      GetElement("multiline_text").SetText(u8"Привет!");
    });
  }
};
/// [Layer example]

class ExampleWindow::TableLayer final :
  public Layer_t
{
public:
  // Интерфейс IWindow:
  void Subscribe(const EventHandlerPtr_t &) override {}
    
public:
  explicit TableLayer(IWindowRocket_t & _Window) :
    Layer_t(_Window, PathToDataDirectory / "table.rml")
  {
    GetElement("id_board_name").SetText(
      u8"<h2><font30>Название доски</font30></h2>");
      
    GetElement("id_list").SetText(
      u8"<button class=\"left\"><font42>\u00D7</font42></button>"
      u8"<p class=\"textleft\"><font36>CSS example</font36></p>"
      u8"<button class=\"right\"><font42>\u00D7</font42></button>"
      u8"<button class=\"right\"><font42>\u00D7</font42></button>");
      
    GetElement("multiline_text").SetText(
      u8"Строки:\r\n"
      u8"- Первая.\r\n"
      u8"- Вторая.\r\n"
      u8"- Третья.\r\n"
      u8"- Четвертая.\r\n"
      u8"- Пятая.\r\n"
      u8"- Шестая.");
  }
};

class ExampleWindow::ScrollLayer final :
  public Layer_t
{
public:
  // Интерфейс IWindow:
  void Subscribe(const EventHandlerPtr_t &) override {}
    
public:
  explicit ScrollLayer(IWindowRocket_t & _Window) :
    Layer_t(_Window, PathToDataDirectory / "scroll.rml")
  {
    const auto Height = ::std::max(GetWidth(), GetHeight());
      
    GetElement("id_body").SetStyle(
      (::boost::format("font-size: %1%px;") % (Height / 30)).str());
  }
};

// ************************************************************************** //

/// [Constructor main window]
ExampleWindow::ExampleWindow(WindowRocket_t & _WindowRocket) :
  m_WindowRocket(_WindowRocket),
  m_Events(_WindowRocket)
{
  // Набор строк локализации приложения.
  m_WindowRocket.Set(
  {
    { u8"[HELLO]", u8"Привет" },
  });
    
  // Экран, который будет отображаться при старте программы.
  m_WindowRocket.PushLayer<DemoLayer1>();
    
  using namespace ::covellite::events;
    
  // Подписка на события перехода к другому экрану.
  m_Events[Example.Button1]
    .Connect([&]() { m_WindowRocket.PushLayer<DemoLayer2>(); });
  m_Events[Example.Button2]
    .Connect([&]() { m_WindowRocket.PushLayer<TableLayer>(); });
  m_Events[Example.Button3]
    .Connect([&]() { m_WindowRocket.PushLayer<ScrollLayer>(); });
}
/// [Constructor main window]
