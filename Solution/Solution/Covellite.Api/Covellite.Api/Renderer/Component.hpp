
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
  class Position;
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
    IsEnabled(_pComponent->GetValue(uT("is_enabled"), false)),
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
public:
  const T * const pData;
  const size_t Count;

private:
  ::std::vector<T> m_Data;

public:
  explicit Buffer(const ComponentPtr_t & _pComponent) :
    pData(_pComponent->GetValue<const T *>(uT("data"), m_Data.data())),
    Count(_pComponent->GetValue(uT("count"), m_Data.size()))
  {
    // 17 Октябрь 2018 14:38 (unicornum.verum@gmail.com)
    TODO("Заполнение m_Data из файла, если указано имя файла, а не данные.");
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
  const String_t Kind;
  const ::std::string Version;
  const ::std::string Entry;

public:
  explicit Shader(const ComponentPtr_t & _pComponent) :
    Buffer(_pComponent),
    Kind(_pComponent->GetValue(uT("kind"), uT("Unknown"))),
    Version(_pComponent->GetValue<::std::string>(uT("version"), "fx_unknown")),
    Entry(_pComponent->GetValue<::std::string>(uT("entry"), "Unknown"))
  {

  }
};

class Component::Position
{
public:
  const float X;
  const float Y;
  const float Z;

public:
  explicit Position(const ComponentPtr_t & _pComponent) :
    X(_pComponent->GetValue(uT("x"), 0.0f)),
    Y(_pComponent->GetValue(uT("y"), 0.0f)),
    Z(_pComponent->GetValue(uT("z"), 0.0f))
  {

  }
};

} // namespace renderer

} // namespace api

} // namespace covellite
