
#include "stdafx.h"
#include <Covellite\Core\Settings.hpp>
#include <alicorn\application\current-module.hpp>
  
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
    
# if BOOST_OS_WINDOWS
    
  // Параметры, используемые только в Windows
  (*pSettings).AddExtra(uT("AppRootPath"),
    ::alicorn::system::application::CurrentModule::GetAppRootPath());
    
  (*pSettings).SetDefault(uT("PathToFontsDirectory"),
    uT("{AppRootPath}/data/fonts"),
    uT("Путь к папке расположения шрифтов, которые используются в .rcss файлах."));
    
  (*pSettings)[uT("Window")].SetDefault(uT("GraphicsApi"), uT("DirectX11"),
    uT("Используемый для рендеринга графический Api."));
  (*pSettings)[uT("Window")].SetDefault(uT("IsFullScreen"), false,
    uT("Полноэкранный/оконный режим работы программы."));
  (*pSettings)[uT("Window")].SetDefault(uT("IsResized"), false,
    uT("Разрешение/запрет изменения размеров окна программы мышью."));
    
  (*pSettings)[uT("Window")][uT("Size")].SetDefault(uT("Width"), 480,
    uT("Ширина клиентской области окна программы."));
  (*pSettings)[uT("Window")][uT("Size")].SetDefault(uT("Height"), 762,
    uT("Высота клиентской области окна программы."));
    
# elif BOOST_OS_ANDROID
    
  // Параметры, используемые только в Android
  (*pSettings).SetDefault(uT("PathToFontsDirectory"),
    uT("data/fonts"),
    uT("Путь к папке расположения шрифтов, которые используются в .rcss файлах."));
    
  (*pSettings)[uT("Window")].SetDefault(uT("GraphicsApi"), uT("OpenGLES"),
    uT("Используемый для рендеринга графический Api."));
    
# endif
    
  // Общие параметры для всех платформ
  (*pSettings)[uT("Window")][uT("BackgroundColor")].SetDefault(uT("R"), 0,
    uT("Цвет фона окна программы по умолчанию: красная компонента [0...255]."));
  (*pSettings)[uT("Window")][uT("BackgroundColor")].SetDefault(uT("G"), 0,
    uT("Цвет фона окна программы по умолчанию: зеленая компонента [0...255]."));
  (*pSettings)[uT("Window")][uT("BackgroundColor")].SetDefault(uT("B"), 255,
    uT("Цвет фона окна программы по умолчанию: синяя компонента [0...255]."));
  (*pSettings)[uT("Window")][uT("BackgroundColor")].SetDefault(uT("A"), 255,
    uT("Цвет фона окна программы по умолчанию: прозрачность [0...255]."));
    
  return pSettings;
}
  
} // namespace std
  
} // namespace extension
  
} // namespace alicorn
