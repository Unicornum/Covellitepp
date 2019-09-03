
/**
* \page CovelliteApiShadersPage Шейдеры
*  
*  Прежде чем тело шейдера (переданное в качестве параметров \b data и \b count)
*  будет передано компилятору, ему будет добавлен заголовок и футер.
*  
*  Заголовок
*  ---------
*  
* ### Версия шейдера
*  
* \code
#version 330 core // Только для OpenGL
#version 300 es // Только для OpenGLES3
* \endcode
*
* ### Предопределенные макросы типа шейдера
*  
*  - \b COVELLITE_SHADER_DESKTOP или \b COVELLITE_SHADER_MOBILE
*  - \b COVELLITE_SHADER_HLSL или \b COVELLITE_SHADER_GLSL
*  - \b COVELLITE_SHADER_VERTEX или \b COVELLITE_SHADER_PIXEL
*  
* ### Файл вспомогательных определений для создания унифицированных шейдеров
*  
* \include ./Solution/Covellite.Api/Covellite.Api/Renderer/Shaders/Predefined.fx
*
* ### Файл объвления структур для передачи данных шейдеру
*  
* \include ./Solution/Covellite.Api/Covellite.Api/Renderer/Shaders/Data.h
*
* ### Файл объявления форматов входных данных шейдеров
*
* \include ./Solution/Covellite.Api/Covellite.Api/Renderer/Shaders/Input.fx
*  
*  Тело шейдера
*  ------------
*  
*  Тело шейдера должно содержать определение функции, имя которой передается
*  компоненту шейдера как параметр \b entry. Имя этой функции может быть любым,
*  но сигнатура ее должна соответствовать одной из следующего набора (тип
*  входного параметра определяет тип шейдера и формат передаваемых ему данных):
*
* \code

Pixel entry(Polygon _Value); // Вертексный шейдер для рендеринга 2D объектов (deprecated)
Pixel entry(Polyhedron _Value); // Вертексный шейдер для рендеринга 3D объектов (deprecated)
Pixel entry(Vertex _Value); // Вертексный шейдер
float4 entry(Pixel _Value); // Пиксельный шейдер

* \endcode
*  
*  Если указаны бинарные данные шейдера (параметры \b data и \b count),
*  в качестве тела шейдера будет использован переданный буфер, если нет,
*  то вместо них будут использоваться шейдеры по умолчанию (те, что использует
*  сам фреймворк для рендеринга Gui):
*
* \include ./Solution/Covellite.Api/Covellite.Api/Renderer/Shaders/Default.fx
*  
*  Футер
*  -----
*  
*  В конце тела шейдера добавляется определение функции, которая будет
*  вызывать функцию, указанную в качестве параметра \b entry (это сделано
*  для возможности создания единого унифицированного HLSL и GLSL тела шейдера).
*  
* ### HLSL (вертексный шейдер)
*  
*  Ничего не добавляется, указанная функция вызывается напрямую.
*  
* ### HLSL (пиксельный шейдер)
*  
* \code

float4 psMain(Pixel _Value) : SV_Target
{
  return entry(_Value);
}

* \endcode
*  
* ### GLSL (вертексный шейдер)
*
* \code

out Pixel PixelValue;
void main()
{
  Vertex InputData;
  InputData.Position = CovelliteVertexPosition;
  InputData.TexCoord = CovellteVertexTexCoord;
  InputData.Extra = CovelliteVertexExtra;
  PixelValue = entry(InputData);
  gl_Position = PixelValue.ScreenPos;
}

* \endcode
*
* ### GLSL (пиксельный шейдер)
*
* \code

in Pixel PixelValue;
out vec4 OutPixelColor;
void main()
{
  OutPixelColor = entry(PixelValue);
}

* \endcode
*/
