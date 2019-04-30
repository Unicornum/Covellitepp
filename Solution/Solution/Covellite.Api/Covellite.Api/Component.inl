
#pragma once
#include <Covellite/Api/Component.hpp>
#include <boost/core/ignore_unused.hpp>
#include <alicorn/std/memory.hpp>

namespace covellite
{

namespace api
{

/**
* \brief
*  ������� �������� �������� �����������.
* \details
*  - ������������ ��� ����, ����� ������������� �������� ������������ � ����.
*  
* \param [in] _Params
*  �������� ������ ���������� � ���� ������ ��� ��� - �������� (�������� �����
*  ���� ������ ����).
*
* \return
*  ��������� ������ ����������.
*  
* \exception std::exception
*  - �������� ���������� (��������� ��. �������� ����������).
*/
inline /*static*/ auto Component::Make(const SourceParams_t & _Params) -> ComponentPtr_t
{
  using Pool_t = ::alicorn::extension::std::pool<>;

  Params_t Params;

  for (const auto & Value : _Params)
  {
    Params[Hasher_t{}(Value.first)] = Value.second;
  }

  return Pool_t::make_unique<Component>(Params, ConstructorTag{});
}

/**
* \brief
*  ����������� ������.
*  
* \param [in] _Params
*  �������� ������ ���������� � ���� ������ ��� ��� - �������� (�������� �����
*  ���� ������ ����).
* \param [in] _Tag
*  �� ������������. ������������ ��� ����, ����� ��������� �������� �������� 
*  ���� ������� Make(); �������� ����������� ������������ ������, �.�.
*  � ���� ������ ������� make_unique<>() ����� �� ������ ��������� �������
*  ������.
*/
inline Component::Component(const Params_t & _Params, ConstructorTag _Tag) :
  m_Params(_Params),
  Id(GetValue(GetHashId(), uT("Unknown"))),
  Type(GetValue(GetHashType(), uT("Unknown"))),
  Kind(GetValue(GetHashKind(), uT("Unknown")))
{
  ::boost::ignore_unused(_Tag);
}

/**
* \brief
*  ������� �������� ���������� ���������� ���� � ����� �������� � ��������� 
*  ������.
* \details
*  - ������� �� ��������� ����������� ����������� ��������� � ��������� ���.
*  
* \param [in] _Name
*  ��������� ��� ������������ ���������.
*
* \return \b false
*  - ��� ��������� �� ��������� � ���������.
*  - � ���������� ��� ��������� � ��������� ������.
*/
template<class T>
inline bool Component::IsType(const Name_t & _Name) const
{
  auto itValue = m_Params.find(m_Hasher(_Name));
  if (itValue == m_Params.end()) return false;

  return (itValue->second.type() == typeid(T));
}

/**
* \brief
*  ������� ��������� �������� ���������.
* \details
*  - ������� ������������� ��� ��������� �������� ��������� ���������� ���� 
*  � ��������� ������.
*  - ���� ��������� � ��������� ������ �� ����������, ������� ������ ��������� 
*  �������� �� ���������.
*  - ���� �������� � ��������� ������ ������ ��������� ��������, ��� ����� 
*  ������������� � �������� ���������� ����.
*  
* \param [in] _Name
*  ��������� ��� ���������.
* \param [in] _DefaultValue
*  �������� �� ���������.
*  
* \return \b Value
*  �������� ��������� ���������� ����.
*  
* \exception std::exception
*  - �������� � ��������� ������ �������� ��������, ��� �������� �� ���������
*  � ���������.
*  - �������� � ��������� ������ �������� ��������� ��������, ������� �� 
*  ����� ���� ������������� � ��������� ���.
*/
template<class T>
T Component::GetValue(const Name_t & _Name, const T & _DefaultValue) const
{
  return GetValue(m_Hasher(_Name), _DefaultValue);
}

template<bool, bool>
class Component::Convertor
{
public:
  template<class T>
  inline static T To(const String_t &)
  {
    // ������� ������� �� ����������, ������������� ������������� ��-�� ����,
    // ��� ::boost::lexical_cast<T>() �� ������������� ��� ����������
    // � ���������.
    return nullptr;
  }
};

template<>
class Component::Convertor<false, false>
{
public:
  template<class T>
  inline static T To(const String_t & _Value)
  {
    return ::boost::lexical_cast<T>(_Value);
  }
};

/**
* \brief
*  ������� ��������� �������� ���������.
* \details
*  - ������� ������������� ��� �������� ��������� �������� ��������� ���������� 
*  ���� � ��������� ����� ����� (��� ��������� ��� �������� ������� ��������,
*  ��� ����� ��������� ������� - ���� ��� - � ������������ � ����������).
*  - ���� ��������� � ��������� ����� �� ����������, ������� ������ ���������
*  �������� �� ���������.
*  - ���� �������� � ��������� ����� ������ ��������� ��������, ��� �����
*  ������������� � �������� ���������� ����.
*
* \param [in] _Hash
*  ��� ����� ���������.
* \param [in] _DefaultValue
*  �������� �� ���������.
*
* \return \b Value
*  �������� ��������� ���������� ����.
*
* \exception std::exception
*  - �������� � ��������� ������ �������� ��������, ��� �������� �� ���������
*  � ���������.
*  - �������� � ��������� ������ �������� ��������� ��������, ������� ��
*  ����� ���� ������������� � ��������� ���.
*/
template<class T>
T Component::GetValue(size_t _Hash, const T & _DefaultValue) const
{
  auto itValue = m_Params.find(_Hash);
  if (itValue == m_Params.end()) return _DefaultValue;

  if (itValue->second.type() == typeid(String_t))
  {
    using Updater_t = ::std::function<void(const float)>;

    return Convertor<::std::is_pointer<T>::value, 
      ::std::is_same<T, Updater_t>::value>::template To<T>(
      ::covellite::any_cast<String_t>(itValue->second));
  }

  return ::covellite::any_cast<T>(itValue->second);
}

/**
* \brief
*  ������� ��������� �������� ���������.
* \details
*  - ������  � ���� �� ��������� ����� ������������� �������� ������ �����,
*  �� ��� ���� ������� ���������, ��� ������� ��������� �������� ���������
*  ����������� � ��������� ��� ������ ��������� ��������, ��� ���������
*  ���� �� �������������.
*  
* \param [in] _Name
*  ��� ���������.
* \param [in] _Value
*  ����� �������� ���������.
*/
template<class T>
inline void Component::SetValue(const Name_t & _Name, const T & _Value)
{
  m_Params[m_Hasher(_Name)] = _Value;
}

inline /*static*/ size_t Component::GetHashId(void)
{
  static const size_t Hash = Hasher_t{}(uT("id"));
  return Hash;
}

inline /*static*/ size_t Component::GetHashType(void)
{
  static const size_t Hash = Hasher_t{}(uT("type"));
  return Hash;
}

inline /*static*/ size_t Component::GetHashKind(void)
{
  static const size_t Hash = Hasher_t{}(uT("kind"));
  return Hash;
}

} // namespace api

} // namespace covellite
