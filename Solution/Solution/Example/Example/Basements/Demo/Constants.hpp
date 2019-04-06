
#pragma once
#include <chrono>
#include <alicorn/cpp/math.hpp>
#include <alicorn/std/string.hpp>

namespace alicorn
{

namespace extension
{

namespace cpp
{

namespace math
{

namespace
{

// 22 Март 2019 18:04 (unicornum.verum@gmail.com)
TODO("Перенести в alicorn::cpp::math");
template<class T>
class Constant final
{
public:
  static constexpr T PI = static_cast<T>(math::PI);
};

} // unnamed namespace

} // namespace math

} // namespace cpp

} // namespace extension

} // namespace alicorn

namespace basement
{

namespace model
{

namespace
{

class Constant final
{
  using Clock_t = ::std::chrono::system_clock;
  using TimePoint_t = ::std::chrono::time_point<Clock_t>;
  using String_t = ::alicorn::extension::std::String;

public:
  static const TimePoint_t BeginTime;
  static constexpr auto TimeStepSecond = 0.5f;
# ifdef _DEBUG
  static constexpr auto CellRadius = 12;
# else
  static constexpr auto CellRadius = 30;
# endif
  static constexpr auto CellSize = 2 * CellRadius + 1;

public:
  class Player final
  {
  public:
    static constexpr auto Height = 1.75f;
    static const String_t ComponentPositionId;
    static const String_t ComponentRotationId;
  };
};

/*static*/ const Constant::TimePoint_t Constant::BeginTime = 
  Clock_t::now();
/*static*/ const Constant::String_t Constant::Player::ComponentPositionId =
  uT("Demo.Player.Position");
/*static*/ const Constant::String_t Constant::Player::ComponentRotationId =
  uT("Demo.Player.Rotation");

} // unnamed namespace

} // namespace model

} // namespace basement
