
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
  using Component_t = ::covellite::api::Component;

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
  explicit Rasterizer(Component_t & _Component) :
    CullMode(_Component[uT("cull")].Default(uT("Back")))
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
  explicit Scissor(Component_t & _Component) :
    IsEnabled(_Component[uT("enabled")].Default(false)),
    Left(_Component[uT("left")].Default(0)),
    Top(_Component[uT("top")].Default(0)),
    Right(_Component[uT("right")].Default(0)),
    Bottom(_Component[uT("bottom")].Default(0))
  {

  }
};

template<class T>
class Component::Buffer
{
  using Data_t = ::std::vector<T>;
  static const Data_t & GetFakeData(void) 
  {
    static const Data_t FakeData;
    return FakeData;
  };

public:
  const Data_t Data;
  const int Dimension;

private:
  inline static Data_t GetContent(Component_t & _Component, const Data_t & _Data)
  {
    return ::std::move((Data_t &)_Component[uT("content")].Default(_Data));
  }

public:
  explicit Buffer(Component_t & _Component, const Data_t & _Data = GetFakeData()) :
    Data(GetContent(_Component, _Data)),
    Dimension(_Component[uT("dimension")].Default(3))
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
  Texture(Component_t & _Component, const String_t & _DefaultDestination) :
    Buffer(_Component),
    pTextureData(Data.empty() ? nullptr : Data.data()),
    Width(_Component[uT("width")].Default(0)),
    Height(_Component[uT("height")].Default(0)),
    Destination(_Component[uT("destination")].Default(_DefaultDestination)),
    IsUsingMipmapping(_Component[uT("mipmapping")].Default(false)),
    IsMapping(_Component[uT("mapper")].IsType<const cbBufferMap_t<const void> &>())
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
  static String_t GetShaderType(
    const ::std::string & _Entry,
    const uint8_t * _pBegin, 
    const uint8_t * _pEnd,
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
        (Instance[i] == uT("i")) ? "int4" : "";

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
  Shader(Component_t & _Component, const ::std::vector<uint8_t> & _Data) :
    Buffer(_Component, _Data),
    Entry((const ::std::string &)_Component[uT("entry")].Default("Unknown")),
    Kind(GetShaderType(Entry, Data.data(), Data.data() + Data.size(), ReturnType)),
    Instance{ GetInstance(_Component[uT("instance")].Default(uT(""))) }
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
  Transform(Component_t & _Component, const float _DefaultValue) :
    X(_Component[GetHashX()].Default(_DefaultValue)),
    Y(_Component[GetHashY()].Default(_DefaultValue)),
    Z(_Component[GetHashZ()].Default(_DefaultValue))
  {

  }
};

class Component::Position :
  public Transform
{
public:
  explicit Position(Component_t & _Component) :
    Transform(_Component, 0.0f)
  {

  }
};

class Component::Rotation :
  public Transform
{
public:
  explicit Rotation(Component_t & _Component) :
    Transform(_Component, 0.0f)
  {

  }
};

class Component::Scale :
  public Transform
{
public:
  explicit Scale(Component_t & _Component) :
    Transform(_Component, 1.0f)
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
  explicit Fog(Component_t & _Component) :
    Color(_Component[uT("color")].Default(0xFFFFFFFF)),
    Near(_Component[uT("near")].Default(10.0f)),
    Far(_Component[uT("far")].Default(100.0f)),
    Density(_Component[uT("density")].Default(1.0f))
  {

  }
};

} // namespace renderer

} // namespace api

} // namespace covellite
