
#pragma once
#include <Covellite/Api/Component.hpp>

namespace covellite
{

namespace api
{

/**
* \ingroup CovelliteApiGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Класс создания рендеров компонентов.
*  
* \version
*  1.0.0.0        \n
* \date
*  03 Ноябрь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class Component::Renders final
{
public:
  using Render_t = ::std::function<void(void)>;
  using Renders_t = ::std::vector<Render_t>;
  using Object_t = ::std::vector<ComponentPtr_t>;
  using Creator_t = ::std::function<Render_t(const ComponentPtr_t &)>;
  using Creators_t = ::std::map<Type_t, Creator_t>;

public:
  Renders_t Obtain(const Object_t &);
  void Remove(const Object_t &);

private:
  Render_t Create(const ComponentPtr_t &);

private:
  const Creators_t m_Creators;
  ::std::map<Id_t, Render_t> m_AllExistingRenders;

public:
  explicit Renders(const Creators_t &);
};

} // namespace api

} // namespace covellite
