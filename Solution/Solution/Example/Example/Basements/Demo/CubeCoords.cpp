
#include "stdafx.h"
#include "CubeCoords.hpp"

using namespace basement::model;

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
