
#include "stdafx.h"
#include "OpenGLCommonStatic.hpp"
#include <glm/glm.force.hpp>

#if BOOST_COMP_MSVC 
# pragma warning(push)
# pragma warning(disable: 4996)
#elif defined BOOST_COMP_GNUC
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <Covellite/Api/Vertex.hpp>
#if BOOST_COMP_MSVC 
# pragma warning(pop)
#elif defined BOOST_COMP_GNUC
# pragma GCC diagnostic pop
#endif

#include <Covellite/Api/Defines.hpp>
#include "OpenGLCommon.Texture.hpp"
#include "Shaders/Shaders.hpp"
#include "GraphicApi.Constants.hpp"

namespace covellite
{

namespace api
{

namespace renderer
{

template<class>
class Support
{
};

template<>
class Support<::Camera>
{
public:
  static void Update(const ::Camera &) {}
};

template<>
class Support<::Fog>
{
public:
  static void Update(const ::Fog &) {}
};

template<>
class Support<::Object>
{
public:
  template<class TLights>
  static void Update(const TLights & Lights, const int _MaxPointLightCount)
  {
    if (Lights.Ambient.IsValid == 0 &&
      Lights.Direction.IsValid == 0 &&
      Lights.Points.UsedSlotCount == 0)
    {
      glDisable(GL_LIGHTING);
      return;
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);

    static const GLfloat DefaultAmbientColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, DefaultAmbientColor);

    int Index = GL_LIGHT0;

    auto SetLight = [&](
      const ::glm::vec4 & _Ambient,
      const ::glm::vec4 & _Diffuse,
      const ::glm::vec4 & _Specular,
      const ::glm::vec4 & _Position = { 0.0f, 0.0f, 1.0f, 0.0f },
      const ::glm::vec4 & _Attenuation = ::glm::vec4{ 1.0f, 0.0f, 0.0f, 0.0f })
    {
      glEnable(Index);
      glLightfv(Index, GL_AMBIENT, ::glm::value_ptr(_Ambient));
      glLightfv(Index, GL_DIFFUSE, ::glm::value_ptr(_Diffuse));
      glLightfv(Index, GL_SPECULAR, ::glm::value_ptr(_Specular));
      glLightfv(Index, GL_POSITION, ::glm::value_ptr(_Position));
      glLightf(Index, GL_CONSTANT_ATTENUATION, _Attenuation.x);
      glLightf(Index, GL_LINEAR_ATTENUATION, _Attenuation.y);
      glLightf(Index, GL_QUADRATIC_ATTENUATION, _Attenuation.z);
      Index++;
    };

    static const auto DefaultColor = GraphicApi::ARGBtoFloat4<color_t>(0xFF000000);

    if (Lights.Ambient.IsValid != 0)
    {
      const auto & Light = Lights.Ambient;

      const auto Color = Light.Color;
      SetLight(Color, DefaultColor, DefaultColor);
    }

    if (Lights.Direction.IsValid != 0)
    {
      const auto & Light = Lights.Direction;

      const auto Color = Light.Color;
      SetLight(DefaultColor, Color, Color, Light.Direction);
    }

    for (int i = 0; i < Lights.Points.UsedSlotCount; i++)
    {
      const auto & Light = Lights.Points.Lights[i];

      const auto Color = Light.Color;
      SetLight(DefaultColor, Color, Color, Light.Position, Light.Attenuation);
    }

