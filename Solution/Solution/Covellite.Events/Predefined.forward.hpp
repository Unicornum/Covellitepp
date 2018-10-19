
#pragma once

/**
* \file
*  ���� ��������������� �����������, ������� ����� �������������� �� ���� 
*  �������� ����������.
*
* \todo
*  �������� boost::any �� std::any ����� ��� ����� �������� (��� Android - 
*  ������� �� ����� ������ Android NDK, � Windows - �������� � ���������� 
*  ������� ��������� ������ ���������).
*/

namespace boost { class any; }

namespace covellite
{

using Any_t = ::boost::any;

} // namespace covellite
