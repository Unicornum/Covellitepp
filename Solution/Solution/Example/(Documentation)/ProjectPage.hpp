
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
* ### Проект Example/Example.Android
*  
*  Список классов проекта \ref ExampleGroup
*  
*  Проект главного запускаемого модуля примера программы ('точка входа').
*  
* ### Проект Packaging
*
*  Проект сборки .apk файла примера программы.
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
