
#pragma once
#include <alicorn/cpp/math.hpp>
#include <alicorn/std/string.hpp>
#include <alicorn/std.fast/unordered-map.hpp>
#include "GraphicApi.hpp"

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

private:
  template<class, template <class> class, class ... TArgs>
  void Create(TArgs && ...);
  template<class T>
  Data<T> & GetBuffer(void);
  friend GraphicApi;

private:
  ::alicorn::extension::std::fast::unordered_map<::std::size_t, ::covellite::Any_t> m_Data;
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
  Data(void) noexcept { memset(&m_Data, 0, sizeof(m_Data)); }
  Data(const Data &) = delete;
  Data(Data &&) = delete;
  Data & operator= (const Data &) = delete;
  Data & operator= (Data &&) = delete;
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

template<class T>
inline void GraphicApi::Constants::Update(void)
{
  GetBuffer<T>().Update();
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
  m_pConstants->Create<::Object, TBuffer>(_Args ...);
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

} // namespace renderer

} // namespace api

} // namespace covellite
