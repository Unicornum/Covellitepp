
/**
* \page CovelliteApiBriefPage
*  Проект классов-оберток для графического API.
*/

/**
* \page CovelliteApiPage Covellite.Api
*  
*  Список классов проекта \ref CovelliteApiGroup
*  
*  Введение
*  --------
*  
* \copydoc CovelliteApiBriefPage
*  
*  Подробнее
*  ---------
*  
*  - Классы проекта предоставляют реализации для различных графических API
*  (OpenGL/OpenGL ES/DirectX).
*  - Для каждого конкретного графического API создается отдельный класс.
*  - Каждый класс доступен только для использования в коде для конкретной 
*  платформы.
*  
* \image html ".\Solution\Covellite.Api\(Documentation)\Classes.png"
*  
* ### Covellite.Api.es.Android
*  
*  Проект для сборки библиотеки, содержащей реализацию только для OpenGL ES.
*  Оставлен для обратной совместимости, для возможности запускать программы
*  на базе Covellite++ на устройствах, не поддерживающих OpenGL ES3.
*  
*  Как настроить сборку и запуск APK файла, содержащего две реализации, см.
*  [здесь](\ref aRunBackwardCompatibilityLib).
*  
*  Компонентная система
*  --------------------
*  
* \subpage CovelliteApiComponentSystemPage
*/
