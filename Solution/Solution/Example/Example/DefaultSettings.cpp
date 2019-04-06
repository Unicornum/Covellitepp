
#include "stdafx.h"
#include <Covellite/App/Settings.hpp>
  
namespace alicorn
{
  
namespace extension
{
  
namespace std
{
  
template<>
/*static*/ SectionPtr_t Singleton<Section_t>::Make(void)
{
  auto pSettings = ::std::make_unique<Section_t>(uT("Covellitepp"));
    
  // Функция AddExtra() используется для параметров, которые не должен менять
  // пользователь (они не будут видны в программе изменения настроек),
  // SetDefault() - для параметров, которые пользователь может изменить
  // при необходимости.
    
  (*pSettings).AddExtra(uT("PathToFontsDirectory"),
    uT("data/fonts"));  // Путь к папке расположения шрифтов, которые 
                        // используются в .rcss файлах.
  (*pSettings).AddExtra(uT("PathToTextureDirectory"),
    uT("data"));        // Путь к папке расположения текстур, которые 
                        // используются в проекте примера.
    
  (*pSettings)[uT("Window")].SetDefault(uT("IsFullScreen"),
    false, uT("Полноэкранный/оконный режим работы программы."));
  (*pSettings)[uT("Window")].AddExtra(uT("IsResized"),
    true); // Разрешение/запрет изменения размеров окна программы мышью.
  (*pSettings)[uT("Window")].SetDefault(uT("GraphicsApi"), 
    uT("Auto"), uT("Используемый для рендеринга графический Api."));
    
  (*pSettings)[uT("Window")][uT("Size")].SetDefault(uT("Width"),
    480, uT("Ширина клиентской области окна программы."));
  (*pSettings)[uT("Window")][uT("Size")].SetDefault(uT("Height"),
    762, uT("Высота клиентской области окна программы."));
  (*pSettings)[uT("Window")][uT("Size")].AddExtra(uT("MinClientWidth"),
    480); // Минимальная ширина клиентской области окна программы.
  (*pSettings)[uT("Window")][uT("Size")].AddExtra(uT("MinClientHeight"),
    480); // Минимальная высота клиентской области окна программы.
    
  (*pSettings)[uT("Window")][uT("BackgroundColor")].AddExtra(uT("R"), 
    255); // Цвет фона окна программы по умолчанию: красная компонента [0...255].
  (*pSettings)[uT("Window")][uT("BackgroundColor")].AddExtra(uT("G"), 
    0); // Цвет фона окна программы по умолчанию: зеленая компонента [0...255].
  (*pSettings)[uT("Window")][uT("BackgroundColor")].AddExtra(uT("B"), 
    0); // Цвет фона окна программы по умолчанию: синяя компонента [0...255].
  (*pSettings)[uT("Window")][uT("BackgroundColor")].AddExtra(uT("A"), 
    255); // Цвет фона окна программы по умолчанию: прозрачность [0...255].
    
  return pSettings;
}
  
} // namespace std
  
} // namespace extension
  
} // namespace alicorn
