
#pragma once
#include <Covellite\Rocket\File.hpp>
#include <Covellite\Rocket\System.hpp>

namespace std { template<class> class shared_ptr; }

namespace covellite
{

namespace rocket
{

class IStringTranslator;

/**
* \ingroup CovelliteRocketGroup
* \brief
*  Класс входит в проект \ref CovelliteRocketPage \n
*  Класс, инкапсулирующий инициализацию/разрушение библиотеки libRocket.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  1.2.0.0        \n
*  1.3.0.0        \n
* \date
*  19 Декабрь 2016    \n
*  26 Декабрь 2016    \n
*  29 Сентябрь 2017    \n
*  08 Октябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2017
*/
class Initializer final
{
  using RenderInterfacePtr_t = 
    ::std::shared_ptr<Rocket::Core::RenderInterface>;
  using StringTranslatorPtr_t = 
    ::std::shared_ptr<covellite::rocket::IStringTranslator>;
  class Helper;

public:
  class Data
  {
  public:
    RenderInterfacePtr_t  m_pRenderInterface;
    StringTranslatorPtr_t m_pStringTranslator;
  };

private:
  covellite::rocket::File   m_File;
  covellite::rocket::System m_System;
  RenderInterfacePtr_t      m_pRenderInterface;

public:
  explicit Initializer(const Data &);
  ~Initializer(void) noexcept;
};

} // namespace rocket

} // namespace covellite