    while (glIsEnabled(Index) && Index < (GL_LIGHT0 + _MaxPointLightCount))
    {
      glDisable(Index++);
    }
  }

public:
  static void Update(const ::Object & _Object) 
  {
    Support<::Object>::Update(
      _Object.Lights, COVELLITE_MAX_LIGHT_POINT_OBJECT_COUNT);
  }
};

template<>
class Support<::Matrices>
{
public:
  static void Update(const ::Matrices &) {}
};

template<>
class Support<::SceneLights>
{
public:
  static void Update(const ::SceneLights & Lights)
  {
    Support<::Object>::Update(Lights, COVELLITE_MAX_LIGHT_POINT_SCENE_COUNT);
  }
};

template<class T>
class OpenGLCommonStatic::ConstantBuffer :
  public GraphicApi::Constants::Data<T>
{
public:
  void Update(void) const override
  {
    Support<T>::Update(Constants::Data<T>::m_Data);
  }
};

OpenGLCommonStatic::OpenGLCommonStatic(const Data_t & _Data, const String_t & _PreVersion) :
  OpenGLCommon{ _Data, _PreVersion }
{
  MakeConstants<ConstantBuffer>();
}

auto OpenGLCommonStatic::CreateCamera(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const auto CameraId = _pComponent->Id;

  const auto Camera = (_pComponent->Kind == uT("Perspective")) ?
    GetCameraPerspective(_pComponent) :
    GetCameraOrthographic(_pComponent);

  return [=](void)
  {
    Camera();

    m_pConstants->SetCameraId(CameraId);
    m_pConstants->Update<::SceneLights>();
  };
}

auto OpenGLCommonStatic::CreateState(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const auto CreateAlphaTestState = [&](void)
  {
    const auto Value = _pComponent->GetValue(uT("discard"), 0.0f);

    return [=](void)
    {
      glEnable(GL_ALPHA_TEST);
      glAlphaFunc(GL_GREATER, Value);
    };
  };

  if (_pComponent->Kind == uT("AlphaTest"))
  {
    return CreateAlphaTestState();
  }

  return OpenGLCommon::CreateState(_pComponent);
}

auto OpenGLCommonStatic::CreateFog(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const auto sStyle = _pComponent->GetValue(uT("style"), uT("linear"));

  const auto Style =
    (sStyle == uT("linear")) ? GL_LINEAR :
    (sStyle == uT("exp")) ? GL_EXP :
    (sStyle == uT("exp2")) ? GL_EXP2 : (
      throw STD_EXCEPTION << "Unknown style fog: " << sStyle <<
        " [" << _pComponent->Id << "].");

  const auto pFogData =
    m_ServiceComponents.Get({ { uT("Fog"), _pComponent } })[0];

  return [=](void)
  {
    const Component::Fog Fog{ pFogData };

    glEnable(GL_FOG);
    glHint(GL_FOG_HINT, GL_NICEST);
    glFogfv(GL_FOG_COLOR, 
      ::glm::value_ptr(GraphicApi::ARGBtoFloat4<::glm::vec4>(Fog.Color)));
    glFogi(GL_FOG_MODE, Style);
    glFogf(GL_FOG_START, Fog.Near);
    glFogf(GL_FOG_END, Fog.Far);
    glFogf(GL_FOG_DENSITY, Fog.Density);
  };
}

auto OpenGLCommonStatic::CreateMaterial(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  auto GetColor = [&](const String_t & _Name)
  {
    return GraphicApi::ARGBtoFloat4<::glm::vec4>(
      _pComponent->GetValue(_Name, 0xFF000000));
  };

  const auto Ambient = GetColor(uT("ambient"));
  const auto Diffuse = GetColor(uT("diffuse"));
  const auto Specular = GetColor(uT("specular"));
  const auto Emission = GetColor(uT("emission"));
  const float Shininess[] = { _pComponent->GetValue(uT("shininess"), 0.0f) };

  return [=](void)
  {
    // GL_FRONT на Android'е вызывает ошибку!!!
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ::glm::value_ptr(Ambient));
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ::glm::value_ptr(Diffuse));
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ::glm::value_ptr(Specular));
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, ::glm::value_ptr(Emission));
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Shininess);
  };
}

auto OpenGLCommonStatic::CreateLight(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  return DoCreateLight<::SceneLights>(_pComponent, true);
}

