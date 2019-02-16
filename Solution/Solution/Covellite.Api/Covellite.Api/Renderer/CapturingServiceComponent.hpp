
#pragma once
#include <deque>
#include <alicorn/std/string.hpp>
#include <Covellite/Api/Component.hpp>

namespace covellite
{

namespace api
{

namespace renderer
{

/**
* \ingroup CovelliteApiRendererGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ����� ��� ������ �� ���������������� ������������.
* \details
*  - ��������������� ���������� - ����������, ��� ������� �� ��������� �������,
*  � ������ ����� ��� ������ ���� ��������� � �������������� ��������� ������ 
*  �����������; ����� ������������� ������� ��� ��������� ��������� 
*  ��������������� �����������.
*  
* \version
*  1.0.0.0        \n
* \date
*  13 ������� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2019
*/
class CapturingServiceComponent final
{
  using Kind_t = ::alicorn::extension::std::String;
  using ComponentPtr_t = ::std::shared_ptr<Component>;
  using Expected_t = ::std::pair<Kind_t, ComponentPtr_t>;
  using Hadler_t = ::std::function<void(const ComponentPtr_t &)>;
  using Handlers_t = ::std::map<Kind_t, Hadler_t>;

public:
  void Add(const ComponentPtr_t &);
  ::std::vector<ComponentPtr_t> Get(const ::std::vector<Expected_t> &);
  void Process(const Handlers_t &);

private:
  ::std::deque<ComponentPtr_t> m_Components;
};

} // namespace renderer

} // namespace api

} // namespace covellite
