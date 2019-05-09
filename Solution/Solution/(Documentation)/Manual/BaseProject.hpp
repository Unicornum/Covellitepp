
/**
* \page CreateBaseProjectBriefPage
*  
*  - ������� ������ ������ ��������� ������ ���������
*  (\ref MainModuleWindowsUsingFrameworkAnchor "Windows"/
*  \ref MainModuleAndroidUsingFrameworkAnchor "Android").
*  - ������� \ref MainApplicationClassUsingFrameworkAnchor "�����" ���������.
*  - �������� ���������� ������� ���������
*  \ref SettingsUsingFrameworkAnchor "����������" ���������� �� ���������.
*  - �������� ���������� ������� ��������
*  \ref ReadFileDataUsingFrameworkAnchor "������� ����������� �������� �������",
*  ������� ����� �������������� ���������� ��� ������ ������ �� ������.
*  
*/

/**
* \page CreateBaseProjectPage �������� �������� ������� ����������
*  
* \copydoc CreateBaseProjectBriefPage
*
* \anchor MainModuleWindowsUsingFrameworkAnchor
*
*  �������� ������� ������ ��������� ������ ��������� ��� Windows
*  --------------------------------------------------------------
*
*  ������� \b ������ ������ ���������� Windows:
*
* \image html ".\Solution\(Documentation)\Manual\Vs2015.Windows.png"
*
*  �������� � ������ .ico ���� ������ ��������� � .rc ����, ������������ ������
*  ��������� ��� \b ApplicationIcon (������ ��� ������ ����� ������������
*  � ��������� ���� ���������):
*
* \include ./Solution/Example/Example/Icon.rc
*
* \note
*  - � �������� ��������� ���� ��������� ����� ������������ �������� �������
*  \b \_\_APPLICATION_NAME\_\_ �� ����� ������ �������.
*  - ����� ������ (�������������/�������), ������� � ���� ���� ���� ���������
*  �������� � [����������](\ref SettingsUsingFrameworkAnchor) ����������.
*
* \anchor MainModuleAndroidUsingFrameworkAnchor
*
*  �������� ������� ������ ��������� ������ ��������� ��� Android
*  --------------------------------------------------------------
*
*  ### �������� ������� ������ .apk ����� ���������
*
*  ������� "������� ���������� Android":
*
* \image html ".\Solution\(Documentation)\Manual\Vs2015.Android.png"
*
*  �������� � ��� java ��� �������� ������� dispatchKeyEvent() 
*  ([��� ��� �������� � ����� ��� �����](\ref InputRussianLettersPage)).
*
*  \subpage BackgroundServicePage
*
* \note
*  ����� ��������� �������� � ������������� ������ (��� ������-����), ����������
*  �������� � AndroidManifest.xml
* \code
<application ... android:theme="@android:style/Theme.Holo.NoActionBar.Fullscreen">
* \endcode
*
*  ### �������� ������� ������ �������� ������ ���������
*
*  ������� ������ .so ����� (������� ����� ����������� ��� ������
*  NativeActivity):
*
* \image html ".\Solution\(Documentation)\Manual\Vs2015.So.png"
*
*  ������� Packaging �������� ��� � ������, � � AndroidManifest.xml ��������
*  ������ �������� ������ ����� ������ 
*  ([���������](\subpage InputRussianLettersPage)).
*
* \anchor MainApplicationClassUsingFrameworkAnchor
*
*  �������� ������ ���������
*  -------------------------
*
*  \b ����� \b ��������� �������� ������� ���� ��������� ���������, �������� ��
*  ��������� ��������� ��������� � ��������/���������� �������� ���� ���������.
*
* \note
*  ��� ��������� ���� ������� ���������, ��� NativeActivity ��������� �������
*  ������ ��������� ������ ���� ���, ��� ������ ������� ���������, ����� ����
*  ��� ������ ����������� ��������� ��������� �������� ������� ����� �����.
*  ��� ���� ������ ������ ��������� ����� ����������� ������ ��� ������, ��
*  ��� ����������� ������� ����� ����� ���������, �������������� ��
*  ���������� ���������.
*
*  ������� �����-��������� �� covellite::app::Application, ������������
*  �������� �������� ������ ��������� ���� ���������� (�� ������� ���
*  ����������), ����������� �� ����������� ������� � �������� ��� ���������
*  ���� �������.
*
* \snippet .\Solution\Example\Example\ExampleApp.cpp Constructor main application class
*
*  [�������](\ref CovelliteEventsIdGroup), ������� ����� ������������ ����������.
*
* \note
*  �������� �� ������� \b Application.Start �����������, � �����������
*  ������� ������� ����� ���� ���������; � �������� ������ �������� ����
*  ��������� ������� ������� ����� ��������� �����. \n
*  �������� �� ������ \b Application.Exit �� ���������, ���� �����
*  ���������� �������������.
* 
*  - \b ExampleWindow - ����� �������� ���� ��������� (��. \ref CreateGuiCodePage).
*  - \b ExtraWindow - ����� ���� ���������, �������������� ���������������
*  ��������� � ����, ������ ������� ���e� ������������ ������ GUI (��������,
*  ��� ����� ���� ����� �� 2D (\ref Draw2DScenePage) ��� 3D 
*  (\ref Draw3DScenePage) ��������).
*  
* \snippet .\Solution\Example\Example\ExampleApp.cpp Create main window
*
*  ��� �������� ����� ����� ��������� ������������ ������
*  \b APPLICATION_ENTRY_POINT, � �������� ��������� �������� ������� ���
*  ���������� ������ ���������.
*
* \snippet .\Solution\Example\Example\ExampleApp.cpp Create entry point
*
* \anchor SettingsUsingFrameworkAnchor
*
*  ���������
*  ---------
*
* ### ��� ���� ��������
*
*  | ��������             | ������                        | ��������                                                                |
*  | :------------------- | :---------------------------- | :---------------------------------------------------------------------- |
*  | PathToFontsDirectory | Root                          | ���� � ����� ������������ �������, ������� ������������ � .rcss ������. |
*  | R                    | Root\\Window\\BackgroundColor | ���� ���� ���� ��������� �� ���������: ������� ���������� [0...255].    |
*  | G                    | Root\\Window\\BackgroundColor | ���� ���� ���� ��������� �� ���������: ������� ���������� [0...255].    |
*  | B                    | Root\\Window\\BackgroundColor | ���� ���� ���� ��������� �� ���������: ����� ���������� [0...255].      |
*  | A                    | Root\\Window\\BackgroundColor | ���� ���� ���� ��������� �� ���������: ������������ [0...255].          |
*
* ### ������ ��� Windows
*
*  | ��������        | ������             | ��������                                                                          |
*  | :-------------- | :----------------- | :-------------------------------------------------------------------------------- |
*  | GraphicsApi     | Root\\Window       | ������������ ��� ���������� ����������� Api (Auto, DirectX11, DirectX10, OpenGL). |
*  | IsFullScreen    | Root\\Window       | �������������/������� ����� ������ ���������.                                     |
*  | IsResized       | Root\\Window       | ����������/������ ��������� �������� ���� ��������� �����.                        |
*  | Width           | Root\\Window\\Size | ������ ���������� ������� ���� ���������.                                         |
*  | Height          | Root\\Window\\Size | ������ ���������� ������� ���� ���������.                                         |
*  | MinClientWidth  | Root\\Window\\Size | ����������� ������ ���������� ������� ���� ���������.                             |
*  | MinClientHeight | Root\\Window\\Size |  ����������� ������ ���������� ������� ���� ���������.                            |
*
* ### ������ ��� Android
*
*  | ��������    | ������       | ��������                                                      |
*  | :---------- | :----------- | :------------------------------------------------------------ |
*  | GraphicsApi | Root\\Window | ������������ ��� ���������� ����������� Api (Auto, OpenGLES). |
*
* ### ��������� �� ���������
*
*  � ��� ������� ���������� ����� �������� ���������� ������� ���������
*  �������� ���������� �� ���������. ������� \b � \b ������������ \b �������
*  ������ ������������� �������� �� ��������� ��� ���� ����������.
*
* \include .\Solution\Example\Example\DefaultSettings.cpp
*
* \anchor ReadFileDataUsingFrameworkAnchor
*
*  ��������� ������ ������ �� ������
*  ---------------------------------
*
*  � ��� ������� ���������� ����� �������� ���������� ������� ���������
*  ������� ������ ������ �� ������ (� �������� ������� ��������� �������������
*  ������ ����������� �������� �������, ������� ������ ������ ������ � �����):
*
* \include .\Solution\Example\Example\ReadFileData.cpp
*
*  [libRocket]: https://barotto.github.io/libRocketDoc/ "libRocket"
*  [��������� css ��������]: https://barotto.github.io/libRocketDoc/pages/rcss.html "libRocket CSS"
*/
