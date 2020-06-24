
#include "stdafx.h"
#include "Id.impl.hpp"

using namespace ::covellite::expanse;

/**
* \brief
*  ������� ��������� ����������� ��������������.
*/
ObjectId_t Id::GetFreeId(void) noexcept
{
  if (!m_FreeIds.empty())
  {
    const auto Result = m_FreeIds.front();
    m_FreeIds.pop();
    return Result;
  }

  return m_NextId++;
}

/**
* \brief
*  ������� �������� �������������� ���������� �������.
*/
void Id::AddRemovedObjectId(const ObjectId_t & _Id)
{
  m_FreeIds.push(_Id);
}

Id::~Id(void)
{
  LOGGER(Info) << uT("Maximum number of objects: ") << m_NextId;
}
