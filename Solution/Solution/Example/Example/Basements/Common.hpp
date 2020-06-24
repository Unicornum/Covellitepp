
#pragma once
#include <vector>
#include <alicorn/std/string.forward.hpp>
#include <alicorn/boost/filesystem.forward.hpp>
#include <Covellite/Api/Renders.hpp>
#include "IBasement.hpp"

namespace basement
{

/**
* \ingroup ExampleBasementGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ����� ����� ������ ��������� 3D �����.
*
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
* \date
*  29 ������ 2019    \n
*  22 ���� 2020    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019 - 2020
*/
class Common :
  public IBasement
{
protected:
  using String_t = ::alicorn::extension::std::String;
  using Path_t = ::boost::filesystem::path;
  using Component_t = ::covellite::api::Component;
  using Updater_t = ::covellite::api::Updater_t;
  using WindowExpanse_t = ::covellite::expanse::IWindow;
  using ObjectId_t = ::covellite::expanse::ObjectId_t;
  using GameObject_t = ::covellite::expanse::GameObject_t;

protected:
  class Id final
  {
  public:
    String_t GetStringId(void) const;
    bool operator< (const Id &) const;

  private:
    static size_t GetUniqueId(void);

  private:
    size_t m_Id;

  public:
    Id(void);
  };

protected:
  static GameObject_t LoadTexture(const Path_t &, const String_t &, 
    const String_t & = uT("albedo"));
  ObjectId_t CreateObject(const GameObject_t &);
  void AddToRenderQueue(const ObjectId_t, const size_t = 0);

private:
  WindowExpanse_t * m_pWindowExpanse = nullptr;
  ::std::vector<ObjectId_t> m_AllObjects;

protected:
  Common(WindowExpanse_t &);
  ~Common(void);
};

} // namespace basement
