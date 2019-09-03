
/**
* \page ExampleBriefPage
*  ������ ������� ������� ������������� ����������.
*/

/**
* \page ExamplePage Example
*  
*  ��������
*  --------
*  
* \copydoc ExampleBriefPage
*  
* \subpage ScreenshotsPage
*  
*  �������� [����������������� ������](\ref ExampleDemoPage).
*  
*  �������
*  -------
*  
* ### ������ Assets
*
*  ������ ������ ��������� � �� �������� � .pack.
*  
* \anchor aRunBackwardCompatibilityLib
* ### ������� Example/Example.Android/Examle.es.Android
*  
*  ������ ������� ������� \ref ExampleGroup
*  
*  ������ �������� ������������ ������ ������� ��������� ('����� �����').
*  
*  ������� Example.Android/Examle.es.Android ���������� ������ ���, ��� � ���
*  ������ ������ ������� API (19/15), � ���������� ��� ����������� � ���
*  ����� ������� Covellite++.props ������ ���������� ��� ������ ��� ������
*  OpenGL ES, � ������ - OpenGL ES3.
*  
* ### ������ Packaging
*
*  ������ ������ .apk ����� Android-������ ������� ���������.
*  
*  ������� � ������ ��������� ��� ������� Example.Android/Examle.es.Android,
*  � ���������� ���� ��� ��������� ���������� ����������� � .apk �����.
*  
*  ��������� ������� ��� ��� ���� ���������� � ����������� �� API �����������
*  ���������� ������������ ��������� �������:
*  
*  � ���� res/values/settings.xml �������� �������� � ������ ����������,
*  ������� ������ ����������� �� ����������� � API ���� 18.
*  
* \code
  <string name="EntryPointLibName">Example.es</string>
* \endcode
*  
*  � ���� res/values-v18/settings.xml �������� �������� � ������ ����������,
*  ������� ������ ����������� �� ����������� � API18+.
*
* \code
  <string name="EntryPointLibName">Example</string>
* \endcode
*  
*  � ���� AndroidManifest.xml ��������� ������ �� ���� ��������
*  
* \code
  <activity ...>
    <meta-data android:name="android.app.lib_name" android:value="@string/EntryPointLibName"/>
* \endcode
*
* ### ������ Settings
*
*  ������ ��������� ��������� �������� ���������� �������.
*
* ### ������ Version
*  
*  ���������� ���� version.xml, ���������� ������� ������ ������� � 
*  ������������� android:versionCode � ����� AndroidManifest.xml.
*  
*  ��� ������ �������������� �� Externals.
*  
* ### ������ VersionSetup
*  
*  ��������� ���������, ����������� �������� Version ����� �������, �����
*  ���� version.xml ��� ����������� � ����� Packaging\\res\\values.
*  
*  ���������� ������������� � ����� ������� Packaging, ����� ����� ����
*  �������� ��������� Version.exe ���� � ���� �����.
*/

/**
* \defgroup ExampleGroup Example
*  ������ ������� ������� ������� ���������.
*/
