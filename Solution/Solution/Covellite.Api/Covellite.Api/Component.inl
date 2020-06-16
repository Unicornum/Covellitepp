
#pragma once
#include <Covellite/Api/Component.hpp>
#include <boost/core/ignore_unused.hpp>
#include <alicorn/std.memory.hpp>

namespace covellite
{

namespace api
{

/**
* \brief
*  Функция создания объектов компонентов.
* \details
*  - Понадобилась для того, чтобы гарантировать создание компонентовв в пуле.
*  
* \param [in] _Params
*  Исходные данные компонента в виде набора пар имя - значение (значения могут
*  быть любого типа).
*
* \return
*  Созданный объект компонента.
*  
* \exception std::exception
*  - Действие невозможно (подробнее см. описание исключения).
*/
inline /*static*/ auto Component::Make(const SourceParams_t & _Params) -> ComponentPtr_t
{
  Params_t Params;

  for (const auto & Param : _Params)
  {
    auto & Value = Params[GetHash(Param.first)];
    Value.hType = Param.second.type().hash_code();
    Value.Value = Param.second;
  }

  using Pool_t = ::alicorn::extension::std::pool<>;

  return Pool_t::make_unique<Component>(Params, ConstructorTag{});
}

/**
* \brief
*  Функция получения хеша для имени параметра.
*  
* \param [in] _Value
*  Исходное значение.
*  
* \return \b Value
*  Хеш входного значения.
*/
inline /*static*/ size_t Component::GetHash(const Name_t & _Value)
{
  static const Hasher_t Hasher;
  return Hasher(_Value);
}

/**
* \brief
*  Конструктор класса.
*  
* \param [in] _Params
*  Исходные данные компонента в виде набора пар имя - значение (значения могут
*  быть любого типа).
* \param [in] _Tag
*  Не используется. Предназначен для того, чтобы запретить создание объектов 
*  мимо функции Make(); закрытый конструктор использовать нельзя, т.к.
*  в этом случае функция make_unique<>() также не сможет создавать объекты
*  класса.
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
*  Функция проверки совпадения типа значения параметра с указанным типом.
* \details
*  - Функция не проверяет возможность конвертации параметра в указанный тип.
*
* \return \b false
*  - Тип параметра не совпадает с указанным.
*/
template<class T>
inline bool Component::Param::IsType(void) const
{
  static const auto hType = typeid(T).hash_code();
  return (this->hType == hType);
}

/**
* \brief
*  Функция задания значения параметра по умолчанию.
* \details
*  - Если при обращении к значению окажется, что оно не было установлено,
*  оно будет создано и ему будет присвоено значение, указанное в качестве
*  параметра этой функции.
*/
template<class T>
inline Component::Param & Component::Param::Default(const T & _Value)
{
  if (!::covellite::has_value(this->Value)) Set(_Value);
  return *this;
}

template<typename S, typename T>
class is_streamable
{
  template<typename SS, typename TT>
  static auto test(int) -> decltype(
    ::std::declval<SS &>() << ::std::declval<TT>(), ::std::true_type());

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
    // Функция никогда не вызывается, потребовалась исключительно из-за того,
    // что ::boost::lexical_cast<T>() не компилируется для указателей и
    // не streamable'ных типов.
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
*  Оператор получения значения параметра.
* \details
*  - Оператор предназначен для автоматической конвертации значения параметра
*  в тип переменной, которой он присваивается.
*  - Если параметр хранит строковое значение, и оно может быть преобразовано
*  в указанный тип, оно будет преобразовано в значение указанного типа.
*
* \exception std::exception
*  - Параметр с указанным именем содержит значение, тип которого не совпадает
*  с указанным.
*  - Параметр с указанным именем содержит строковое значение, которое не
*  может быть преобразовано в указанный тип.
*/
template<class T>
Component::Param::operator T & (void)
{
  try
  {
    return ::covellite::any_cast<T &>(this->Value);
  }
  catch (const ::std::exception &)
  {
    // значение еще не было присвоено или параметр содержит значение другого типа
  }

  if (IsType<String_t>())
  {
    using Type_t = typename ::std::remove_const<T>::type;

    constexpr auto IsConvertable =
      !::std::is_pointer<T>::value &&
      !::std::is_reference<T>::value &&
      is_streamable<::std::iostream, Type_t>::value;

    Set(Convertor<IsConvertable>::template To<Type_t>(
      ::covellite::any_cast<const String_t &>(this->Value)));
  }

  return ::covellite::any_cast<T &>(this->Value);
}

template<class T>
Component::Param::operator const T & (void) const
{
  return ::covellite::any_cast<const T &>(this->Value);
}

/**
* \brief
*  Оператор изменения значения параметра.
* \details
*  - Одному  и тому же параметру можно устанавливать значения разных типов,
*  но при этом следует учитывать, что функция получения значения параметра
*  преобразует в указанный тип только строковые значения, все остальные
*  типы не преобразуются.
*
* \param [in] _Value
*  Новое значение параметра.
*/
template<class T>
Component::Param & Component::Param::operator= (const T & _Value)
{
  try
  {
    ::covellite::any_cast<T &>(this->Value) = _Value;
    return *this;
  }
  catch (const ::std::exception &)
  {
    // значение еще не было присвоено или параметр содержит значение другого типа
  }

  Set(_Value);
  return *this;
}

template<class T>
inline void Component::Param::Set(const T & _Value)
{
  static const auto hType = typeid(T).hash_code();

  this->Value = _Value;
  this->hType = hType;
}

/**
* \brief
*  Оператор получения временной переменной для доступа к параметру компонента.
*
* \param [in] _Name
*  Строковое имя параметра.
*/
inline Component::Param & Component::operator[] (const Name_t & _Name)
{
  return (*this)[GetHash(_Name)];
}

/**
* \brief
*  Оператор получения временной переменной для доступа к параметру компонента.
*
* \param [in] _Hash
*  Хеш имени параметра.
*/
inline Component::Param & Component::operator[] (const size_t & _Hash)
{
  return m_Params[_Hash];
}

/**
* \brief
*  Оператор получения временной переменной для доступа к параметру компонента.
*
* \param [in] _Name
*  Строковое имя параметра.
*  
* \exception std::exception
*  - Параметр с указанным именем не существует.
*/
inline const Component::Param & Component::operator[] (const Name_t & _Name) const
{
  return (*this)[GetHash(_Name)];
}

/**
* \brief
*  Оператор получения временной переменной для доступа к параметру компонента.
*
* \param [in] _Hash
*  Хеш имени параметра.
*
* \exception std::exception
*  - Параметр с указанным хешем не существует.
*/
inline const Component::Param & Component::operator[] (const size_t & _Hash) const
{
  const auto itParam = m_Params.find(_Hash);
  if (itParam == ::std::end(m_Params))
  {
    throw STD_EXCEPTION << "Unknown parameter: " << _Hash;
  }

  return itParam->second;
}

/**
* \deprecated
*  Функция устарела и будет удалена в следующей стабильной версии, вместо
*  нее следует использовать функцию IsType() через operator[].
* \brief
*  Функция проверки совпадения указанного типа с типом значения с указанным
*  именем.
* \details
*  - Функция не проверяет возможность конвертации параметра в указанный тип.
*
* \param [in] _Name
*  Строковое имя проверяемого параметра.
*
* \return \b false
*  - Тип параметра не совпадает с указанным.
*  - У компонента нет параметра с указанным именем.
*/
template<class T>
inline bool Component::IsType(const Name_t & _Name) const
{
  auto itValue = m_Params.find(GetHash(_Name));
  return (itValue != ::std::end(m_Params)) ? itValue->second.IsType<T>() : false;
}

/**
* \deprecated
*  Функция устарела и будет удалена в следующей стабильной версии, вместо
*  нее следует использовать operator[].
* \brief
*  Функция получения значения параметра.
* \details
*  - Функция предназначена для получения значения параметра указанного типа 
*  с указанным именем.
*  - Если параметра с указанным именем не существует, функция вернет указанное 
*  значение по умолчанию.
*  - Если параметр с указанным именем хранит строковое значение, оно будет 
*  преобразовано в значение указанного типа.
*  
* \param [in] _Name
*  Строковое имя параметра.
* \param [in] _DefaultValue
*  Значение по умолчанию.
*  
* \return \b Value
*  Значение параметра указанного типа.
*  
* \exception std::exception
*  - Параметр с указанным именем содержит значение, тип которого не совпадает
*  с указанным.
*  - Параметр с указанным именем содержит строковое значение, которое не 
*  может быть преобразовано в указанный тип.
*/
template<class T>
inline T Component::GetValue(const Name_t & _Name, const T & _DefaultValue) const
{
  return GetValue<T>(GetHash(_Name), _DefaultValue);
}

/**
* \deprecated
*  Функция устарела и будет удалена в следующей стабильной версии, вместо
*  нее следует использовать operator[].
* \brief
*  Функция получения значения параметра.
* \details
*  - Функция предназначена для быстрого получения значения параметра указанного 
*  типа с указанным хэшем имени (для параметра имя которого заранее известно,
*  хеш можно вычислить заранее - один раз - и использовать в дальнейшем).
*  - Если параметра с указанным хэшем не существует, функция вернет указанное
*  значение по умолчанию.
*  - Если параметр с указанным хэшем хранит строковое значение, оно будет
*  преобразовано в значение указанного типа.
*
* \param [in] _Hash
*  Хэш имени параметра.
* \param [in] _DefaultValue
*  Значение по умолчанию.
*
* \return \b Value
*  Значение параметра указанного типа.
*
* \exception std::exception
*  - Параметр с указанным именем содержит значение, тип которого не совпадает
*  с указанным.
*  - Параметр с указанным именем содержит строковое значение, которое не
*  может быть преобразовано в указанный тип.
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
      !::std::is_pointer<T>::value &&
      !::std::is_reference<T>::value &&
      is_streamable<::std::iostream, T>::value;

    return Convertor<IsConvertable>::template To<T>(
      ::covellite::any_cast<const String_t &>(Data.Value));
  }

  return ::covellite::any_cast<const T &>(Data.Value);
}

/**
* \deprecated
*  Функция устарела и будет удалена в следующей стабильной версии, вместо
*  нее следует использовать operator[].
* \brief
*  Функция установки значения параметра.
* \details
*  - Одному  и тому же параметру можно устанавливать значения разных типов,
*  но при этом следует учитывать, что функция получения значения параметра
*  преобразует в указанный тип только строковые значения, все остальные
*  типы не преобразуются.
*  
* \param [in] _Name
*  Имя параметра.
* \param [in] _Value
*  Новое значение параметра.
*/
template<class T>
inline void Component::SetValue(const Name_t & _Name, const T & _Value)
{
  SetValue(GetHash(_Name), _Value);
}

/**
* \deprecated
*  Функция устарела и будет удалена в следующей стабильной версии, вместо
*  нее следует использовать operator[].
* \brief
*  Функция установки значения параметра.
* \details
*  - Одному  и тому же параметру можно устанавливать значения разных типов,
*  но при этом следует учитывать, что функция получения значения параметра
*  преобразует в указанный тип только строковые значения, все остальные
*  типы не преобразуются.
*
* \param [in] _hName
*  Хэш имени параметра.
* \param [in] _Value
*  Новое значение параметра.
*/
template<class T>
inline void Component::SetValue(const size_t _hName, const T & _Value)
{
  m_Params[_hName] = _Value;
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
