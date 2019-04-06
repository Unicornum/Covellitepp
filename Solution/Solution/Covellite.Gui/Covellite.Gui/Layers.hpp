
#pragma once
#include <vector>

namespace std { template<class> class shared_ptr; }

namespace covellite
{

namespace gui
{

class ILayer;

/**
* \ingroup CovelliteGuiGroup
* \brief
*  ����� ������ � ������ \ref CovelliteGuiPage \n
*  ����� ������ �����.
* \details
*  - ������������ ����� ����, � ������� �������� �������� ����, �����������
*  �� ��� �������.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
* \date
*  09 ������� 2016    \n
*  16 ���� 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2016 - 2019
*/
class Layers final
{
  using LayerPtr_t = ::std::shared_ptr<ILayer>;

private:
  ::std::vector<LayerPtr_t> m_Layers;

public:
  void Push(const LayerPtr_t &);
  bool Pop(void);
};

} // namespace gui

} // namespace covellite
