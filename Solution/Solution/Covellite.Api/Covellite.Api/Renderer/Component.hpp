
#pragma once
#include <alicorn/std/string.hpp>
#include <alicorn/std/string/encoding.hpp>
#include <alicorn/std/regex.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <Covellite/Api/Component.inl>
#include <Covellite/Api/Defines.hpp>

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
  class Rasterizer;
  class Scissor;
  template<class>
  class Buffer;
  class Texture;
  class Shader;
  class Shader2;
  class Transform;
  class Position;
  class Rotation;
  class Scale;
  class Fog;
};

class Component::Rasterizer
{
public:
  const String_t CullMode;

public:
  explicit Rasterizer(const ComponentPtr_t & _pComponent) :
    CullMode(_pComponent->GetValue(uT("cull"), uT("Back")))
  {

  }
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
  inline ::std::vector<T> GetBody(void) const { return { pData, pData + Count }; }

public:
  const T * const pData;
  const size_t Count;
  const int Dimension;

public:
  explicit Buffer(const ComponentPtr_t & _pComponent) :
    pData(_pComponent->GetValue<const T *>(uT("data"), m_Data.data())),
    Count(_pComponent->GetValue(uT("count"), m_Data.size())),
    Dimension(_pComponent->GetValue(uT("dimension"), 3))
  {
  }
  Buffer(const ComponentPtr_t & _pComponent, const ::std::vector<T> & _Data) :
    pData(_pComponent->GetValue(uT("data"), _Data.data())),
    Count(_pComponent->GetValue(uT("count"), _Data.size())),
    Dimension(_pComponent->GetValue(uT("dimension"), 3))
  {
  }
};

class Component::Texture :
  public Buffer<uint8_t>
{
public:
  const uint8_t * const pTextureData; // может быть nullptr 
  const int Width;
  const int Height;
  const String_t Destination;
  const bool IsUsingMipmapping;
  const bool IsMapping;

public:
  explicit Texture(const ComponentPtr_t & _pComponent) :
    Buffer(_pComponent),
    pTextureData(_pComponent->GetValue<const uint8_t *>(uT("data"), nullptr)),
    Width(_pComponent->GetValue(uT("width"), 0)),
    Height(_pComponent->GetValue(uT("height"), 0)),
    Destination(_pComponent->GetValue(uT("destination"), uT("diffuse"))),
    IsUsingMipmapping(_pComponent->GetValue(uT("mipmapping"), false)),
    IsMapping(_pComponent->IsType<const cbBufferMap_t<const void> &>(uT("mapper")))
  {

  }
};

class Component::Shader :
  public Buffer<uint8_t>
{
public:
  const ::std::string Entry;
  ::std::string ReturnType;
  const String_t Kind;
  const ::std::vector<String_t> Instance;

private:
  static String_t GetShaderType(const ::std::string & _Entry,
    const uint8_t * _pBegin, const uint8_t * _pEnd,
    ::std::string & _ReturnType)
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
        _ReturnType = Line.substr(0, Line.find(" ", 0));

        const auto TypeBegin = EntryPosition + _Entry.length() + 1;
        const auto TypeEnd = Line.find(" ", TypeBegin);

        using namespace ::alicorn::extension::std;

        return string_cast<String, Encoding::Ascii128>(
          Line.substr(TypeBegin, TypeEnd - TypeBegin));
      }

      pLastBreak = pBreak + 1;
    }
  }

  static ::std::vector<String_t> GetInstance(const String_t _Value)
  {
    ::std::vector<String_t> Result;

    if (_Value == uT("")) return Result;

    namespace regex = ::alicorn::extension::std::regex;

    regex::Match mInstanceDataStruct{ uT("(?:[if]4)*") };
    if (!mInstanceDataStruct.IsMatch(_Value))
    {
      throw STD_EXCEPTION << "Unexpected instance value: " << _Value;
    }

    regex::Search sInstanceDataStruct{ uT("(?:([if])4)") };
    if (!sInstanceDataStruct.IsSearch(_Value)) return Result;

    const auto & Elements = sInstanceDataStruct.GetCoincided();

    for (::std::size_t i = 0; i < Elements.size(); i++)
    {
      Result.push_back(Elements[i].second[0]);
    }

    return Result;
  };

  ::std::string GetInstanceInput(const ::std::string & _Input) const
  {
    static const ::std::string InstanceBlockDeclaration =
      "/* place for instance variables */";
    ::std::string InstanceBlockImplementation;

    for (::std::size_t i = 0; i < Instance.size(); i++)
    {
      const auto Index = ::std::to_string(i + 1);
      const auto Type =
        (Instance[i] == uT("f")) ? "float4" :
        (Instance[i] == uT("i")) ? "int4" :
        nullptr;

      InstanceBlockImplementation += ::std::string{ "COVELLITE_IN " } +
        Type + " iValue" + Index + " COVELLITE_INPUT_SEMANTIC(TEXCOORD" +
        Index + ");" + (char)0x5C + "\r\n";
    }

    return ::boost::algorithm::replace_first_copy(_Input,
      InstanceBlockDeclaration, InstanceBlockImplementation);
  };

public:
  inline ::std::vector<uint8_t> GetInstanceInput(
    const ::std::vector<uint8_t> & _Input) const
  {
    if (Instance.empty()) return _Input;

    const auto strInput = 
      GetInstanceInput(::std::string{ _Input.cbegin(), _Input.cend() });
    return ::std::vector<uint8_t>{ strInput.cbegin(), strInput.cend() };
  }

  ::std::string GetInstanceCopyData(void) const
  {
    ::std::string InstanceCopyDataImplementation;

    for (::std::size_t i = 0; i < Instance.size(); i++)
    {
      const auto iValue = "iValue" + ::std::to_string(i + 1);

      InstanceCopyDataImplementation += 
        "  InputData." + iValue + " = " + iValue + ";\r\n";
    }

    return InstanceCopyDataImplementation;
  }

public:
  Shader(const ComponentPtr_t & _pComponent, const ::std::vector<uint8_t> & _Data) :
    Buffer(_pComponent, _Data),
    Entry(_pComponent->GetValue<::std::string>(uT("entry"), "Unknown")),
    Kind(GetShaderType(Entry, pData, pData + Count, ReturnType)),
    Instance{ GetInstance(_pComponent->GetValue(uT("instance"), uT(""))) }
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

class Component::Fog
{
public:
  const uint32_t Color;
  const float Near;
  const float Far;
  const float Density;

public:
  explicit Fog(const ComponentPtr_t & _pComponent) :
    Color(_pComponent->GetValue(uT("color"), 0xFFFFFFFF)),
    Near(_pComponent->GetValue(uT("near"), 10.0f)),
    Far(_pComponent->GetValue(uT("far"), 100.0f)),
    Density(_pComponent->GetValue(uT("density"), 1.0f))
  {

  }
};

} // namespace renderer

} // namespace api

} // namespace covellite
