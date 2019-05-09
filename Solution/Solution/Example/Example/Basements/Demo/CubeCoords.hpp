
#pragma once

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
    static_cast<uint32_t>(1) << (8 * sizeof(int32_t) - 1);

public:
  inline int GetX(void) const { return X; }
  inline int GetY(void) const { return Y; }
  inline int GetZ(void) const { return -(X + Y); }

public:
  CubeCoords operator + (const CubeCoords &) const;
  CubeCoords operator - (const CubeCoords &) const;
  CubeCoords operator * (const int32_t) const;
  CubeCoords TurnLeft(void) const { return CubeCoords{ -Y, X + Y }; }
  inline CubeCoords TurnRight(void) const { return CubeCoords{ X + Y, -X }; }
  CubeCoords TurnLeft(const uint32_t) const;
  CubeCoords TurnRight(const uint32_t) const;
  ::std::pair<float, float> ToPlane(void) const;
  uint64_t GetHash(void) const;

private:
  int32_t X, Y;

public:
  explicit CubeCoords(const int = 0, const int = 0);
  explicit CubeCoords(const uint64_t);
};

inline CubeCoords CubeCoords::operator + (const CubeCoords & _CubeCoords) const
{
  return CubeCoords{ X + _CubeCoords.X, Y + _CubeCoords.Y };
}

inline CubeCoords CubeCoords::operator - (const CubeCoords & _CubeCoords) const
{
  return CubeCoords{ X - _CubeCoords.X, Y - _CubeCoords.Y };
}

inline CubeCoords CubeCoords::operator * (const int32_t _Value) const
{
  return CubeCoords{ X * _Value, Y * _Value };
}

inline uint64_t CubeCoords::GetHash(void) const
{
# if BOOST_COMP_MSVC
#   pragma warning(push)
#   pragma warning(disable: 26451)
# endif

  return (static_cast<uint64_t>(Y + HalfRange) << Offset) +
    static_cast<uint64_t>(X + HalfRange);

# if BOOST_COMP_MSVC
#   pragma warning(pop)
# endif
}

} // namespace model

} // namespace basement
