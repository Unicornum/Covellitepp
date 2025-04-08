
#pragma once
#include <deque>
#include "IGraphicApi.hpp"
#include "Api.forward.hpp"

namespace covellite
{

namespace api
{

namespace renderer
{

/**
* \ingroup CovelliteApiRendererGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ��������������� �����, ���������� ����� ��� ���� ���������� ���.
*
* \version
*  1.0.0.0        \n
* \date
*  11 ������ 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019 - 2020
*/
class GraphicApi :
  public Registator_t<IGraphicApi>
{
  using Components_t = ::std::deque<ComponentPtr_t>;

protected:
  using Data_t = ::covellite::api::renderer::SettingsData;

public:
  // ��������� IGraphicApi:
  void PresentFrame(void) override;
  const Creators_t & GetCreators(void) const noexcept final;

protected:
  virtual Render_t CreateCamera(const ComponentPtr_t &) = 0;
  virtual Render_t CreateBkSurface(const ComponentPtr_t &) = 0;
  virtual Render_t CreateState(const ComponentPtr_t &) = 0;
  virtual Render_t CreateShader(const ComponentPtr_t &) = 0;
  virtual Render_t CreateTexture(const ComponentPtr_t &) = 0;
  virtual Render_t CreateTextureArray(const ComponentPtr_t &) = 0;
  virtual Render_t CreateBuffer(const ComponentPtr_t &) = 0;
  virtual Render_t CreateTransform(const ComponentPtr_t &) = 0;
  virtual Render_t CreatePresentBuffer(const ComponentPtr_t &) = 0;

public:
  template<class TColor>
  static TColor ARGBtoFloat4(uint32_t);

private:
  Render_t CreateUpdater(const ComponentPtr_t &) const;

protected:
  /**
  * \ingroup CovelliteApiRendererGroup
  * \brief
  *  ����� ������ � ������ \ref CovelliteApiPage \n
  *  ����� ��� ������ �� ���������������� ������������.
  * \details
  *  - ��������������� ���������� - ����������, ��� ������� �� ��������� �������,
  *  � ������ ����� ��� ������ ���� ��������� � �������������� ��������� ������
  *  �����������; ����� ������������� ������� ��� ��������� ���������
  *  ��������������� �����������.
  *
  * \version
  *  1.0.0.0        \n
  *  2.0.0.0        \n
  * \date
  *  13 ������� 2019    \n
  *  16 ���� 2020    \n
  * \author
  *  CTAPOBEP (unicornum.verum@gmail.com)
  * \copyright
  *  � CTAPOBEP 2019
  */
  class CapturingServiceComponent final
  {
    using Kind_t = ::alicorn::extension::std::String;
    using Services_t = ::std::vector<ComponentPtr_t>;
    using Expected_t = ::std::vector<::std::pair<Kind_t, ComponentPtr_t>>;
    using Hadler_t = ::std::function<void(const ComponentPtr_t &)>;
    using Handlers_t = ::std::map<Kind_t, Hadler_t>;

  public:
    static Services_t Get(const ComponentPtr_t &, const Expected_t &);
    static void Process(const ComponentPtr_t &, const Handlers_t &);
  };

protected:
  bool m_IsResizeWindow = false;
  const uint8_t Align1[7] = { 0 };

private:
  using TimePoint_t = ::std::chrono::system_clock::time_point;
  const TimePoint_t m_StartProgram;
  float             m_CurrentFrameTime;
  const uint8_t Align2[4] = { 0 };

protected:
  class Constants;
  ::std::shared_ptr<Constants> m_pConstants;
  template<template <class> class TBuffer, class ... TArgs>
  void MakeConstants(TArgs && ...);

private:
  Creators_t m_Creators;

protected:
  GraphicApi(void);
  GraphicApi(const GraphicApi &) = delete;
  GraphicApi(GraphicApi &&) = delete;
  GraphicApi & operator= (const GraphicApi &) = delete;
  GraphicApi & operator= (GraphicApi &&) = delete;
  ~GraphicApi(void) = default;
};

} // namespace renderer

} // namespace api

} // namespace covellite

namespace std
{

template<class T>
istream & operator>>(istream &, shared_ptr<T> &)
{
  throw STD_EXCEPTION << "��� �� ������ ����������, ����� ��� ���������������";
}

} // namespace std
