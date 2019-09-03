
#include "stdafx.h"
#include <Covellite/Api/Window.hpp>
#include <alicorn/std/string.hpp>
#include <alicorn/patterns/factory.hpp>

using namespace covellite::api;

/*static*/ auto Window::GetRenderers(void) -> ::std::vector<String_t>
{
  return
  {
    uT("OpenGLES3"),
    uT("OpenGLES2"),
    uT("OpenGLES")
  };
}

/*static*/ auto Window::MakeImpl(const String_t & _Api,
  const renderer::SettingsData & _Data) -> IGraphicApiPtr_t
{
  using namespace ::alicorn::modules::patterns;

  if (_Api != uT("Auto"))
  {
    return factory::make_unique<renderer::IGraphicApi>(_Api, _Data);
  }

  for (const auto & Name : GetRenderers())
  {
    try
    {
      return factory::make_unique<renderer::IGraphicApi>(Name, _Data);
    }
    catch (const ::std::exception & _Ex)
    {
      // Сюда попадаем, если указанного рендера нет или он недоступен 
      // на текущем устройстве.

      LOGGER(Warning) << uT("Create graphics API ") << Name << " error: "
        << _Ex.what();
    }
  }

  throw STD_EXCEPTION << "Unknown Api: " << _Api;
}
