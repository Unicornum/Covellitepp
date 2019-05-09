
#include "stdafx.h"
#include "CubeCoords.hpp"
#include <alicorn/cpp/math.hpp>
#include <alicorn/std/exception.hpp>

using namespace basement::model;

CubeCoords::CubeCoords(const int _X, const int _Y) :
  X(_X),
  Y(_Y)
{
  if (::alicorn::extension::cpp::IS_DEBUG_CONFIGURATION)
  {
    const CubeCoords Example{ GetHash() };
    if (GetX() != Example.GetX() || GetY() != Example.GetY())
    {
      throw STD_EXCEPTION << "Unexpected CubeCoords hash value.";
    }
  }
}

CubeCoords::CubeCoords(const uint64_t _Hash) :
  X((_Hash & 0xFFFFFFFF) - HalfRange),
  Y((_Hash >> Offset) - HalfRange)
{
}

CubeCoords CubeCoords::TurnLeft(const uint32_t _Count) const
{
  auto Result = *this;

  for (uint32_t i = 0; i < _Count; i++)
  {
    Result = Result.TurnLeft();
  }

  return Result;
}

CubeCoords CubeCoords::TurnRight(const uint32_t _Count) const
{
  auto Result = *this;

  for (uint32_t i = 0; i < _Count; i++)
  {
    Result = Result.TurnRight();
  }

  return Result;
}

::std::pair<float, float> CubeCoords::ToPlane(void) const
{
  namespace math = ::alicorn::extension::cpp::math;

  static constexpr auto KoeffX = 0.5f * 1.5f;
  static const auto KoeffY = 0.5f * math::Root<2>(3.0f);

  return { KoeffX * X, KoeffY * (Y + 0.5f * X) };
}
