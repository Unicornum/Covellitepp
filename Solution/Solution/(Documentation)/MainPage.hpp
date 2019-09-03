
/**
* \mainpage notitle
* \anchor MainPage
* 
*  Covellite++
*  ===========
*  
*  Описание фреймворка
*  -------------------
*
*  Фреймворк базируется на [libRocket] и предназначен для создания приложений,
*  содержащих стандартные элементы управления (кнопки, поля ввода, ckeckbox'ы,
*  radiobutton'ы и т.п.) с возможностью указания стиля их отображения
*  (аналог css).
*  
*  Представляет собой набор классов-оберток для создания:
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
* ### Основные характеристики
*  
*  - Кросс-платформенный:
*   + Windows (тестируется на Windows 7/10 x64).
*   + Android (тестируется на устройствах с ARM API15 и ARM64 API24).
*  - Позволяет встраивать код дополнительной отрисовки кадра, поверх которого
*  будет отображаться GUI.
*  - Быстрый (Windows версия выдает ~1500 fps в режиме непрерывной отрисовки
*  на ноутбуке, купленном в 2013-м году).
*  
* ### Доступные реализации
*  
*  Задаются как параметр настроек \b GraphicsApi в разделе \b Window.
*  
*  - \b DirectX11 (шейдеры) - требует Windows 7 и выше + поддержка со стороны
*  железа.
*  - \b OpenGL (шейдеры) - требует Windows 7 и выше + поддержка со стороны
*  железа.
*  - \b OpenGLES3 (шейдеры) - требует Android 4.3+.
*  - \b OpenGLES (статический конвеер) - требует Android 4.0+.
*  - \b Auto - будет использоваться графический Api, максимально доступный 
*  на данном устройстве:
*   + Windows: DirectX11//OpenGL.
*   + Android: OpenGLES3//OpenGLES.
*  
* \note
*  Все реализации со статическим конвеером оставлены для обратной совместимости,
*  расширятся и исправляться не будут; будут удалены впоследствии.
*  
*  Видео
*  -----
*  
*  [Демонстрация рендеринга перемещения по бесконечному случайно генерируемому ландшафту](https://www.youtube.com/watch?v=ZlwrSg_TVN0)
*  
*  [Рендеринг анимированных объектов](https://www.youtube.com/watch?v=t6BJoWVIIZA)
*  
*  [Поддержка использования PBR-материалов](https://www.youtube.com/watch?v=8lqK8l__3D0)
*  
*  Скриншоты
*  ---------
*
*  ### Windows
*
* \htmlonly

<img class="PreviewImage" src="Covellitepp_v2.10.jpg">
<img class="PreviewImage" src="Covellitepp_v2.11.jpg">
<img class="PreviewImage" src="Covellitepp_v2.12.jpg">
<img class="PreviewImage" src="Covellitepp_v2.13.jpg">
<img class="PreviewImage" src="Covellitepp_v2.14.jpg">

* \endhtmlonly
*
* \htmlonly

<img class="PreviewImage" src="Covellitepp_v2.15.jpg">
<img class="PreviewImage" src="Covellitepp_v2.16.jpg">
<img class="PreviewImage" src="Covellitepp_v2.17.jpg">
<img class="PreviewImage" src="Covellitepp_v2.18.jpg">
<img class="PreviewImage" src="Covellitepp_v2.19.jpg">

* \endhtmlonly
*
*  ### Android
*
* \htmlonly

<img class="PreviewImage" src="Covellitepp_v2.20.jpg">
<img class="PreviewImage" src="Covellitepp_v2.21.jpg">
<img class="PreviewImage" src="Covellitepp_v2.22.jpg">
<img class="PreviewImage" src="Covellitepp_v2.23.jpg">
<img class="PreviewImage" src="Covellitepp_v2.24.jpg">

* \endhtmlonly
*
* \htmlonly

<img class="PreviewImage" src="Covellitepp_v2.25.jpg">
<img class="PreviewImage" src="Covellitepp_v2.26.jpg">
<img class="PreviewImage" src="Covellitepp_v2.27.jpg">
<img class="PreviewImage" src="Covellitepp_v2.28.jpg">
<img class="PreviewImage" src="Covellitepp_v2.29.jpg">

* \endhtmlonly
* \copydoc PreviewPage
*  
*  ----------------------------------------------------------------------------
*  
* \subpage UsingFrameworkPage
*  
* \subpage DownloadsPage
*
* \subpage HistoryPage
*  
* \subpage SolutionPage
*
* \subpage RequirementsPage
*  
*  Лицензия
*  --------
*  
*  [Boost Software License](http://www.boost.org/LICENSE_1_0.txt)
*
*  Автор(ы)
*  --------
*  
* \copyrightsolution
*  
*  ----------------------------------------------------------------------------
*  
*  [libRocket]: https://github.com/libRocket/libRocket "libRocket"
*/

