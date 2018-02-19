
#pragma once

namespace covellite
{

namespace egl
{

class Display;
class Config;
class Surface;

/**
* \ingroup gCovelliteEglClasses
* \brief
*  Класс входит в проект \ref CovelliteEglPage \n
*  Класс-обетка для eglContext.
* \details
*  - Предназначен для гарантированного корректного уничтожения созданного
*  объекта.
*
* \version
*  1.0.0.0        \n
* \date
*  19 Ноябрь 2016    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016
*/
class Context final
{
public:
  void MakeCurrent(const covellite::egl::Surface &) const;

private:
  EGLDisplay m_Display;
  EGLContext m_Context;

public:
  Context(const covellite::egl::Display &, const covellite::egl::Config &);
  ~Context(void) noexcept;

private:
  FRIEND_TEST(Context_test, /*DISABLED_*/Test_Using);
};

} // namespace egl

} // namespace covellite
