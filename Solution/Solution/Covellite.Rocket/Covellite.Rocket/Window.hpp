
#pragma once
#include <map>
#include <memory>
#include <alicorn\boost\filesystem.forward.hpp>
#include <Covellite\Events\Events.hpp>
#include <Covellite\Core\IWindow.hpp>
#include <Covellite\Core\Window.hpp>
#include <Covellite\Core\Params.hpp>
#include <Covellite\App\IWindow.hpp>
#include <Covellite\Rocket\IWindow.hpp>
#include <Covellite\Rocket\Rocket.forward.hpp>
#include <Covellite\Rocket\Layers.hpp>

namespace covellite
{

namespace api { class IWindow; }

namespace rocket
{

class Initializer;
class StringTranslator;

/**
* \ingroup gCovelliteRocketClasses
* \brief
*  Класс входит в проект \ref CovelliteRocketPage \n
*  Класс окна, отрисовывающий GUI при помощи libRocket.
* \details
*  - Является оберткой для Rocket::Core::Context и инкапсулирет в себе
*  взаимодействие libRocket и событий Android (касание экрана, ввод текста,
*  отрисовка элементов управления, получение событий от libRocket).
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
  public ::covellite::rocket::IWindow,
  public ::covellite::core::IWindow,
  public ::covellite::app::IWindow,
  public covellite::core::Window
{
  using WindowApi_t = ::covellite::api::IWindow;
  using WindowApiPtr_t = ::std::shared_ptr<WindowApi_t>;
  using Utf8String_t = ::std::string;
  using StringBank_t = ::std::map<Utf8String_t, Utf8String_t>;
  using Path_t = ::boost::filesystem::path;
  using LayerPtr_t = ::std::shared_ptr<ILayer>;
  using Layers_t = covellite::rocket::Layers;
  using Initializer_t = covellite::rocket::Initializer;
  using InitializerPtr_t = ::std::unique_ptr<Initializer_t>;
  using StringTranslator_t = covellite::rocket::StringTranslator;
  using StringTranslatorPtr_t = ::std::shared_ptr<StringTranslator_t>;
  using Context_t = Rocket::Core::Context;
  using ContextPtr_t = ::std::unique_ptr<Context_t, void(*)(Context_t *)>;
  using Vector_t = Rocket::Core::Vector2i;

public:
  class ClickEventListener;
  using ClickEventListenerPtr_t = ::std::shared_ptr<ClickEventListener>;

public:
  // Интерфейс core::IWindow:
  void Subscribe(const EventHandlerPtr_t & _Events) override;

public:
  // Интерфейс events::IEvents:
  operator Events_t (void) const override;

public:
  // Интерфейс rocket::IWindow:
  Document_t * LoadDocument(const PathToFile_t &) override;

public:
  void Set(const StringBank_t &);

public:
  // Функции работы со слоями (экранами) окна.
  template<class TLayer>
  ::std::shared_ptr<TLayer> AddLayer(void);
  template<class TLayer>
  void PushLayer(void);
  void Back(void);

private:
  Vector_t GetContextSize(void) const;
  void DoDrawWindow(void);
  static void LoadFonts(void);

private:
  const WindowApi_t     & m_WindowApi;
  Events_t                m_Events;
  ClickEventListenerPtr_t m_pClickEventListener;
  EventHandlerPtr_t       m_pEvents;
  StringTranslatorPtr_t   m_pStringTranslator;
  InitializerPtr_t        m_pInitializer;
  ContextPtr_t            m_pContext;
  Layers_t                m_Layers;

public:
  explicit Window(const WindowApi_t &);
  explicit Window(const WindowApiPtr_t &);
  ~Window(void) noexcept;
};

} // namespace rocket

} // namespace covellite

#include <Covellite\Rocket\Window.inl>
