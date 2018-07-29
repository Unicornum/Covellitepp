
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
    // Вычисление значения идентификатора для события любого типа.
    return ::std::hash<T>()(_Value); 
  }
};

template<class T>
class Id<T, true>
{
public:
  inline size_t operator() (const T & _Value)
  {
    // Отдельная реализация вычисления значения идентификатора для enum 
    // потребовалась из-за того, что в Android версии компиляция вычисления
    // хеша для enum завершается с ошибкой.
    return _Value;
  }
};

/**
* \brief
*  Оператор получения объекта требуемого сигнала.
*
* \param [in] _Id
*  Объект идентификатора сигнала (может быть объектом любого класса, для
*  которого определена специализация std::hash<>: число, перечисление,
*  std::string и т.д.).
*/
template<class TType>
inline auto Events::operator[] (const TType & _Id) -> Signal
{
  return Signal{ *this, typeid(TType).hash_code(), 
    Id<TType, ::std::is_enum<TType>::value>{}(_Id)};
}

} // namespace events

} // namespace covellite
