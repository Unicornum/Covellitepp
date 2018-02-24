
#include "stdafx.h"
#include <Covellite\Core\Settings.hpp>
#include <alicorn\application\current-module.hpp>
  
namespace alicorn
{
  
namespace extension
{
  
namespace std
{
  
using namespace ::covellite::core;
  
template<>
/*static*/ SectionPtr_t Settings_t::Make(void)
{
  auto pSettings = ::std::make_unique<Section_t>(uT("Covellitepp"));
    
  const auto PathToRootDirectory =
    ::alicorn::system::application::CurrentModule::GetAppRootPath();
    
  (*pSettings).SetDefault(uT("PathToFontsDirectory"),
    PathToRootDirectory / "data" / "fonts",
    uT("Путь к папке расположения шрифтов, которые используются в .rcss файлах."));
    
  (*pSettings)[uT("Window")].SetDefault(uT("IsFullScreen"), false,
    uT("Полноэкранный/оконный режим работы программы (только для Windows)."));
  (*pSettings)[uT("Window")][uT("BackgroundColor")].SetDefault(uT("R"), 0,
    uT("Цвет фона окна программы по умолчанию: красная компонента [0...255]."));
  (*pSettings)[uT("Window")][uT("BackgroundColor")].SetDefault(uT("G"), 0,
    uT("Цвет фона окна программы по умолчанию: зеленая компонента [0...255]."));
  (*pSettings)[uT("Window")][uT("BackgroundColor")].SetDefault(uT("B"), 255,
    uT("Цвет фона окна программы по умолчанию: синяя компонента [0...255]."));
  (*pSettings)[uT("Window")][uT("BackgroundColor")].SetDefault(uT("A"), 255,
    uT("Цвет фона окна программы по умолчанию: прозрачность [0...255]."));
  (*pSettings)[uT("Window")][uT("Size")].SetDefault(uT("Width"), 480,
    uT("Ширина клиентской области окна программы (только для Windows)."));
  (*pSettings)[uT("Window")][uT("Size")].SetDefault(uT("Height"), 762,
    uT("Высота клиентской области окна программы (только для Windows)."));
    
  return pSettings;
}
  
} // namespace std
  
} // namespace extension
  
} // namespace alicorn
