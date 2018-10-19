
#pragma once
#include <vector>
#include <alicorn/std/string.hpp>
#include <Covellite/Predefined.hpp>
#include "IGraphicApi.hpp"
#include "Api.forward.hpp"

namespace covellite
{

namespace api
{

namespace renderer
{

/**
* \ingroup CovelliteApiRenderGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Класс общей логики реализации рендеринга GUI.
*  
* \version
*  1.0.0.0        \n
* \date
*  16 Октябрь 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2018
*/
class CommonImpl :
  public Registator_t<IGraphicApi>
{
protected:
  using String_t = ::alicorn::extension::std::String;
  using Render_t = ::std::function<void(void)>;
  using ObjectRenders_t = ::std::vector<Render_t>;

public:
  // Интерфейс IGraphicApi:
  ITexture * Create(const ITexture::Data &) final;
  void Destroy(ITexture *) final;
  IGeometry * Create(const IGeometry::Data &) final;
  void Destroy(IGeometry *) final;
  void EnableScissorRegion(int, int, int, int) final;
  void DisableScissorRegion(void) final;
  void Render(void) final {}
  void Render(IGeometry *, float, float) final;

protected:
  void BeginScene(void);
  void EndScene(void);

public:
  class Component;
  using Object_t = ::std::vector<Component>;
  using Creator_t = ::std::function<Render_t(const Component &)>;
  using Creators_t = ::std::map<String_t, Creator_t>;

  class Scissor;
  class ConstantBuffer;
  template<class>
  class Buffer;
  class Texture;
  class Shader;
  class Position;

  ObjectRenders_t Obtain(const Object_t &);
  void Remove(const Object_t &);

protected:
  virtual const Creators_t & GetCreators(void) const = 0;

private:
  // Переменные для обмена данными между разными компонентами.
  float m_PositionX = 0.0f, m_PositionY = 0.0f, m_PositionZ = 0.0f;
  int m_Left = 0, m_Top = 0, m_Right = 0, m_Bottom = 0;

private:
  ::std::map<::alicorn::extension::std::String, Render_t> m_Components;

  Object_t m_DefaultRenders;
  ::std::map<IGeometry *, ::std::vector<Render_t>> m_Objects;
  ::std::vector<Render_t> m_RenderQueue;

public:
  CommonImpl(void);
  ~CommonImpl(void);
};

class CommonImpl::Component
{
public:
  using Type_t = String_t;
  using Id_t = String_t;
  using Name_t = String_t;
  using Params_t = ::std::map<Name_t, ::covellite::Any_t>;

public:
  template<class T>
  class Reference
  {
  private:
    T m_DefaultValue;

  public:
    const T & Value;

  public:
    Reference(const Component & _Component, const String_t & _ParamName,
      const T & _DefaultValue) :
      m_DefaultValue(_DefaultValue),
      Value(*_Component.GetRawValue<T *>(_ParamName, &m_DefaultValue))
    {

    }
  };

public:
  template<class T>
  T GetValue(const Name_t & _Name, const T & _DefaultValue) const
  {
    auto itValue = m_Params.find(_Name);
    if (itValue == m_Params.end()) return _DefaultValue;

    return ::boost::lexical_cast<T>(
      ::covellite::any_cast<String_t>(itValue->second));
  }

  template<class T>
  T GetRawValue(const Name_t & _Name, const T & _DefaultValue) const
  {
    auto itValue = m_Params.find(_Name);
    if (itValue == m_Params.end()) return _DefaultValue;

    return ::covellite::any_cast<T>(itValue->second);
  }

private:
  const Params_t m_Params;

public:
  const Type_t Type;
  const Id_t Id;

public:
  explicit Component(const Params_t & _Params) :
    m_Params(_Params),
    Type(GetValue<Type_t>(uT("type"), uT("Unknown"))),
    Id(GetValue<Id_t>(uT("id"), uT("Unknown")))
  {

  }
};

class CommonImpl::Scissor
{
public:
  const bool IsEnabled;
  Component::Reference<int> Left;
  Component::Reference<int> Top;
  Component::Reference<int> Right;
  Component::Reference<int> Bottom;

public:
  explicit Scissor(const Component & _Component) :
    IsEnabled(_Component.GetValue(uT("IsEnabled"), false)),
    Left(_Component, uT("left"), 0),
    Top(_Component, uT("top"), 0),
    Right(_Component, uT("right"), 0),
    Bottom(_Component, uT("bottom"), 0)
  {

  }
};

template<class T>
class CommonImpl::Buffer
{
public:
  const T * const pData;
  const size_t Count;

private:
  ::std::vector<T> m_Data;

public:
  explicit Buffer(const Component & _Component) :
    pData(_Component.GetRawValue<const T *>(uT("data"), m_Data.data())),
    Count(_Component.GetRawValue(uT("count"), m_Data.size()))
  {
    // 17 Октябрь 2018 14:38 (unicornum.verum@gmail.com)
    TODO("Заполнение m_Data из файла, если указано имя файла, а не данные.");
  }
};

class CommonImpl::Texture :
  public Buffer<uint8_t>
{
public:
  const int Width;
  const int Height;

public:
  explicit Texture(const Component & _Component) :
    Buffer(_Component),
    Width(_Component.GetRawValue(uT("width"), 0)),
    Height(_Component.GetRawValue(uT("height"), 0))
  {

  }
};

class CommonImpl::Shader :
  public Buffer<uint8_t>
{
public:
  const String_t Kind;
  const ::std::string Version;
  const ::std::string Entry;

public:
  explicit Shader(const Component & _Component) :
    Buffer(_Component),
    Kind(_Component.GetValue<String_t>(uT("kind"), uT("Unknown"))),
    Version(_Component.GetValue<::std::string>(uT("version"), "fx_unknown")),
    Entry(_Component.GetValue<::std::string>(uT("entry"), "Unknown"))
  {

  }
};

class CommonImpl::Position
{
public:
  Component::Reference<float> X;
  Component::Reference<float> Y;
  Component::Reference<float> Z;

public:
  explicit Position(const Component & _Component) :
    X(_Component, uT("x"), 0.0f),
    Y(_Component, uT("y"), 0.0f),
    Z(_Component, uT("z"), 0.0f)
  {

  }
};

} // namespace renderer

} // namespace api

} // namespace covellite
