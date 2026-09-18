
#pragma once

namespace covellite::api
{

/**
* \ingroup CovelliteApiGroup
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Класс компиляции шейдеров.
* \details
*  - Класс для тестирования компилируемости шейдеров (с учетом заголовка,
*  предоставляемого Covellite++) во время компиляции решения.
*  
* \note
*  Компиляция шейдера HLSL работает прямо так, для компиляции шейдера GLSL
*  требуется создание окна и контекста OpenGL.
*  
* \version
*  1.0.0.0        \n
* \date
*  13 Сентябрь 2026    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2026
*/
class CompileShader final
{
  using ComponentPtr_t = ::std::shared_ptr<class Component>;

public:
  static void AsHLSL(const ComponentPtr_t &);
  static void AsGLSL(const ComponentPtr_t &);
};

} // namespace covellite::api
