
#pragma once
#include <alicorn/std/string.forward.hpp>
#include <alicorn/boost/filesystem.hpp>
#include <Covellite/Covellite.hpp>
#include <Covellite/Api/Renders.hpp>

/**
* \ingroup ExampleGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Простой пример класса промежуточного кода.
*  
* \version
*  1.0.0.0        \n
* \date
*  04 Ноябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017
*/
class ExtraWindow final :
  public ::covellite::app::IWindow
{
  using Events_t = ::covellite::events::Events;
  using WindowApi_t = ::covellite::api::IWindow;
  using String_t = ::alicorn::extension::std::String;
  using Path_t = ::boost::filesystem::path;
  using Component_t = ::covellite::api::Component;
  using Renders_t = Component_t::Renders::Renders_t;

public:
  operator const WindowApi_t & (void) const;

private:
  void LoadTexture(const Path_t &, const String_t &);
  size_t BuildLights(void);
  size_t BuildCamera(const Component_t::ComponentPtr_t &);
  size_t BuildCube(const Component_t::ComponentPtr_t &, float, float);
  size_t BuildPlane(uint32_t, float, float, float);

private:
  const WindowApi_t & m_WindowApi;
  Events_t            m_Events;
  ::std::shared_ptr<Component_t::Renders> m_pRenders;
  ::std::unordered_map<size_t, Renders_t> m_Objects;

public:
  explicit ExtraWindow(const WindowApi_t &);
};
