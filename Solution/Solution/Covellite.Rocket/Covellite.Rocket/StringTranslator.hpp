
#pragma once
#include <Covellite\Rocket\IStringTranslator.hpp>
#include <map>

namespace covellite
{

namespace rocket
{

/**
* \ingroup gCovelliteRocketClasses
* \brief
*  Класс входит в проект \ref CovelliteRocketPage \n
*  Класс преобразователя строк.
* \details
*  - Предназначен для преобразования строк из .rml файла в строки, отображаемые
*  на экране (используется - в первую очередь - для локализации программы).
*  - При создании объекта covellite::rocket::Window ему следует передать
*  набор строк, который будет использоваться для преобразования.
*  
* \version
*  1.0.0.0        \n
* \date
*  06 Октябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2017
*/
class StringTranslator final :
  public IStringTranslator
{
  using Bank_t = ::std::map<Utf8String_t, Utf8String_t>;

public:
  void Set(const Bank_t &);

public:
  // Интерфейс IStringTranslator:
  Utf8String_t Translate(const Utf8String_t &) override;

private:
  Bank_t m_Strings;
};

} // namespace rocket

} // namespace covellite
