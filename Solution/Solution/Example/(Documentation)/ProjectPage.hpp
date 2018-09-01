
/**
* \page ExampleBriefPage
*  Раздел примера использования фреймворка.
*/

/**
* \page ExamplePage Example
*  
*  Введение
*  --------
*  
* \copydoc ExampleBriefPage
*  
*  Проект Packaging
*  ----------------
*  
*  Проект сборки .apk файла примера программы.
*  
*  Проект Example
*  --------------
*  
*  Список классов проекта \ref ExampleGroup
*  
*  Проект главного запускаемого модуля примера программы ('точка входа').
*
*  Проект Version
*  --------------
*
*  Генерирует файл version.xml, содержащий текущую версию решения и устанвливает
*  android:versionCode в файле AndroidManifest.xml.
*
*  Сам проект экспортируется из Externals.
*
*  Проект VersionSetup
*  -------------------
*
*  Запускает программу, построенную проектом Version таким образом, чтобы
*  файл version.xml был сформирован в папке Packaging\\res\\values.
*
*  Специально располагается в папке проекта Packaging, чтобы можно было
*  передать программе Version.exe путь к этой папке.
*  
*  Пример использования
*  ====================
*
*  Скриншоты
*  ---------
*
* \image html ".\Solution\Example\(Documentation)\ExampleH.png"
* \n
* \image html ".\Solution\Example\(Documentation)\ExampleW.png"
*  
*  Образцы используемых .rml файлов
*  --------------------------------
*  
*  ### Layer 1
*  
* \include ".\Solution\Example\Packaging\assets\data\demo.rml"
*  
*  ### Layer 2
*  
* \include ".\Solution\Example\Packaging\assets\data\demo2.rml"
*
*/

/**
* \defgroup ExampleGroup Example
*  Группа классов проекта примера программы.
*/
