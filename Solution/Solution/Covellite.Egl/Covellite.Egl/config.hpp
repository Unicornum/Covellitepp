
#pragma once

class Surface_test;

namespace covellite
{

namespace egl
{

class Display;
class Surface;
class Context;

/**
* \ingroup CovelliteEglGroup
* \brief
*  Класс входит в проект \ref CovelliteEglPage \n
*  Класс-обертка для EGLConfig.
*
* \version
*  1.0.0.0        \n
* \date
*  18 Ноябрь 2016    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016
*/
class Config final
{
private:
  EGLConfig m_Config;

public:
  Config(const covellite::egl::Display &, ANativeWindow *);

private:
  friend class Surface;
  friend class Context;
  friend Surface_test;
  FRIEND_TEST(Config_test, /*DISABLED_*/Test_Constructor);
  FRIEND_TEST(Context_test, /*DISABLED_*/Test_Using);
  FRIEND_TEST(Context_test, /*DISABLED_*/Test_CreateContext_Fail);
  FRIEND_TEST(Context_test, /*DISABLED_*/Test_MakeCurrent_Fail);
};

} // namespace egl

} // namespace covellite
