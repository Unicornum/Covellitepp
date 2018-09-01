
#pragma once
#include <Covellite/Os/Predefined.hpp>

/**
* \defgroup CovelliteApiRenderGroup Render
* \ingroup CovelliteApiGroup
*  Группа классов реализаций рендеринга при помощи конкретных графических Api.
*/

namespace covellite
{

namespace api
{

namespace render
{

/**
* \ingroup CovelliteApiRenderGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Интерфейсный класс обобщенного рендера.
*  
* \version
*  1.0.0.0        \n
* \date
*  21 Август 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class IRender
{
public:
  /// Класс для передачи значений настроек программы реализации рендера.
  class Data final
  {
  public:
    class Color final
    {
    public:
      float R, G, B, A;
    };

  public:
    Handle_t  Handle;       ///< Хэнд окна операционной системы.
    int       Top;          ///< Смещение верхнего края клиентской части окна.
    Color     BkColor;      ///< Цвет фона окна по умолчанию.
    bool      IsFullScreen; ///< Признак работы программы в полноэкранном 
                            ///< режиме.
  };

public:
  virtual void ClearWindow(void) = 0;
  virtual void Present(void) = 0;
  virtual void ResizeWindow(int32_t, int32_t) = 0;

public:
  virtual ~IRender(void) {}
};

} // namespace render

} // namespace api

} // namespace covellite
