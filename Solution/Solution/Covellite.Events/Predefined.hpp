
#pragma once

/**
* \file
*  ���� �����������, ������� ����� �������������� �� ���� �������� ����������.
*  
* \todo
*  �������� boost::any_cast �� std::any_cast ����� ��� ����� �������� 
*  (��� Android - ������� �� ����� ������ Android NDK, � Windows - �������� 
*  � ���������� ������� ��������� ������ ���������).
*/

#include <boost/any.hpp>
#include <Covellite/Predefined.forward.hpp>

namespace covellite
{

template<class T>
inline T any_cast(const Any_t & _Value)
{
  return ::boost::any_cast<T>(_Value);
}

template<class T>
inline T any_cast(Any_t & _Value)
{
  return ::boost::any_cast<T>(_Value);
}

} // namespace covellite
