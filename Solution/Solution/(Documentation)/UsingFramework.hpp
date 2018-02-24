
/**
* \page UsingFrameworkPage Manual
*  
*  Подключение к решению
*  ---------------------
*
*  - Установить свойство svn:externals с именем Externals и путем
* \code

https://svn/svn/Prebuilt.no.backup/Solution/Properties/Utilities/Externals/Alicorn/Covellite/_Root

* \endcode
*  - Подключить к страницам свойств решения файл Covellite++.props из папки
*  Externals/Covellite.
*  
*  Использование фреймворка
*  ========================
*  
*  - Создать проект сборки основного модуля программы
*  (\ref MainModuleWindowsUsingFrameworkAnchor "Windows"/
*  \ref MainModuleAndroidUsingFrameworkAnchor "Android").
*  - Создать .rcss и .rml файлы описания отдельных 
*  \ref ScreenDescriptionUsingFrameworkAnchor "экранов программы".
*  - Создать \ref ScreenClassesUsingFrameworkAnchor "классы" отдельных экранов 
*  программы.
*  - Создать \ref MainWindowClassUsingFrameworkAnchor "класс" главного окна 
*  программы.
*  - Создать \ref MainApplicationClassUsingFrameworkAnchor "класс" программы.
*  - Добавить рализация функции установки 
*  \ref SettingsUsingFrameworkAnchor "параметров" фреймворка по умолчанию.
*  - Добавить реализацию функции создания 
*  \ref ReadFileDataUsingFrameworkAnchor "объекта виртуальной файловой системы",
*  который будет использоваться программой для чтения данных из файлов.
*  
* \anchor MainModuleWindowsUsingFrameworkAnchor
*  Создание проекта сборки основного модуля программы для Windows
*  --------------------------------------------------------------
*
*  Создать \b пустой проект приложения Windows:
*  
* \image html ".\Solution\(Documentation)\Vs2015.Windows.png"
*  
*  Добавить в проект .ico файл иконки программы и .rc файл, определяющий иконку 
*  программы как \b ApplicationIcon (именно эта иконка будет отображаться 
*  в заголовке окна программы):
*  
* \include ./Solution/Example/Example/Icon.rc
*  
* \note
*  - В качестве заголовка окна программы будет использовано значение макроса
*  \b \_\_APPLICATION_NAME\_\_ из файла версии решения.
*  - Режим работы (полноэкранный/оконный), размеры и цвет фона окна программы 
*  задаются в настройках приложения (название параметров см. в описаниях 
*  классов окон).
*  
* \anchor MainModuleAndroidUsingFrameworkAnchor
*  Создание проекта сборки основного модуля программы для Android
*  --------------------------------------------------------------
*
*  ### Создание проекта сборки .apk файла программы
*  
*  Создать "Простое приложение Android":
*  
* \image html ".\Solution\(Documentation)\Vs2015.Android.png"
*  
*  Добавить в его java код перехват функции dispatchKeyEvent() (как это 
*  делается и зачем это нужно, см. \subpage InputRussianLettersPage).
*  
*  \subpage BackgroundServicePage
*  
* \note
*  Чтобы программа работала в полноэкранном режиме (без статус-бара), необходимо
*  добавить в AndroidManifest.xml
* \code
<application ... android:theme="@android:style/Theme.Holo.NoActionBar.Fullscreen">
* \endcode
*  
*  ### Создание проекта сборки главного модуля программы
*  
*  Создать проект .so файла (который будет загружаться при старте 
*  NativeActivity):
*  
* \image html ".\Solution\(Documentation)\Vs2015.So.png"
*  
*  Проекту Packaging добавить его в ссылки, а в AndroidManifest.xml добавить 
*  запись загрузки именно этого модуля (подробнее см. 
*  \subpage InputRussianLettersPage).
*  
* \anchor ScreenDescriptionUsingFrameworkAnchor
*  Создание .rcss и .rml файлов описания отдельных экранов программы
*  -----------------------------------------------------------------
*  
*  Как это делается см. в документации/примерах [libRocket]. \n
*  Доступные css свойства: [libRocket CSS].
*  
*  Все файлы (.rml, .rcss, .png) следует поместить в папку assets проекта
*  Packaging (пути к этим файлам в программе задаются относительно этой папки,
*  а в самом файле - относительно папки расположения файла).
*  
*  Файлы .rml могут содержать в качестве текста не ASCII символы, но в этом 
*  случае их следует сохранять как utf8, а используемые шрифты должны 
*  поддерживать соответствующие Unicode символы.
*  
* \note
*  Файлы .rcss не должны сохраняться как utf8, т.к. они включаются целиком
*  в состав файлов .rml при парсинге, что приводит к появлению Assert'ов при
*  запуске Debug версии программы.
*  
* \anchor ScreenClassesUsingFrameworkAnchor
*  Создать классы отдельных экранов программы
*  ------------------------------------------
*  
*  Для каждого экрана программы необходимо создать класс-наследник от класса
*  covellite::rocket::Layer, передав ему в качестве второго параметра 
*  конструктора путь к соответствующему .rml файлу.
*  
* \snippet .\Solution\Example\Example\ExampleWindow.cpp Layer example
*  
*  Если необходим экран с заголовком, содержащим текст, заданный в заголовке
*  .rml файла, создать класс наследник слоя, в конструкторе которого передать
*  базовому классу идентификатор заголовка из его объявления в .rml файле.
*
* \snippet .\Solution\Example\Example\ExampleWindow.cpp Title layer
*
* \anchor MainWindowClassUsingFrameworkAnchor
*  Создание класса главного окна программы
*  ---------------------------------------
*  
*  Класс главного окна программы отвечает за переключение пользовательских 
*  экранов и обработку пользовательского ввода.
*  
*  Создать класс-наследник от класса covellite::core::IWindow (а также 
*  covellite::core::Window, если необходимо, например, завершать работу 
*  программы по нажатию кнопки), в конструкторе котрого установить набор строк
*  для локализации приложения и добавить стартовый экран программы.
*  
* \snippet .\Solution\Example\Example\ExampleWindow.cpp Constructor main window
*  
*  Подписаться на нужные события следует в функции Subscribe().
*  
* \snippet .\Solution\Example\Example\ExampleWindow.cpp Subscribe
*  
* \note
*  Цвет фона можно задавать как поле \b background-color для body в .rcss файле, 
*  но в этом случае для заполнения всего окна следует в стилях rml и body 
*  поставить
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
*  Идентификаторы элементов управления передаются в функцию-обработчик 
*  события Event::Click в нижнем регистре независимо от того, каким образом 
*  они были заданы в .rml файле.
*
*  В классе окна событие Event::Click будет вызываться для всех элементов 
*  управления всех активных пользовательских экранов, при этом здесь имеет 
*  смысл обрабатывать события, непосредственно связанные с работой окна 
*  (переход к другому слою или выход из программы).
*  
*  Другие события, которые может обрабатывать окно, см. в описании класса
*  covellite::core::Event.
*  
* \anchor MainApplicationClassUsingFrameworkAnchor
*  Создание класса программы
*  -------------------------
*  
*  Класс программы содержит главный цикл обработки сообщений, отвечает за 
*  обработку системных сообщений и создание/разрушение главного окна программы.
*  
* \note
*  При написании кода следует учитывать, что NativeActivity загружает главный
*  модуль программы только один раз, при первом запуске программы, после чего
*  при каждой последующей активации программы вызывает функцию точки входа.
*  При этом объект класса программы будет создаваться каждый раз заново, но
*  все статические объекты будут иметь состояния, унаследованные от 
*  предыдущей активации.
*
*  Создать класс-наследник от covellite::core::Application, конструктору
*  которого передать способ отрисовки окна приложения (по событию или
*  непрерывно), подписаться на необходимые события и добавить код обработки 
*  этих событий.
*  
* \snippet .\Solution\Example\Example\ExampleApp.cpp Constructor main application class
*  
*  Список получаемых событий см. в описании класса covellite::core::Event.
*  
* \note
*  Подписка на событие \b Event::Create обязательна, в обработчике
*  следует создать набор окон программы; в качестве класса главного окна 
*  программы следует указать ранее созданный класс. \n
*  Подписка на собыие \b Event::Destroy не требуется, окно будет
*  уничтожено автоматически.
*  
* \snippet .\Solution\Example\Example\ExampleApp.cpp Create main window
*  
*  Для создания точки входа программы использовать макрос
*  \b APPLICATION_ENTRY_POINT, в качестве параметра которого указать имя 
*  созданного класса программы.
*  
* \snippet .\Solution\Example\Example\ExampleApp.cpp Create entry point
*  
* \anchor SettingsUsingFrameworkAnchor
*  Настройки по умолчанию
*  ----------------------
*  
*  В код проекта необходимо также добавить реализацию функции установки
*  значения параметров по умолчанию:
*  
* \include .\Solution\Example\Example\DefaultSettings.cpp
*
* \anchor ReadFileDataUsingFrameworkAnchor
*  Настройка чтения данных из файлов
*  ---------------------------------
*
*  В код проекта необходимо также добавить реализацию функции настройки
*  способа чтения данных из файлов (в качестве примера приведено использование
*  класса виртуальной файловой системы, который просто читает данные с диска):
*
* \include .\Solution\Example\Example\ReadFileData.cpp
*
*  [libRocket]: https://barotto.github.io/libRocketDoc/ "libRocket"
*  [libRocket CSS]: https://barotto.github.io/libRocketDoc/pages/rcss.html "libRocket CSS"
*/