auto OpenGLCommonStatic::CreateTexture(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const auto pTextureData =
    m_ServiceComponents.Get({ { uT("Texture"), _pComponent } })[0];

  static const ::std::vector<String_t> IgnoreDestination =
  {
    uT("metalness"),
    uT("roughness"),
    uT("normal"),
    uT("occlusion"),
  };

  const auto IsContained = [](const auto & _Container, const auto & _Value)
  {
    const auto itValue =
      ::std::find(_Container.cbegin(), _Container.cend(), _Value);
    return (itValue != _Container.cend());
  };

  const auto Destination =
    pTextureData->GetValue(uT("destination"), uT("albedo"));

  if (Destination == uT("albedo"))
  {
    const auto pTexture =
      ::std::make_shared<Texture>(Component::Texture{ pTextureData });

    return [=](void)
    {
      pTexture->Bind();
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_TexParameters.MinFilter);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_TexParameters.MagFilter);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_TexParameters.WrapS);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_TexParameters.WrapT);
    };
  }
  else if (!IsContained(IgnoreDestination, Destination))
  {
    throw STD_EXCEPTION << "Unexpected destination texture: " << Destination <<
      uT(" [id=") << _pComponent->Id << uT("].");
  }

  return nullptr;
}

#if BOOST_COMP_MSVC 
# pragma warning(push)
# pragma warning(disable: 4996)
#elif defined BOOST_COMP_GNUC
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

auto OpenGLCommonStatic::CreateBuffer(const ComponentPtr_t & _pBuffer) -> Render_t /*override*/
{
  const auto pBufferData = 
    m_ServiceComponents.Get({ { uT("Buffer"), _pBuffer } })[0];

  auto CreateConstantLightsBuffer = [&](void) -> Render_t
  {
    using Type_t = cbBufferMap_t<::Lights_t>;

    if (!_pBuffer->IsType<const Type_t &>(uT("mapper")))
    {
      throw STD_EXCEPTION << "Unexpected buffer format [" <<
        "id: " << _pBuffer->Id << ", " <<
        "type: " << _pBuffer->Type << ", " <<
        "kind: " << _pBuffer->Kind << "].";
    }

    const auto cbBufferMapper = _pBuffer->GetValue(uT("mapper"), Type_t{});
    if (!cbBufferMapper)
    {
      throw STD_EXCEPTION << "Unexpected empty mapper: " << _pBuffer->Id;
    }

    return [=](void)
    {
      cbBufferMapper(&m_pConstants->Get<::Object>().Lights);
      m_pConstants->Update<::Object>();
    };
  };

  auto CreateIndexBuffer = [&](void) -> Render_t
  {
    using Type_t = int;

    if (!pBufferData->IsType<const Type_t *>(uT("data")))
    {
      return CreateConstantLightsBuffer();
    }

    const Component::Buffer<Type_t> Info{ pBufferData };

    const ::std::vector<Type_t> BufferData{ Info.pData, Info.pData + Info.Count };

    return [=](void)
    {
      m_DrawElements = [=](void)
      {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(BufferData.size()),
          GL_UNSIGNED_INT, BufferData.data());
      };
    };
  };

  auto CreateVertexBuffer = [&](void) -> Render_t
  {
    using Type_t = ::covellite::api::Vertex;
    using BufferMapper_t = cbBufferMap_t<Type_t>;

    if (!pBufferData->IsType<const Type_t *>(uT("data")))
    {
      return CreateIndexBuffer();
    }

    const Component::Buffer<Type_t> Info{ pBufferData };

    const auto pData = ::std::make_shared<::std::vector<Type_t>>(
      Info.pData, Info.pData + Info.Count);

    const auto & cbBufferMapper =
      _pBuffer->GetValue<const BufferMapper_t &>(uT("mapper"), nullptr);

    const Render_t FlatRender = [=](void)
    {
      glVertexPointer(2, GL_FLOAT, sizeof(Type_t), &pData->data()->px);
      glEnableClientState(GL_VERTEX_ARRAY);

      glColorPointer(4, GL_FLOAT, sizeof(Type_t), &pData->data()->ex);
      glEnableClientState(GL_COLOR_ARRAY);

      glDisableClientState(GL_NORMAL_ARRAY);

      glTexCoordPointer(2, GL_FLOAT, sizeof(Type_t), &pData->data()->tu);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    };

    const Render_t StaticRender = [=](void)
    {
      glVertexPointer(3, GL_FLOAT, sizeof(Type_t), &(pData->data()->px));
      glEnableClientState(GL_VERTEX_ARRAY);

      glDisableClientState(GL_COLOR_ARRAY);

      glNormalPointer(GL_FLOAT, sizeof(Type_t), &(pData->data()->ex));
      glEnableClientState(GL_NORMAL_ARRAY);

      glTexCoordPointer(2, GL_FLOAT, sizeof(Type_t), &(pData->data()->tu));
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    };

    const Render_t DynamicRender = [=](void)
    {
      const auto IsDirty = cbBufferMapper(nullptr);
      if (IsDirty) cbBufferMapper(pData->data());

      StaticRender();
    };

    return (cbBufferMapper != nullptr) ? DynamicRender :
      (Info.Dimension == 2) ? FlatRender : StaticRender;
  };

  auto CreatePolyhedronVertexBuffer = [&](void) -> Render_t
  {
    using Type_t = ::covellite::api::vertex::Polyhedron;
    using BufferMapper_t = cbBufferMap_t<Type_t>;

    if (!pBufferData->IsType<const Type_t *>(uT("data")))
    {
      return CreateVertexBuffer();
    }

    const Component::Buffer<Type_t> Info{ pBufferData };

    const auto pData = ::std::make_shared<::std::vector<Type_t>>(
      Info.pData, Info.pData + Info.Count);

    const auto & cbBufferMapper =
      _pBuffer->GetValue<const BufferMapper_t &>(uT("mapper"), nullptr);

    const Render_t StaticRender = [=](void)
    {
      glVertexPointer(3, GL_FLOAT, sizeof(Type_t), &(pData->data()->x));
      glEnableClientState(GL_VERTEX_ARRAY);

      glDisableClientState(GL_COLOR_ARRAY);

      glNormalPointer(GL_FLOAT, sizeof(Type_t), &(pData->data()->nx));
      glEnableClientState(GL_NORMAL_ARRAY);

      glTexCoordPointer(2, GL_FLOAT, sizeof(Type_t), &(pData->data()->tu));
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    };

    const Render_t DynamicRender = [=](void)
    {
      const auto IsDirty = cbBufferMapper(nullptr);
      if (IsDirty) cbBufferMapper(pData->data());

      StaticRender();
    };

    return (cbBufferMapper == nullptr) ? StaticRender : DynamicRender;
  };

  auto CreatePolygonVertexBuffer = [&](void) -> Render_t
  {
    using Type_t = ::covellite::api::vertex::Polygon;

    if (!pBufferData->IsType<const Type_t *>(uT("data")))
    {
      return CreatePolyhedronVertexBuffer();
    }

    const Component::Buffer<Type_t> Info{ pBufferData };

    const ::std::vector<Type_t> BufferData{ Info.pData, Info.pData + Info.Count };

    return [=](void)
    {
      glVertexPointer(2, GL_FLOAT, sizeof(Type_t), &BufferData[0].x);
      glEnableClientState(GL_VERTEX_ARRAY);

      glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Type_t), &BufferData[0].ABGRColor);
      glEnableClientState(GL_COLOR_ARRAY);

      glDisableClientState(GL_NORMAL_ARRAY);

      glTexCoordPointer(2, GL_FLOAT, sizeof(Type_t), &BufferData[0].u);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    };
  };

  return CreatePolygonVertexBuffer();
}

