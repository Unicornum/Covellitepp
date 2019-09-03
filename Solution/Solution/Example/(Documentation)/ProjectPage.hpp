
/**
* \page ExampleBriefPage
*  Раздел проекта примера использования фреймворка.
*/

/**
* \page ExamplePage Example
*  
*  Введение
*  --------
*  
* \copydoc ExampleBriefPage
*  
* \subpage ScreenshotsPage
*  
*  Описание [демонстрационного режима](\ref ExampleDemoPage).
*  
*  Проекты
*  -------
*  
* ### Проект Assets
*
*  Проект данных программы и их упаковки в .pack.
*  
* \anchor aRunBackwardCompatibilityLib
* ### Проекты Example/Example.Android/Examle.es.Android
*  
*  Список классов проекта \ref ExampleGroup
*  
*  Проект главного запускаемого модуля примера программы ('точка входа').
*  
*  Проекты Example.Android/Examle.es.Android отличаются только тем, что у них
*  указан разный целевой API (19/15), в результате при подключении к ним
*  файла свойств Covellite++.props первый собирается для работы при помощи
*  OpenGL ES, а второй - OpenGL ES3.
*  
* ### Проект Packaging
*
*  Проект сборки .apk файла Android-версии примера программы.
*  
*  Проекту в ссылки добавлены оба проекта Example.Android/Examle.es.Android,
*  в результате чего обе собранные библиотеки оказываются в .apk файле.
*  
*  Настройка запуска той или иной библиотеки в зависимости от API конкретного
*  устройства производится следующим образом:
*  
*  В файл res/values/settings.xml добавлен параметр с именем библиотеки,
*  которая должна запускаться на устройствах с API ниже 18.
*  
* \code
  <string name="EntryPointLibName">Example.es</string>
* \endcode
*  
*  В файл res/values-v18/settings.xml добавлен параметр с именем библиотеки,
*  которая должна запускаться на устройствах с API18+.
*
* \code
  <string name="EntryPointLibName">Example</string>
* \endcode
*  
*  В файл AndroidManifest.xml добавлена ссылка на этот параметр
*  
* \code
  <activity ...>
    <meta-data android:name="android.app.lib_name" android:value="@string/EntryPointLibName"/>
* \endcode
*
* ### Проект Settings
*
*  Проект программы изменения настроек приложения примера.
*
* ### Проект Version
*  
*  Генерирует файл version.xml, содержащий текущую версию решения и 
*  устанавливает android:versionCode в файле AndroidManifest.xml.
*  
*  Сам проект экспортируется из Externals.
*  
* ### Проект VersionSetup
*  
*  Запускает программу, построенную проектом Version таким образом, чтобы
*  файл version.xml был сформирован в папке Packaging\\res\\values.
*  
*  Специально располагается в папке проекта Packaging, чтобы можно было
*  передать программе Version.exe путь к этой папке.
*/

/**
* \defgroup ExampleGroup Example
*  Группа классов проекта примера программы.
*/
