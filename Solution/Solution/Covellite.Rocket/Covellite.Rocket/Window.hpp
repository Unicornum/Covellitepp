
#pragma once
#include <map>
#include <memory>
#include <alicorn\boost\filesystem.forward.hpp>
#include <Covellite\Core\IWindow.hpp>
#include <Covellite\Core\Window.hpp>
#include <Covellite\Core\Params.hpp>
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
*  1.1.0.0        \n
*  1.2.0.0        \n
*  1.3.0.0        \n
*  1.4.0.0        \n
*  1.5.0.0        \n
* \date
*  28 Ноябрь 2016    \n
*  20 Декабрь 2016    \n
*  26 Декабрь 2016    \n
*  28 Декабрь 2016    \n
*  19 Сентябрь 2017    \n
*  13 Октябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2017
*/
class Window final :
  public ::covellite::core::IWindow,
  public covellite::core::Window
{
  using WindowApiPtr_t = ::std::shared_ptr<::covellite::api::IWindow>;
  using StringTranslatorPtr_t = 
    ::std::shared_ptr<covellite::rocket::StringTranslator>;
  using Utf8String_t = ::std::string;
  using StringBank_t = ::std::map<Utf8String_t, Utf8String_t>;
  using Path_t = ::boost::filesystem::path;
  using Document_t = Rocket::Core::ElementDocument;
  using Vector_t = Rocket::Core::Vector2i;
  using Initializer_t = covellite::rocket::Initializer;
  using InitializerPtr_t = ::std::unique_ptr<Initializer_t>;
  using LayerPtr_t = ::std::shared_ptr<ILayer>;
  using Layers_t = covellite::rocket::Layers;
  using Context_t = Rocket::Core::Context;
  using ContextPtr_t = ::std::unique_ptr<Context_t, void(*)(Context_t *)>;

public:
  // Интерфейс IWindow:
  void Subscribe(const EventHandlerPtr_t & _Events) override;

public:
  void Set(const StringBank_t &);

public:
  // Функции работы со слоями (экранами) окна.
  template<class TLayer>
  ::std::shared_ptr<TLayer> AddLayer(void);
  void Back(void);

private:
  Vector_t GetContextSize(void) const;
  void DoDrawWindow(void);
  static void LoadFonts(void);

private:
  Layers_t                m_Layers;
  WindowApiPtr_t          m_pWindowApi;
  const int32_t           m_StatusBarHeight;
  StringTranslatorPtr_t   m_pStringTranslator;
  EventHandlerPtr_t       m_pEvents;
  InitializerPtr_t        m_pInitializer;
  ContextPtr_t            m_pContext;

public:
  explicit Window(const WindowApiPtr_t &);
  ~Window(void) noexcept;
};

} // namespace rocket

} // namespace covellite

#include <Covellite\Rocket\Window.inl>
