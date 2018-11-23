
#pragma once
#include <Covellite/Api/Renders.hpp>

namespace covellite
{

namespace api
{

/**
* \ingroup CovelliteApiGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ������������ �����, ��������������� ������ � ������� ����������.
*  
* \version
*  1.0.0.0        \n
* \date
*  05 ������ 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
class RenderInterface :
  public Rocket::Core::RenderInterface
{
protected:
  using RendersPtr_t = ::std::shared_ptr<Component::Renders>;

public:
  virtual RendersPtr_t GetRenders(void) const = 0;
};

} // namespace api

} // namespace covellite
