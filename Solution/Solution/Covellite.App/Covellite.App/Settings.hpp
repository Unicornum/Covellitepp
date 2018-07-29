
#pragma once
#include <alicorn\std\singleton.hpp>
#include <alicorn\settings.hpp>

namespace covellite
{

namespace app
{

using Section_t = alicorn::modules::settings::Section;
using SectionPtr_t = ::std::unique_ptr<Section_t>;

/**
* \ingroup CovelliteAppGroup
* \brief
*  ����� ������ � ������ \ref CovelliteAppPage \n
*  ����� ������� � ���������� ����������.
* \details
*  - ������������ ��������� ��������� ������ ���������� ������� Make(),
*  ������� ������ �������� � ���������� �������� ���������� �� ���������.
*  
* \snippet .\Solution\Covellite.App\Test.Covellite.App\Test.Settings\Settings_test.cpp SetDefaultValues
*  
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
* \date
*  07 ������ 2017    \n
*  23 ������� 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2017 - 2018
*/
using Settings_t = ::alicorn::extension::std::Singleton<const Section_t &>;

} // namespace app

} // namespace covellite

namespace alicorn
{

namespace extension
{

namespace std
{

using namespace ::covellite::app;

template<>
/*static*/ SectionPtr_t Singleton<Section_t>::Make(void);

} // namespace std

} // namespace extension

} // namespace alicorn
