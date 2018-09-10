
#include "stdafx.h"
#include <Covellite/Rocket/Window.hpp>
#include <alicorn/platform/app-info.hpp>
#include <alicorn/std/string.hpp>
#include <alicorn/boost/string-cast.hpp>
#include <Covellite/Events.hpp>
#include <Covellite/Core/Settings.hpp>
#include <Covellite/Core/Event.hpp>
#include <Covellite/Core/EventHandler.hpp>
#include <Covellite/Core/ClickEventListener.hpp>
#include <Covellite/Os/Events.hpp>
#include <Covellite/Api/IWindow.hpp>
#include <Covellite/Api/Events.hpp>
#include <Covellite/Rocket/Initializer.hpp>
#include <Covellite/Rocket/StringTranslator.hpp>
#include <Covellite/Rocket/ClickEventListener.hpp>

using namespace covellite::rocket;

Window::Window(const WindowApi_t & _Window) :
  m_WindowApi(_Window),
  m_Events(_Window),
  m_pClickEventListener(ClickEventListener::Make(_Window)),
  m_pEvents(::std::make_shared<EventHandler_t>(EventHandler_t::Dummy{})),
  m_pStringTranslator(::std::make_unique<covellite::rocket::StringTranslator>()),
  m_pInitializer(::std::make_unique<Initializer_t>(Initializer_t::Data
    {
      m_WindowApi.MakeRenderInterface(),
      m_pStringTranslator
    })),
  m_pContext(Rocket::Core::CreateContext("main", GetContextSize()),
    [](Rocket::Core::Context * _pContext) { _pContext->RemoveReference(); })
{
  if (m_pContext == nullptr)
  {
    throw STD_EXCEPTION << "Create context failed.";
  }

  Rocket::Controls::Initialise();

  if (::alicorn::extension::cpp::IS_DEBUG_CONFIGURATION)
  {
    // Инициализация через установку контекста сделана из-за того, что 
    // Rocket::Debugger::Initialise() можно вызывать только один раз в рамках
    // работы одного .so модуля (повторный вызов приводит к ошибке в логе
    // и Debugger работать не будет), Android программа грузит .so модуль
    // только один раз при первом старте, а при повторной активации программы
    // просто вызывает функции ранее загруженного модуля.
    if (!Rocket::Debugger::SetContext(m_pContext.get()))
    {
      Rocket::Debugger::Initialise(m_pContext.get());
    }
  }

  m_pContext->AddEventListener("click", m_pClickEventListener.get(), false);

  LoadFonts();

  m_Events[events::Window.Resize]
    .Connect([&]() { m_pContext->SetDimensions(GetContextSize()); });

  m_Events[events::Cursor.Motion]
    .Connect([&](const events::Cursor_t::Position & _Position)       
  { 
    m_pContext->ProcessMouseMove(_Position.X, 
      _Position.Y - m_WindowApi.GetClientRect().Top, 0);
  });
  m_Events[events::Cursor.Touch]
    .Connect([&]() { m_pContext->ProcessMouseButtonDown(0, 0); });
  m_Events[events::Cursor.Release]
    .Connect([&]() { m_pContext->ProcessMouseButtonUp(0, 0); });

  m_Events[events::Key.Back]
    .Connect([&]() { Back(); });
  m_Events[events::Key.Down]
    .Connect([&](const events::Key_t::Code & _Code)
  {
    m_pContext->ProcessKeyDown(SystemToRocketKeyCode(_Code), 0);
  });
  m_Events[events::Key.Pressed]
    .Connect([&](const events::Key_t::Code & _Code) 
  { 
    m_pContext->ProcessTextInput(static_cast<Rocket::Core::word>(_Code));  
  });

  m_Events[events::Drawing.Do]
    .Connect([&]() { DoDrawWindow(); });
}

/**
* \deprecated
*  Устаревший конструктор, используемый совместно с кодом из проекта
*  \ref CovelliteCorePage; будет удален в следующей стабильной версии. \n
*  Использовать второй конструктор совместно с кодом проекта 
*  \ref CovelliteAppPage.
*/
Window::Window(const WindowApiPtr_t & _pWindowsApi) :
  Window(*_pWindowsApi)
{
}

Window::~Window(void) noexcept
{
  m_pEvents->Unsubscribe(m_pContext.get());
  m_pContext->RemoveEventListener("click", m_pClickEventListener.get(), false);
}

