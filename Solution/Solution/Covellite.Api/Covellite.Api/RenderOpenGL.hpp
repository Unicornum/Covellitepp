
#pragma once

namespace covellite
{

namespace api
{

/**
* \ingroup gCovelliteApiClasses
* \brief
*  Класс входит в проект \ref CovelliteApiPage \n
*  Класс реализации интерфейса рендеринга libRocket.
*
* \version
*  1.0.0.0        \n
*  1.1.0.0        \n
*  1.1.1.0        \n
*  1.1.2.0        \n
* \date
*  07 Декабрь 2016    \n
*  18 Декабрь 2016    \n
*  19 Декабрь 2016    \n
*  28 Сентябрь 2017    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2016 - 2017
*/
class RenderOpenGL final :
  public Rocket::Core::RenderInterface
{
public:
  void RenderGeometry(Rocket::Core::Vertex *, int, int *, int,
    Rocket::Core::TextureHandle, const Rocket::Core::Vector2f &) override;
  void EnableScissorRegion(bool) override;
  void SetScissorRegion(int, int, int, int) override;
  bool LoadTexture(Rocket::Core::TextureHandle &, Rocket::Core::Vector2i &,
    const Rocket::Core::String &) override;
  bool GenerateTexture(Rocket::Core::TextureHandle &,
    const Rocket::Core::byte *, const Rocket::Core::Vector2i &) override;
  void ReleaseTexture(Rocket::Core::TextureHandle) override;

private:
  const int m_StatusBarHeight;

public:
  explicit RenderOpenGL(int);
};

} // namespace api

} // namespace covellite
