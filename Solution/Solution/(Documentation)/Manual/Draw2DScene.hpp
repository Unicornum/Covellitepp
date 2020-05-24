
/**
* \page Draw2DSceneBriefPage
* 
*  �������� ��������� � ���� ��������� ������� �������� � �������������� 
*  [������������ �������](\ref CovelliteApiComponentSystemPage).
*/

/**
* \page Draw2DScenePage ��������� 2D ��������
*  
* \copydoc Draw2DSceneBriefPage
*  
*  ������������ �������
*  ====================
*
* \copydoc CovelliteApiGlossaryPage
*  
* \note
*  - � ������ �������� ��� \b �������� ��������������� '������ 3D �����', 
*  � ��� \b ������ - '3D �����'.
*  - ����������� ���������� ��� State.Scissor ������������� ������
*  ����������, � �������� ����������� ��������� type == 'State' � 
*  kind == 'Scissor'.
*  - ��� \b �������� \b ���������� ��������������� ������ C++ ������ ����������.
*  .
*  ��������� � ����������� � �� ����������
*  ��. \ref CovelliteApiComponentSystemPage.
*  
*  ����� ��������
*  ==============
*  
*  ���� [������������ �������](\ref CovelliteApiComponentSystemPage) 
*  ������������� - � ������ ������� - ��� ��������� ���������� ����, 
*  ��� �� ����� ��� ��������� ����� �������� � ���� ������� ������� ������� 
*  (������ ���� ����� ������������ ��� ��������� GUI). ���� ������� � ���, 
*  ��� ��� ������ ����������� ������������� ������ (������� '�������' 
*  �� ��������� Oxy �� ������� ������������� �������� ��� z) ������������ 
*  �������, ��������� �� �������������, ������� ������� ����� � ��������� Oxy.
*  
*  ������������ ������ ������:
*
* \snippet .\Solution\Example\Example\Basements\Simple2DGame.hpp Vertex format
*  
*  ��������� ������������ ��� ������ ������������ \b �����, ���������� �������:
*  - \b ������ - ���������� ������, ������������ ������ ��������� �����.
*  - \b ����� \b �������� (��������, ��������� �������� ����������).
*  - \b ��������, ������� ��������������� ��������� ��������� �����������
*  �� ������.
*  
*  ���� ����� ���� ���������, ��� ���������� ���������� ���� ������� �����
*  ����� �������������� ������ ����������; ����� ����� ������������ ���������
*  ��������� ����, ������ �� ������� ������������ � ����������� �� ����� 
*  �������.
*  
*  �������� � �������� �������� ��� ����������� ������������ ����� ������,
*  ���������� ��� ������ ������� covellite::api::IWindow::GetRenders().
*  
* \warning
*  ���������� OpenGL �������, ����� ������� ����������� � ��������������
*  � ��� �� ������, � ������� ���� ������� ���� ������������ Api.
*  
* \note
*  ��� �������� �������� ��� ������ \b �������� \b ����������� � ����� � ��� �� 
*  \b id ����� ��������� ���� � ��� �� ������ ������� (��� ���������, ��������, 
*  ���� ��� ������������ ����� ����� ��� ���� �������� ��������, � ����� 
*  ���������� � ��� ����� \b ������� \b �����������, ���������� ������ 
*  ��������������), �� ��� ���� ������� ���������, ��� ��� ��������� ����� 
*  ����������� ������� �� ��������� (��������, ���������� Data ������ ����� 
*  ������������� � ������������ ��������� ������ �����������). ��� ����� 
*  ����������� �������� \b id �� ����� ������ � ��� ������������� ������������� 
*  ����������� ��������� ������ � ���� �� ���������� (��������, ��� ����������� 
*  ����������� ������ ��������), ������� ������������ ���� � ��� �� \b ������ 
*  \b ����������.
*  
*  ������ ������� ����� ������������ �������, ����������� ���� � �� �� 
*  ��������, � ���� ������ ��� �������� �������� ����� ������� ��� ������� 
*  (���� ���) �������, � ���������� ������������ �� ����� ���������� 
*  � ���������������� id.
*  
*  ������ �������� ������� ��������� ������, ������� � ���������� �����
*  �������������� ��� ���� ��������:
*  
* \snippet .\Solution\Example\Example\Basements\Simple2DGame.cpp Common objects
*
* \note
*  ����� � �������� � �������� �������� ����� �������������� ��������������,
*  ��������� �� ���� �������������, �� �� �������� ������� ����� ���� �����
*  ����� �� ������ ���������� ������������� (������������ �������� ����
*  ����������� ������).
*
*  ������ � ��������� �������� ����������
*  --------------------------------------
*  
* \note
*  ����� � ����� ��� ����������� ������ �������� ����������� � ����� �����
*  ��������, � \b ����� ������������ ����� ������ ��������������� ��������
*  � ���� ������.
*  
*  � ������ ������� ����������� ������, ��������� �� ��������:
*  - ��������� �������� ������ ����� �������, ����� ����� ���� ������ ��������
*  � ������ ���������.
*  - ��������������� ������.
*  - �������, ������������ ������������ ������������.
*  - ������� �������� ������ ��������.
*  - ������� ������������� ����������� �������:
*  
* \snippet .\Solution\Example\Example\Basements\Simple2DGame.cpp Create camera
*  
*  ���� ������ �� ���������� ����� �������, ��������� �������� ������������
*  � ������� ������� �� �������� (��, ��� ������� ����� ����� ���������� ������
*  ���, ������� ���� ������� �����), ��� ���������� ������ ������� ���������
*  �������� ����� ������������ �� ������������� �� ��� z (��������� ���
*  ��������� �������� ��. � �������� ���������� ��������).
*  
* \note
*  ��� ������������� ������ ������� ������� ���������, ��� ��������������
*  ����� ������ �� �������, ������� ��������� ����� ��� z � ��������� ���������
*  [-1.0f...1.0f], � ������ �������������� ���������� ��������������� (����, 
*  ��������, �������� z � Data.Scale ����� ���������� � 0.0f, �� ���������� �� 
*  ��������� �� ��� z ��� ������� ����� �������������� � ������� ������� ��
*  ��������).
*  
*  ��������� ��������
*  ------------------
*  
*  ������, ������������ �� ������, ������������ ����� ����� ��������, �������
*  ��������� �� ������ ������ ����������� � ������ ��������:
*  - ��������� ������.
*  - ���������� ������.
*  - �������� (������ ��� ���������������� ��������).
*  - ���������� �����.
*  - ��������� �����.
*  - ���������� ���������/����������/��������������� (�� �����������).
*  - Present.Geometry (������ �� ������������ ��������� �������, �������
*  ������ ������������� ���������).
*  
*  ��������� ��� ��������� ���������� (OpenGL ��� DirectX) ��������� �������
*  � ������ ��������, ��� ����������� �������������������� ���� ���������� 
*  ������������ ������� ��� ���� �������������� ���������� (��������, ���
*  DirectX ������������ ��������� Data.Shader.HLSL); �� ���������� ��� 
*  �������� ������������ Api ����� ��������������� ��� �������� ��������.
*  
*  ���������� ����� ����������� � ����������� �� ����, ����� ������
*  (������ ����������� ������ ��� ����������������) ����� ������������.
*  
*  ���������� Data.Position/Data.Rotation/Data.Scale ������������ ��� 
*  �����������/��������/��������� �������� �������� ��� �� ������������
*  (���������� ���������� ����� �������� ���������� ��� ���������� �����), 
*  ������:
*  - ����������� ������ ���� ����� ���� ��������� (��� ���������, ��������,
*  ��������� ������ ��������, ������� ������ ���� ����������� ������������
*  ���� ����� ������������ �������, �� ��� ���� ���������/��������� ������;
*  �� ������ ������ ���������� ������������ ����� \b ������ \b ����������,
*  ������������� ������ �������� � ���������� \b id ������� ������� �� ����).
*  - ����� ������� ���������� ����������� (�������-����������� ���� �������
*  ������� ������ ����� [0, 0] � ������� ��������, � ����� ��� �������� 
*  ����������� �������, � �����������-������� ���� ������� �������� �� ����� 
*  [0, 0], � ����� ��� ������� ������ ��� � �������� ��������).
*  
* \todo
*  - �������� ��������, ����������� ������� ����� �������-�������� �
*  ��������-�������.
*
*  ��������� ������ ������� �� ������ ������������� �������� �� 1.0f ����� ����
*  X � Y, ���������� ��� ������� �������� �������� ���������� �������:
*  - ������������ ���������� �����, ���������� ���������� ������ � ��������.
*  - ������������ ���������� �����, ���������� ���������� ������ � ��������
*  �������� (��������, ������� \b �������� ������ ���� ���������) � �������
*  ������� �������� ��������� Data.Scale, �������� ����������� ���������
*  \b x � \b y, ������������� ���������� �� ������ �������� � ��������.
*
* ### ������� ������
*  
*  ������ ������������ �������� �������� �������� ������ �������; �� �������:
*  - ���������� ������ ��� ��������� �������� ��� �������� (������ �����,
*  ������� ������������ ���������, ���������� ������ id).
*  - ���������� �����, ���������� ���������� ��� ������� ������� ������ ������.
*  - ����� ��� ���� �������� (������ �����) �������� �����.
*  - ����� ����������� ��������� ������������ (�������� �������� ������������
*  ������ ���������).
*  - ��������� ��������������� �� �������� ������ (�� ������� ������ ������
*  ����������� ������� ����).
*  - ��������� �������� ���� �������� (��� ����� ��������������� ������ 
*  ��������� � ������ ������).
*  - ��������� ��������� �������.
*  
* \snippet .\Solution\Example\Example\Basements\Simple2DGame.cpp Vertex buffer
* \snippet .\Solution\Example\Example\Basements\Simple2DGame.cpp Colored object
* \snippet .\Solution\Example\Example\Basements\Simple2DGame.cpp Common object renders
*  
* \note
*  - ����� ��� ���� �������� ���������� ������ ��������������� ������ � �������.
*  - ����� ������ ������ ����� ����������, ��� ��������� ������������ 
*  ����������� ������� ������.
*  - ���������� ���������� ��� ������ ������� �� ����� ������ � �����
*  ���������������.
*  
* ### ���������������� ������
*  
*  ��������, ������� ����� ��������������, ����� ���������� �������, �����
*  ����� ��������� �� ��� �� �� ��������������.
*  
* \snippet .\Solution\Example\Example\Basements\Common.cpp Load texture
*
*  ������ ������������ ����������������� �������; �� �������:
*  - ���������� ������ ��� ��������� �������� c ��������� (������ �����,
*  ������� ������������ ���������, ���������� ������ id).
*  - �������� (������� �����).
*  - ���������� �����, ���������� ���������� ��� ������� ������� ������ ������.
*  - ����� ��� ���� �������� (������ �����) �������� �����.
*  - ����� ����������� ��������� ������������ (�������� �������� ������������
*  ������ ���������).
*  - ��������� ��������������� �� �������� ������ (�� ������� ������ ������
*  ����������� ������� ����).
*  - ��������� ��������� �������.
*
* \snippet .\Solution\Example\Example\Basements\Simple2DGame.cpp Vertex buffer
* \snippet .\Solution\Example\Example\Basements\Simple2DGame.cpp Textured object
* \snippet .\Solution\Example\Example\Basements\Simple2DGame.cpp Common object renders
*  
* \note
*  - ����� ��� ���� �������� ���������� ������ ��������������� ������ � �������.
*  - ����� ������������ ��������, ���������� ��������� ����������� ���
*  ������ ��������, �� ��� ���� ��� ����� ���������� ������������� ��������� 
*  ���� �������� \b ������� �������, �.�. Present.Geometry ���������
*  ������������� ������� ����� ��������� �������.
*  - ����� ������ � �������� ����������� (����������): ���������� ������� 
*  �������� ����������, ������ �������� ������, ����� ���������� ������ ������).
*  
*  ��������
*  --------
*  
*  ���������������� ��������� ������� �������������� ������ ��������
*  ����� ���� ����������� ����������� ���������:
*  - ������� ��� ������� ����� ��������� ������ � � ���������� ������������ ���
*  ������� ��������������� (��������, ��������� ��������� �� ��������� ������
*  ��� ���������� �����).
*  - ������� ������, �� ������� �������� ��������, ���������� \b ��� ����� 
*  ��������, ����� ���� ������������ ���� ������, ��������� ��������� 
*  State.Scissor ��� ��������� ������ ����� � ��������� ��� ������ �����
*  �������, ����� ������������ ������ ����� ��������.
*  - ������� ������, �� ������� �������� ��������, ���������� \b ��� �����
*  ��������, ����� ���� ������������ ���� ������, ��������� ������,
*  ������� ����� ������ ���������� ���������� ����� �������, ����� ������������
*  ������ ����� ��������.
*/