#if BOOST_COMP_MSVC 
# pragma warning(pop)
#elif defined BOOST_COMP_GNUC
# pragma GCC diagnostic pop
#endif

auto OpenGLCommonStatic::CreateTransform(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  using PreRender_t = ::std::function<void(::glm::mat4 &)>;

  const auto GetPreRenderDefaultGeometry = [&](void) -> PreRender_t
  {
    const auto PreRender = GetPreRenderGeometry();

    return [=](::glm::mat4 & _Matrix)
    {
      PreRender(_Matrix);
    };
  };

  const auto GetPreRenderStaticGeometry = [&](void) -> PreRender_t
  {
    ::glm::mat4 MatrixWorld = ::glm::identity<::glm::mat4>();
    GetPreRenderGeometry()(MatrixWorld);

    return [=](::glm::mat4 & _Matrix)
    {
      _Matrix *= MatrixWorld;
    };
  };

  const auto GetPreRenderBillboardGeometry = [&](void) -> PreRender_t
  {
    const auto PreRender = OpenGLCommonStatic::GetPreRenderBillboardGeometry();

    return [=](::glm::mat4 & _Matrix)
    {
      ::glm::mat4 MatrixWorld = ::glm::transpose(_Matrix);
      PreRender(MatrixWorld);

      _Matrix *= MatrixWorld;
    };
  };

  const auto PreRender =
    (_pComponent->Kind == uT("Unknown")) ? GetPreRenderDefaultGeometry() :
    (_pComponent->Kind == uT("Static")) ? GetPreRenderStaticGeometry() :
    (_pComponent->Kind == uT("Billboard")) ? GetPreRenderBillboardGeometry() :
      throw STD_EXCEPTION << "Unexpected transform component: " <<
        " [id=" << _pComponent->Id << ", kind: " << _pComponent->Kind << "].";

  return [=](void)
  {
    // Сохраняем в стеке матрицу вида, сформированную камерой, чтобы после
    // рендеринга меша (при котором она будет домножена на матрицу 
    // трансформации) можно было восстановить ее обратно.
    glPushMatrix();

    ::glm::mat4 MatrixView;
    glGetFloatv(GL_MODELVIEW_MATRIX, ::glm::value_ptr(MatrixView));

    PreRender(MatrixView);

    glLoadMatrixf(::glm::value_ptr(MatrixView));
  };
}

