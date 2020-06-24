
#pragma once
#include <Covellite/Events/Events.hpp>
#include <Covellite/App/IWindow.hpp>
#include <Covellite/Api/IWindow.hpp>
#include <Covellite/Expanse/Defines.hpp>
#include <Covellite/Expanse/IWindow.hpp>

namespace covellite
{

namespace expanse
{

/**
* \ingroup CovelliteExpanseGroup
* \brief
*  ����� ������ � ������ \ref CovelliteExpansePage \n
*  ����� ����, ��������������� ����������� ���� ����������� �������.
* \details
*  - ��������� �������� ������.
*
* \version
*  1.0.0.0        \n
* \date
*  18 ���� 2020    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2020
*/
class Window final :
  public ::covellite::expanse::IWindow,
  public ::covellite::app::IWindow
{
  using WindowApi_t = ::covellite::api::IWindow;

public:
  // ��������� events::IEvents:
  operator Events_t (void) const noexcept override;

public:
  // ��������� IGameScene:
  ObjectId_t CreateObject(const GameObject_t &) override;
  void RemoveObject(const ObjectId_t) override;

public:
  // ��������� I3DScene:
  void Add(const size_t, const ObjectId_t, const size_t = 0) override;

private:
  class Scene;
  Events_t                 m_Events;
  ::std::unique_ptr<Scene> m_pScene;

public:
  explicit Window(const WindowApi_t &);
  ~Window(void);
};

} // namespace expanse

} // namespace covellite
