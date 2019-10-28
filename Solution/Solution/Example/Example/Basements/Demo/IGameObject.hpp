
#pragma once
#include "Defines.hpp"

namespace basement
{

namespace model
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ������������ ����� �������� �������.
*
* \version
*  1.0.0.0        \n
* \date
*  28 ���� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
class IGameObject
{
protected:
  using Objects_t = ::std::vector<Object_t>;

public:
  class Support final
  {
  public:
    enum Value
    {
      Skybox = 0, // ����� ������� �������� �����!
      Camera,

      Invalid,
    };
  };

  class Landscape final
  {
  public:
    enum Value
    {
      // ������� �����������, �� ���������� ���������� ��������
      None = Support::Invalid, // ����� ������!
      Sand,
      Rock,
      Well,

      // ������� �����������, ���������� ���������� �������
      Grass,
      Bush,
      Tree,

      Invalid,
    };
  };

  class Extra final
  {
  public:
    enum Value
    {
      Water = Landscape::Invalid, // ���� - ����� �������� �����������!
      Loader,  // ������ ���������� ���������, �.�. ��� updater �������
               // �������, ������� ��� ��������� � ������ ����������.

      Invalid,
    };
  };

  class Another final
  {
  public:
    enum Value
    {
      Animated = Extra::Invalid,
      Particles,
      Shadows,

      Invalid,
    };
  };

public:
  /**
  * \brief
  *  ������� ��������� ���� �������� �������.
  */
  virtual size_t GetType(void) const = 0;

  /**
  * \brief
  *  ������� ��������� ����������� ������ ����������� �������� �������.
  */
  virtual Objects_t GetObject(const Any_t & = {}) const = 0;

public:
  virtual ~IGameObject(void) = default;
};

} // namespace model

} // namespace basement
