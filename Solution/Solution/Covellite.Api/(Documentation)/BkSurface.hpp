
/**
* \page CovelliteApiBkSurface ��������� �� ����������� �����������
*  
*  ��������� �� ����������� ����������� ��������� ����������� ���������
*  � ��������(�) �������(��) ����� ����� ������������ ��� ���������� ������
*  �����. ��� ������������ ��� ��������������� ���������� � ����������
*  ����� ������ ��� ���������� ���������, ���� �� �������� �
*  [�������������](https://habr.com/ru/post/453692/).
*  
*  ��������� �� ����������� �����������
*  ====================================
*  
*  ������ ������������� ���������� �� ����������� ����������� �������������
*  ��������� � ��������� ��������.
*  - ������ ������:
*   + �������� ������� ���������� \b BkSurface � ������ ������������ �������,
*     � ������� � ����� ������������ ������.
*   + ���������� � ������������� ��������� ����������� ������� ��� ������
*     � ��������� ������� ������������.
*  - ����������� �������:
*   + �������� �������� �������, ���������� ������, ���������� �� ������
*     �������.
*  
*  BkSurface
*  ---------
*  
*  ����� ������������ �� �������, ����������� ������������, �����������
*  �������� ���������� � �������������� � ��� ���������. ������� ���������,
*  ��� ������ ���������� \b Camera ��������� ����� �������������� �����������
*  ���������� (�.�. ����������� ����� �� �����), � ���������� \b State ��������
*  � ������� ������������ ����������, ������� ��������� \b BkSurface ����������
*  ������� ����� ������������ ������ � �������� �������� ����������.
*  
*  ��������� \b BkSurface ������� ���� ��� ��������� �����������
*  \b Data.Texture (����� ������������ �� 0 �� 8 ������� ��� ��������
*  ����������� � ���� ��� ������ �������), ������� �� ��������� � �������� ����
*  ���������� ���� ����������� ��������:
*  - ��� ���� ���������� (���� ��� �������) ������������ �� ���������
*  \b destination ���������� \b Data.Texture ('depth' ��� ������ ������� � ����
*  ��� ������ ������� ��������).
*  - ������ �������� ����������� (������� � ����) ������������ �������� ��������
*  ����������� \b Data.Texture ���������� \b BkSurface.
*  
* \note
*  - ��������� \b BkSurface ������� ����� �������� ��� ���������� ���
*  ����������� \b Data.Texture, ������ �� ������� ����� ��������� � ���������
*  viewport'a ������, ��������� � ������ (���������� �������� ����������
*  \b width � \b height ����� ���������������).
*  - ���������� ����������� \b Data.Texture ����� ����������� ������� ���������
*  ������� ��� �������� ���������� \b width � \b height.
*  - ��� ��������� �������� ���� ��������� �������� ����� ����������� 
*  � ������ ���������, �������� ���������� \b width � \b height ����� �����
*  ������������.
*  
*  ��������� �������
*  -----------------
*  
*  ��� ������������� ��������� ������������ � ��������� ������� ��������
*  ����������� ���������� ��� ���������� �������� ������������ ����������
*  ������, ������������ ����������� ������ ����������� ������� �����������
*  ���������.
*  
* \note
*  ������ ��������� ��������� ������������� ��������� ��������� ���������������
*  �������, ������� ����� ��������� ��������������� � �������� ��� ����������
*  DirectX � OpenGL/ES3.
*  
*  ���������� (��� ��������� ����� ���� �����, ����� - ���������� ����� ������,
*  �� 1 �� 8):
*  
* \code
COVELLITE_DECLARE_MULTI_TARGET_STRUCTURE(PsOutput, 3);
* \endcode
*  
*  �������������:
*  
* \code
PsOutput psExample(Pixel _Value)
{
  ...
  PsOutput Result;
  Result.Target[0] = float4(...);
  Result.Target[1] = float4(...);
  Result.Target[2] = float4(...);
  return Result;
}
* \endcode
*  
*  ������������� �������
*  ---------------------
*  
*  ��� ������������� ���������� �� ������ ������� ������� ��� ����������
*  �� ��������� �������� ���������� ������� ������� ������� ������� �����������
*  \b Texture, ������� �� �� �� ����� ������� ����������� \b Data.Texture,
*  ��� ���� �������� ���������� \b BkSurface �� ������ �������.
*  
* \note
*  ��� ����, ����� �� ����������� �������� �������������� ������ �� ��������,
*  ������� ����������� ��� ���������� �� ����������� �����������, ����������
*  ���������� ���������� �������� ����������� \b Data.Texture, ����������
*  ���������� \b BkSurface � ����������� ������� �� ������ ��������. ��� ������
*  ���� �� �� ����� ������� ������� �����������, ���������� ���������������
*  ������������!
*/
