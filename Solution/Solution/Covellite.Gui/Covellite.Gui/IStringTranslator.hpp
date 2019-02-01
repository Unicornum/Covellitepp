
#pragma once

namespace covellite
{

namespace gui
{

/**
* \ingroup CovelliteGuiGroup
* \brief
*  ����� ������ � ������ \ref CovelliteGuiPage \n
*  ������������ ����� ��������������� ����� libRocket.
*  
* \version
*  1.0.0.0        \n
* \date
*  21 ������� 2016    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2016
*/
class IStringTranslator
{
protected:
  using Utf8String_t = ::std::string;

public:
  /// ������� �������������� '�����' ������ (����������� �� .rml �����)
  /// � ������, ������� ����� ������������ �� ������ (�������������� 
  /// �������������, � ������ �������, ��� ����������� ����������).
  virtual Utf8String_t Translate(const Utf8String_t &) = 0;

public:
  virtual ~IStringTranslator(void) = default;
};

} // namespace gui

} // namespace covellite
