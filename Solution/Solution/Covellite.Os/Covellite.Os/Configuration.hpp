
#pragma once
#include <alicorn\std\string.forward.hpp>

namespace covellite
{

namespace os
{

/**
* \ingroup CovelliteOsGroup
* \brief
*  ����� ������ � ������ \ref CovelliteOsPage \n
*  ����� ������� � ������� ���������� �������.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
* \date
*  27 ��� 2017    \n
*  24 ������� 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017
*/
class Configuration final
{
  using String_t = ::alicorn::extension::std::String;

public:
  /**
  * \brief
  *  ������� ��������� �������� ���������� �����.
  * \details
  *  - ���������� ������� - ��������� ������, ������� ����� ���������������
  *  ������������ ��� ���������� ����� �����, ����������� ��������������
  *  ������, ������������ ����������.
  *
  * \return "English"
  *  ��� ����������� �����.
  * \return "Russian"
  *  ��� �������� �����.
  * \return "Default"
  *  ���� ���������� ���� ������� �� �������.
  */
  String_t GetSystemLanguage(void) const;

private:
  String_t GetRawSystemLanguage(void) const;
};

} // namespace os

} // namespace covellite
