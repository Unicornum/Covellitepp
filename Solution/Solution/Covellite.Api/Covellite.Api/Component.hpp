
#pragma once
#include <unordered_map>
#include <alicorn/std/string.hpp>
#include <Covellite/Predefined.hpp>

namespace covellite
{

namespace api
{

/**
* \ingroup CovelliteApiGroup
* \brief
*   ласс входит в проект \ref CovelliteApiPage \n
*   ласс компонента.
* \details
* \ref CovelliteApiComponentSystemPage
*  
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
* \date
*  24 ќкт€брь 2018    \n
*  13 ‘евраль 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018 - 2019
*/
class Component final
{
  template<bool>
  class Convertor;
  class ConstructorTag {};

public:
  using String_t = ::alicorn::extension::std::String;
  using Name_t = String_t;
  using Hasher_t = ::std::hash<Name_t>;
  using Params_t = ::std::unordered_map<size_t, ::covellite::Any_t>;
  using SourceParams_t = ::std::map<Name_t, ::covellite::Any_t>;
  using Id_t = String_t;
  using Type_t = String_t;
  using Kind_t = String_t;
  using ComponentPtr_t = ::std::shared_ptr<Component>;

public:
  class Renders;

public:
  static ComponentPtr_t Make(const SourceParams_t &);

public:
  template<class T>
  bool IsType(const Name_t &) const;
  template<class T>
  T GetValue(const Name_t &, const T &) const;
  template<class T>
  T GetValue(size_t, const T &) const;
  template<class T>
  void SetValue(const Name_t &, const T &);

private:
  static size_t GetHashId(void);
  static size_t GetHashType(void);
  static size_t GetHashKind(void);

private:
  Params_t m_Params;
  Hasher_t m_Hasher;

public:
  const Id_t Id;
  const Type_t Type;
  const Kind_t Kind;

public:
  Component(const Params_t &, ConstructorTag);
};

} // namespace api

} // namespace covellite
