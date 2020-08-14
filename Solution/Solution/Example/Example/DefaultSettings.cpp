
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
    uT("Data/Fonts"));  // Путь к папке расположения шрифтов, которые 
                        // используются в .rcss файлах.
  (*pSettings).AddExtra(uT("PathToTextureDirectory"),
    uT("Data"));        // Путь к папке расположения текстур, которые 
                        // используются в проекте примера.
  (*pSettings).AddExtra(uT("PathToMeshDirectory"),
    uT("Data"));        // Путь к папке расположения файлов 3D объектов, которые 
                        // используются в проекте примера.
  (*pSettings).AddExtra(uT("PathToSoundsDirectory"),
    uT("Data/Sounds")); // Путь к папке расположения звуковых файлов, которые 
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
    0x80); // Цвет фона окна программы по умолчанию: красная компонента [0...255].
  (*pSettings)[uT("Window")][uT("BackgroundColor")].AddExtra(uT("G"), 
    0x80); // Цвет фона окна программы по умолчанию: зеленая компонента [0...255].
  (*pSettings)[uT("Window")][uT("BackgroundColor")].AddExtra(uT("B"), 
    0x80); // Цвет фона окна программы по умолчанию: синяя компонента [0...255].
  (*pSettings)[uT("Window")][uT("BackgroundColor")].AddExtra(uT("A"), 
    0x80); // Цвет фона окна программы по умолчанию: прозрачность [0...255].

  (*pSettings)[uT("Demo")].SetDefault(uT("IsParticleInstance"),
    true, uT("Использовать instancing для рендеринга частиц."));
  (*pSettings)[uT("Demo")].SetDefault(uT("ParticlesCount"),
    10000, uT("Количество частиц."));
  (*pSettings)[uT("Demo")].SetDefault(uT("IsCubeInstance"),
    true, uT("Использовать instancing для рендеринга кубиков."));
  (*pSettings)[uT("Demo")].SetDefault(uT("IsNightMode"),
    false, uT("Режим рендеринга ночь/день."));
  (*pSettings)[uT("Demo")].SetDefault(uT("PointLightsPercent"),
    5, uT("Количесто точечных источников света ночью в процентах от общей площади игровой сцены."));
  (*pSettings)[uT("Demo")].SetDefault(uT("IsAutoRun"),
    false, uT("Включение/выключение автоматического перемещения по ландшафту."));

  return pSettings;
}
  
} // namespace std
  
} // namespace extension
  
} // namespace alicorn
