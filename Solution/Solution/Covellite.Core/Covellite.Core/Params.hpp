
#pragma once
#include <boost/any.hpp>

/**
* \defgroup gCovelliteCoreParamsClasses Params
* \ingroup gCovelliteCoreClasses
*  ������ ������� ����������, ������������ ������������ �������.
*/

namespace covellite
{

namespace core
{

namespace params
{

/**
* \ingroup gCovelliteCoreParamsClasses
* \brief
*  ����� ������ � ������ \ref CovelliteCorePage \n
*  �����-�������� ��� ������������� ��������� � ��������� ��� ����������.
* \details
*  - ������������ ������ ��� ������ ������������ ������� ��� ����������,
*  � �������� �������-����������� ������������ ������� ��� ����������.
*
* \version
*  1.0.0.0        \n
* \date
*  23 �������� 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017
*/
class Empty final
{
};

/**
* \ingroup gCovelliteCoreParamsClasses
* \brief
*  ����� ������ � ������ \ref CovelliteCorePage \n
*  ���������, ������������ ������� �������� ������ �� ������.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
* \date
*  23 �������� 2017    \n
*  24 ������� 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017
*/
class Motion final
{
public:
  int32_t X;      ///< X ���������� ������� � ��������.
  int32_t Y;      ///< Y ���������� ������� � ��������.
};

/**
* \ingroup gCovelliteCoreParamsClasses
* \brief
*  ����� ������ � ������ \ref CovelliteCorePage \n
*  ���������, ������������ ������� ������� ������ ����������.
*
* \version
*  1.0.0.0        \n
* \date
*  23 �������� 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017
*/
class KeyPressed final
{
public:
  int32_t UnicodeKeyCode;
};

/**
* \ingroup gCovelliteCoreParamsClasses
* \brief
*  ����� ������ � ������ \ref CovelliteCorePage \n
*  ���������, ������������ �������� �������/���������� ������ ����������.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  1.2.0.0        \n
* \date
*  23 �������� 2017    \n
*  01 ������ 2017    \n
*  04 ������ 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017
*/
class KeyCode final
{
  using RocketKeyCode_t = Rocket::Core::Input::KeyIdentifier;

public:
  int32_t AsciiKeyCode; ///< ����������� ��� ������� ������� ����������
                        ///< ������������ �������.

public:
  RocketKeyCode_t ToRocketKeyCode(void) const;
};

/**
* \ingroup gCovelliteCoreParamsClasses
* \brief
*  ����� ������ � ������ \ref CovelliteCorePage \n
*  ���������, ������������ ������� ������.
*
* \version
*  1.0.0.0        \n
* \date
*  23 �������� 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017
*/
class Error final
{
public:
  ::std::string Description;
};

/**
* \ingroup gCovelliteCoreParamsClasses
* \brief
*  ����� ������ � ������ \ref CovelliteCorePage \n
*  ����� ���������� �� ��������, ��������� ��������� �������.
* \details
*  - \b �����!!! ��� �������� ���������� � ������� ��������.
*  
* \version
*  1.0.0.0        \n
* \date
*  11 ������� 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017
*/
class Click final
{
public:
  ::std::string m_DocumentId; ///< ������������� ������������� ��������� (����)
                              ///< ��������, ��� �������� ���������� �������.  
  ::std::string m_Tag;        ///< �������� xml ���� ��������.
  ::std::string m_Type;       ///< ��� �������� (�������� �������� \b type).
};

} // namespace params

/**
* \ingroup gCovelliteCoreClasses
* \brief
*  ����� ������ � ������ \ref CovelliteCorePage \n
*  ��������������� ����� ��� �������� ���������� ������������ �������.
* \details
*  - ����� ��� ����, ����� � �������-������������ ������� � �������� ���������
*  ����� ���� ��������� ��������������� ���� �� �������
*  \ref gCovelliteCoreParamsClasses.
*
* \version
*  1.0.0.0        \n
*  1.0.1.0        \n
* \date
*  23 �������� 2017    \n
*  13 ������� 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017
*/
class Params final
{
  using Params_t = ::boost::any;

public:
  template<class T>
  inline operator const T & (void) const { return ::boost::any_cast<const T &>(m_Params); }

private:
  const Params_t m_Params;

public:
  template<class T>
  inline Params(const T & _Value) : m_Params(_Value) {}
};

} // namespace core

} // namespace covellite
