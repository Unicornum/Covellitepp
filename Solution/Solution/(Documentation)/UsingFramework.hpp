
/**
* \page UsingFrameworkPage ������������� ����������
*  
*  ����������� � �������
*  ---------------------
*
*  - ���������� �������� svn:externals � ������ Externals � �����
* \code

https://svn/svn/Prebuilt.no.backup/Solution/Solution

* \endcode
*  - ���������� � ��������� ������� ������� ���� Covellite++.props �� �����
*  Externals/Covellite.
*  
*  ������������� ����������
*  ========================
*  
*  - ������� ������ ������ ��������� ������ ���������.
*  - ������� .rcss � .rml ����� �������� ��������� ������� ���������.
*  - ������� ������ ��������� ������� ���������.
*  - ������� ����� �������� ���� ���������.
*  - ������� ����� ���������.
*  
*  �������� ������� ������ ��������� ������ ��������� ��� Windows
*  --------------------------------------------------------------
*
*  ������� \b ������ ������ ���������� Windows:
*  
* \image html ".\Solution\(Documentation)\Vs2015.Windows.png"
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
*  �������� � ���������� ���������� (�������� ���������� ��. � ��������� 
*  ������� ����).
*  
*  �������� ������� ������ ��������� ������ ��������� ��� Android
*  --------------------------------------------------------------
*
*  ### �������� ������� ������ .apk ����� ���������
*  
*  ������� "������� ���������� Android":
*  
* \image html ".\Solution\(Documentation)\Vs2015.Android.png"
*  
*  �������� � ��� java ��� �������� ������� dispatchKeyEvent() (��� ��� 
*  �������� � ����� ��� �����, ��. \subpage InputRussianLettersPage).
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
* \image html ".\Solution\(Documentation)\Vs2015.So.png"
*  
*  ������� Packaging �������� ��� � ������, � � AndroidManifest.xml �������� 
*  ������ �������� ������ ����� ������ (��������� ��. 
*  \subpage InputRussianLettersPage).
*  
*  �������� .rcss � .rml ������ �������� ��������� ������� ���������
*  -----------------------------------------------------------------
*  
*  ��� ��� �������� ��. � ������������/�������� [libRocket]. \n
*  ��������� css ��������: [libRocket CSS].
*  
*  ��� ����� (.rml, .rcss, .png) ������� ��������� � ����� assets �������
*  Packaging (���� � ���� ������ � ��������� �������� ������������ ���� �����,
*  � � ����� ����� - ������������ ����� ������������ �����).
*  
*  ����� .rml ����� ��������� � �������� ������ �� ASCII �������, �� � ���� 
*  ������ �� ������� ��������� ��� utf8, � ������������ ������ ������ 
*  ������������ ��������������� Unicode �������.
*  
*  ������� ������ ��������� ������� ���������
*  ------------------------------------------
*  
*  ��� ������� ������ ��������� ���������� ������� �����-��������� �� ������
*  covellite::rocket::Layer, ������� ��� � �������� ������� ��������� 
*  ������������ ���� � ���������������� .rml �����.
*  
* \snippet .\Solution\Example\Example\ExampleWindow.cpp Layer example
*  
*  ���� ��������� ����� � ����������, ���������� �����, �������� � ���������
*  .rml �����, ������� ����� ��������� ����, � ������������ �������� ��������
*  �������� ������ ������������� ��������� �� ��� ���������� � .rml �����.
*
* \snippet .\Solution\Example\Example\ExampleWindow.cpp Title layer
*
*  �������� ������ �������� ���� ���������
*  ---------------------------------------
*  
*  ����� �������� ���� ��������� �������� �� ������������ ���������������� 
*  ������� � ��������� ����������������� �����.
*  
*  ������� �����-��������� �� ������ covellite::core::IWindow (� ����� 
*  covellite::core::Window, ���� ����������, ��������, ��������� ������ 
*  ��������� �� ������� ������), � ������������ ������� ���������� ����� �����
*  ��� ����������� ���������� � �������� ��������� ����� ���������.
*  
* \snippet .\Solution\Example\Example\ExampleWindow.cpp Constructor main window
*  
*  ����������� �� ������ ������� ������� � ������� Subscribe().
*  
* \snippet .\Solution\Example\Example\ExampleWindow.cpp Subscribe
*  
* \note
*  ���� ���� ����� �������� ��� ���� \b background-color ��� body � .rcss �����, 
*  �� � ���� ������ ��� ���������� ����� ���� ������� � ������ rml � body 
*  ���������
* \code

rml
{
  top: 0px;
  left: 0px;
  width: 100%;
  height: 100%;
}
  
body
{
  top: 0px;
  left: 0px;
  width: 100%;
  height: 100%;
  background-color: blue;

* \endcode
*  
* \note
*  �������������� ��������� ���������� ���������� � �������-���������� 
*  ������� Event::Click � ������ �������� ���������� �� ����, ����� ������� 
*  ��� ���� ������ � .rml �����.
*
*  � ������ ���� ������� Event::Click ����� ���������� ��� ���� ��������� 
*  ���������� ���� �������� ���������������� �������, ��� ���� ����� ����� 
*  ����� ������������ �������, ��������������� ��������� � ������� ���� 
*  (������� � ������� ���� ��� ����� �� ���������).
*  
*  ������ �������, ������� ����� ������������ ����, ��. � �������� ������
*  covellite::core::Event.
*  
*  �������� ������ ���������
*  -------------------------
*  
*  ����� ��������� �������� ������� ���� ��������� ���������, �������� �� 
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
*  ������� �����-��������� �� covellite::core::Application, ������������
*  �������� �������� ������ ��������� ���� ���������� (�� ������� ���
*  ����������), ����������� �� ����������� ������� � �������� ��� ��������� 
*  ���� �������.
*  
* \snippet .\Solution\Example\Example\ExampleApp.cpp Constructor main application class
*  
*  ������ ���������� ������� ��. � �������� ������ covellite::core::Event.
*  
* \note
*  �������� �� ������� \b Event::Create �����������, � �����������
*  ������� ������� ����� ���� ���������; � �������� ������ �������� ���� 
*  ��������� ������� ������� ����� ��������� �����. \n
*  �������� �� ������ \b Event::Destroy �� ���������, ���� �����
*  ���������� �������������.
*  
* \snippet .\Solution\Example\Example\ExampleApp.cpp Create main window
*  
*  ��� �������� ����� ����� ��������� ������������ ������
*  \b APPLICATION_ENTRY_POINT, � �������� ��������� �������� ������� ��� 
*  ���������� ������ ���������.
*  
* \snippet .\Solution\Example\Example\ExampleApp.cpp Create entry point
*  
*  ��������� �� ���������
*  ----------------------
*  
*  � ��� ������� ���������� ����� �������� ���������� ������� ���������
*  �������� ���������� �� ���������:
*  
* \include .\Solution\Example\Example\DefaultSettings.cpp
*
*  [libRocket]: https://github.com/libRocket/libRocket "libRocket"
*  [libRocket CSS]: http://librocket.com/wiki/documentation/RCSS "libRocket CSS"
*/
