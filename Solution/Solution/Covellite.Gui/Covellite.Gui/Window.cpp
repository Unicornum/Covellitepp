
#include "stdafx.h"
#include <Covellite/Gui/Window.hpp>
#include <alicorn/platform/app-info.hpp>
#include <alicorn/std/class-info.hpp>
#include <alicorn/std/string.hpp>
#include <alicorn/boost/string-cast.hpp>
#include <Covellite/Events.hpp>
#include <Covellite/App/Events.hpp>
#include <Covellite/App/Settings.hpp>
#include <Covellite/App/Vfs.hpp>
#include <Covellite/Os/Events.hpp>
#include <Covellite/Api/IWindow.hpp>
#include <Covellite/Api/Events.hpp>
#include <Covellite/Gui/Renderer.hpp>
#include <Covellite/Gui/Initializer.hpp>
#include <Covellite/Gui/StringTranslator.hpp>
#include <Covellite/Gui/EventListener.hpp>
#include <Covellite/Gui/Events.hpp>
#include "SystemToGuiKeyCode.hpp"

#ifndef __USING_GTEST
# include <alicorn\logger.hpp>
#endif

#include <FreeType/FreeType.lib.hpp>

using namespace covellite::gui;

Window::Window(const WindowApi_t & _Window) :
  m_WindowApi(_Window),
  m_Events(_Window),
  m_pRenderer(::std::make_shared<covellite::gui::Renderer>(_Window.GetRenders())),
  m_pEventListener(EventListener::Make(_Window)),
  m_pStringTranslator(::std::make_unique<covellite::gui::StringTranslator>()),
  m_pInitializer(::std::make_unique<Initializer_t>(Initializer_t::Data
    {
      m_pRenderer,
      m_pStringTranslator
    })),
  m_pContext(CovelliteGui::Core::CreateContext("main", GetContextSize()),
    [](CovelliteGui::Core::Context * _pContext) { CovelliteGuiRemove(_pContext); })
{
  if (m_pContext == nullptr)
  {
    throw STD_EXCEPTION << "Create context failed.";
  }

  CovelliteGui::Controls::Initialise();

  if (::alicorn::extension::cpp::IS_DEBUG_CONFIGURATION)
  {
    // Инициализация через установку контекста сделана из-за того, что 
    // CovelliteGui::Debugger::Initialise() можно вызывать только один раз в рамках
    // работы одного .so модуля (повторный вызов приводит к ошибке в логе
    // и Debugger работать не будет), Android программа грузит .so модуль
    // только один раз при первом старте, а при повторной активации программы
    // просто вызывает функции ранее загруженного модуля.
    if (!CovelliteGui::Debugger::SetContext(m_pContext.get()))
    {
      CovelliteGui::Debugger::Initialise(m_pContext.get());
    }
  }

  m_pContext->AddEventListener(::covellite::events::Click.m_EventType.c_str(), 
    m_pEventListener.get(), false);
  m_pContext->AddEventListener(::covellite::events::Press.m_EventType.c_str(),
    m_pEventListener.get(), false);
  m_pContext->AddEventListener(::covellite::events::Change.m_EventType.c_str(),
    m_pEventListener.get(), false);

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
    m_pContext->ProcessKeyDown(SystemToGuiKeyCode(_Code), 0);
  });
  m_Events[events::Key.Pressed]
    .Connect([&](const events::Key_t::Code & _Code) 
  {
    // На Android'e при нажатии управляющих кнопок генерируется событие
    // Key.Pressed с нулем в качестве кода нажатой кнопки.
    if (_Code < 0x20) return;

    m_pContext->ProcessTextInput(static_cast<CovelliteGuiUnicode_t>(_Code));
  });

  m_Events[events::Drawing.Do]
    .Connect([&]() { DoDrawWindow(); });
}

Window::~Window(void) noexcept
{
  m_pContext->RemoveEventListener(::covellite::events::Click.m_EventType.c_str(), 
    m_pEventListener.get(), false);
  m_pContext->RemoveEventListener(::covellite::events::Press.m_EventType.c_str(),
    m_pEventListener.get(), false);
  m_pContext->RemoveEventListener(::covellite::events::Change.m_EventType.c_str(),
    m_pEventListener.get(), false);
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
*  Функция загрузки документа из указанного файла.
*/
Window::DocumentPtr_t Window::LoadDocument(const PathToFile_t & _Path) /*override*/
{
  const auto Start = ::std::chrono::system_clock::now();

  auto pResult = DocumentPtr_t(m_pContext->LoadDocument(Layer::Convert(_Path).c_str()),
    [](Document_t * _pDocument) { CovelliteGuiRemove(_pDocument); });

  ::std::chrono::duration<double> TimeCall =
    ::std::chrono::system_clock::now() - Start;

  LOGGER(Info) << _Path.string() << ": " << TimeCall.count();

  return pResult;
}

/**
* \brief
*  Функция установки набора для преобразования строк.
* \details
*  - Подробнее см. функцию covellite::gui::StringTranslator::Set().
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
  LOGGER(Info) << "Pop layer";

  const auto IsExistsLayer = m_Layers.Pop();
  if (!IsExistsLayer)
  {
    m_Events[::covellite::events::Application.Exit]();
  }
}

/**
* \brief
*  Функция реаизации вставки нового слоя.
*/
void Window::PushLayer(const LayerPtr_t & _pLayer)
{
  m_Layers.Push(_pLayer);

  LOGGER(Info) << "Push layer [" <<
    ::alicorn::extension::std::ClassInfo::GetPureName(*_pLayer) << "]";
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
*  Функция добработки события covellite::events::Drawing.Do.
*/
void Window::DoDrawWindow(void)
{
  m_pContext->Update();
  m_pContext->Render();
  m_pRenderer->RenderScene();
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
void Window::LoadFonts(void)
{
  const auto & CovelliteppSection = 
    ::covellite::app::Settings_t::GetInstance();
  const auto PathToFontsDirectory =
    CovelliteppSection.Get<Path_t>(uT("PathToFontsDirectory"));
  const auto & Vfs = 
    ::covellite::app::Vfs_t::GetInstance();

  Vfs.BrowsingFiles(PathToFontsDirectory, [&](const Path_t & _FontFile)
  {
    // RmlUi не сохраняет копию переданных данных, поэтому прочитанные
    // файлы следует хранить до конца использования.
    m_RawDataFonts[_FontFile] = ::std::move(Vfs.GetData(_FontFile));

    CovelliteGuiLoadFontFace(m_RawDataFonts[_FontFile]);
  });
}
