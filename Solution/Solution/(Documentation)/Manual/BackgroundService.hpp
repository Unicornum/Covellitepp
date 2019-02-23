
/**
* \page BackgroundServicePage ������� ���������� ����� � ���� �++
*  
*  ��������
*  --------
*  
*  ���� �� ���� �++ ��������� �������������� ����� (boost::thread), �� ��� 
*  ����������� ��������� �������� ���� �� ���� �������� (������ ������� 
*  ����������� ��� ����������):
*  - ������ � ������������� ��������� ������ ����� ������ ����� ��������� ������
*  ������ (��������, ��-�� �������� �������� .so �����).
*  - ������� ������ � ����� ������, ��� ������� ��� ���������� ����������
*  ����� ����� (��� � 20-30 �����, ���� ���� ���������� �������� � 1 ���.,
*  ������, ���������� ��������� ������).
*  
*  �������
*  -------
*  
*  ��� ��������������� ������ �������� ������ �++ � ��������� ���������� 
*  ������� ��� ���������� ����������, ����������:
*  
*  - �������� � ������ \b Packaging ����� �������, ������� ��� ������ ����� 
*  ��������� ������� .so ���� �++ ��������� � ��������� � ������� ������
*  ����� ����������� �������� (���� ������� ����� ���������, ����� ������� ��
*  '�������' ������):
*  
*  ### BackgroundService.java
*  
* \include ".\Solution\(Documentation)\Manual\BackgroundService.java"
*  
*  - ���������������� � \b AndroidManifest.xml ��������� ����� ��� ������:
*  
* \code
  
<application ...>
  <service android:name="com.Simple.BackgroundService" android:exported="false"/>
  ...
    
* \endcode
*  
*  - � ������, ����������� �� \b NativeActivity, �������� ������ ������� 
*  (����� ������� ��� ������ ��������� ���������):
*  
* \code

import android.os.Bundle;
import android.content.Intent;
    
public class Simple extends NativeActivity
{
  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    startService(new Intent(this, BackgroundService.class));
  }
  
* \endcode
*  
*  - � ���� �++ �������� �������, ������� ����� ���������� ��� ��������
*  .so ����� ��� ������ ������� � � ��� ��������� ������� ������� �����:
*  
* \code
  
extern "C"
{
  
jint JNI_OnLoad(JavaVM *, void *)
{
  // ����� ����� ��������� �����
  
  return JNI_VERSION_1_6;
}
  
} // extern "C"
  
* \endcode
*  
* \note
*  ����� ��������, ��� ������ ����� ���� ���������� � ����������� � ����� 
*  ������ ������� (.so ���� ����� �������� � ������� ����� ��������� ������),
*  ������� ������� ����������� ����������� ������, ���������� ����� 
*  �������� (ANativeActivity *) ��� ��������� ���������.
*/