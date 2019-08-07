
#pragma once
#include <thread>
#include <GLMath.hpp>
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
public:
  Objects_t GetObject(const Any_t &) const override;

private:
  void BuildParticles(void);

private:
  class Particle
  {
  public:
    ::glm::vec3 Position;
    ::glm::vec3 Speed;
    float HalfSize;
    float Time;
  };

private:
  ::std::shared_ptr<::std::vector<Particle>> m_pParticles;
  ::std::vector<Vertex_t>                    m_Vertexes;
  ::std::vector<::std::vector<int>>          m_Indices;
  ::std::vector<::std::thread>               m_Threads;

private:
  Particles(void);
  friend GameObject;
};

} // namespace model

} // namespace basement