auto OpenGLCommonStatic::CreatePresentBuffer(const ComponentPtr_t &_pBuffer) -> Render_t /*override*/
{
  const auto pBufferData =
    m_ServiceComponents.Get({ { uT("Buffer"), _pBuffer } })[0];

  const Component::Buffer<int> Info{ pBufferData };
  const ::std::vector<int> BufferData{ Info.pData, Info.pData + Info.Count };

  return [=](void)
  {
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(BufferData.size()),
      GL_UNSIGNED_INT, BufferData.data());
    glBindTexture(GL_TEXTURE_2D, 0);

    // Восстанавливаем матрицу вида, сформированную камерой.
    glPopMatrix();
  };
}

auto OpenGLCommonStatic::CreateGeometry(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  using PreRender_t = ::std::function<void(void)>;

  const auto GetPreRenderDefaultGeometry = [&](void) -> PreRender_t
  {
    const auto PreRender = GetPreRenderGeometry();

    return [=](void)
    {
      ::glm::mat4 MatrixView;
      glGetFloatv(GL_MODELVIEW_MATRIX, ::glm::value_ptr(MatrixView));

      PreRender(MatrixView);

      glLoadMatrixf(::glm::value_ptr(MatrixView));
    };
  };

  const auto GetPreRenderStaticGeometry = [&](void) -> PreRender_t
  {
    ::glm::mat4 MatrixWorld = ::glm::identity<::glm::mat4>();
    GetPreRenderGeometry()(MatrixWorld);

    return [=](void)
    {
      ::glm::mat4 MatrixView;
      glGetFloatv(GL_MODELVIEW_MATRIX, ::glm::value_ptr(MatrixView));

      MatrixView *= MatrixWorld;

      glLoadMatrixf(::glm::value_ptr(MatrixView));
    };
  };

  const auto GetPreRenderBillboardGeometry = [&](void) -> PreRender_t
  {
    const auto PreRender = OpenGLCommonStatic::GetPreRenderBillboardGeometry();

    return [=](void)
    {
      ::glm::mat4 MatrixView;
      glGetFloatv(GL_MODELVIEW_MATRIX, ::glm::value_ptr(MatrixView));

      ::glm::mat4 MatrixWorld = ::glm::transpose(MatrixView);
      PreRender(MatrixWorld);

      MatrixView *= MatrixWorld;

      glLoadMatrixf(::glm::value_ptr(MatrixView));
    };
  };

  const auto Variety = _pComponent->GetValue(uT("variety"), uT("Default"));

  const auto PreRender =
    (Variety == uT("Default")) ? GetPreRenderDefaultGeometry() :
    (Variety == uT("Static")) ? GetPreRenderStaticGeometry() :
    (Variety == uT("Billboard")) ? GetPreRenderBillboardGeometry() :
    throw STD_EXCEPTION << "Unexpected variety: " << Variety <<
    " [id=" << _pComponent->Id << "].";

  return [=](void)
  {
    // Сохраняем в стеке матрицу вида, сформированную камерой, чтобы после
    // рендеринга меша (при котором она будет домножена на матрицу 
    // трансформации) можно было восстановить ее обратно.
    glPushMatrix();

    PreRender();
    m_DrawElements();
    glBindTexture(GL_TEXTURE_2D, 0);

    // Восстанавливаем матрицу вида, сформированную камерой.
    glPopMatrix();
  };
}

