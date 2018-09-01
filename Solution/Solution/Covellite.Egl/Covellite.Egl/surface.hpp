
#pragma once

namespace covellite
{

namespace egl
{

class Display;
class Config;
class Context;

/**
* \ingroup CovelliteEglGroup
* \brief
*  Класс входит в проект \ref CovelliteEglPage \n
*  Класс-обертка для EGLSurface.
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
class Surface final
{
public:
  void SwapBuffers(void) const;
  int32_t GetWidth(void) const;
  int32_t GetHeight(void) const;

private:
  int32_t GetValue(int32_t) const;

private:
  EGLDisplay m_Display;
  EGLSurface m_Surface;

public:
  Surface(const covellite::egl::Display &, const covellite::egl::Config &,
    ANativeWindow *);
  ~Surface(void) noexcept;

private:
  friend class Context;
  FRIEND_TEST(Surface_test, /*DISABLED_*/Test_CreateSurface);
  FRIEND_TEST(Context_test, /*DISABLED_*/Test_Using);
  FRIEND_TEST(Context_test, /*DISABLED_*/Test_MakeCurrent);
  FRIEND_TEST(Context_test, /*DISABLED_*/Test_MakeCurrent_Fail);
};

} // namespace egl

} // namespace covellite
