
#pragma once
#include <queue>
#include <Covellite/Expanse/Defines.hpp>

namespace covellite
{

namespace expanse
{

/**
* \ingroup CovelliteExpanseGroup
* \brief
*  ����� ������ � ������ \ref CovelliteExpansePage \n
*  ����� �������������� �������.
* \details
*  - ����� ��� ����, ����� �������������, ��� ������� ��������/3D �������
*  ������������� ���������� ������������� � ��� ���� �������������� ����
*  �������� �������� �������� ����������� ������������������ �� 0 ��
*  ������������� �������� (����� 3D ������� ����� ������� � vector'e �
*  �������� �� ����� ������).
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
class Id final
{
public:
  ObjectId_t GetFreeId(void) noexcept;
  void AddRemovedObjectId(const ObjectId_t & _Id);

private:
  ObjectId_t m_NextId = 0;
  ::std::queue<ObjectId_t> m_FreeIds;

public:
  Id(void) = default;
  Id(const Id &) = delete;
  Id(Id &&) = delete;
  Id & operator= (const Id &) = delete;
  Id & operator= (Id &&) = delete;
  ~Id(void);
};

} // namespace expanse

} // namespace covellite