/**
* \page ScreenShotsPage ScreenShots
*
* \image html ".\Solution\Solution\(Documentation)\Screenshots\Covellitepp_v2.10.jpg"
* \image html ".\Solution\Solution\(Documentation)\Screenshots\Covellitepp_v2.11.jpg"
* \image html ".\Solution\Solution\(Documentation)\Screenshots\Covellitepp_v2.12.jpg"
* \image html ".\Solution\Solution\(Documentation)\Screenshots\Covellitepp_v2.13.jpg"
* \image html ".\Solution\Solution\(Documentation)\Screenshots\Covellitepp_v2.14.jpg"
* \image html ".\Solution\Solution\(Documentation)\Screenshots\Covellitepp_v2.15.jpg"
* \image html ".\Solution\Solution\(Documentation)\Screenshots\Covellitepp_v2.16.jpg"
* \image html ".\Solution\Solution\(Documentation)\Screenshots\Covellitepp_v2.17.jpg"
* \image html ".\Solution\Solution\(Documentation)\Screenshots\Covellitepp_v2.18.jpg"
* \image html ".\Solution\Solution\(Documentation)\Screenshots\Covellitepp_v2.19.jpg"
* \image html ".\Solution\Solution\(Documentation)\Screenshots\Covellitepp_v2.20.jpg"
* \image html ".\Solution\Solution\(Documentation)\Screenshots\Covellitepp_v2.21.jpg"
* \image html ".\Solution\Solution\(Documentation)\Screenshots\Covellitepp_v2.22.jpg"
* \image html ".\Solution\Solution\(Documentation)\Screenshots\Covellitepp_v2.23.jpg"
* \image html ".\Solution\Solution\(Documentation)\Screenshots\Covellitepp_v2.24.jpg"
* \image html ".\Solution\Solution\(Documentation)\Screenshots\Covellitepp_v2.25.jpg"
* \image html ".\Solution\Solution\(Documentation)\Screenshots\Covellitepp_v2.26.jpg"
* \image html ".\Solution\Solution\(Documentation)\Screenshots\Covellitepp_v2.27.jpg"
* \image html ".\Solution\Solution\(Documentation)\Screenshots\Covellitepp_v2.28.jpg"
* \image html ".\Solution\Solution\(Documentation)\Screenshots\Covellitepp_v2.29.jpg"
*/

/**
* \page PreviewPage
* \htmlonly

<style>

.PreviewImage {
  width:100%;
  max-width:200px;
  border-radius: 10px;
  cursor: pointer;
  transition: 0.3s;
}

.PreviewImage:hover {
  opacity: 0.7;
}

.modal{
  display: none;
  position: fixed;
  z-index: 9999;
  padding-top: 100px;
  left : 0;
  top : 0;
  width: 100%;
  height: 100%;
  overflow: auto;
  background-color: rgb(0,0,0);
  background-color: rgba(0,0,0,0.9);
}

.modal-content{
  margin: auto;
  display: block;
  height: 80%;
}

#caption{
  margin: auto;
  display: block;
  width: 80%;
  max-width: 700px;
  text-align: center;
  color: #ccc;
  padding: 10px 0;
  height: 150px;
}

.modal-content, #caption{
  animation-name: zoom;
  animation-duration: 0.6s;
}

@keyframes zoom{
  from {transform:scale(0)}
  to {transform:scale(1)}
}

.close{
  position: absolute;
  top: 15px;
  right: 35px;
  color: #f1f1f1;
  font-size: 40px;
  font-weight: bold;
  transition: 0.3s;
}

.close:hover,
.close : focus{
  color: #bbb;
  text-decoration: none;
  cursor: pointer;
}

@media only screen and (max-width: 700px) {
  .modal-content{
    width: 100 % ;
  }
}
</style>

<!-- Окно предварительного просмотра -->
<div id="PreviewWindow" class="modal">
  <span class="close">&times;</span>
  <img class="modal-content" id="FullSizeImage">
</div>

<script>

// Для всех изображений...
var Images = document.getElementsByClassName('PreviewImage');

for (i = 0; i < Images.length; i++)
{
  Images[i].onclick = function()
  {
    document.getElementById('PreviewWindow').style.display = "block";
    document.getElementById("FullSizeImage").src = this.src;
  };
}

// Для элемента, закрывающего окно предпросмотра...
var Close = document.getElementsByClassName("close")[0];

Close.onclick = function()
{
  document.getElementById('PreviewWindow').style.display = "none";
}

</script>

* \endhtmlonly
*/