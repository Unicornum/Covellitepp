
#pragma once
#include <Covellite/Expanse/Defines.hpp>

namespace covellite
{

namespace expanse
{

/**
* \ingroup CovelliteExpanseGroup
* \brief
*  ����� ������ � ������ \ref CovelliteExpansePage \n
*  ��������� ������ 3D �����.
*  
* \version
*  1.0.0.0        \n
* \date
*  18 ���� 2020    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2020
*/
class I3DScene
{
public:
  /**
  * \brief
  *  ������� ���������� �������������� ������� � ������� ����������.
  * \details
  *  ��������� ��. � �������� ������ C3DScene.
  */
  virtual void Add(const size_t, const ObjectId_t, const size_t = 0) = 0;

  /**
  * \brief
  *  ������� ���������� �������������� ������� � ������� ����������.
  * \details
  *  ��������� ��. � �������� ������ C3DScene.
  */
  virtual void Add(const ObjectId_t) = 0;

public:
  virtual ~I3DScene(void) = default;
};

} // namespace expanse

} // namespace covellite