/**
* \brief
*  Функция подписки на события приложения.
* \details
*  - Класс-наследник (если он переопределяет функцию) должен вызвать функцию
*  базового класса.
*/
void Window::Subscribe(const EventHandlerPtr_t & _pEvents) /*override*/
{
  m_pEvents = _pEvents;
  m_pEvents->Subscribe(m_pContext.get());

#if BOOST_COMP_MSVC
# pragma warning(push)
# pragma warning(disable: 4996)
#endif

  using namespace ::covellite::core;

#if BOOST_COMP_MSVC
# pragma warning(pop)
#endif

  (*m_pEvents)[Event::Drawing]
    .connect(::std::bind(&Window::DoDrawWindow, this));
  (*m_pEvents)[Event::Back]
    .connect(::std::bind(&Window::Back, this));

  (*m_pEvents)[Event::Resize]
    .connect([&](const Params &) { m_pContext->SetDimensions(GetContextSize()); });
  (*m_pEvents)[Event::Touch]
    .connect([&](const Params &) { m_pContext->ProcessMouseButtonDown(0, 0); });
  (*m_pEvents)[Event::Release]
    .connect([&](const Params &) { m_pContext->ProcessMouseButtonUp(0, 0); });

  (*m_pEvents)[Event::Motion]
    .connect([&](const params::Motion & _Params)
      { m_pContext->ProcessMouseMove(_Params.X, _Params.Y - m_WindowApi.GetClientRect().Top, 0); });
  (*m_pEvents)[Event::KeyPressed]
    .connect([&](const params::KeyPressed & _KeyCode)
      { m_pContext->ProcessTextInput(static_cast<Rocket::Core::word>(_KeyCode.UnicodeKeyCode)); });
  (*m_pEvents)[Event::KeyDown]
    .connect([&](const params::KeyCode & _KeyCode)
      { m_pContext->ProcessKeyDown(_KeyCode.ToRocketKeyCode(), 0); });
}

/**
* \brief
*  Оператор получения объекта событий фреймворка.
*/
Window::operator Events_t (void) const /*override*/
{
  return m_Events;
}

/**
* \brief
*  Функция загрузки документа libRocket из указанного файла.
*/
Window::Document_t * Window::LoadDocument(const PathToFile_t & _Path) /*override*/
{
  // 23 Июль 2018 19:41 (unicornum.verum@gmail.com)
  TODO("Возвращать unique_ptr с deleter'ом, вызывающим RemoveReference()?");

  return m_pContext->LoadDocument(_Path);
}

/**
* \brief
*  Функция установки набора для преобразования строк.
* \details
*  - Подробнее см. функцию covellite::rocket::StringTranslator::Set().
*/
void Window::Set(const StringBank_t & _Bank)
{
  m_pStringTranslator->Set(_Bank);
}

/**
* \brief
*  Функция возврата к предыдущему слою.
* \details
*  - При добавлении слоев они вталкиваются в стек, эта функция выталкивает слой
*  с вершины стека и активирует предыдущий.
*  - Если текущий слой - единственный, работа программы будет завершена.
*/
void Window::Back(void)
{
  const auto IsExistsLayer = m_Layers.Pop();
  if (!IsExistsLayer) Exit();
}

/**
* \brief
*  Функция получения размеров панели контекста в пикселях.
* \details
*  - Размеры панели соответствуют размерам клиентской области окна программы.
*/
Window::Vector_t Window::GetContextSize(void) const
{
  const auto Rect = m_WindowApi.GetClientRect();
  return Vector_t(Rect.Width, Rect.Height - Rect.Top);
}

/**
* \brief
*  Функция добработки события covellite::core::Event::Draw.
*/
void Window::DoDrawWindow(void)
{
  m_pContext->Update();
  m_pContext->Render();
}

/**
* \brief
*  Функция загрузки используемых шрифтов.
* \details
*  - Функция загрузит все файлы шрифтов из папки, указанной как значение
*  параметра \b PathToFontsDirectory раздела \b Covellitepp настроек.
*  - Ошибка (если таковая возникнет) будет записана в лог через интерфейс
*  System.
*/
/*static*/ void Window::LoadFonts(void)
{
  using Path_t = ::boost::filesystem::path;

  class Directory
  {
    using fnCallBack_t = void(*)(const Path_t &);

  public:
    static void Iterate(const Path_t & _PathToFontsDirectory, 
      const fnCallBack_t & _fnCallBack)
    {
# if BOOST_OS_WINDOWS

      using ItDir_t = ::boost::filesystem::directory_iterator;

      for (ItDir_t itPathToFont{ _PathToFontsDirectory };
        itPathToFont != ItDir_t{}; ++itPathToFont)
      {
        _fnCallBack(itPathToFont->path());
      }

# elif BOOST_OS_ANDROID

      // 07 Ноябрь 2017 12:03 (unicornum.verum@gmail.com)
      TODO("Тест Android версии чтения шрифтов.");

      using AppInfo_t = ::alicorn::system::platform::AppInfo;

      auto * pAssetManager = AppInfo_t::Get<ANativeActivity>().assetManager;
      if (pAssetManager == nullptr) return;

      auto * pAssetDir = AAssetManager_openDir(pAssetManager,
        _PathToFontsDirectory.string().c_str());
      if (pAssetDir == nullptr) return;

      const char * filename = nullptr;
      while ((filename = AAssetDir_getNextFileName(pAssetDir)) != nullptr)
      {
        _fnCallBack(_PathToFontsDirectory / filename);
      }

      AAssetDir_close(pAssetDir);

# endif
    }
  };

#if BOOST_COMP_MSVC
# pragma warning(push)
# pragma warning(disable: 4996)
#endif

  const auto CovelliteppSection = ::covellite::core::Settings_t::GetInstance();

#if BOOST_COMP_MSVC
# pragma warning(pop)
#endif

  const auto PathToFontsDirectory =
    CovelliteppSection.Get<Path_t>(uT("PathToFontsDirectory"));

  Directory::Iterate(PathToFontsDirectory, [](const Path_t & _PathToFont)
  {
    Rocket::Core::FontDatabase::LoadFontFace(_PathToFont.string().c_str());
  });
}
