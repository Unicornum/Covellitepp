
#pragma once
#include <utility>
#include <alicorn/cpp/math.hpp>

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

  inline CubeCoords TurnLeft(void) const { return CubeCoords{ -Y, X + Y }; }
  inline CubeCoords TurnRight(void) const { return CubeCoords{ X + Y, -X }; }

  inline ::std::pair<float, float> ToPlane(void) const
  {
    namespace math = ::alicorn::extension::cpp::math;

    return { 1.5f * X, math::Root<2>(3.0f) * (Y + X / 2.0f) };
  }

private:
  int X, Y;

public:
  explicit CubeCoords(const int _X = 0, const int _Y = 0) :
    X(_X),
    Y(_Y)
  {

  }
};

} // namespace model

} // namespace basement
