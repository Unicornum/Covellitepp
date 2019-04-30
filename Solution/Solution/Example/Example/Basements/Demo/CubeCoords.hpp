
#pragma once
#include <utility>
#include <alicorn/cpp/math.hpp>
#include <alicorn/std/exception.hpp>
#include "Constants.hpp"

namespace basement
{

namespace model
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс манипуляции кубическими координатами.
*
* \version
*  1.0.0.0        \n
* \date
*  29 Март 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class CubeCoords final
{
  static constexpr uint64_t Offset = 8 * sizeof(uint64_t) / 2;
  static constexpr uint32_t HalfRange = 
    static_cast<uint32_t>(1 << (8 * sizeof(int32_t) - 1));

public:
  inline int GetX(void) const { return X; }
  inline int GetY(void) const { return Y; }
  inline int GetZ(void) const { return -(X + Y); }

public:
  inline CubeCoords operator + (const CubeCoords & _CubeCoords) const
  {
    return CubeCoords{ X + _CubeCoords.X, Y + _CubeCoords.Y };
  }

  inline CubeCoords operator - (const CubeCoords & _CubeCoords) const
  {
    return CubeCoords{ X - _CubeCoords.X, Y - _CubeCoords.Y };
  }

  inline CubeCoords operator* (const int32_t _Value) const
  {
    return CubeCoords{ X * _Value, Y * _Value };
  }

  inline CubeCoords TurnLeft(void) const { return CubeCoords{ -Y, X + Y }; }
  inline CubeCoords TurnRight(void) const { return CubeCoords{ X + Y, -X }; }
  CubeCoords TurnLeft(const uint32_t) const;
  CubeCoords TurnRight(const uint32_t) const;

  inline ::std::pair<float, float> ToPlane(void) const
  {
    namespace math = ::alicorn::extension::cpp::math;

    static constexpr auto KoeffX = 0.5f * 1.5f;
    static const auto KoeffY = 0.5f * math::Root<2>(3.0f);

    return { KoeffX * X, KoeffY * (Y + 0.5f * X) };
  }

  inline uint64_t GetHash(void) const
  {
    return (static_cast<uint64_t>(Y + HalfRange) << Offset) + 
      static_cast<uint64_t>(X + HalfRange);
  }

private:
  int32_t X, Y;

public:
  explicit CubeCoords(const int _X = 0, const int _Y = 0) :
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
  explicit CubeCoords(const uint64_t _Hash) :
    X((_Hash & 0xFFFFFFFF) - HalfRange),
    Y((_Hash >> Offset) - HalfRange)
  {
  }
};

} // namespace model

} // namespace basement
