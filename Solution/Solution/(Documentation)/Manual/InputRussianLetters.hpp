
/**
* \page InputRussianLettersPage ���� ������� ���� � ����������
*  
*  ��������
*  --------
*  
*  ��� ���������, ��� ������� ������ ���������� � ������� ��������� ��������
*  ���������� �������� ��������� \b AKEY_EVENT_ACTION_MULTIPLE (������ 
*  \b AKEY_EVENT_ACTION_UP � \b AKEY_EVENT_ACTION_DOWN). ��� ���� �������
*  AKeyEvent_getKeyCode(), AKeyEvent_getScanCode(), AKeyEvent_getFlags(),
*  AKeyEvent_getMetaState() � AKeyEvent_getRepeatCount() ���������� 0.
*  
*  ����� ����, ��� ASCII �������� ����, ������������ �������� 
*  AKeyEvent_getKeyCode() �� �������� Unicode ��������� ���������������
*  �������� � ������� �������������� �������������� � ������� ������� java.
*  
*  �������
*  -------
*  
*  ��� ����, ����� � �������� ���������� ����� ���� ����� �������� Unicode ���� 
*  ������� ������ ���������� (������� �� ASCII �������), ���������� ������� 
*  ���������:
*  - ������, ��������� ���������������� APK ���� ��������� ������� ���������
*  ��� "������� ���������� Android":
*  
* \image html ".\Solution\(Documentation)\Manual\Vs2015.Android.png"
*  
*  - � java ���� ����� ������� ������������ ����� �� NativeActivity � 
*  �������������� � ��� ������� dispatchKeyEvent():
*  
* \code
  
import android.app.NativeActivity;
import android.view.KeyEvent;
  
public class MainActivity extends NativeActivity
{
  @Override
  public boolean dispatchKeyEvent(KeyEvent event)
  {
    int keyaction = event.getAction();
    final int ACTION_KEY_PRESSED = 32;
      
    if(keyaction == KeyEvent.ACTION_UP)
    {
      int keyCode = event.getUnicodeChar();
      super.dispatchKeyEvent(new KeyEvent(ACTION_KEY_PRESSED, keyCode));
    }
    else if(keyaction == KeyEvent.ACTION_MULTIPLE)
    {
      if (event.getCharacters().length() == 1)
      {
        int keyCode = event.getCharacters().charAt(0);
        return super.dispatchKeyEvent(new KeyEvent(ACTION_KEY_PRESSED, keyCode));
      }
    }
      
    return super.dispatchKeyEvent(event);
  }
}
  
* \endcode
*  
*  - � ���� \b AndroidManifest.xml (� ������ \b activity) ������� ��������
*  ��� �������� ����������, ������� ����� ����������� ��� ������ ���������
*  (��� �������� lib � ���������� .so), �������� ��� libRocket.so �������
*  ��������:
*  
* \code

<activity ...>
  <meta-data android:name="android.app.lib_name"
             android:value="Rocket"/>
  ...
</activity>

* \endcode
*  
*  - �������� � ������ ������� APK ������ ������� �������� ���������� (.so).
*  
*  ����� ����������� ����������� ��� ������� ������ �������� ���������� Android 
*  �������� ���������� ����� �������� ���������, � ��������: 
*  - ������� AInputEvent_getType(event) ���������� AINPUT_EVENT_TYPE_KEY.
*  - ������� AKeyEvent_getAction(event) ���������� ACTION_KEY_PRESSED (��� ���,
*  �������� � java ���� dispatchKeyEvent()).
*  - ������� AKeyEvent_getKeyCode(event) ���������� Unicode ��� ������� ������� 
*  ������.
*/
