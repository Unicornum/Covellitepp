
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
  Params_t Params;

  for (const auto & Value : _Params)
  {
    const auto hType = Value.second.type().hash_code();

    Params[GetHash(Value.first)] = { hType, Value.second };
  }

  using Pool_t = ::alicorn::extension::std::pool<>;

  return Pool_t::make_unique<Component>(Params, ConstructorTag{});
}

/**
* \brief
*  ������� ��������� ���� ��� ����� ���������.
*  
* \param [in] _Value
*  �������� ��������.
*  
* \return \b Value
*  ��� �������� ��������.
*/
inline /*static*/ size_t Component::GetHash(const Name_t & _Value)
{
  static const Hasher_t Hasher;
  return Hasher(_Value);
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
  auto itValue = m_Params.find(GetHash(_Name));
  if (itValue == m_Params.end()) return false;

  static const auto hType = typeid(T).hash_code();
  return (itValue->second.hType == hType);
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
inline T Component::GetValue(const Name_t & _Name, const T & _DefaultValue) const
{
  return GetValue<T>(GetHash(_Name), _DefaultValue);
}

template<typename S, typename T>
class is_streamable
{
  template<typename SS, typename TT>
  static auto test(int) -> decltype(
    ::std::declval<SS&>() << ::std::declval<TT>(), ::std::true_type());

  template<typename, typename>
  static auto test(...) -> ::std::false_type;

public:
  static const bool value = decltype(test<S, T>(0))::value;
};

template<bool>
class Component::Convertor
{
public:
  template<class T>
  static T To(const String_t &)
  {
    // ������� ������� �� ����������, ������������� ������������� ��-�� ����,
    // ��� ::boost::lexical_cast<T>() �� ������������� ��� ���������� �
    // �� streamable'��� �����.
    throw 0;
  }
};

template<>
class Component::Convertor<true>
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
T Component::GetValue(const size_t _Hash, const T & _DefaultValue) const
{
  auto itValue = m_Params.find(_Hash);
  if (itValue == m_Params.end()) return _DefaultValue;

  const auto & Data = itValue->second;

  static const auto hTypeString = typeid(String_t).hash_code();

  if (Data.hType == hTypeString)
  {
    constexpr auto IsConvertable = 
      !::std::is_pointer<T>::value &
      !::std::is_reference<T>::value & 
      is_streamable<::std::iostream, T>::value;

    return Convertor<IsConvertable>::template To<T>(
      ::covellite::any_cast<String_t>(Data.Value));
  }

  return ::covellite::any_cast<T>(Data.Value);
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
  SetValue(GetHash(_Name), _Value);
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
* \param [in] _hName
*  ��� ����� ���������.
* \param [in] _Value
*  ����� �������� ���������.
*/
template<class T>
inline void Component::SetValue(const size_t _hName, const T & _Value)
{
  static const auto hType = typeid(T).hash_code();

  auto & Data = m_Params[_hName];

  Data.hType = hType;
  Data.Value = _Value;
}

inline /*static*/ size_t Component::GetHashId(void)
{
  static const size_t Hash = GetHash(uT("id"));
  return Hash;
}

inline /*static*/ size_t Component::GetHashType(void)
{
  static const size_t Hash = GetHash(uT("type"));
  return Hash;
}

inline /*static*/ size_t Component::GetHashKind(void)
{
  static const size_t Hash = GetHash(uT("kind"));
  return Hash;
}

} // namespace api

} // namespace covellite
