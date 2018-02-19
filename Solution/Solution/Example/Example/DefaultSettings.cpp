
#include "stdafx.h"
#include <Covellite\Core\Settings.hpp>
#include <alicorn\application\current-module.hpp>
  
using namespace covellite::core;
  
/*static*/ void Settings::SetDefaultValues(Section_t & _Covellitepp)
{
  using namespace ::alicorn::extension::std;
    
  const auto PathToRootDirectory =
    ::alicorn::system::application::CurrentModule::GetAppRootPath();
    
  _Covellitepp.SetDefault(uT("PathToFontsDirectory"), 
    PathToRootDirectory / "data" / "fonts",
    uT("Путь к папке расположения шрифтов, которые используются в .rcss файлах."));
    
  _Covellitepp[uT("Window")].SetDefault(uT("IsFullScreen"), false,
    uT("Полноэкранный/оконный режим работы программы (только для Windows)."));
  _Covellitepp[uT("Window")][uT("BackgroundColor")].SetDefault(uT("R"), 0,
    uT("Цвет фона окна программы по умолчанию: красная компонента [0...255]."));
  _Covellitepp[uT("Window")][uT("BackgroundColor")].SetDefault(uT("G"), 0,
    uT("Цвет фона окна программы по умолчанию: зеленая компонента [0...255]."));
  _Covellitepp[uT("Window")][uT("BackgroundColor")].SetDefault(uT("B"), 255,
    uT("Цвет фона окна программы по умолчанию: синяя компонента [0...255]."));
  _Covellitepp[uT("Window")][uT("BackgroundColor")].SetDefault(uT("A"), 255,
    uT("Цвет фона окна программы по умолчанию: прозрачность [0...255]."));
  _Covellitepp[uT("Window")][uT("Size")].SetDefault(uT("Width"), 480,
    uT("Ширина клиентской области окна программы (только для Windows)."));
  _Covellitepp[uT("Window")][uT("Size")].SetDefault(uT("Height"), 762,
    uT("Высота клиентской области окна программы (только для Windows)."));
}
