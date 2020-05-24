﻿
/**
* \page CovelliteApiGlossaryPage Глоссарий
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
*  в 3D сцене, создается на основе одного или нескольких компонентов.
*  - \b Объект \b 3D \b сцены - объект (набор рендеров), из которых состоит
*  3D сцена.
*  - \b 3D \b сцена - набор объектов 3D сцены, которые отображаются в окне
*  программы; является 'зеркалом' той части объектов игровой сцены, которые
*  попадают в поле зрения камеры.
*/

/**
* \page CovelliteApiComponentSystemPage Компонентная система
*  
*  Используемые термины
*  ====================
*  
* \copydoc CovelliteApiGlossaryPage
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
*  предусмотрено значение по умолчанию.
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
*  - Для этого компонента не создается рендера, вместо этого он захватывается
*  и используется следующим рендером, который будет создан (какие данные нужны 
*  конкретным рендерам см. в описании соответствующих компонентов).
*  - Идентификатор этого компонента не используется.
*  
*  | kind                         | Параметры                | Тип параметра (значение по умолчанию) | Описание параметра                                                                                       |
*  | ---------------------------- | ------------------------ | ------------------------------------- | -------------------------------------------------------------------------------------------------------- |
*  | Position (\ref Footnote "1") | x, y, z                  | float (0.0f)                          | Положение (координаты) в пространстве                                                                    |
*  | Rotation (\ref Footnote "1") | x, y, z                  | float (0.0f)                          | Ориентация (углы поворота вокруг соответствующей оси в радианах) в пространстве                          |
*  | Scale (\ref Footnote "1")    | x, y, z                  | float (1.0f)                          | Масштабирующие коэффициенты по соответствующим осям                                                      |
*  | Rect (\ref Footnote "1")     | left, top, right, bottom | int                                   | Границы прямоугольника                                                                                   |
*  | Texture                      | data                     | const uint8_t *                       | Бинарные данные текстуры в формате R8G8B8A8                                                              |
*  | ^                            | width                    | int                                   | Ширина изображения в пикселях                                                                            |
*  | ^                            | height                   | int                                   | Высота изображения в пикселях                                                                            |
*  | ^                            | destination              | String_t (albedo)                     | Назначение текстуры (albedo, metalness, roughness, normal, occlusion)                                    |
*  | ^                            | mipmapping               | bool (false)                          | Генерировать mip'ы для текстуры; только для текстур, не используемых как внеэкранные поверхности         |
*  | ^                            | mapper                   | std::function<bool(const void *)>     | Функция обратного вызова для чтения данных текстуры (каждый пиксель - это число uint32_t в формате ABGR) |
*  | Shader                       | data                     | const uint8_t *                       | Содержимое текстового файла шейдера в бинарном виде                                                      |
*  | ^                            | count                    | size_t                                | Размер буфера данных шейдера в байтах                                                                    |
*  | ^                            | entry                    | String_t                              | Имя функции точки входа шейдера                                                                          |
*  | ^                            | instance                 | String_t                              | Описатель (вида 'f4f4i4') структуры [инстанс-буфера](\ref CovelliteApiInstancing)                        |
*  | Buffer                       | data                     | const Vertex *                        | Данные вертексного буфера для отрисовки плоских объектов (\ref Footnote "2")                             |
*  | ^                            | ^                        | const int *                           | Данные индексного буфера                                                                                 |
*  | ^                            | count                    | size_t                                | Количество элементов в буфере, переданном в \b data или инстанс-буфере                                   |
*  | ^                            | mapper                   | std::function<bool(void *)>           | Функция обратного вызова для заполнения [инстанс-буфера](\ref CovelliteApiInstancing)                    |
*  | ^                            | size                     | size_t                                | Размер [инстанс-буфера](\ref CovelliteApiInstancing) в байтах                                            |
*  
* \deprecated
*  Следующие компоненты передачи данных устарели и будут удалены в следующей
*  стабильной версии.
*  
*  | kind                            | Параметры               | Тип параметра                | Описание параметра                                                                     |
*  | ------------------------------- | ----------------------- | ---------------------------- | ---------------------------------------------------------------------------------------|
*  | Direction (\ref Footnote "1")   | x, y, z                 | float                        | Вектор направления (из указанной точки в начало координат) в пространстве              |
*  | Attenuation (\ref Footnote "1") | const, linear, exponent | float                        | Настройки затухания для точечного источника света                                      |
*  | Buffer                          | data                    | const [Vertex::Polygon] *    | Данные вертексного буфера для отрисовки плоских объектов (\ref Footnote "2")           |
*  | ^                               | ^                       | const [Vertex::Polyhedron] * | Данные вертексного буфера для отрисовки объемных объектов                              |
*  | ^                               | dimension               | int                          | 2 или 3 (по умолчанию) - размерность отрисовываемого объекта                           |
*  | Fog                             | color                   | uint32_t (0xFFFFFFFF)        | Цвет тумана в формате ARGB                                                             |
*  | ^                               | near, far               | float                        | Расстояния до передней и задней дистанции при расчете тумана (нужны для linear)        |
*  | ^                               | density                 | float                        | Коэффициент, определяющий скорость возрастания плотности тумана (нужен для exp и exp2) |
*
* \note
*  Параметр \b dimension для вертексного буфера имеет смысл только для 
*  формата вершин [Vertex] и реализаций со статическим конвеером для указания
*  того, как следует интерпретировать данные вертексного буфера - 2D или 3D
*  объекты; шейдерные реализации просто передают буфер шейдеру и поэтому
*  следует устанавливать шейдер, который будет правильно использовать
*  переданные данные.
*  
*  ### Camera
* 
*  Компонент виртуальной камеры.
*
*  | kind         | Требуется Data (kind) | Параметры | Тип параметра (значение по умолчанию) | Описание параметра                                                  |
*  |--------------|-----------------------|-----------|---------------------------------------|---------------------------------------------------------------------|
*  | Orthographic | Position              | -         | -                                     |                                                                     |
*  | Perspective  | Position, Rotation    | distance  | float (0.0f)                          | Расстояние от позиции, заданной компонентом Data.Position до камеры |
*  | ^            | ^                     | fov       | float (90.0f)                         | Угол поля зрения по вертикали в градусах                            |
*  | ^            | ^                     | znear     | float (0.01f)                         | Ближняя плоскость отсечения                                         |
*  | ^            | ^                     | zfar      | float (200.0f)                        | Дальняя плоскость отсечения                                         |
*  
* \note
*  - Камера определяет способ отрисовки сцены и ее рендер должен быть первым
*  в списке 3D сцены.
*  - Компонент формирует матрицы вида/проекции и передает их шейдеру
*  (константный буфер шейдера \b CameraData).
*  - Сформированные матрицы вида/проекции после вызова рендера будут записаны
*  исходному объекту компонента камеры как параметры \b view и \b projection
*  в виде объекта класса glm::mat4 (матрицы записываются в том же виде, что
*  и при передаче шейдеру, т.е. транспонированными).
*  
*  ---
*  
*  \b Ортографическа камера использует левостороннюю систему координат, 
*  в которой:
*  - Начало координат находится в левом верхнем углу экрана (viewport'ом
*  в Windows является вся клиентская часть окна, в Android - все за исключением
*  заголовка окна).
*  - Координаты вдоль оси X увеличиваются вправо.
*  - Координаты вдоль оси Y увеличиваются вниз.
*  - Камера смотрит на плоскость Oxy со стороны положительных значений оси Z.
*  - Одному пикселю окна программы соответствует изменение координаты на 1.0f.
*
* \todo
*  - Добавить картинку с осями координат для Windows и Android.
*  
*  \b Перспективная камера использует правостороннюю систему координат:
*  - Координаты вдоль оси X увеличиваются с запада на восток.
*  - Координаты вдоль оси Y увеличиваются с юга на север.
*  - Координаты вдоль оси Z (высота над плоскостью Oxy) увеливаются снизу вверх.
*  - Камера смотрит в точку, заданную компонентом Data.Position с расстояния
*  \b distance (т.е. реализован вид 'от третьего лица', если нужен вид 
*  'от первого лица', установить distance в ноль).
*  - Data.Rotation - это углы поворота самой камеры относительно точки
*  Data.Position (т.е. направление, \b обратное направлению взгляда).
*  - Отсутствие компонента Data.Rotation (нулевые значения углов ориентации)
*  соответствует направлению взгляда из точки, смещенной от Data.Position на 
*  distance вдоль оси X в положительном направлении.
*  
*  ### BkSurface
*  
*  Компонент внеэкранной поверхности, предназначен для рендеринга в текстуру(ы).
*  
*  | kind         | Требуется Data (kind) | Параметры | Тип параметра | Описание параметра                                                  |
*  |--------------|-----------------------|-----------|---------------|---------------------------------------------------------------------|
*  | -            | Texture               | -         | -             |                                                                     |
*  
*  Подробнее о использовании компонента см. \ref CovelliteApiBkSurface.
*  
*  ### State
*  
*  Компонент изменения состояния конвеера рендеринга.
*  
*  | kind      | Требуется Data (kind) | Параметры | Тип параметра | Описание параметра                                                                             |
*  | --------- | --------------------- | --------- | ------------- | ---------------------------------------------------------------------------------------------- |
*  | Clear     |                       | color     | uint32_t      | Цвет заливки буфера кадра                                                                      |
*  | Depth     |                       | enabled   | bool (false)  | Включение/отключение использования буфера глубины (используется инвертировааный буфер глубины) |
*  | ^         | ^                     | clear     | bool (false)  | Включение/отключение очистки буфера глубины                                                    |
*  | ^         | ^                     | overwrite | bool (true)   | Включение/отключение перезаписи буфера глубины                                                 |
*  | Blend     |                       |           |               | Включение использования прозрачности                                                           |
*  | Sampler   |                       |           |               | Включение использования сглаживания текстур                                                    |
*  | Scissor   | Rect                  | enabled   | bool          | В случае false Data.Rect не нужен                                                              |
*  | AlphaTest |                       | discard   | float         | Отбрасывание значений цвета, у которых значение Alpha-канала меньше или равно указанному       |
*  
* \note
*  Каждая камера при рендеринге отключает:
*  - Использование внеэкранной поверхности (устаналивает вывод нап экран).
*  - Использование буфера глубины.
*  - Блендинг.
*  - Alpha test (компонент имеет смысл только для реализаций со статическим
*  конвеером; реализации, которые используют шейдеры, должны делать это при
*  помощи шейдера).
*  
* ### Shader
*
*  Компонент шейдера.
*
*  | kind   | Требуется Data (kind)      |
*  |--------|----------------------------|
*  | -      | Shader (\ref Footnote "3") |
*
* \note
*  - Тип создаваемого шейдера (вертексный или пиксельный) выводится
*  автоматически из входного типа данных указанной функции точки входа.
*  - Всем шейдерам в качестве заголовочных файлов передаются:
*    + Файл, содержащий описания структур для передачи данных шейдеру
*  (\ref CovelliteApiFxGroup).
*    + Файл, содержащий описания форматов входных данных шейдеров
*  - Если не указаны бинарные данные шейдера (параметры \b data и \b count),
*  вместо них будут использоваться шейдеры по умолчанию (те, что использует
*  фреймворк для рендеринга Gui).
*  - В том случае, если в тексте шейдера содержится ошибка, рендер не будет
*  создан и попытка отрендерить объект без установленного шейдера может
*  привести к падению видеодрайвера. Поэтому хорошей практикой является
*  установка объекту камеры компонентов шейдеров по умолчанию (без передачи
*  данных шейдера \b data и \b count), которые в этом случае позволят
*  отрендерить объект 'абы как'.
*
*  Подробнее о том, как формируется окончательный текст шейдера, передаваемый
*  компилятору, см. \ref CovelliteApiShadersPage.
*  
* ### Texture
*  
*  Компонент текстуры.
*  
*  | kind      | Требуется Data (kind)       |
*  |-----------|-----------------------------|
*  | -         | Texture (\ref Footnote "3") |
*  
* \note
*  - Реализации со статическим конвеером используют только единственную
*  текстуру (тип можно не указывать), устанавливая ее в нулевой текстурный слот.
*  - Параметр \b destination компонента данных текстуры предназначен
*  для указания роли текстуры в рамках логики \b PBR-текстурирования.
*  - В шейдерных реализациях для рендеринга с использованием PBR требуется
*  специальный пиксельный шейдер, т.к. текстуры просто передаются шейдеру
*  в слоты:
*   + HLSL (\b albedo - t0, \b metalness - t1, \b roughness - t2,
*           \b normal - t3, \b occlusion - t4, \b depth - t5).
*   + GLSL (роль не указана - TexDiffuse, \b albedo - TexAlbedo,
*           \b metalness - TexMetalness, \b roughness - TexRoughness,
*           \b normal - TexNormal, \b occlusion - TexOcclusion,
*           \b depth - TexDepth).
*  - Параметр \b mapper используется для установки функции обратного вызова,
*  которая вызывается при активации рендера и предназначена для \b чтения
*  содержимого буфера текстуры (только для текстур, используемых в качестве
*  внеэкранных поверхностей). Нужно понимать, что при этом данные текстуры
*  копируются из видеопамяти в системную память, что является тяжелой операцией
*  и использование ее при каждой активации текстуры (возврат true при каждом
*  вызове функции обратного вызова со значением nullptr) сильно ударит
*  по производительности. Чтение данных из текстуры имеет смысл использовать
*  в качестве особой, разовой операции; например, при обработке клика мышью.
*  
* ### Buffer
*  
*  Компонент буферов геометрии меша и констант шейдера.
*  
*  | kind     | Требуется Data (kind)      | Параметры | Тип параметра                         | Описание параметра                                                                                                   |
*  |----------|----------------------------|-----------|---------------------------------------|----------------------------------------------------------------------------------------------------------------------|
*  | Vertex   | Buffer (\ref Footnote "3") | mapper    | std::function<bool(const [Vertex] *)> | Функция обратного вызова для изменения буфера                                                                        |
*  | Constant | ^                          | ^         | std::function<bool(const void *)>     | Функция обратного вызова для заполнения [пользовательского](\ref CovelliteApiUserConstantBuffer) буфера              |
*  | ^        | ^                          | size      | size_t                                | Размер константного буфера в байтах (когда в качестве \b mapper используется std::function<bool(void *)>)            |
*  | ^        | ^                          | name      | String_t                              | Имя переменной [пользовательского](\ref CovelliteApiUserConstantBuffer) буфера в шейдере ('cbUserData' по умолчанию) |
*  
* \todo
*  Требуется обобщение способа передачи параметров константного и
*  инстанс-буфера, т.к. они совпадают, но в первом случае они передаются самому
*  компоненту, а во втором - компоненту Data.Buffer.
*  
* \deprecated
*  Следующие компоненты устарели и будут удалены в следующей стабильной версии,
*  вместо вертексного буфера [Vertex::Polyhedron] следует использовать [Vertex],
*  вместо индексного буфера - компонент Present.Index, передавая данные ему.
*  
*  | kind     | Требуется Data (kind)      | Параметры | Тип параметра                                     | Описание параметра                                                        |
*  |----------|----------------------------|-----------|---------------------------------------------------|---------------------------------------------------------------------------|
*  | Constant | Buffer (\ref Footnote "3") | mapper    | std::function<bool(const Lights_t *)>             | Функция обратного вызова для формирования списка источников света объекта |
*  | Vertex   | ^                          | ^         | std::function<bool(const [Vertex::Polyhedron] *)> | Функция обратного вызова для изменения буфера                             |
*  | Index    | ^                          |           |                                                   |                                                                           |
*  
* \note
*  - Параметр \b kind можно не указывать, тип буфера выводится автоматически
*  из типа переданных ему данных.
*  - Порядок обхода вершин вертексного буфера - против часовой стрелки.
*  - Если передан параметр \b mapper, будет создан буфер, который можно
*  изменять (только содержимое, причем можно изменять только часть значений;
*  размер буфера, равный значению параметра \b count изменить нельзя) прямо
*  во время работы программы. Переданный функциональный объект будет вызываться
*  два раза:
*   + Со значением параметра nullptr - вернуть true или false в зависимости от
*  того, следует ли обновлять буфер или нет.
*   + С указателем на начало памяти буфера, если обновление требуется
*  (возращаемое значение не используется).
*  - Константный буфер для формирования списка источников света объекта
*  в шейдерных реализациях просто сохраняет их во внутренний буфер, после чего
*  они передаются шейдеру вместе с матрицей трансформации объекта (константный
*  буфер шейдера \b ObjectData.Lights), поэтому этот компонент должен
*  располагаться перед компонентом \b Transform.
*  
*  ---
*  
*  Логика работы с источниками света через константный буфер с ограниченным
*  количеством слотов для точечных источников света построена исходя из
*  оптимизации расчета освещения каждого объекта (как передача данных, так и
*  расчет освещения для большого количества источников света шейдером очень
*  времязатратная операция, fps катастрофически падает уже при паре десятков
*  источников света в сцене). Сцена может содержать сотни точечных источников
*  света, но здравый смысл подсказывает, что они будут распределены по сцене
*  более-менее равномерно и каждый конкретный объект будет освещен небольшим
*  (единицы) количеством источников света. Поэтому строить набор компонентов
*  объектов сцены имеет смысл следующим образом:
*  - При загрузке сцены все объекты, являющиеся точечными источниками света,
*  добавляют свои параметры в общий список источников света сцены.
*  - Камере добавляется компонент Buffer.Constant, функция обратного вызова
*  которого (\b mapper) формирует значения фонового и направленного источников
*  света и обнуляет количество слотов точечных источников света.
*  - Каждому объекту сцены добавляется компонент Buffer.Constant, функция
*  обратного вызова которого перебирает список всех точечных источников света
*  и добавляет в список источников света объекта те, которые освещают его
*  сильнее всего (например, самые близкие); максимальное количество -
*  \b COVELLITE_MAX_LIGHT_POINT_OBJECT_COUNT (если этого количества
*  недостаточно, можно для этой цели использовать
*  [пользовательский](\ref CovelliteApiUserConstantBuffer) константный буфер).
*  - Расчет радиуса действия для точечного источника света можно посмотреть
*  [здесь](https://habr.com/ru/post/420565/).
*  - Для статичных объектов набор статичных же источников света можно
*  формировать один раз.
*  
* ### Transform
*
*  Компонент установки положения/ориентации/масштабирования объекта.
*
*  | kind      | Требуется Data (kind)                       | Описание параметра                                                                               |
*  |-----------|---------------------------------------------|--------------------------------------------------------------------------------------------------|
*  | -         | Position/Rotation/Scale (\ref Footnote "4") | Объект, положение/ориентация/масштаб которого \b могут изменятся во время работы программы       |
*  | Static    | ^                                           | Объект, положение/ориентация/масштаб которого \b не \b будут изменятся во время работы программы |
*  | Billboard | Position (\ref Footnote "4")                | Объект, всегда развернутый 'лицом' к камере                                                      |
*
* \note
*  - Компонент формирует матрицу трансформации объекта и передает ее шейдеру
*  (константный буфер шейдера \b ObjectData.World).
*  - У статического объекта мировая матрица будет вычислена один раз при его
*  создании и в дальнейшем изменить его положение/ориентацию/масштаб будет
*  невозможно; динамический объект захватывает переданные ему компоненты Data,
*  поэтому его положение/ориентацию/масштаб можно менять в каждом кадре
*  изменяя значения параметров этих компонентов.
*  - 'Лицом' billboard'a считается плоскость Oxy при взгляде со стороны
*  положительных значений вдоль оси z.
*
* ### Present
*
*  Компонент отправки информации в конвеер рендеринга.
*
*  | kind     | Требуется Data (kind)      |
*  |----------|----------------------------|
*  | Index    | Buffer (\ref Footnote "3") |
*  | Instance | Buffer                     |
*
* \note
*  - Компонент Present должен завершать набор компонентов каждого объекта
*  3D сцены.
*  - Указание значения \b kind не обязательно; если задан только Buffer.Index,
*  будет создан компонент Present.Index, если заданы и Buffer.Index и
*  Buffer.Instance - [Present.Instance](\ref CovelliteApiInstancing).
*  
* ### Updater
*  
*  Компонент обновления объекта. Позволяет установить функциональный объект
*  (функцию обратного вызова), связанную с конкретным объектом.
*  
*  
*  | Параметры | Тип параметра                    | Описание параметра                                                               |
*  |-----------|----------------------------------|----------------------------------------------------------------------------------|
*  | function  | std::function<void(const float)> | Функциональный объект, который будет вызываться при активации рендера компонента |
*  
* \note
*  - Переданному функциональному объекту при вызове будет передаваться время
*  рендеринга текущего кадра (отсчет ведется с момента запуска программы)
*  в секундах.
*  - Гарантируется, что в рамках рендеринга одного кадра все updater'ы всех
*  объектов получат одно и то же значение времени.
*  - Компоненту можно установить новый функциональный объект в любой момент
*  времени (это может делать даже сам вызванный функциональный объект во время
*  работы, безо всяких ограничений).
*  
*  ---
*  
*  Если необходимо, чтобы updater объекта вызывался только тогда, когда
*  рендерится сам объект (например, когда он находится в поле зрения камеры),
*  то компонент updater'a можно добавить в список компонентов самого объекта
*  и забыть о нем. Если же updater нужно вызывать независимо от того,
*  рендерится объект в текущем кадре или нет, то необходимо компонент его
*  updater'a поместить в отдельный игровой объект и вызывать его рендер
*  индивидуально.
*  
*  ### Fog (deprecated)
*
*  Компонент тумана.
*
* \deprecated
*  Компонент устарел и будет удалены в следующей стабильной версии, вместо него
*  следует использовать пользовательский константный буфер и явным образом
*  задействовать его в шейдере.
*
*  | kind | Требуется Data (kind)   | Параметры | Тип параметра | Описание параметра                           |
*  |------|-------------------------|-----------|---------------|----------------------------------------------|
*  | -    | Fog (\ref Footnote "3") | style     | String_t      | Тип тумана: linear (по умолчанию), exp, exp2 |
*
* \note
*  - Шейдерные реализации просто передают параметры шейдеру (константный буфер
*  шейдера \b FogData) и их необходимо явно задействовать в оном.
*  - В реализация со статическим конвеером камера при рендеринге отключает
*  использование тумана.
*
*  ### Material (deprecated)
*
*  Компонент материала (нужен для расчета освещения).
*  
* \deprecated
*  Компонент имеет смысл только для реализаций со статическим конвеером,
*  шейдерные реализации должны использовать PBR-текстурирование.
*
*  | Параметры | Тип параметра | Описание параметра               |
*  |-----------|---------------|----------------------------------|
*  | ambient   | uint32_t      | Цвет в формате ARGB (0xFF333333) |
*  | diffuse   | uint32_t      | Цвет в формате ARGB (0xFFCCCCCC) |
*  | specular  | uint32_t      | Цвет в формате ARGB (0xFF000000) |
*  | emission  | uint32_t      | Цвет в формате ARGB (0xFF000000) |
*  | shininess | int32_t       | Значение в диапазоне 0...0x80    |
*
* \note
*  - Материал должен устанавливаться для каждого объекта 3D сцены, которому
*  требуется расчет освещения.
*
*  ### Light (deprecated)
*
*  Компонент источника света.
*
* \deprecated
*  Компонент устарел и будет удален в следующей стабильной версии, вместо него
*  использовать компонент Buffer.Constant с функцией обратного вызова,
*  получающей Lights_t *.
*
*  | kind      | Требуется Data (kind) | Параметры | Тип параметра | Описание параметра                                    |
*  |-----------|-----------------------|-----------|---------------|-------------------------------------------------------|
*  | Ambient   | -                     | color     | uint32_t      | Цвет (ambient) в формате ARGB (0xFF00FF00)            |
*  | Direction | Direction             | color     | uint32_t      | Цвет (diffuse и specular) в формате ARGB (0xFF00FF00) |
*  | Point     | Position, Attenuation | color     | uint32_t      | Цвет (diffuse и specular) в формате ARGB (0xFF00FF00) |
*  
* \note
*  - Фоновых и направленных источников света может быть не более одного,
*  точечных может быть несколько.
*  - Пропущенные параметры устанавливаются в 0xFF000000.
*  - Параметры компонента Data.Attenuation используются следующим образом: \n
*  bright = color / (const + (linear + exponent * distance) * distance).
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
*  ---
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
*  При использовании точечных источников света следует учитывать, что
*  реализации, использующие статический конвеер (и с этим ничего нельзя
*  сделать) реализуют расчет освещения по Гуро (вычисляются цвета вершин,
*  промежуточные получаются путем интерполяции). Т.е. источник света рядом
*  с большой поверхностью, заданной четырьмя вершинами (факел на стене) не даст
*  реалистичного круглого пятна света - вместо этого будет получена однотонная
*  слабо освещенная поверхность, т.к. вершины расположены далеко и
*  (скорее всего) будут иметь черный цвет. В этом случае следует использовать
*  карты освещения или написать собственный шейдер, реализующий другой метод
*  расчета освещения.
*
*  Пример использования разных типов расчета освещения применительно к одной
*  и той же плоскости, заданной четырьмя вершинами:
*
* \image html ".\Solution\Covellite.Api\(Documentation)\Lights.png"
*
* ### Present (deprecated)
*
*  Компонент отправки информации в конвеер рендеринга.
*
* \deprecated
*  Компонент Present.Geometry устарел и будет удален в следующей стабильной
*  версии, вместо него использовать компоненты Present.Index (или
*  Present.Instance) и Transform.
*
*  | kind     | Требуется Data (kind)                       | Параметры | Тип параметра | Описание параметра                                      |
*  |----------|---------------------------------------------|-----------|---------------|---------------------------------------------------------|
*  | Geometry | Position/Rotation/Scale (\ref Footnote "4") | variety   | String_t      | Параметр отсутствует - динамический объект              |
*  | ^        | ^                                           | ^         | ^             | Static - статический объект                             |
*  | ^        | Position (\ref Footnote "4")                | ^         | ^             | Billboard - объект, всегда развернутый 'лицом' к камере |
*
* \note
*  - Компонент Present должен завершать набор компонентов каждого объекта
*  3D сцены и должен иметь уникальный id для каждого конкретного объекта.
*  - У статического объекта мировая матрица будет вычислена один раз при его
*  создании и в дальнейшем изменить его положение/ориентацию/масштаб будет
*  невозможно; динамический объект захватывает переданные ему компоненты Data,
*  поэтому его положение/ориентацию/масштаб можно менять в каждом кадре
*  изменяя значения параметров этих компонентов.
*  - 'Лицом' billboard'a считается плоскость Oxy при взгляде со стороны
*  положительных значений вдоль оси z.
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
*  -# [Vertex::Polygon] создан специально для рендеринга элементов Gui 
*  и соответствует формату данных, получаемых от libRocket.
*  -# Параметры можно указывать непосредственно основному компоненту, 
*  без использования компонента Data.
*  -# Может быть несколько компонентов, что можно использовать для 
*  привязки (совместного движения) объектов друг к другу (в этом случае
*  необходимо гарантировать, чтобы в качестве общего компонента использовался
*  ОДИН И ТОТ ЖЕ объект, совпадения идентификаторов компонентов недостаточно!).
*  
*  [Vertex::Polygon]: \ref covellite::api::vertex::Polygon
*  [Vertex::Polyhedron]: \ref covellite::api::vertex::Polyhedron
*  [Vertex]: \ref covellite::api::Vertex
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
*  нажатия кнопок, при получении которых генерирует события действий игрока.
*  - Игровой мир (Model) подписывается на события действий игрока:
*   + Обновляет состояние игрока и объектов игровой сцены.
*   + В очередь создания объектов добавляются объекты, которые теперь попадают
*     в зону действия камеры.
*   + Из игровой сцены удаляются объекты, расположенные слишком далеко
*     от камеры.
*  - 3D сцена (View):
*   + По событию создания объекта запрашивает у модели набор компонентов
*     объекта и создает его рендер.
*   + По событию удаления объекта удаляет его рендер.
*   + По событию отрисовки окна запрашивает у модели актуальынй набор игровых
*     объектов и активирует их рендеры.
*  
*  Примеры использования компонентов
*  =================================
*  
* \ref Draw2DScenePage
*  
* \ref Draw3DScenePage
*/
