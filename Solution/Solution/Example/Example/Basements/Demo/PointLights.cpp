
#include "stdafx.h"
#include "PointLights.hpp"
#include <alicorn/logger.hpp>
#include "CubeCoords.hpp"

using namespace basement::model;

class PointLights::Data 
{
public:
  ::std::map<uint64_t, Point_t> m_Lights;
  ::std::map<uint64_t, Points_t> m_CellLights;
  ::std::map<uint64_t, Points> m_CellLights2;
};

PointLights::PointLights(void) :
  m_pData{ ::std::make_shared<Data>() }
{

}

PointLights::~PointLights(void) noexcept
{
  LOGGER(Info) << "MaxSceneLightCount: " << m_MaxSceneLightCount;
  LOGGER(Info) << "MaxObjectLightCount: " << m_MaxObjectLightCount;
}

void PointLights::Add(const CubeCoords & _CellPosition, const float _Height)
{
  const auto ARGBtoFloat4 = [](uint32_t _HexColor)
  {
    return ::glm::vec4
    {
      ((_HexColor & 0x00FF0000) >> 16) / 255.0f,
      ((_HexColor & 0x0000FF00) >> 8) / 255.0f,
      ((_HexColor & 0x000000FF) >> 0) / 255.0f,
      ((_HexColor & 0xFF000000) >> 24) / 255.0f
    };
  };

  auto & Light = m_pData->m_Lights[_CellPosition.GetHash()];

  const auto WorldPosition = _CellPosition.ToPlane();

  Light.Color = ARGBtoFloat4(0xFFAFAFFF);
  Light.Position = { WorldPosition.first, WorldPosition.second, _Height + 0.25f, 1.0f };

  const auto constant = 0.5f;
  const auto linear = 0.0f;
  const auto exponent = 2.0f;

  namespace math = ::alicorn::extension::cpp::math;

  const auto lightMax = math::Max(Light.Color.x,
    math::Max(Light.Color.y, Light.Color.z)) * 256.0f / 32.0f; // 32 - это 
  // ambient, т.е. радиус - расстояние, на котором цвет от источника света 
  // сливается с фоном.

  // Источник: https://habr.com/ru/post/420565/
  const auto radius = (-linear + math::Root<2>(linear * linear - 
    4.0f * exponent * (constant - lightMax))) / (2.0f * exponent);

  Light.Attenuation = { constant, linear, exponent, radius };
}

void PointLights::Remove(const CubeCoords & _CellPosition)
{
  m_pData->m_Lights.erase(_CellPosition.GetHash());
  m_pData->m_CellLights.erase(_CellPosition.GetHash());
  m_pData->m_CellLights2.erase(_CellPosition.GetHash());
}

Points_t PointLights::Get(const CubeCoords & _CellPosition, const float _Height) const
{
  m_MaxSceneLightCount = 
    ::std::max(m_MaxSceneLightCount, m_pData->m_Lights.size());

  const auto itCellLights = m_pData->m_CellLights.find(_CellPosition.GetHash());
  if (itCellLights == m_pData->m_CellLights.end())
  {
    const auto WorldPosition = _CellPosition.ToPlane();

    ///////////// Список источников света, которые освещают объект /////////////

    ::std::map<float, Point_t> CellPoints;

    for (const auto & Light : m_pData->m_Lights)
    {
      const auto & Position = Light.second.Position;
      const auto & Radius = Light.second.Attenuation.w;

      const auto Dx = Position.x - WorldPosition.first;
      const auto Dy = Position.y - WorldPosition.second;
      const auto Dz = Position.z - _Height;

      const auto DistanceSq = Dx * Dx + Dy * Dy + Dz * Dz;

      if (DistanceSq <= Radius * Radius)
      {
        CellPoints[DistanceSq] = Light.second;
      }
    }

    m_MaxObjectLightCount = ::std::max(m_MaxObjectLightCount, CellPoints.size());

    //////////////// Список четырех ближайших источников света  ////////////////

    auto & Result = m_pData->m_CellLights[_CellPosition.GetHash()];
    Result.UsedSlotCount = 0;

    for (const auto & Light : CellPoints)
    {
      Result.Lights[Result.UsedSlotCount++] = Light.second;
      if (Result.UsedSlotCount == COVELLITE_MAX_LIGHT_POINT_OBJECT_COUNT) break;
    }
  }

  return m_pData->m_CellLights[_CellPosition.GetHash()];
}

const PointLights::Points & PointLights::GetUserConstantBuffer(
  const CubeCoords & _CellPosition, 
  const float _Height) const
{
  m_MaxSceneLightCount =
    ::std::max(m_MaxSceneLightCount, m_pData->m_Lights.size());

  const auto itCellLights = m_pData->m_CellLights2.find(_CellPosition.GetHash());
  if (itCellLights == m_pData->m_CellLights2.end())
  {
    const auto WorldPosition = _CellPosition.ToPlane();

    /////////// Список всех источников света, которые освещают объект //////////

    ::std::map<float, Point_t> CellPoints;

    for (const auto & Light : m_pData->m_Lights)
    {
      const auto & Position = Light.second.Position;
      const auto & Radius = Light.second.Attenuation.w;

      const auto Dx = Position.x - WorldPosition.first;
      const auto Dy = Position.y - WorldPosition.second;
      const auto Dz = Position.z - _Height;

      const auto DistanceSq = Dx * Dx + Dy * Dy + Dz * Dz;

      if (DistanceSq <= Radius * Radius)
      {
        CellPoints[DistanceSq] = Light.second;
      }
    }

    m_MaxObjectLightCount = ::std::max(m_MaxObjectLightCount, CellPoints.size());

    ///////////////// Список восьми ближайших источников света  ////////////////

    auto & Result = m_pData->m_CellLights2[_CellPosition.GetHash()];
    Result.UsedSlotCount = 0;

    for (const auto & Light : CellPoints)
    {
      Result.Lights[Result.UsedSlotCount++] = Light.second;
      if (Result.UsedSlotCount == 8) break;
    }
  }

  return m_pData->m_CellLights2[_CellPosition.GetHash()];
}
