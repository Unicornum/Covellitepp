
#pragma once
#include <Covellite\Rocket\Rocket.forward.hpp>
#include <Covellite\Core\IWindow.hpp>
#include <Covellite\Events\Events.hpp>
#include <Covellite\Rocket\ILayer.hpp>
#include <Covellite\Rocket\IWindow.hpp>
#include <alicorn\boost\filesystem.forward.hpp>

namespace covellite
{

namespace rocket
{

/**
* \ingroup CovelliteRocketGroup
* \brief
*  Класс входит в проект \ref CovelliteRocketPage \n
*  Класс одного слоя (пользовательского экрана) программы.
* \details
*  - Представляет собой обертку над документом libRocket и предназначен для
*  работы с отдельным экраном программы.
*  - Класс-наследник указывается в качестве параметра шаблона функции 
*  covellite::rocket::Window::AddLayer<>(). 
*  - Класс-наследник должен переопределить функцию 
*  covellite::core::IWindow::Subscribe(), в которой может подписаться на
*  нужные ему события.
*  
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
* \date
*  20 Декабрь 2016    \n
*  31 Июль 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2018
*/
class Layer :
  public ::covellite::core::IWindow,
  public ::covellite::rocket::ILayer
{
  using Utf8String_t = ::std::string;
  using DocumentId_t = ::std::string;
  using ElementId_t = ::std::string;

protected:
  using Context_t = Rocket::Core::Context;
  using Document_t = Rocket::Core::ElementDocument;
  using Element_t = Rocket::Core::Element;
  using Path_t = ::boost::filesystem::path;
  using String_t = Rocket::Core::String;

public:
  class Element
  {
  public:
    void SetText(const Utf8String_t &);
    Utf8String_t GetText(void) const;
    void SetStyle(const Utf8String_t &);
    void SetFocus(void);

  private:
    Element_t * const m_pElement;

  public:
    // cppcheck-suppress noExplicitConstructor
    Element(Element_t *);
  };

public:
  // Интерфейс ILayer:
  void Show(void) final;
  void Hide(void) final;

public:
  DocumentId_t GetId(void) const;
  Element GetElement(const ElementId_t &) const;
  int GetWidth(void) const;
  int GetHeight(void) const;
  void SetFontSize(double = 1.0);

private:
  static String_t Convert(const Path_t &);

protected:
  ::covellite::events::Events m_Events;

private:
  Document_t * const m_pDocument;

protected:
  Layer(rocket::IWindow &, const Path_t &);
  Layer(rocket::IWindow &, const Path_t &, const ::std::string &);

public:
  Layer(Context_t *, const Path_t &);
  Layer(Context_t *, const Path_t &, const ::std::string &);
  ~Layer(void) noexcept;
};

} // namespace rocket

} // namespace covellite
