
#pragma once
#include <thread>
#include <glm/glm.force.hpp>
#include "Defines.hpp"
#include "GameObject.hpp"

namespace basement
{

namespace model
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*   ласс входит в проект \ref ExamplePage \n
*   ласс системы частиц.
* \details
*  - ѕроизводительность системы частиц чрезвычайно сильно зависит от того,
*  какую площадь на экране занимают частицы в общей сложности (все упираетс€ в
*  пиксельный шейдер, да), другими словами, чем больше нужно частиц, тем
*  меньшего размера должна быть кажда€ частица.
*  - ѕроизводительность системы частиц заметным образом зависит от размера
*  используемой текстуры, поэтому лучше делает ее изначально как можно меньше.
*  - »спользование OpenMP дл€ циклов обработки частиц дл€ небольшого количества
*  (дес€тки тыс€ч) снижает производительность, дл€ 150к - немного увеличивает,
*  дл€ миллиона fps возрастает с 8 до 14.
*  - –ендеринг буфера част€ми не вли€ет на производительность.
* \note
*  ¬се измерени€ производились на старом ноутбуке.
*
* \version
*  1.0.0.0        \n
* \date
*  02 јвгуст 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Particles final :
  public GameObject
{
  class Particle
  {
  public:
    ::glm::vec3 Position;
    ::glm::vec3 Speed;
    float HalfSize;
    float Time;
  };

  class Instance
  {
  public:
    static String_t GetDescriptor(void) { return uT("i4f4"); }
    int32_t i1, i2, i3, i4; // «аглушка, чтобы провер€ть корректность смещени€
    float x, y, z, HalfSize;
  };

  using ParticlesPtr_t = ::std::shared_ptr<::std::vector<Particle>>;

public:
  Objects_t GetObject(const Any_t &) const override;

private:
  void BuildVertexesParticles(void);
  void BuildInstansedParticles(void);
  Objects_t GetVertexesObject(void) const;
  Objects_t GetInstancedObject(void) const;

private:
  const bool                        m_IsParticleInstance;
  const ::std::size_t               m_Count;
  const float                       m_ParticleHalfSize = 0.03f;
  ParticlesPtr_t                    m_pParticles;
  ::std::vector<Vertex_t>           m_Vertexes;
  ::std::vector<::std::vector<int>> m_Indices;
  //::std::vector<::std::thread>      m_Threads;

private:
  Particles(void);
  friend GameObject;
};

} // namespace model

} // namespace basement
