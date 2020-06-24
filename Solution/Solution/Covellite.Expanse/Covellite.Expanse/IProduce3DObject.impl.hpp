
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
*  ������������ ����� ��������/�������� 3D ��������.
*
* \version
*  1.0.0.0        \n
* \date
*  19 ���� 2020    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2020
*/
class IProduce3DObject
{
public:
  /**
  * \brief
  *  ������� ���������� 3D ������� � 3D �����.
  * \details
  *  ��������� ��. � �������� ������ C3DScene.
  */
  virtual void Add(const ObjectId_t, const C3DObject_t &) = 0;

  /**
  * \brief
  *  ������� �������� 3D ������� �� 3D c����.
  * \details
  *  ��������� ��. � �������� ������ C3DScene.
  */
  virtual void Remove(const ObjectId_t) = 0;

public:
  IProduce3DObject(void) = default;
  IProduce3DObject(const IProduce3DObject &) = delete;
  IProduce3DObject(IProduce3DObject &&) = delete;
  IProduce3DObject & operator= (const IProduce3DObject &) = delete;
  IProduce3DObject & operator= (IProduce3DObject &&) = delete;
  virtual ~IProduce3DObject(void) = default;
};

} // namespace expanse

} // namespace covellite
