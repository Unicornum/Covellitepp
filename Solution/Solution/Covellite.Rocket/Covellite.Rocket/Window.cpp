
#include "stdafx.h"
#include <Covellite\Rocket\Window.hpp>
#include <alicorn\platform\app-info.hpp>
#include <Covellite\Core\Settings.hpp>
#include <Covellite\Core\Event.hpp>
#include <Covellite\Core\EventHandler.hpp>
#include <Covellite\Core\ClickEventListener.hpp>
#include <Covellite\Api\IWindow.hpp>
#include <Covellite\Rocket\Initializer.hpp>
#include <Covellite\Rocket\StringTranslator.hpp>

using namespace covellite::rocket;

Window::Window(const WindowApiPtr_t & _pWindowsApi) :
  m_pWindowApi(_pWindowsApi),
  m_StatusBarHeight(GetStatusBarHeight()),
  m_pStringTranslator(::std::make_unique<covellite::rocket::StringTranslator>()),
  m_pEvents(::std::make_shared<EventHandler_t>(EventHandler_t::Dummy{})),
  m_pInitializer(::std::make_unique<Initializer_t>(Initializer_t::Data
    {
      m_pWindowApi->MakeRenderInterface(),
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
    // и Debugger работать не будет), а android программа грузит .so модуль
    // только один раз при первом старте, а при повторной активации программы
    // просто вызывает функции ранее загруженного модуля.
    if (!Rocket::Debugger::SetContext(m_pContext.get()))
    {
      Rocket::Debugger::Initialise(m_pContext.get());
    }
  }

  LoadFonts();
}

Window::~Window(void) noexcept
{
  m_pEvents->Unsubscribe(m_pContext.get());
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

  using namespace ::covellite::core;

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
      { m_pContext->ProcessMouseMove(_Params.X, _Params.Y - m_StatusBarHeight, 0); });
  (*m_pEvents)[Event::KeyPressed]
    .connect([&](const params::KeyPressed & _KeyCode)
      { m_pContext->ProcessTextInput(static_cast<Rocket::Core::word>(_KeyCode.UnicodeKeyCode)); });
  (*m_pEvents)[Event::KeyDown]
    .connect([&](const params::KeyCode & _KeyCode)
      { m_pContext->ProcessKeyDown(_KeyCode.ToRocketKeyCode(), 0); });
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
  return Vector_t(m_pWindowApi->GetWidth(), 
    m_pWindowApi->GetHeight() - m_StatusBarHeight);
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

  const auto CovelliteppSection = ::covellite::core::Settings_t::GetInstance();

  const auto PathToFontsDirectory =
    CovelliteppSection.Get<Path_t>(uT("PathToFontsDirectory"));

  Directory::Iterate(PathToFontsDirectory, [](const Path_t & _PathToFont)
  {
    Rocket::Core::FontDatabase::LoadFontFace(_PathToFont.string().c_str());
  });
}