auto OpenGLCommonStatic::GetCameraCommon(void) -> Render_t
{
  const auto DisableDepthRender = GetDepthRender(false, false, false);

  return [=](void)
  {
    DisableDepthRender();

    glDisable(GL_FOG);
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_DITHER);

    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
  };
}

auto OpenGLCommonStatic::GetCameraOrthographic(const ComponentPtr_t &) -> Render_t
{
  const auto CommonRender = GetCameraCommon();

  const auto ServiceComponents = m_ServiceComponents.Get(
    {
      { uT("Position"), api::Component::Make({}) },
    });

  return [=](void)
  {
    const Component::Position Pos{ ServiceComponents[0] };

    CommonRender();

    GLfloat Viewport[4] = { 0 };
    glGetFloatv(GL_VIEWPORT, Viewport);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glOrtho(
      Viewport[0] + Pos.X, Viewport[0] + Viewport[2] + Pos.X,
      Viewport[1] + Viewport[3] + Pos.Y, Viewport[1] + Pos.Y,
      1.0f, -1.0f);
  };
}

auto OpenGLCommonStatic::GetCameraPerspective(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto CommonRender = GetCameraCommon();

  const auto ServiceComponents = m_ServiceComponents.Get(
    {
      { uT("Position"), api::Component::Make({}) },
      { uT("Rotation"), api::Component::Make({}) },
    });

  return [=](void)
  {
    CommonRender();

    GLfloat Viewport[4] = { 0 };
    glGetFloatv(GL_VIEWPORT, Viewport);

    // ************************** Матрица проекции ************************** //

    const auto AngleY = _pComponent->GetValue(uT("fov"), 90.0f) *
      static_cast<float>(::alicorn::extension::cpp::math::GreedToRadian);
    const float zFar = 200.0f;

    const ::glm::mat4 Perspective = ::glm::perspectiveFovRH(AngleY,
      Viewport[2], Viewport[3], zFar, 0.01f);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(::glm::value_ptr(Perspective));

    // **************************** Матрица вида **************************** //

    // Точка, куда смотрит камера - задается как компонент Data.Position.
    const Component::Position Look{ ServiceComponents[0] };

    auto GetEye = [&](void) -> ::glm::vec3
    {
      // Расстояние от камеры до Look.
      const auto Distance = _pComponent->GetValue(uT("distance"), 0.0f) + 0.1f;

      // Точка, где расположена камера - вычисляется на основе Look, Distance и
      // компонента Data.Rotation.

      const Component::Position Rot{ ServiceComponents[1] };

      ::glm::mat4 Transform = ::glm::identity<::glm::mat4>();

      Transform = ::glm::translate(Transform,
        ::glm::vec3{ Look.X, Look.Y, Look.Z });
      Transform = ::glm::rotate(Transform,
        Rot.Z, ::glm::vec3{ 0.0f, 0.0f, 1.0f });
      Transform = ::glm::rotate(Transform,
        Rot.Y, ::glm::vec3{ 0.0f, 1.0f, 0.0f });
      Transform = ::glm::rotate(Transform,
        Rot.X, ::glm::vec3{ 1.0f, 0.0f, 0.0f });

      return Transform * ::glm::vec4{ Distance, 0.0f, 0.0f, 1.0f };
    };

    const ::glm::mat4 LookAt = ::glm::lookAtRH(
      GetEye(),
      ::glm::vec3{ Look.X, Look.Y, Look.Z },
      ::glm::vec3{ 0.0f, 0.0f, 1.0f }); // Up

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(::glm::value_ptr(LookAt));
  };
}

} // namespace renderer

} // namespace api

} // namespace covellite
