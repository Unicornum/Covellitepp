
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
*  � CTAPOBEP 2019
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
  const Creators_t & GetCreators(void) const final;

protected:
  virtual Render_t CreateCamera(const ComponentPtr_t &) = 0;
  virtual Render_t CreateBkSurface(const ComponentPtr_t &) { return nullptr; }
  virtual Render_t CreateState(const ComponentPtr_t &) = 0;
  virtual Render_t CreateFog(const ComponentPtr_t &) = 0;
  virtual Render_t CreateShader(const ComponentPtr_t &) { return nullptr; }
  virtual Render_t CreateTexture(const ComponentPtr_t &) = 0;
  virtual Render_t CreateBuffer(const ComponentPtr_t &) = 0;
  virtual Render_t CreateTransform(const ComponentPtr_t &) { return nullptr; }
  virtual Render_t CreatePresentBuffer(const ComponentPtr_t &) { return nullptr; }

protected:
  // deprecated
  virtual Render_t CreateMaterial(const ComponentPtr_t &) { return nullptr; }
  virtual Render_t CreateLight(const ComponentPtr_t &) { return nullptr; }
  virtual Render_t CreateGeometry(const ComponentPtr_t &) { return nullptr; }

public:
  template<class TColor>
  static TColor ARGBtoFloat4(uint32_t);

private:
  Render_t CreatePresent(const ComponentPtr_t &);
  Render_t CreateUpdater(const ComponentPtr_t &) const;

private:
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
  * \date
  *  13 ������� 2019    \n
  * \author
  *  CTAPOBEP (unicornum.verum@gmail.com)
  * \copyright
  *  � CTAPOBEP 2019
  */
  class CapturingServiceComponent final
  {
    using Kind_t = ::alicorn::extension::std::String;
    using Expected_t = ::std::vector<::std::pair<Kind_t, ComponentPtr_t>>;
    using Hadler_t = ::std::function<void(const ComponentPtr_t &)>;
    using Handlers_t = ::std::map<Kind_t, Hadler_t>;

  public:
    ::std::vector<ComponentPtr_t> Get(const Expected_t &);
    void Process(const Handlers_t &);

  private:
    Components_t & m_Components;

  public:
    explicit CapturingServiceComponent(Components_t &);
  };

  Components_t m_Components;

protected:
  bool m_IsResizeWindow = false;
  CapturingServiceComponent m_ServiceComponents;

private:
  using TimePoint_t = ::std::chrono::system_clock::time_point;
  const TimePoint_t m_StartProgram;
  float             m_CurrentFrameTime;

protected:
  class Constants;
  ::std::shared_ptr<Constants> m_pConstants;
  template<template <class> class TBuffer, class ... TArgs>
  void MakeConstants(TArgs && ...);

protected:
  template<class TCamera>
  Render_t DoCreateCamera(const ComponentPtr_t &);
  template<class TFog>
  Render_t DoCreateFog(const ComponentPtr_t &, const bool = false);
  template<class TLight>
  Render_t DoCreateLight(const ComponentPtr_t &, const bool = false);

private:
  Creators_t m_Creators;

protected:
  GraphicApi(void);
};

} // namespace renderer

} // namespace api

} // namespace covellite
