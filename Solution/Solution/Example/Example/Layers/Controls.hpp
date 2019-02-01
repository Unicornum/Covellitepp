
#pragma once
#include <alicorn/std/string.forward.hpp>
#include "Layer.hpp"

namespace layers
{

/**
* \ingroup ExampleGroup
* \brief
*   ласс входит в проект \ref ExamplePage \n
*  ќкно примера работы с элементами управлени€.
*  
* \version
*  1.0.0.0        \n
* \date
*  21 январь 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Controls final :
  public Layer
{
  using String_t = ::alicorn::extension::std::String;

private:
  void AddPart(const String_t &, const String_t &);
  void AddParameter(const String_t &, const String_t &, const String_t &,
    const String_t &, const String_t &);
  static String_t BuildButton(const String_t &, const String_t &, bool);

private:
  int m_Index = 0;
  bool m_IsSoundEnabled = false;
  int m_Age = 20;

public:
  explicit Controls(IWindowGui_t &);
};

} // namespace layers
