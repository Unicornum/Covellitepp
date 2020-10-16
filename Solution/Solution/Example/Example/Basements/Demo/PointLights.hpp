
#pragma once
#include "Lights.hpp"

namespace basement
{

namespace model
{

class CubeCoords;

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс набора точечных источников света сцены.
*  
* \version
*  1.0.0.0        \n
* \date
*  15 Сентябрь 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class PointLights final
{
public:
  void Add(const CubeCoords &, const float);
  void Remove(const CubeCoords &);
  Points_t Get(const CubeCoords &, const float) const;
  const Points_t & GetUserConstantBuffer(const CubeCoords &, const float) const;

private:
  class Data;
  ::std::shared_ptr<Data> m_pData;
  mutable ::std::size_t m_MaxSceneLightCount = 0;
  mutable ::std::size_t m_MaxObjectLightCount = 0;

public:
  PointLights(void);
  ~PointLights(void) noexcept;
};

} // namespace model

} // namespace basement
