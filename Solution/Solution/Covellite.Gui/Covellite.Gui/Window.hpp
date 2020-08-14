
#pragma once
#include <map>
#include <memory>
#include <alicorn\boost\filesystem.forward.hpp>
#include <Covellite\Events\Events.hpp>
#include <Covellite\App\IWindow.hpp>
#include <Covellite\Gui\IWindow.hpp>
#include <Covellite\Gui\Layers.hpp>

namespace covellite
{

namespace api { class IWindow; }

namespace gui
{

class Initializer;
class StringTranslator;
class Renderer;

/**
* \ingroup CovelliteGuiGroup
* \brief
*  Класс входит в проект \ref CovelliteGuiPage \n
*  Класс окна, отрисовывающий GUI.
* \details
*  - Является оберткой для CovelliteGui::Core::Context и инкапсулирет в себе
*  взаимодействие библиотеки GUI и событий Android (касание экрана, ввод текста,
*  отрисовка элементов управления, получение событий библиотеки).
*  - Событие OnClick() будет вызываться для всех элементов управления всех
*  активных слоев (пользовательских экранов) окна.
*  - При выборе текстовых полей ввода автоматически вызывает клавиатуру.
*  
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
* \date
*  28 Ноябрь 2016    \n
*  23 Июль 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2018
*/
class Window final :
  public ::covellite::gui::IWindow,
  public ::covellite::app::IWindow
{
  using WindowApi_t = ::covellite::api::IWindow;
  using WindowApiPtr_t = ::std::shared_ptr<WindowApi_t>;
  using Utf8String_t = ::std::string;
  using StringBank_t = ::std::map<Utf8String_t, Utf8String_t>;
  using Path_t = ::boost::filesystem::path;
  using LayerPtr_t = ::std::shared_ptr<ILayer>;
  using Layers_t = covellite::gui::Layers;
  using Initializer_t = covellite::gui::Initializer;
  using InitializerPtr_t = ::std::unique_ptr<Initializer_t>;
  using StringTranslator_t = covellite::gui::StringTranslator;
  using StringTranslatorPtr_t = ::std::shared_ptr<StringTranslator_t>;
  using Context_t = CovelliteGui::Core::Context;
  using ContextPtr_t = ::std::unique_ptr<Context_t, void(*)(Context_t *)>;
  using Vector_t = CovelliteGui::Core::Vector2i;
  using RendererPtr_t = ::std::shared_ptr<covellite::gui::Renderer>;

public:
  class EventListener;
  using EventListenerPtr_t = ::std::shared_ptr<EventListener>;

public:
  // Интерфейс events::IEvents:
  operator Events_t (void) const override;

public:
  // Интерфейс gui::IWindow:
  DocumentPtr_t LoadDocument(const PathToFile_t &) override;

public:
  void Set(const StringBank_t &);

public:
  // Функции работы со слоями (экранами) окна.
  template<class TLayer>
  TLayer & PushLayer(void);
  void Back(void);

private:
  void PushLayer(const LayerPtr_t &);

private:
  Vector_t GetContextSize(void) const;
  void DoDrawWindow(void);
  void LoadFonts(void);

private:
  const WindowApi_t   & m_WindowApi;
  Events_t              m_Events;
  RendererPtr_t         m_pRenderer;
  EventListenerPtr_t    m_pEventListener;
  StringTranslatorPtr_t m_pStringTranslator;
  InitializerPtr_t      m_pInitializer;
  ContextPtr_t          m_pContext;
  Layers_t              m_Layers;
  ::std::map<Path_t, ::std::vector<uint8_t>> m_RawDataFonts;

public:
  explicit Window(const WindowApi_t &);
  ~Window(void) noexcept;
};

} // namespace gui

} // namespace covellite

#include <Covellite\Gui\Window.inl>
