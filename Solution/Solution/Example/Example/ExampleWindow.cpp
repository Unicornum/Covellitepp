
#include "stdafx.h"
#include "ExampleWindow.hpp"
#include <boost\format.hpp>
#include <alicorn\logger.hpp>
#include <alicorn\version.hpp>
#include <alicorn\application\current-module.hpp>

static const auto PathToDataDirectory =
  ::alicorn::system::application::CurrentModule::GetAppRootPath() / "data";

/// [Title layer]
class ExampleWindow::DemoLayer1 final :
  public Layer_t
{
public:
  // Интерфейс IWindow:
  void Subscribe(const EventHandlerPtr_t &) override {}
    
public:
  explicit DemoLayer1(Context_t * _pContex) :
    Layer_t(_pContex, PathToDataDirectory / "demo.rml", "title")
  {
    ::alicorn::system::version::Info Info;
    using namespace ::alicorn::extension::std;
      
    GetElement("id_header").SetText(
      "<h1>Covellite++ v" + string_cast<::std::string, Locale::UTF8>(
        Info.GetValue(uT("FileVersionShort"))) + "</h1>");
  }
};
/// [Title layer]

/// [Layer example]
class ExampleWindow::DemoLayer2 final :
  public Layer_t
{
public:
  // Интерфейс IWindow:
  void Subscribe(const EventHandlerPtr_t & _pEvents) override
  {
    using ::covellite::core::Event;
    using ::std::placeholders::_1;
      
    // Подписываемся на событие нажатия кнопки с идентификатором "id_hello".
    // Необходимо сохранить созданный сигнал и отключить его в деструкторе,
    // чтобы исключить вызов фунции не существующего объекта при повторном
    // открытии слоя.
    m_Hello = (*_pEvents)[Event::Click]["id_hello"]
      .connect(::std::bind(&DemoLayer2::DoHello, this, _1));
  }
    
private:
  void DoHello(const ::covellite::core::params::Click & _Value)
  {
    // Функция будет вызвана при нажатии кнопки id_hello.
      
    // Проверяем, что событие произошло для текущего слоя (это позволит
    // делать элементы управления с одинаковыми идентификаторами в разных .rml 
    // файлах; если все идентификаторы всех элементов во всех .rml файлах 
    // гарантированно уникальны, проверку можно опустить).
    if (_Value.m_DocumentId != GetId()) return;
      
    // Выполняем действие, необходимое при нажатии на кнопку.
    GetElement("multiline_text").SetText(u8"Привет!");
  }

private:
  ::boost::signals2::connection m_Hello;
    
public:
  explicit DemoLayer2(Context_t * _pContex) : 
    Layer_t(_pContex, PathToDataDirectory / "demo2.rml")
  {
  }
  ~DemoLayer2(void)
  {
    m_Hello.disconnect();
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
  explicit TableLayer(Context_t * _pContex) :
    Layer_t(_pContex, PathToDataDirectory / "table.rml")
  {
    GetElement("id_board_name").SetText(
      "<h2><font30>Название доски</font30></h2>");

    GetElement("id_list").SetText(
      u8"<button class=\"left\"><font42>\u00D7</font42></button>"
      u8"<p class=\"textleft\"><font36>CSS example</font36></p>"
      u8"<button class=\"right\"><font42>\u00D7</font42></button>"
      u8"<button class=\"right\"><font42>\u00D7</font42></button>");

    GetElement("multiline_text").SetText(
      "Строки:\r\n"
      "- Первая.\r\n"
      "- Вторая.\r\n"
      "- Третья.\r\n"
      "- Четвертая.\r\n"
      "- Пятая.\r\n"
      "- Шестая.");
  }
};

class ExampleWindow::ScrollLayer final :
  public Layer_t
{
public:
  // Интерфейс IWindow:
  void Subscribe(const EventHandlerPtr_t &) override {}

public:
  explicit ScrollLayer(Context_t * _pContex) :
    Layer_t(_pContex, PathToDataDirectory / "scroll.rml")
  {
    const auto Height = ::std::max(GetWidth(), GetHeight());

    GetElement("id_body").SetStyle(
      (::boost::format("font-size: %1%px;") % (Height / 30)).str());
  }
};

// ************************************************************************** //

/// [Constructor main window]
ExampleWindow::ExampleWindow(const WindowRocketPtr_t & _pWindowRocket) :
  m_pWindowRocket(_pWindowRocket)
{
  // Набор строк локализации приложения.
  m_pWindowRocket->Set(
  {
    { u8"[HELLO]", u8"Привет" },
  });
    
  // Экран, который будет отображаться при старте программы.
  m_pWindowRocket->AddLayer<DemoLayer1>();
}
/// [Constructor main window]

/// [Subscribe]
void ExampleWindow::Subscribe(const EventHandlerPtr_t & _pEvents) /*override*/
{
  using ::covellite::core::Event;
  using ::covellite::rocket::Window;
    
  // Подписка на события выбора элементов управления.
  (*_pEvents)[Event::Click]["button1"]
    .connect(::std::bind(&Window::AddLayer<DemoLayer2>, m_pWindowRocket));
  (*_pEvents)[Event::Click]["button2"]
    .connect(::std::bind(&Window::AddLayer<TableLayer>, m_pWindowRocket));
  (*_pEvents)[Event::Click]["button3"]
    .connect(::std::bind(&Window::AddLayer<ScrollLayer>, m_pWindowRocket));
  (*_pEvents)[Event::Click]["exit"]
    .connect(::std::bind(&ExampleWindow::Exit, this));
}
/// [Subscribe]
