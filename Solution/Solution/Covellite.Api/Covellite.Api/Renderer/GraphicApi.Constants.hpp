
#pragma once
#include <alicorn/std/string.hpp>
#include "GraphicApi.hpp"
#include "../robin_hood.hpp"

namespace covellite
{

namespace api
{

namespace renderer
{

class GraphicApi::Constants
{
public:
  template<class>
  class Data;

private:
  using CameraId_t = String_t;
  template<class T>
  using DataPtr_t = ::std::shared_ptr<Data<T>>;

public:
  template<class T>
  T & Get(void);
  template<class>
  void Update(void);
  void SetCameraId(const CameraId_t &);

private:
  template<class, template <class> class, class ... TArgs>
  void Create(TArgs && ...);
  template<class T>
  Data<T> & GetBuffer(void);
  friend GraphicApi;

private:
  ::robin_hood::unordered_map<::std::size_t, ::covellite::Any_t> m_Data;
  ::std::map<CameraId_t, ::Lights>  m_Lights;
  CameraId_t                        m_CurrentCameraId;
};

template<class T>
class GraphicApi::Constants::Data
{
public:
  T m_Data;

public:
  virtual void Update(void) const = 0;

public:
  Data(void) { memset(&m_Data, 0, sizeof(m_Data)); }
  virtual ~Data(void) = default;
};

template<class T, template <class> class TBuffer, class ... TArgs>
void GraphicApi::Constants::Create(TArgs && ... _Args)
{
  static const auto hCode = typeid(T).hash_code();

  const DataPtr_t<T> pBuffer = ::std::make_shared<TBuffer<T>>(_Args ...);
  m_Data[hCode] = pBuffer;
}

template<class T>
inline T & GraphicApi::Constants::Get(void)
{
  return GetBuffer<T>().m_Data;
}

template<>
inline ::Lights & GraphicApi::Constants::Get(void)
{
  return m_Lights[m_CurrentCameraId];
}

template<class T>
inline void GraphicApi::Constants::Update(void)
{
  GetBuffer<T>().Update();
}

inline void GraphicApi::Constants::SetCameraId(const CameraId_t & _CameraId)
{
  m_CurrentCameraId = _CameraId;
  GetBuffer<::Lights>().m_Data = Get<::Lights>();

  memset(&Get<::Lights>(), 0, sizeof(::Lights));
}

template<class T>
auto GraphicApi::Constants::GetBuffer(void) -> Data<T> &
{
  static const auto hCode = typeid(T).hash_code();

  const auto itConstant = m_Data.find(hCode);
  if (itConstant == m_Data.end())
  {
    throw STD_EXCEPTION << "Unexpected type: " << typeid(T).name();
  }

  return *::covellite::any_cast<DataPtr_t<T> &>(itConstant->second);
}

template<template <class> class TBuffer, class ... TArgs>
void GraphicApi::MakeConstants(TArgs && ... _Args)
{
  m_pConstants = ::std::make_shared<Constants>();
  m_pConstants->Create<::Camera, TBuffer>(_Args ...);
  m_pConstants->Create<::Matrices, TBuffer>(_Args ...);
  m_pConstants->Create<::Fog, TBuffer>(_Args ...);
  m_pConstants->Create<::Lights, TBuffer>(_Args ...);
}

template<class TColor>
/*static*/ TColor GraphicApi::ARGBtoFloat4(uint32_t _HexColor)
{
  return TColor
  {
    ((_HexColor & 0x00FF0000) >> 16) / 255.0f,
    ((_HexColor & 0x0000FF00) >> 8) / 255.0f,
    ((_HexColor & 0x000000FF) >> 0) / 255.0f,
    ((_HexColor & 0xFF000000) >> 24) / 255.0f
  };
}

template<class TFog>
auto GraphicApi::DoCreateFog(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto pFogData =
    m_ServiceComponents.Get({ { uT("Fog"), _pComponent } })[0];

  return [=](void)
  {
    const Component::Fog RawFogData{ pFogData };

    auto & Fog = m_pConstants->Get<TFog>();
    Fog.Color = ARGBtoFloat4<color_t>(RawFogData.Color);
    Fog.Near = RawFogData.Near;
    Fog.Far = RawFogData.Far;
    Fog.Density = RawFogData.Density;
  };
}

template<class TLight>
auto GraphicApi::DoCreateLight(
  const ComponentPtr_t & _pComponent,
  const bool _IsStatic) -> Render_t
{
  const auto GetColor = [=](void)
  {
    return ARGBtoFloat4<color_t>(_pComponent->GetValue(uT("color"), 0xFF000000));
  };

  // 15 Август 2019 00:19 (unicornum.verum@gmail.com)
  TODO("??? Почему версия для статического конвеера отличается ???");
  const auto ServiceComponents = _IsStatic ?
    m_ServiceComponents.Get(
    {
      { uT("Position"), api::Component::Make({ { uT("z"), 1.0f }, { uT("w"), 0.0f } }) },
      { uT("Direction"), api::Component::Make({ { uT("z"), 1.0f } }) },
      { uT("Attenuation"), api::Component::Make({ }) },
    }) :
    m_ServiceComponents.Get(
    {
      { uT("Position"), api::Component::Make({ }) },
      { uT("Direction"), api::Component::Make({ { uT("x"), 1.0f } }) },
      { uT("Attenuation"), api::Component::Make({ }) },
    });

  auto CreateAmbient = [&](void)
  {
    return [=](void)
    {
      auto & Light = m_pConstants->Get<TLight>().Ambient;

      Light.IsValid = 1;
      Light.Color = GetColor();
    };
  };

  auto CreateDirection = [&](void)
  {
    const auto pDirection = ServiceComponents[1];

    return [=](void)
    {
      auto & Light = m_pConstants->Get<TLight>().Direction;

      Light.IsValid = 1;
      Light.Color = GetColor();

      const Component::Position Direction{ pDirection };
      Light.Direction = { Direction.X, Direction.Y, Direction.Z, 0.0f };
    };
  };

  auto CreatePoint = [&](void)
  {
    auto pPosition = ServiceComponents[0];
    auto pAttenuation = ServiceComponents[2];

    return [=](void)
    {
      const Component::Position Position{ pPosition };

      auto & Lights = m_pConstants->Get<TLight>().Points;

      if (Lights.UsedSlotCount >= MAX_LIGHT_POINT_COUNT)
      {
        // 04 Январь 2019 19:30 (unicornum.verum@gmail.com)
        TODO("Запись в лог информации об избыточных источниках света.");
        return;
      }

      auto & Light = Lights.Lights[Lights.UsedSlotCount++];

      Light.Color = GetColor();
      Light.Position = { Position.X, Position.Y, Position.Z,
        pPosition->GetValue(uT("w"), 1.0f) }; // w == 1.0f - point light
                                      // Такой сложный способ из-за того,
                                      // что здесь должен быть 0.0f, если
                                      // pPosition не задан.
      Light.Attenuation =
      {
        pAttenuation->GetValue(uT("const"), 1.0f),
        pAttenuation->GetValue(uT("linear"), 0.0f),
        pAttenuation->GetValue(uT("exponent"), 0.0f),
        0.0f
      };
    };
  };

  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Ambient"), CreateAmbient },
    { uT("Direction"), CreateDirection },
    { uT("Point"), CreatePoint },
  };

  return Creators[_pComponent->Kind]();
}

} // namespace renderer

} // namespace api

} // namespace covellite
