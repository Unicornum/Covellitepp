
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
    // Функция никогда не вызывается, потребовалась исключительно из-за того,
    // что ::boost::lexical_cast<T>() не компилируется для указателей.
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
T Component::GetValue(const Name_t & _Name, const T & _DefaultValue) const
{
  return GetValue(m_Hasher(_Name), _DefaultValue);
}

/**
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
  m_Params[m_Hasher(_Name)] = _Value;
}

} // namespace api

} // namespace covellite
