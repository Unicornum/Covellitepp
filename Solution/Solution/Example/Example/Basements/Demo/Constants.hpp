
#pragma once
#include <chrono>
#include <alicorn/cpp/math.hpp>
#include <alicorn/std/string.hpp>
#include <Covellite/App/Settings.hpp>

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
TODO("Перенести в alicorn/cpp/math.hpp");
template<class T>
class Constant final
{
public:
  static constexpr T Pi = static_cast<T>(math::PI);
  static constexpr T RadianToGreed = static_cast<T>(math::RadianToGreed);
};

} // unnamed namespace

} // namespace math

} // namespace cpp

} // namespace extension

} // namespace alicorn

namespace basement
{

namespace
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс-контейнер констант, используемых в коде демонстрационного режима.
*  
* \version
*  1.0.0.0        \n
* \date
*  11 Апрель 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Constant final
{
  using TimePoint_t = ::std::chrono::time_point<::std::chrono::system_clock>;
  using String_t = ::alicorn::extension::std::String;

public:
  class Camera final
  {
  public:
    static const float Distance;
    static const float Fov;
    static const float Pitch;

    // Дистанция смещения камеры назад подбирается таким образом, чтобы угол
    // Fov обеспечивал отсутствие артефактов по краям изображения.
    static constexpr auto BackOffset = 7.5f;
    static const float NearClippingPlane;
    static const float FarClippingPlane;

  public:
    inline static float Squaring(const float _Value) { return _Value * _Value; }
  };

public:
  class Player final
  {
  public:
    static const float Height;
    static const String_t ComponentPositionId;
    static const String_t ComponentRotationId;
  };

public:
  static const TimePoint_t BeginTime;
  static constexpr auto TimeStepSecond = 0.5f;
  static const int CellRadius;
  static const int CellSize;

public:
  template<class T>
  inline static T GetSettings(const ::alicorn::extension::std::String & _Name)
  {
    return ::covellite::app::Settings_t::GetInstance()[uT("Demo")].Get<T>(_Name);
  }
};

namespace math = ::alicorn::extension::cpp::math;

/*static*/ const float Constant::Camera::Distance =
  1.5f;
/*static*/ const float Constant::Camera::Fov =
  math::Constant<float>::Pi / 3.0f;
/*static*/ const float Constant::Camera::Pitch = - math::Constant<float>::Pi *
  (::alicorn::extension::cpp::IS_DEBUG_CONFIGURATION ? 0.25f : 0.22f);
/*static*/ const float Constant::Camera::NearClippingPlane =
  Constant::Camera::Squaring(Constant::Camera::BackOffset - 0.75f);

/*static*/ const float Constant::Player::Height =
  1.75f;
/*static*/ const Constant::String_t Constant::Player::ComponentPositionId =
  uT("Demo.Player.Position");
/*static*/ const Constant::String_t Constant::Player::ComponentRotationId =
  uT("Demo.Player.Rotation");

/*static*/ const Constant::TimePoint_t Constant::BeginTime =
  ::std::chrono::system_clock::now();
/*static*/ const int Constant::CellRadius = 
  static_cast<int>(1.33f * 
  (-Constant::Player::Height + Constant::Camera::Distance * 
    math::radian::Sin(Constant::Camera::Pitch)) / math::radian::Tan(
      Constant::Camera::Pitch + Constant::Camera::Fov / 2.0f));
/*static*/ const int Constant::CellSize =
  2 * CellRadius + 1;

/*static*/ const float Constant::Camera::FarClippingPlane = 
  Constant::Camera::Squaring(
    Constant::Camera::BackOffset + Constant::CellRadius - 1);

} // unnamed namespace

} // namespace basement
