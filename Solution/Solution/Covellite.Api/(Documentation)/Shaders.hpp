
/**
* \page CovelliteApiShadersPage �������
*  
*  ������ ��� ���� ������� (���������� � �������� ���������� \b data � \b count)
*  ����� �������� �����������, ��� ����� �������� ��������� � �����.
*  
*  ���������
*  ---------
*  
* ### ������ �������
*  
* \code
#version 330 core // ������ ��� OpenGL
#version 300 es // ������ ��� OpenGLES3
* \endcode
*
* ### ���������������� ������� ���� �������
*  
*  - \b COVELLITE_SHADER_DESKTOP ��� \b COVELLITE_SHADER_MOBILE
*  - \b COVELLITE_SHADER_HLSL ��� \b COVELLITE_SHADER_GLSL
*  - \b COVELLITE_SHADER_VERTEX ��� \b COVELLITE_SHADER_PIXEL
*  
* ### ���� ��������������� ����������� ��� �������� ��������������� ��������
*  
* \include ./Solution/Covellite.Api/Covellite.Api/Renderer/Shaders/Predefined.fx
*
* ### ���� ��������� �������� ��� �������� ������ �������
*  
* \include ./Solution/Covellite.Api/Covellite.Api/Renderer/Shaders/Data.h
*
* ### ���� ���������� �������� ������� ������ ��������
*
* \include ./Solution/Covellite.Api/Covellite.Api/Renderer/Shaders/Input.fx
*  
*  ���� �������
*  ------------
*  
*  ���� ������� ������ ��������� ����������� �������, ��� ������� ����������
*  ���������� ������� ��� �������� \b entry. ��� ���� ������� ����� ���� �����,
*  �� ��������� �� ������ ��������������� ����� �� ���������� ������ (���
*  �������� ��������� ���������� ��� ������� � ������ ������������ ��� ������):
*
* \code

Pixel entry(Polygon _Value); // ���������� ������ ��� ���������� 2D �������� (deprecated)
Pixel entry(Polyhedron _Value); // ���������� ������ ��� ���������� 3D �������� (deprecated)
Pixel entry(Vertex _Value); // ���������� ������
float4 entry(Pixel _Value); // ���������� ������

* \endcode
*  
*  ���� ������� �������� ������ ������� (��������� \b data � \b count),
*  � �������� ���� ������� ����� ����������� ���������� �����, ���� ���,
*  �� ������ ��� ����� �������������� ������� �� ��������� (��, ��� ����������
*  ��� ��������� ��� ���������� Gui):
*
* \include ./Solution/Covellite.Api/Covellite.Api/Renderer/Shaders/Default.fx
*  
*  �����
*  -----
*  
*  � ����� ���� ������� ����������� ����������� �������, ������� �����
*  �������� �������, ��������� � �������� ��������� \b entry (��� �������
*  ��� ����������� �������� ������� ���������������� HLSL � GLSL ���� �������).
*  
* ### HLSL (���������� ������)
*  
*  ������ �� �����������, ��������� ������� ���������� ��������.
*  
* ### HLSL (���������� ������)
*  
* \code

float4 psMain(Pixel _Value) : SV_Target
{
  return entry(_Value);
}

* \endcode
*  
* ### GLSL (���������� ������)
*
* \code

out Pixel PixelValue;
void main()
{
  Vertex InputData;
  InputData.Position = CovelliteVertexPosition;
  InputData.TexCoord = CovellteVertexTexCoord;
  InputData.Extra = CovelliteVertexExtra;
  PixelValue = entry(InputData);
  gl_Position = PixelValue.ScreenPos;
}

* \endcode
*
* ### GLSL (���������� ������)
*
* \code

in Pixel PixelValue;
out vec4 OutPixelColor;
void main()
{
  OutPixelColor = entry(PixelValue);
}

* \endcode
*/
