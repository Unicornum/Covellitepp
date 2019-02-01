
/**
* \page InputRussianLettersPage Ввод русских букв с клавиатуры
*  
*  Проблема
*  --------
*  
*  Как оказалось, при нажатии кнопок клавиатуры в русской раскладке нативное
*  приложение получает сообщения \b AKEY_EVENT_ACTION_MULTIPLE (вместо 
*  \b AKEY_EVENT_ACTION_UP и \b AKEY_EVENT_ACTION_DOWN). При этом функции
*  AKeyEvent_getKeyCode(), AKeyEvent_getScanCode(), AKeyEvent_getFlags(),
*  AKeyEvent_getMetaState() и AKeyEvent_getRepeatCount() возвращают 0.
*  
*  Кроме того, для ASCII символов коды, возвращаемые функцией 
*  AKeyEvent_getKeyCode() не являются Unicode символами соответствующих
*  символов и требуют дополнительное преобразование с вызовом функций java.
*  
*  Решение
*  -------
*  
*  Для того, чтобы в нативном приложении можно было сразу получать Unicode коды 
*  нажатых кнопок клавиатуры (включая не ASCII символы), необходимо сделать 
*  следующее:
*  - Проект, создающий результатирующий APK файл программы следует создавать
*  как "Простое приложение Android":
*  
* \image html ".\Solution\(Documentation)\Manual\Vs2015.Android.png"
*  
*  - В java коде этого проекта унаследовать класс от NativeActivity и 
*  переопределить в нем функцию dispatchKeyEvent():
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
*  - В файл \b AndroidManifest.xml (в раздел \b activity) проекта добавить
*  имя нативной библиотеки, которая будет загружаться при старте программы
*  (без префикса lib и расширения .so), например для libRocket.so следует
*  добавить:
*  
* \code

<activity ...>
  <meta-data android:name="android.app.lib_name"
             android:value="Rocket"/>
  ...
</activity>

* \endcode
*  
*  - Добавить в ссылки проекту APK проект главной нативной библиотеки (.so).
*  
*  После приведенных манипуляций при нажатии кнопок экранной клавиатуры Android 
*  нативное приложение будет получать сообщение, у которого: 
*  - Функция AInputEvent_getType(event) возвращает AINPUT_EVENT_TYPE_KEY.
*  - Функция AKeyEvent_getAction(event) возвращает ACTION_KEY_PRESSED (это код,
*  заданный в java коде dispatchKeyEvent()).
*  - Функция AKeyEvent_getKeyCode(event) возвращает Unicode код символа нажатой 
*  кнопки.
*/
