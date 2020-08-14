
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
  Rasterizer(const Rasterizer &) = delete;
  Rasterizer(Rasterizer &&) = delete;
  Rasterizer & operator= (const Rasterizer &) = delete;
  Rasterizer & operator= (Rasterizer &&) = delete;
  ~Rasterizer(void) = default;
};

class Component::Scissor
{
public:
  const int Left;
  const int Top;
  const int Right;
  const int Bottom;
  const bool IsEnabled;
  const uint8_t Align[3] = { 0 };

public:
  explicit Scissor(Component_t & _Component) :
    Left(_Component[uT("left")].Default(0)),
    Top(_Component[uT("top")].Default(0)),
    Right(_Component[uT("right")].Default(0)),
    Bottom(_Component[uT("bottom")].Default(0)),
    IsEnabled(_Component[uT("enabled")].Default(false))
  {

  }
  Scissor(const Scissor &) = delete;
  Scissor(Scissor &&) = delete;
  Scissor & operator= (const Scissor &) = delete;
  Scissor & operator= (Scissor &&) = delete;
  ~Scissor(void) = default;
};

template<class T>
class Component::Buffer
{
  static const Buffer_t<T> & GetFakeData(void) noexcept
  {
    static const Buffer_t<T> FakeData;
    return FakeData;
  };

public:
  const Buffer_t<T> Data;
  const int Dimension;
  const uint8_t Align[4] = { 0 };

private:
  inline static Buffer_t<T> GetContent(
    Component_t & _Component, 
    const Buffer_t<T> & _Data)
  {
    return ::std::move((Buffer_t<T> &)_Component[uT("content")].Default(_Data));
  }

public:
  explicit Buffer(Component_t & _Component, const Buffer_t<T> & _Data = GetFakeData()) :
    Data(GetContent(_Component, _Data)),
    Dimension(_Component[uT("dimension")].Default(3))
  {
  }
  Buffer(const Buffer &) = delete;
  Buffer(Buffer &&) = delete;
  Buffer & operator= (const Buffer &) = delete;
  Buffer & operator= (Buffer &&) = delete;
  ~Buffer(void) = default;
};

class Component::Texture :
  public Buffer<uint8_t>
{
public:
  const uint8_t * const pTextureData; // может быть nullptr 
  const int Width;
  const int Height;
  const String_t Name;
  const String_t Destination;
  const int Index;
  const int Capacity;
  const bool IsUsingMipmapping;
  const bool IsMapping;
  const uint8_t Align[6] = { 0 };

public:
  Texture(Component_t & _Component, const String_t & _DefaultDestination) :
    Buffer(_Component),
    pTextureData(Data.empty() ? nullptr : Data.data()),
    Width(_Component[uT("width")].Default(0)),
    Height(_Component[uT("height")].Default(0)),
    Name(_Component[uT("name")].Default(uT("Unknown"))),
    Destination(_Component[uT("destination")].Default(_DefaultDestination)),
    Index(_Component[uT("index")].Default(-1)),
    Capacity(_Component[uT("capacity")].Default(8)),
    IsUsingMipmapping(_Component[uT("mipmapping")].Default(false)),
    IsMapping(_Component[uT("mapper")].IsType<const cbBufferMap_t<const void> &>())
  {

  }
  Texture(const Texture &) = delete;
  Texture(Texture &&) = delete;
  Texture & operator= (const Texture &) = delete;
  Texture & operator= (Texture &&) = delete;
  ~Texture(void) = default;
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
      const auto EntryPosition = Line.find(" " + _Entry + "(", 0);
      if (EntryPosition != ::std::string::npos)
      {
        _ReturnType = Line.substr(0, Line.find(" ", 0));

        const auto TypeBegin = EntryPosition + _Entry.length() 
          + 2; // начальный пробел + скобка после
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
        Index + ");" + char{ 0x5C } + "\r\n";
    }

    return ::boost::algorithm::replace_first_copy(_Input,
      InstanceBlockDeclaration, InstanceBlockImplementation);
  };

public:
  inline BinaryData_t GetInstanceInput(const BinaryData_t & _Input) const
  {
    if (Instance.empty()) return _Input;

    const auto strInput = 
      GetInstanceInput(::std::string{ _Input.cbegin(), _Input.cend() });
    return BinaryData_t{ strInput.cbegin(), strInput.cend() };
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
  Shader(Component_t & _Component, const BinaryData_t & _Data) :
    Buffer(_Component, _Data),
    Entry((const ::std::string &)_Component[uT("entry")].Default("Unknown")),
    Kind(GetShaderType(Entry, Data.data(), Data.data() + Data.size(), ReturnType)),
    Instance{ GetInstance(_Component[uT("instance")].Default(uT(""))) }
  {
  }
  Shader(const Shader &) = delete;
  Shader(Shader &&) = delete;
  Shader & operator= (const Shader &) = delete;
  Shader & operator= (Shader &&) = delete;
  ~Shader(void) = default;
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
  Transform(const Transform &) = delete;
  Transform(Transform &&) = delete;
  Transform & operator= (const Transform &) = delete;
  Transform & operator= (Transform &&) = delete;
  ~Transform(void) = default;
};

class Component::Position :
  public Transform
{
public:
  explicit Position(Component_t & _Component) :
    Transform(_Component, 0.0f)
  {

  }
  Position(const Position &) = delete;
  Position(Position &&) = delete;
  Position & operator= (const Position &) = delete;
  Position & operator= (Position &&) = delete;
  ~Position(void) = default;
};

class Component::Rotation :
  public Transform
{
public:
  explicit Rotation(Component_t & _Component) :
    Transform(_Component, 0.0f)
  {

  }
  Rotation(const Rotation &) = delete;
  Rotation(Rotation &&) = delete;
  Rotation & operator= (const Rotation &) = delete;
  Rotation & operator= (Rotation &&) = delete;
  ~Rotation(void) = default;
};

class Component::Scale :
  public Transform
{
public:
  explicit Scale(Component_t & _Component) :
    Transform(_Component, 1.0f)
  {

  }
  Scale(const Scale &) = delete;
  Scale(Scale &&) = delete;
  Scale & operator= (const Scale &) = delete;
  Scale & operator= (Scale &&) = delete;
  ~Scale(void) = default;
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
  Fog(const Fog &) = delete;
  Fog(Fog &&) = delete;
  Fog & operator= (const Fog &) = delete;
  Fog & operator= (Fog &&) = delete;
  ~Fog(void) = default;
};

} // namespace renderer

} // namespace api

} // namespace covellite
