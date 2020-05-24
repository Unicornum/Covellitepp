
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
*  ����� ������ � ������ \ref ExamplePage \n
*  ����� ������� ������.
* \details
*  - ������������������ ������� ������ ����������� ������ ������� �� ����,
*  ����� ������� �� ������ �������� ������� � ����� ��������� (��� ��������� �
*  ���������� ������, ��), ������� �������, ��� ������ ����� ������, ���
*  �������� ������� ������ ���� ������ �������.
*  - ������������������ ������� ������ �������� ������� ������� �� �������
*  ������������ ��������, ������� ����� ������ �� ���������� ��� ����� ������.
*  - ������������� OpenMP ��� ������ ��������� ������ ��� ���������� ����������
*  (������� �����) ������� ������������������, ��� 150� - ������� �����������,
*  ��� �������� fps ���������� � 8 �� 14.
*  - ��������� ������ ������� �� ������ �� ������������������.
* \note
*  ��� ��������� ������������� �� ������ ��������.
*
* \version
*  1.0.0.0        \n
* \date
*  02 ������ 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
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
    int32_t i1, i2, i3, i4; // ��������, ����� ��������� ������������ ��������
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
