
#pragma once
#include <string>
#include <vector>
#include <GLMath.hpp>
#include <Covellite/Api/robin_hood.hpp>

namespace basement
{

namespace model
{

namespace animation
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ����� ���������� �� ����� ����� ������� ������������� ������.
*
* \version
*  1.0.0.0        \n
* \date
*  08 ���� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
class Bone final
{
public:
  using Index_t = ::std::size_t;
  static const Index_t iNonexistent = static_cast<Bone::Index_t>(-1);

public:
  ::std::string Name;
  Index_t iParent;
  ::glm::mat4 Global;
  ::glm::mat4 Local;
};

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ��� ������ �������.
* \details
*  - ������ � ����� ������� ������ � ������ ������� ������������� ������ ������
*  ������������ � �������� ��������� \b skeleton ���������� \b Data.Skin.
*
* \version
*  1.0.0.0        \n
* \date
*  08 ���� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
using Skeleton_t = ::std::vector<Bone>;

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ����� �������� ������ �� ����� ������� ������������� ������.
*
* \version
*  1.0.0.0        \n
* \date
*  08 ���� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
class SkinVertex final
{
  using iVertex_t = ::std::size_t;

public:
  ::glm::vec4 Position;
  ::glm::vec4 Normal;
  ::std::vector<iVertex_t> iCompleteVertexes;
  ::std::vector<::std::pair<Bone::Index_t, float>> WeightBones;
};

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ��� ������ ������ ������ ������������� ������.
* \details
*  - ������ � ����� ������� ������ �� �������� ���� ������������� ������
*  ������ ������������ � �������� ��������� \b vertexes ���������� \b Data.Skin.
*
* \version
*  1.0.0.0        \n
* \date
*  08 ���� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
using Skin_t = ::std::vector<SkinVertex>;

class Mat4 final
{
public:
  // ��� ����� ��� ����, ����� ��� �������������� ����������� ��������
  // ������������ ��������� �������.
  ::glm::mat4 Value = ::glm::mat4{ 1.0f };
};

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ��� ������ ������ ������ ����� ��������.
* \details
*  - ������ � ����� ������� ������ �������� ������ ������������ � ��������
*  ��������� \b frames ���������� \b Animation.
*
* \version
*  1.0.0.0        \n
* \date
*  09 ���� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
using Frames_t = ::std::vector<::std::pair<float,
  ::robin_hood::unordered_map<::std::string, Mat4>>>;

} // namespace animation

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  ����� ������ � ������ \ref ExamplePage \n
*  ��� ��� ������� � ������ ����� ��������.
*
* \version
*  1.0.0.0        \n
* \date
*  09 ���� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
class Animation final
{
  using Frame_t = ::std::vector<::glm::mat4>;
  using ComponentPtr_t = ::std::shared_ptr<::covellite::api::Component>;

public:
  ::std::size_t GetFrameIndex(const float) const;
  Frame_t GetFrame(const ::std::size_t, const animation::Skeleton_t &) const;

private:
  ::glm::mat4 Get(::std::size_t, const ::std::string &) const;

private:
  const float m_TicksPerSecond;
  const animation::Frames_t m_RawFrames;
  mutable ::std::vector<Frame_t> m_Frames;

public:
  Animation(const float, const animation::Frames_t &);
};

} // namespace model

} // namespace basement
