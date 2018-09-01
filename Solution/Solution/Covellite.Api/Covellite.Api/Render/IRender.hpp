
#pragma once
#include <Covellite/Os/Predefined.hpp>

/**
* \defgroup CovelliteApiRenderGroup Render
* \ingroup CovelliteApiGroup
*  ������ ������� ���������� ���������� ��� ������ ���������� ����������� Api.
*/

namespace covellite
{

namespace api
{

namespace render
{

/**
* \ingroup CovelliteApiRenderGroup
* \brief
*  ����� ������ � ������ \ref CovelliteApiPage \n
*  ������������ ����� ����������� �������.
*  
* \version
*  1.0.0.0        \n
* \date
*  21 ������ 2018    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  � CTAPOBEP 2018
*/
class IRender
{
public:
  /// ����� ��� �������� �������� �������� ��������� ���������� �������.
  class Data final
  {
  public:
    class Color final
    {
    public:
      float R, G, B, A;
    };

  public:
    Handle_t  Handle;       ///< ���� ���� ������������ �������.
    int       Top;          ///< �������� �������� ���� ���������� ����� ����.
    Color     BkColor;      ///< ���� ���� ���� �� ���������.
    bool      IsFullScreen; ///< ������� ������ ��������� � ������������� 
                            ///< ������.
  };

public:
  virtual void ClearWindow(void) = 0;
  virtual void Present(void) = 0;
  virtual void ResizeWindow(int32_t, int32_t) = 0;

public:
  virtual ~IRender(void) {}
};

} // namespace render

} // namespace api

} // namespace covellite
