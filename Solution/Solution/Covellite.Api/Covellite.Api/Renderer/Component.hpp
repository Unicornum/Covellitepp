
#pragma once
#include <alicorn/std/string.hpp>
#include <Covellite/Api/Component.inl>

namespace covellite
{

namespace api
{

namespace renderer
{

/**
* \ingroup CovelliteApiRendererGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Вспомогательный класс для извлечения данных компонентов.
*  
* \version
*  1.0.0.0        \n
* \date
*  18 Ноябрь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class Component final
{
  using String_t = ::alicorn::extension::std::String;
  using ComponentPtr_t = ::std::shared_ptr<::covellite::api::Component>;

public:
  class Scissor;
  template<class>
  class Buffer;
  class Texture;
  class Shader;
  class Transform;
  class Position;
  class Rotation;
  class Scale;
};

class Component::Scissor
{
public:
  const bool IsEnabled;
  const int Left;
  const int Top;
  const int Right;
  const int Bottom;

public:
  explicit Scissor(const ComponentPtr_t & _pComponent) :
    IsEnabled(_pComponent->GetValue(uT("enabled"), false)),
    Left(_pComponent->GetValue(uT("left"), 0)),
    Top(_pComponent->GetValue(uT("top"), 0)),
    Right(_pComponent->GetValue(uT("right"), 0)),
    Bottom(_pComponent->GetValue(uT("bottom"), 0))
  {

  }
};

template<class T>
class Component::Buffer
{
private:
  ::std::vector<T> m_Data;

public:
  const T * const pData;
  const size_t Count;

public:
  explicit Buffer(const ComponentPtr_t & _pComponent) :
    pData(_pComponent->GetValue<const T *>(uT("data"), m_Data.data())),
    Count(_pComponent->GetValue(uT("count"), m_Data.size()))
  {
  }
  Buffer(const ComponentPtr_t & _pComponent, const ::std::vector<T> & _Data) :
    pData(_pComponent->GetValue(uT("data"), _Data.data())),
    Count(_pComponent->GetValue(uT("count"), _Data.size()))
  {
  }
};

class Component::Texture :
  public Buffer<uint8_t>
{
public:
  const int Width;
  const int Height;

public:
  explicit Texture(const ComponentPtr_t & _pComponent) :
    Buffer(_pComponent),
    Width(_pComponent->GetValue(uT("width"), 0)),
    Height(_pComponent->GetValue(uT("height"), 0))
  {

  }
};

class Component::Shader :
  public Buffer<uint8_t>
{
public:
  const ::std::string Version;
  const ::std::string Entry;
  const String_t Kind;

private:
  static String_t GetShaderType(const ::std::string & _Entry,
    const uint8_t * _pBegin, const uint8_t * _pEnd)
  {
    // Определение типа шейдера, функция вернет строку типа параметра 
    // функции точки входа шейдера.

    const auto * pLastBreak = _pBegin;

    while (true)
    {
      const auto * pBreak = ::std::find(pLastBreak, _pEnd, '\n');
      if (pBreak == _pEnd)
      {
        throw STD_EXCEPTION << "Entry point not found: " << _Entry;
      }

      const ::std::string Line{ pLastBreak, pBreak };
      const auto EntryPosition = Line.find(_Entry, 0);
      if (EntryPosition != ::std::string::npos)
      {
        const auto TypeBegin = EntryPosition + _Entry.length() + 1;
        const auto TypeEnd = Line.find(" ", TypeBegin);

        using namespace ::alicorn::extension::std;

        return string_cast<String, Locale::Ascii128>(
          Line.substr(TypeBegin, TypeEnd - TypeBegin));
      }

      pLastBreak = pBreak + 1;
    }
  }

public:
  Shader(const ComponentPtr_t & _pComponent, const ::std::vector<uint8_t> & _Data) :
    Buffer(_pComponent, _Data),
    Version(_pComponent->GetValue<::std::string>(uT("version"), "fx_unknown")),
    Entry(_pComponent->GetValue<::std::string>(uT("entry"), "Unknown")),
    Kind(GetShaderType(Entry, pData, pData + Count))
  {
  }
};

class Component::Transform
{
private:
  inline static size_t GetHashX(void)
  {
    static const size_t Hash = ::std::hash<String_t>{}(uT("x"));
    return Hash;
  }

  inline static size_t GetHashY(void)
  {
    static const size_t Hash = ::std::hash<String_t>{}(uT("y"));
    return Hash;
  }

  inline static size_t GetHashZ(void)
  {
    static const size_t Hash = ::std::hash<String_t>{}(uT("z"));
    return Hash;
  }

public:
  const float X;
  const float Y;
  const float Z;

protected:
  Transform(const ComponentPtr_t & _pComponent, const float _DefaultValue) :
    X(_pComponent->GetValue(GetHashX(), _DefaultValue)),
    Y(_pComponent->GetValue(GetHashY(), _DefaultValue)),
    Z(_pComponent->GetValue(GetHashZ(), _DefaultValue))
  {

  }
};

class Component::Position :
  public Transform
{
public:
  explicit Position(const ComponentPtr_t & _pComponent) :
    Transform(_pComponent, 0.0f)
  {

  }
};

class Component::Rotation :
  public Transform
{
public:
  explicit Rotation(const ComponentPtr_t & _pComponent) :
    Transform(_pComponent, 0.0f)
  {

  }
};

class Component::Scale :
  public Transform
{
public:
  explicit Scale(const ComponentPtr_t & _pComponent) :
    Transform(_pComponent, 1.0f)
  {

  }
};

} // namespace renderer

} // namespace api

} // namespace covellite
