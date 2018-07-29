
#pragma once
#include <Covellite/Events/Signal.hpp>

namespace covellite
{

namespace events
{

template<class T, bool>
class Id
{
public:
  inline size_t operator() (const T & _Value) const 
  {
    // ���������� �������� �������������� ��� ������� ������ ����.
    return ::std::hash<T>()(_Value); 
  }
};

template<class T>
class Id<T, true>
{
public:
  inline size_t operator() (const T & _Value)
  {
    // ��������� ���������� ���������� �������� �������������� ��� enum 
    // ������������� ��-�� ����, ��� � Android ������ ���������� ����������
    // ���� ��� enum ����������� � �������.
    return _Value;
  }
};

/**
* \brief
*  �������� ��������� ������� ���������� �������.
*
* \param [in] _Id
*  ������ �������������� ������� (����� ���� �������� ������ ������, ���
*  �������� ���������� ������������� std::hash<>: �����, ������������,
*  std::string � �.�.).
*/
template<class TType>
inline auto Events::operator[] (const TType & _Id) -> Signal
{
  return Signal{ *this, typeid(TType).hash_code(), 
    Id<TType, ::std::is_enum<TType>::value>{}(_Id)};
}

} // namespace events

} // namespace covellite
