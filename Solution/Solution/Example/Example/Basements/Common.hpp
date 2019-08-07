
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
*   ласс входит в проект \ref ExamplePage \n
*   ласс общей логики отрисовки 3D сцены.
*
* \version
*  1.0.0.0        \n
* \date
*  29 январь 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Common :
  public IBasement
{
protected:
  using String_t = ::alicorn::extension::std::String;
  using Path_t = ::boost::filesystem::path;
  using Component_t = ::covellite::api::Component;
  using Renders_t = Component_t::Renders::Renders_t;
  using RendersPtr_t = ::std::shared_ptr<Component_t::Renders>;

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

public:
  void Render(void) override;

protected:
  void LoadTexture(const Path_t &, const String_t &, const String_t & = uT("albedo"));

protected:
  const RendersPtr_t m_pRenders;
  ::std::map<Id, Renders_t> m_Objects;
  ::std::vector<Id> m_Scene;

protected:
  explicit Common(const RendersPtr_t &);
};

} // namespace basement
