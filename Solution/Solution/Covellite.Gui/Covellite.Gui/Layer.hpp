
#pragma once
#include <alicorn\std\string.forward.hpp>
#include <alicorn\boost\filesystem.forward.hpp>
#include <Covellite\Gui\Rocket.forward.hpp>
#include <Covellite\Events\Events.hpp>
#include <Covellite\Gui\ILayer.hpp>
#include <Covellite\Gui\IWindow.hpp>

namespace covellite
{

namespace gui
{

/**
* \ingroup CovelliteGuiGroup
* \brief
*  Класс входит в проект \ref CovelliteGuiPage \n
*  Класс одного слоя (пользовательского экрана) программы.
* \details
*  - Представляет собой обертку над документом libRocket и предназначен для
*  работы с отдельным экраном программы.
*  - Класс-наследник указывается в качестве параметра шаблона функции 
*  covellite::gui::Window::AddLayer<>(). 
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
  public ::covellite::gui::ILayer
{
  using Utf8String_t = ::std::string;
  using DocumentId_t = ::std::string;
  using ElementId_t = ::std::string;

protected:
  using IWindowGui_t = gui::IWindow;
  using Context_t = Rocket::Core::Context;
  using Document_t = Rocket::Core::ElementDocument;
  using Element_t = Rocket::Core::Element;
  using Path_t = ::boost::filesystem::path;
  using String_t = ::alicorn::extension::std::String;

public:
  /**
  * \ingroup CovelliteGuiGroup
  * \brief
  *  Класс входит в проект \ref CovelliteGuiPage \n
  *  Класс одного элемента .rml файла.
  */
  class Element final
  {
  public:
    void SetFocus(void);
    void SetMeaning(const String_t &);
    String_t GetMeaning(void) const;
    void SetClassStyle(const String_t &);

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
  float EmployFontSize(float);

private:
  static ::std::string Convert(const Path_t &);

protected:
  ::covellite::events::Events m_Events;

private:
  Document_t * const m_pDocument;

protected:
  Layer(gui::IWindow &, const Path_t &);
  Layer(gui::IWindow &, const Path_t &, const ::std::string &);

public:
  ~Layer(void) noexcept;
};

} // namespace gui

} // namespace covellite
