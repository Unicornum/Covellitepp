
#pragma once
#include "GameObject.hpp"
#include "CubeCoords.hpp"
#include "Defines.hpp"
#include "Lights.hpp"

namespace basement
{

namespace model
{

/**
* \ingroup ExampleBasementDemoGroup
* \brief
*  Класс входит в проект \ref ExamplePage \n
*  Класс рендеринга сцены с тенями.
*
* \version
*  1.0.0.0        \n
*  2.0.0.0        \n
* \date
*  25 Сентябрь 2019    \n
* \author
*  CTAPOBEP (unicornum.verum@gmail.com)
* \copyright
*  © CTAPOBEP 2019
*/
class Shadows final :
  public GameObject
{
  using CursorDataPtr_t = ::std::shared_ptr<::basement::Cursor>;
  using ComponentPtr_t = ::covellite::api::Component::ComponentPtr_t;
  using Updater_t = ::covellite::api::Updater_t;

  struct Light_s
  {
    ::glm::mat4 View;
    ::glm::mat4 Projection;
  };

  struct Object_s
  {
    float IdX, IdY, dummy2, dummy3;
  };

  struct Cursor_s
  {
    float X, Y, dummy2, dummy3;
  };

  struct ShaderData
  {
    Light_s  Light;
    Cursor_s Cursor;
    Lights_t Lights;
  };

public:
  Objects_t GetObject(const Any_t &) const override;

private:
  Object_t GetPass1(void) const;
  Object_t GetPass2(void) const;
  Object_t GetPass3(void) const;

private:
  Object_t GetTextureSurface(const bool) const;
  Object_t GetShadowCamera(void) const;
  static Object_t GetShadowShaders(void);
  Object_t GetSceneCamera(void) const;
  static Object_t GetSceneShaders(void);
  Object_t GetShaderData(const CubeCoords &) const;
  Object_t GetTransformData(const CubeCoords &) const;
  static Object_t GetTransform(const Object_t &);
  static Object_t GetFlatCamera(void);
  Object_t GetFlatShaders(const String_t & = uT("psExperimental")) const;
  Object_t GetTextureObject(const String_t &, const float = 0.0f) const;

private:
  float m_ScaleFactor = 1.5f;
  ComponentPtr_t m_pLightCamera;
  Object_t m_ShadowMap;
  Object_t m_SurfaceTextures;
  ComponentPtr_t m_pSceneCamera;
  ComponentPtr_t m_pBkSurface;
  ::std::shared_ptr<ShaderData> m_pShaderData;
  mutable CursorDataPtr_t m_pCursorData;
  Object_t m_SceneObjects_pass1;
  Object_t m_SceneObjects_pass2;

private:
  explicit Shadows(const Another::Value);
  friend GameObject;
};

} // namespace model

} // namespace basement
