﻿
/**
* \page CovelliteApiComponentSystemPage Компонентная система
*  
*  Используемые термины
*  ====================
*  
*  - \b Компонент - объект, содержащий информацию об одном аспекте игрового
*  объекта (таких как текстура, меш, положение и т.п.).
*  - \b Игровой \b объект - объект в игровом мире, содержащий информацию в виде 
*  набора компонентов.
*  - \b Игровой \b мир - набор игровых объектов, разбитый на иерархическую 
*  структуру из ячеек, каждая из которых содержит несколько игровых объектов; 
*  назначение - оптимизация подгрузки и выгрузки игровых объектов (сразу 
*  ячейками).
*  - \b Игровая \b сцена - набор игровых объектов, непосредственно участвующих
*  в игровом процессе; объекты подгружаются (по мере необходимости) из игрового 
*  мира и это именно те объекты, которые должны обновляться в процессе игры.
*  - \b Рендер - объект, предназначенный для отображения игровых объектов
*  в 3D сцене, создается на основе одного или набора нескольких компонентов.
*  - \b Объект \b 3D \b сцены - объект (набор рендеров), из которых состоит
*  3D сцена.
*  - \b 3D \b сцена - набор объектов 3D сцены, которые отображаются в окне 
*  программы; является 'зеркалом' той части объектов игровой сцены, которые
*  попадают в поле зрения камеры.
*
*  Исходные требования
*  ===================
*
*  - Не должно производиться никаких лишних действий (вызовов пустых функций),
*  для каждого объекта (игрового и 3D сцены) должен выполнятся набор только ему
*  необходимых действий.
*  - Отсутствие дублирования информации (все объекты, отрисовываемые при помощи
*  одного меша, должны содержать ссылку на единую область памяти с информацией
*  об этом меше).
*  
*  Компоненты
*  ==========
*
*  Фреймворк предоставляет класс компонента 
*  [Component](\ref covellite::api::Component), а также механизм создания 
*  рендеров для этих компонентов с учетом активного графического Api 
*  (рендеры создаются через объект класса 
*  [Renders](\ref covellite::api::Component::Renders), который может быть 
*  получен при помощи функции GetRenders() класса 
*  [Window](\ref covellite::api::Window)).
*  
*  Класс компонента:
*  - Содержит и предоставляет доступ к параметрам, которые используются 
*  для создания рендеров.
*  - Параметры (кроме обязательных) могут отсутствовать, для таких параметров
*  предусмотрено задание значений по умолчанию.
*  - Обязательный параметр \b id - идентификатор компонента. Для компонентов
*  с одним и тем же идентификатором будет использоваться один и тот же рендер,
*  поэтому объекты, содержащие один и тот же меш, могут содержать компоненты,
*  у которых задан только (один и тот же) идентификатор, а развернутое
*  описание этого компонента можно сделать в другом месте и создать его рендер
*  заранее.
*  - Обязательный параметр \b type - тип компонента, который определяет способ
*  отрисовки этого компонента.
*  - Параметр \b kind - дочерний подтип компонента; у некоторых типов 
*  не используется.
*  - Первоначальная информация может загружаться в виде строк, в дальнейшем
*  (для ускорения рендеринга) при обновлении игровых объектов значения 
*  параметров следует устанавливать того же типа, какой используют рендеры
*  (подробнее см. в описании параметорв компонентов).
*  
* \note
*  Предполагается, что исходная информация о компоненте будет хранится
*  в узлах xml файла, из которых параметры будут загружаться как атрибуты,
*  у которых имя атрибута будет названием параметра, а значение атрибута -
*  значением параметра.
*  
*  Типы компонентов
*  ----------------
*  
*  ### Data
*  
*  Вспомогательный компонент, предназначенный для передачи информации рендерам 
*  других типов.
*  
* \note
*  Для этого компонента не создается рендера, вместо этого он захватывается
*  и используется следующим рендером, который будет создан (какие данные нужны 
*  конкретным рендерам см. в описании соответствующих компонентов).
*  
*  | kind                            | Параметры                | Тип параметра              | Описание параметра                                                              |
*  | ------------------------------- | ------------------------ | -------------------------- | ------------------------------------------------------------------------------- |
*  | Position (\ref Footnote "1")    | x, y, z                  | float                      | Положение (координаты) в пространстве                                           |
*  | Rotation (\ref Footnote "1")    | x, y, z                  | float                      | Ориентация (углы поворота вокруг соответствующей оси в радианах) в пространстве |
*  | Scale (\ref Footnote "1")       | x, y, z                  | float                      | Масштабирующие коэффициенты по соответствующим осям                             |
*  | Direction (\ref Footnote "1")   | x, y, z                  | float                      | Вектор направления в пространстве                                               |
*  | Attenuation (\ref Footnote "1") | const, linear, exponent  | float                      | Настройки затухания для точечного источника света                               |
*  | Rect (\ref Footnote "1")        | left, top, right, bottom | int                        | Границы прямоугольника                                                          |
*  | Texture                         | data                     | const uint8_t *            | Бинарные данные текстуры в формате R8G8B8A8                                     |
*  | ^                               | width                    | int                        | Ширина изображения в пикселях                                                   |
*  | ^                               | height                   | int                        | Высота изображения в пикселях                                                   |
*  | Shader.HLSL                     | data                     | const uint8_t *            | Содержимое текстового файла шейдера DirectX в бинарном виде                     |
*  | ^                               | count                    | size_t                     | Размер данных шейдера в байтах                                                  |
*  | ^                               | version                  | std::string                | Версия шейдера (вида 'ps_4_0')                                                  |
*  | ^                               | entry                    | std::string                | Имя функции точки входа шейдера                                                 |
*  | Vertex.Gui (\ref Footnote "2")  | data                     | const [Vertex::Gui] *      | Данные вертексного буфера                                                       |
*  | ^                               | count                    | size_t                     | Размер данных вертексного буфера в байтах                                       |
*  | Vertex.Textured                 | data                     | const [Vertex::Textured] * | Данные вертексного буфера                                                       |
*  | ^                               | count                    | size_t                     | Размер данных вертексного буфера в байтах                                       |
*  | Index                           | data                     | const int *                | Данные индексного буфера                                                        |
*  | ^                               | count                    | size_t                     | Размер данных индексного буфера в байтах                                        |
*  
* \todo
*  Для шейдеров, текстур, вертексных и индексных буферов добавить параметр -
*  имя файла, из которого должны быть загружены данные.
*  
*  ### State
*  
*  Компонент изменения состояния конвеера рендеринга.
*  
*  | kind       | Требуется Data (kind)   | Параметры                | Тип параметра   | Описание параметра                |
*  | :--------: | :---------------------: | :----------------------: | :-------------: | --------------------------------- |
*  | Blend      |                         |                          |                 |                                   |
*  | Sampler    |                         |                          |                 |                                   |
*  | Scissor    | Rect                    | is_enabled               | bool            | В случае false Data.Rect не нужен |
*  
* \note
*  Каждая камера при рендеринге отключает блендинг.
*  
* \todo
*  Получение параметров Blend и Sampler - через компонент Data.
*  
*  ### Light
*  
*  Компонент источника света.
*  
*  Логика использования источников света предполагает:
*  - Активация рендера камеры создает новую сцену, в которой по умолчанию 
*  источники света отсутствуют (поэтому, если - например - требуется отрисовать 
*  несколько объектов без использования освещения, следует выделить их 
*  в отдельную сцену со своей уникальной камерой).
*  - Компоненты источников света могут располагаться в любом месте списка
*  объектов при создании рендеров (при этом необходимо учитывать, что
*  рендеры источников света добавляют информацию о себе в список источников 
*  света текущей сцены (другими словами - активной камеры), а при рендеринге 
*  текущего кадра используются источники света, установленные во время 
*  рендеринга предыдущего кадра).
*  
*  | kind      | Требуется Data (kind) | Параметры | Тип параметра | Описание параметра               |
*  |-----------|-----------------------|-----------|---------------|----------------------------------|
*  | Ambient   | -                     | ambient   | uint32_t      | Цвет в формате ARGB (0xFF00FF00) |
*  | ^         | ^                     | diffuse   | uint32_t      | Цвет в формате ARGB (0xFF00FF00) |
*  | ^         | ^                     | specular  | uint32_t      | Цвет в формате ARGB (0xFF00FF00) |
*  | Direction | Direction             | ambient   | uint32_t      | Цвет в формате ARGB (0xFF00FF00) |
*  | ^         | ^                     | diffuse   | uint32_t      | Цвет в формате ARGB (0xFF00FF00) |
*  | ^         | ^                     | specular  | uint32_t      | Цвет в формате ARGB (0xFF00FF00) |
*  | Point     | Position, Attenuation | ambient   | uint32_t      | Цвет в формате ARGB (0xFF00FF00) |
*  | ^         | ^                     | diffuse   | uint32_t      | Цвет в формате ARGB (0xFF00FF00) |
*  | ^         | ^                     | specular  | uint32_t      | Цвет в формате ARGB (0xFF00FF00) |
*  
* \note
*  - Фоновых и направленных источников света может быть не более одного, 
*  точечных может быть несколько.
*  - Пропущенные параметры устанавливаются в 0xFF000000.
*  - Параметры компонента Data.Attenuation используются следующим образом: \n
*  bright = color / (const + linear * distance + exponent * distance * distance).
*  - При удалении точечных источников света из 3D сцены следует учитывать, что
*  источник света, не попадающий в поле зрения камеры тем не менее может
*  освещать объекты, попадающие в поле зрения камеры.
*  - Если используется рендеринг при помощи шейдеров (DirectX10/11), то
*  информация об источниках света просто передается пиксельному шейдеру 
*  и необходимо самостоятельно задействовать эту информацию в шейдере (форматы
*  передачи данных шейдерам см. \ref CovelliteApiFxGroup); также для рендеринга
*  сцены без источников света следует использовать пиксельный шейдер, который
*  не использует информацию об источниках света.
*  
*  ### Material
*
*  Компонент материала (нужен для расчета освещения).
*
*  | Параметры | Тип параметра | Описание параметра               |
*  |-----------|---------------|----------------------------------|
*  | ambient   | uint32_t      | Цвет в формате ARGB (0xFF00FF00) |
*  | diffuse   | uint32_t      | Цвет в формате ARGB (0xFF00FF00) |
*  | specular  | uint32_t      | Цвет в формате ARGB (0xFF00FF00) |
*  | emission  | uint32_t      | Цвет в формате ARGB (0xFF00FF00) |
*  | shininess | int32_t       | Значение в диапазоне 0...0x80    |
*  
* \note
*  - Материал должен устанавливаться для каждого объекта 3D сцены, которому
*  требуется рассчет освещения.
*  - Рекомендуется заранее подготовить набор используемых в сцене рендеров 
*  материалов (дерево, металл, камень, пластик и т.п.) и в дальнейшем 
*  использовать рендеры этих компонентов.
*  - Пропущенные параметры устанавливаются в 0xFF000000.
*  - Если используется рендеринг при помощи шейдеров (DirectX10/11):
*   + Информация о материале объекта просто передается шейдеру и необходимо
*   самостоятельно задействовать эту информацию в пиксельном шейдере (форматы
*   передачи данных шейдерам см. \ref CovelliteApiFxGroup).
*   + Компонент материала при создании рендеров должен располагаться после
*   пиксельного шейдера.
*  
* ### Texture
*  
*  Компонент текстуры.
*  
*  | kind      | Требуется Data (kind)       | Параметры | Тип параметра | Описание параметра               |
*  |-----------|-----------------------------|-----------|---------------|----------------------------------|
*  | -         | Texture (\ref Footnote "3") | -         | -             | -                                |
*  
* \todo
*  Параметр для указания индекса текстуры при мультитекстурировании.
*  
* ### Shader
*  
*  Компонент шейдера.
*  
*  | kind                                | Требуется Data (kind)           |
*  |-------------------------------------|---------------------------------|
*  | Vertex.Gui (\ref Footnote "4")      | Shader.HLSL (\ref Footnote "3") |
*  | Vertex.Textured (\ref Footnote "5") | Shader.HLSL (\ref Footnote "3") |
*  | Pixel                               | Shader.HLSL (\ref Footnote "3") |
*  
*  Всем шейдерам в качестве заголовочных файлов передаются:
*  - Файл, содержащий описания структур для передачи данных шейдеру 
*  (\ref CovelliteApiFxGroup).
*  - Файл, содержащий описания входных данных вертексного шейдера
*  
* \snippet ./Solution/Covellite.Api/Covellite.Api/Renderer/fx/Input.fx Input vertex shared structures
*  
* ### Buffer
*  
*  Компонент буферов геометрии меша.
*  
*  | kind                                | Требуется Data (kind)               |
*  |-------------------------------------|-------------------------------------|
*  | Vertex.Gui (\ref Footnote "4")      | Vertex.Gui (\ref Footnote "3")      |
*  | Vertex.Textured (\ref Footnote "5") | Vertex.Textured (\ref Footnote "3") |
*  | Index                               | Index (\ref Footnote "3")           |
*  
* ### Present
*  
*  Компонент отправки информации в конвеер рендеринга.
*  
*  | kind                         | Требуется Data (kind)                         | Параметры | Тип параметра | Описание параметра                                                  |
*  |------------------------------|-----------------------------------------------|-----------|---------------|---------------------------------------------------------------------|
*  | Camera (\ref Footnote "6")   | Position, Rotation (\ref Footnote "7")        | distance  | float         | Расстояние от позиции, заданной компонентом Data.Position до камеры |
*  | ^                            | ^                                             | angle     | float         | Угол поля зрения по вертикали в градусах                            |
*  | ^                            | ^                                             | dept      | String        | Enabled - включает использование буфера глубины                     |
*  | ^                            | ^                                             | focal     | String        | Enabled - включает использование перспективной проекции             |
*  | Geometry (\ref Footnote "8") | Position, Rotation, Scale (\ref Footnote "9") | -         | -             | -                                                                   |
*  
*  Сноски
*  ------
*  
* \anchor Footnote
*  -# Параметры этих компонентов могут изменяться во время работы программы
*  (т.е. для изменения положения объекта в пространстве не нужно его
*  пересоздавать, достаточно установить новые координаты комопоненту его
*  положения), для изменения остальных (например замены 3D модели объекта)
*  необходимо создать новый набор компонентов, удалить из 3D сцены старый
*  и создать новый объект.
*  -# [Vertex::Gui] создан специально для рендеринга элементов Gui 
*  и соответствует формату данных, получаемых от libRocket.
*  -# Параметр \b file можно передавать ТОЛЬКО через компонента Data, все 
*  остальные параметры можно указывать непосредственно основному компоненту, 
*  без использования компонента Data.
*  -# Если вертексный буфер меша передается как [Vertex::Gui].
*  -# Если вертексный буфер меша передается как [Vertex::Textured].
*  -# Предполагается, что камера 'смотрит' в точку, заданную компонентом
*  Data.Position с расстояния \b distance с учетом ориентации, заданной
*  компонентом Data.Rotation (если нужен вид 'от первого лица', установить
*  distance в ноль).
*  -# Отсутствие компонента Data.Rotation или нулевые значения
*  углов ориентации соответствуют направлению взгляда из точки, смещенной 
*  от Data.Position на (distance, 0.0f, 0.0f).
*  -# Компонент должен завершать набор компонентов каждого объекта 3D
*  сцены и должен иметь уникальный id для каждого конкретного объекта.
*  -# Может быть несколько компонентов, что можно использовать для 
*  привязки (совместного движения) объектов друг к другу (в этом случае
*  необходимо гарантировать, чтобы в качестве общего компонента использовался
*  ОДИН И ТОТ ЖЕ объект, совпадения идентификаторов компонентов недостаточно!).
*  
*  [Vertex::Gui]: \ref covellite::api::Vertex::Gui
*  [Vertex::Textured]: \ref covellite::api::Vertex::Textured
*
*  Общая концепция использования компонентов
*  =========================================
*  
*  Основная идея работы с 3D объектами предполагает использование архитектуры
*  программы на основе паттерна MVC.
*  
*  Модель
*  ------
*  
*  - Загружает игровой мир как набор ячеек, каждая из которых ссылается 
*  на место хранения информации о содержащихся в ячейке объектах.
*  - Содержит базу данных игровых объектов и функции, которые позволяют получать 
*  набор компонентов (для обновления и рендеринга) игрового объекта по его 
*  идентификатору.
*  - Содержит игровую сцену(ы), которая включает идентификаторы камеры, а также 
*  источников света и объектов, находящихся рядом с камерой.
*  - При перемещениях игрока добавляются приблизившиеся к камере и удаляются 
*  удалившиеся объекты (группами в ячейках).
*  - На стадии обновления обновляются объекты игровой сцены (можно обновлять 
*  объекты постепенно удаляясь от камеры пока не выйдет заданное на обновление 
*  время).
*  - Состояние 3D сцены является отражением состояния игрового мира, поэтому 
*  компоненты рендеров удаляются из 3D сцены при удалении из игрового мира 
*  ячеек, содержащих описания этих компонентов.
*  
*  Представление
*  -------------
*  
*  - Класс окна, которое создается между окнами Api и Gui.
*  - Содержит, создает и удаляет объекты 3D сцены на основе информации, 
*  полученной от модели.
*  - При построении/обновлении 3D сцены первой должна идти камера.
*  - Изменение геометрии/материала производится удалением/добавлением игрового 
*  объекта.
*  
*  Взаимодействие частей кода игры
*  -------------------------------
*  
*  - Панель GUI с кнопками действий игрока (View) подписывается на события 
*  нажатия кнопок, при нажатии которых вызывает соответствующие функции модели,
*  после чего активирует событие обновления 3D сцены.
*  - Игровая сцена (Model) при действии игрока:
*   + Очищается от объектов, ранее помеченных для удаления.
*   + Обновляет состояние оставшихся объектов.
*   + Объекты, которые выпадают из зоны действия камеры, помечаются на удаление.
*   + Добавляются объекты, которые теперь попадают в зону действия камеры.
*  - По событию обновления 3D сцены, она:
*   + Запрашивает у модели изменения набора активных объектов.
*   + Удаляет объекты, которые больше не попадают в поле зрения камеры.
*   + Добавляет объекты, которые попали в поле зрения камеры.
*   + Рендерит актуальный набор объектов.
*  
*  Примеры использования компонентов
*  =================================
*  
* \todo
*  Пример формирования сцены из набора компонентов.
*/
