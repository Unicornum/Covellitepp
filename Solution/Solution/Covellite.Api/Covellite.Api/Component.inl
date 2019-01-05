
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
    Params[::std::hash<Name_t>{}(Value.first)] = Value.second;
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
  Type(GetValue<Type_t>(uT("type"), uT("Unknown"))),
  Id(GetValue<Id_t>(uT("id"), uT("Unknown")))
{
  ::boost::ignore_unused(_Tag);
}

template<>
class Component::Convertor<true>
{
public:
  template<class T>
  inline static T To(const String_t &)
  {
    // ������� ������� �� ����������, ������������� ������������� ��-�� ����,
    // ��� ::boost::lexical_cast<T>() �� ������������� ��� ����������.
    return nullptr;
  }
};

template<>
class Component::Convertor<false>
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
    return Convertor<::std::is_pointer<T>::value>::template To<T>(
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

} // namespace api

} // namespace covellite
