
#pragma once
#include <map>
#include <alicorn/std/string.hpp>
#include <Covellite/Predefined.hpp>

namespace covellite
{

namespace api
{

/**
* \ingroup CovelliteApiGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ����� ����������.
* \details
*  - �������� � ������������� ������ � �������� ���������� ��� �������� 
*  ����������� ��������, �� ������� ����������� ������� 3D �����.
*  - ��������������, ��� �������� ���������� � ���������� ����� ��������
*  � ����� xml �����, �� ������� ��������� ����� ����������� ��� ��������, 
*  � ������� ��� �������� ����� ��������� ���������, � �������� �������� - 
*  ��������� ���������.
*  - �������������� ���������� ����� ����������� � ���� �����, � ����������
*  �������� ���������� ����� ��������������� ������ ���� (��� ��������������
*  �������������� � ������ � �������).
*  - ��������� (����� ������������) ����� �������������, ��� ����� ���������� 
*  ������������� ������� �������� �� ���������.
*  - ������������ �������� \b id - ������������� ����������. ��� �����������
*  � ����� � ��� �� ��������������� ����� �������������� ���� � ��� �� ������,
*  ������� �������, ���������� ���� � ��� �� ���, ����� ��������� ����������,
*  � ������� ����� ������ (���� � ��� ��) �������������, � �����������
*  �������� ����� ���������� ����� ������� � ������ ����� � ������� ��� ������
*  �������.
*  - ������������ �������� \b type - ��� ����������, ������� ���������� ������
*  ��������� ����� ����������. ���������� ��������:
*   + Camera.
*   + State (\b kind - Blend/Sampler/Scissor; ��� Scissor - \b is_enabled, 
*  \b left, \b right, \b top, \b bottom).
*   + Position (\b x, \b y, \b z).
*   + Buffer (\b kind - Vertex/Index, \b data, \b count).
*   + DrawCall.
*   + Texture (\b data, \b count, \b width, \b height).
*   + Shader (\b kind - Vertex/Pixel, \b data, \b count, \b version, \b entry).
*  
* \version
*  1.0.0.0        \n
* \date
*  24 ������� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
class Component final
{
  template<bool>
  class Convertor;
  class ConstructorTag {};

public:
  using String_t = ::alicorn::extension::std::String;
  using Name_t = String_t;
  using Params_t = ::std::map<Name_t, ::covellite::Any_t>;
  using Type_t = String_t;
  using Id_t = String_t;
  using ComponentPtr_t = ::std::shared_ptr<Component>;

public:
  class Renders;

public:
  static ComponentPtr_t Make(const Params_t &);

public:
  template<class T>
  T GetValue(const Name_t &, const T &) const;
  template<class T>
  void SetValue(const Name_t &, const T &);

private:
  Params_t m_Params;

public:
  const Type_t Type;
  const Id_t Id;

public:
  Component(const Params_t &, ConstructorTag);
};

} // namespace api

} // namespace covellite
