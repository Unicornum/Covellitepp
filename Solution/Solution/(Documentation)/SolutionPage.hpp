
/**
* \page SolutionPage Covellite++
*
* \section sSolutionBrief Описание решения
*  GUI фреймворк для разработки Android-приложений на С++.
*
*  Описание фреймворка
*  ===================
*  
*  Фреймворк базируется на [libRocket] и предназначен для создания приложений,
*  содержащих стандартные элементы управления (кнопки, поля ввода, ckeckbox'ы,
*  radiobutton'ы и т.п.) с возможностью указания стиля их отображения
*  (аналог css).
*  
*  Основные характеристики:
*  - Кросс-платформенный: 
*   + Windows (тестируется на Windows 7 x64).
*   + Android (тестируется на ARM API15).
*  - Позволяет встраивать код дополнительной отрисовки кадра, поверх которого 
*  будет отображаться GUI.
*  - Быстрый (Windows версия выдает ~1500 fps в режиме непрерывной отрисовки
*  на ноутбуке, купленном в 2013-м году).
*  
*  Ппредставляет собой набор классов-оберток для создания:
*  - Главного объекта приложения (с возможность запуска как с непрерывной 
*  отрисовкой окна, так и отрисовкой только по произошедшему событию).
*  - Главного окна приложения.
*  - Слоев окна приложения (каждый слой - отдельный экран, с которым 
*  взамодействует пользователь) с возможностью переключения между слоями с
*  автоматической обработкой нажатия кнопки "Назад".
*  - Обработки событий (как системных, так и действий пользователя) путем
*  подписки на требуемые события.
*  - Локализация программы для выбранного языка.
*  - Android: автоматический вызов клавиатуры при выборе текстовых полей ввода.
*  
*  Описания классов фреймворка: \ref gCovelliteClasses
*  
* \subpage UsingFrameworkPage
*  
*  [libRocket]: https://github.com/libRocket/libRocket "libRocket"
*  
*  Исходные требования
*  -------------------
* \subpage RequirementsPage
*  
*  Разделы решения
*  ===============
*  
*  Структура проектов решения
*  --------------------------
*  
* \image html ".\Solution\(Documentation)\CovelliteProjects.png"
*
*  Build
*  -----
*  Описание раздела \subpage BuildPage
*
*  Проекты решения
*  ---------------
*  
*  ### Covellite.Rocket
*
* \insert_sCovelliteRocketBrief
* \begin_auto_insert_section
*  Проект реализации работы с GUI при помощи [libRocket].
* \end_auto_insert_section
*  
*  Страница проекта \subpage CovelliteRocketPage
*
*  ### Covellite.Api
*
* \insert_sCovelliteApiBrief
* \begin_auto_insert_section
*  Проект классов-оберток для графического API.
* \end_auto_insert_section
*  
*  Страница проекта \subpage CovelliteApiPage
*
*  ### Covellite.Egl
*
* \insert_sCovelliteEglBrief
* \begin_auto_insert_section
*  Проект классов-оберток для функций EGL.
* \end_auto_insert_section
*  
*  Страница проекта \subpage CovelliteEglPage
*
*  ### Covellite.Os
*
* \insert_sCovelliteOsBrief
* \begin_auto_insert_section
*  Проект классов, обеспечивающих взаимодействие с операционной системой.
* \end_auto_insert_section
*  
*  Страница проекта \subpage CovelliteOsPage
*
*  ### Covellite.Core
*
* \insert_sCovelliteCoreBrief
* \begin_auto_insert_section
*  Проект для кода ядра фреймворка.
* \end_auto_insert_section
*  
*  Страница проекта \subpage CovelliteCorePage
*
*  ### Covellite.Jni
*
* \insert_sCovelliteJniBrief
* \begin_auto_insert_section
*  Проект вспомогательных функций, выполняющих действия, недоступные из 
*  нативного кода.
* \end_auto_insert_section
*  
*  Страница проекта \subpage CovelliteJniPage
*
*  ### Example
*
* \insert_sExampleBrief
* \begin_auto_insert_section
*  Раздел примера использования фреймворка.
* \end_auto_insert_section
*  
*  Страница проекта \subpage ExamplePage
*
*  Внешние библиотеки/программы
*  ----------------------------
*  
* \include "Solution\(Documentation)\SdkVersions.txt"
*  
*  Страница используемых библиотек/утилит: [Externals]
*
*  [Externals]: http://unicornum.github.io/Externals/index.html "Externals"
*/
