
#include "stdafx.h"
#include "OpenGLCommonShader.hpp"
#include <alicorn/std/vector.hpp>

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
class Support; // Not implement!!!

template<>
class Support<::Camera>
{
public:
  static void Update(const GLint _ProgramId, const ::Camera & _Matrices)
  {
    const auto MatrixProjectionId = 
      glGetUniformLocation(_ProgramId, "MatricesData.Projection");
    glUniformMatrix4fv(MatrixProjectionId, 1, GL_FALSE, 
      ::glm::value_ptr(_Matrices.Projection));

    const auto MatrixViewId = 
      glGetUniformLocation(_ProgramId, "MatricesData.View");
    glUniformMatrix4fv(MatrixViewId, 1, GL_FALSE, 
      ::glm::value_ptr(_Matrices.View));

    const auto MatrixViewInverseId = 
      glGetUniformLocation(_ProgramId, "MatricesData.ViewInverse");
    glUniformMatrix4fv(MatrixViewInverseId, 1, GL_FALSE, 
      ::glm::value_ptr(_Matrices.ViewInverse));
  }
};

template<>
class Support<::Matrices>
{
public:
  static void Update(const GLint _ProgramId, const ::Matrices & _Matrices)
  {
    const auto MatrixWorldId = 
      glGetUniformLocation(_ProgramId, "MatricesData.World");
    glUniformMatrix4fv(MatrixWorldId, 1, GL_FALSE, 
      ::glm::value_ptr(_Matrices.World));
  }
};

template<>
class Support<::Lights>
{
  static void Update(const GLint _ProgramId, const ::Ambient_t & _Light)
  {
    const auto IsValidId =
      glGetUniformLocation(_ProgramId, "LightsData.Ambient.IsValid");
    const auto ColorId =
      glGetUniformLocation(_ProgramId, "LightsData.Ambient.Color");

    glUniform1i(IsValidId, _Light.IsValid);
    glUniform4fv(ColorId, 1, ::glm::value_ptr(_Light.Color));
  }

  static void Update(const GLint _ProgramId, const ::Direction_t & _Light)
  {
    const auto IsValidId =
      glGetUniformLocation(_ProgramId, "LightsData.Direction.IsValid");
    const auto ColorId =
      glGetUniformLocation(_ProgramId, "LightsData.Direction.Color");
    const auto DirectionId =
      glGetUniformLocation(_ProgramId, "LightsData.Direction.Direction");

    glUniform1i(IsValidId, _Light.IsValid);
    glUniform4fv(ColorId, 1, ::glm::value_ptr(_Light.Color));
    glUniform4fv(DirectionId, 1, ::glm::value_ptr(_Light.Direction));
  }

  static void Update(const GLint _ProgramId, const ::Points_t & _Lights)
  {
    static const ::std::string LightsDataPoints[] =
    {
      "LightsData.Points.Lights[0]",
      "LightsData.Points.Lights[1]",
      "LightsData.Points.Lights[2]",
      "LightsData.Points.Lights[3]",
      "LightsData.Points.Lights[4]",
      "LightsData.Points.Lights[5]",
      "LightsData.Points.Lights[6]",
      "LightsData.Points.Lights[7]",
      "LightsData.Points.Lights[8]",
      "LightsData.Points.Lights[9]",
      "LightsData.Points.Lights[10]",
      "LightsData.Points.Lights[11]",
      "LightsData.Points.Lights[12]",
      "LightsData.Points.Lights[13]",
      "LightsData.Points.Lights[14]",
      "LightsData.Points.Lights[15]",
    };

    constexpr auto LightCount = 
      sizeof(LightsDataPoints) / sizeof(LightsDataPoints[0]);
    static_assert(LightCount >= MAX_LIGHT_POINT_COUNT,
      "Unexpected LightsDataPoints array size.");

    const auto UsedSlotCountId =
      glGetUniformLocation(_ProgramId, "LightsData.Points.UsedSlotCount");
    glUniform1i(UsedSlotCountId, _Lights.UsedSlotCount);

    for (int i = 0; i < _Lights.UsedSlotCount; i++)
    {
      auto & Light = _Lights.Lights[i];

      using namespace ::alicorn::extension::std;

      const auto & Point = LightsDataPoints[i];

      const auto ColorId =
        glGetUniformLocation(_ProgramId, (Point + ".Color").c_str());
      glUniform4fv(ColorId, 1, ::glm::value_ptr(Light.Color));

      const auto PositionId =
        glGetUniformLocation(_ProgramId, (Point + ".Position").c_str());
      glUniform4fv(PositionId, 1, ::glm::value_ptr(Light.Position));

      const auto AttenuationId =
        glGetUniformLocation(_ProgramId, (Point + ".Attenuation").c_str());
      glUniform4fv(AttenuationId, 1, ::glm::value_ptr(Light.Attenuation));
    }
  }

public:
  static void Update(const GLint _ProgramId, const ::Lights & _Lights)
  {
    Update(_ProgramId, _Lights.Ambient);
    Update(_ProgramId, _Lights.Direction);
    Update(_ProgramId, _Lights.Points);
  }
};

template<>
class Support<::Fog>
{
public:
  static void Update(const GLint _ProgramId, const ::Fog & _Fog)
  {
    const auto ColorId = glGetUniformLocation(_ProgramId, "FogData.Color");
    glUniform4fv(ColorId, 1, ::glm::value_ptr(_Fog.Color));

    const auto NearId = glGetUniformLocation(_ProgramId, "FogData.Near");
    glUniform1f(NearId, _Fog.Near);

    const auto FarId = glGetUniformLocation(_ProgramId, "FogData.Far");
    glUniform1f(FarId, _Fog.Far);

    const auto DensityId = glGetUniformLocation(_ProgramId, "FogData.Density");
    glUniform1f(DensityId, _Fog.Density);
  }
};

template<class T>
class OpenGLCommonShader::ConstantBuffer :
  public GraphicApi::Constants::Data<T>
{
public:
  void Update(void) const override
  {
    GLint ProgramId = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &ProgramId);

    Support<T>::Update(ProgramId, Constants::Data<T>::m_Data);
  }
};

OpenGLCommonShader::OpenGLCommonShader(
  const Data_t & _Data, 
  const String_t & _PreVersion, 
  const ::std::string & _ShaderHeader) :
  OpenGLCommon{ _Data, _PreVersion },
  m_pPrograms{ ::std::make_shared<Programs>(_ShaderHeader) }
{
  MakeConstants<ConstantBuffer>();
}

auto OpenGLCommonShader::GetUsingApi(void) const -> String_t /*override*/
{
  const auto UsingApi = OpenGLCommon::GetUsingApi();

  LOGGER(Info) << "GL_VENDOR: " <<
    (const char *)glGetString(GL_VENDOR);
  LOGGER(Info) << "GL_RENDERER: " <<
    (const char *)glGetString(GL_RENDERER);
  LOGGER(Info) << "GL_SHADING_LANGUAGE_VERSION: " <<
    (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);

  GLint ExtensionCount = 0;
  glGetIntegerv(GL_NUM_EXTENSIONS, &ExtensionCount);

  for (GLint i = 0; i < ExtensionCount; i++)
  {
    LOGGER(Info) << "GL_EXTENSIONS: " <<
      (const char *)glGetStringi(GL_EXTENSIONS, i);
  }

  return UsingApi;
}

auto OpenGLCommonShader::CreateCamera(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const auto CameraRender = (_pComponent->Kind == uT("Perspective")) ? 
    GetCameraPerspective(_pComponent) : GetCameraOrthographic(_pComponent);

  const auto CameraId = _pComponent->Id;

  return [=](void)
  {
    CameraRender();

    m_pConstants->SetCameraId(CameraId);
  };
}

auto OpenGLCommonShader::CreateState(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  if (_pComponent->Kind == uT("AlphaTest")) return nullptr;

  return OpenGLCommon::CreateState(_pComponent);
}

auto OpenGLCommonShader::CreateFog(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  return DoCreateFog<::Fog>(_pComponent);
}

auto OpenGLCommonShader::CreateMaterial(const ComponentPtr_t &) -> Render_t /*override*/
{
  return nullptr;
}

auto OpenGLCommonShader::CreateLight(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  return DoCreateLight<::Lights>(_pComponent);
}

auto OpenGLCommonShader::CreateTexture(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  using Destination_t = ::std::pair<String_t, const char *>;

  const auto pTextureDataComponent =
    m_ServiceComponents.Get({ { uT("Texture"), _pComponent } })[0];

  static const ::std::vector<Destination_t> Destinations =
  {
    { uT("albedo"),    "TexAlbedo" },
    { uT("metalness"), "TexMetalness" },
    { uT("roughness"), "TexRoughness" },
    { uT("normal"),    "TexNormal" },
    { uT("occlusion"), "TexOcclusion" },
    { uT("diffuse"),   "TexDiffuse" },
  };

  const Component::Texture TextureData{ pTextureDataComponent };

  const auto itValue = ::std::find_if(Destinations.cbegin(), 
    Destinations.cend(), [&](const Destination_t & _Dest) 
      { 
        return (TextureData.Destination == _Dest.first); 
      });
  if (itValue == Destinations.cend())
  {
    throw STD_EXCEPTION << "Unexpected destination texture: " <<
      TextureData.Destination << uT(" [id=") << _pComponent->Id << uT("].");
  }

  const auto pTexture = ::std::make_shared<Texture>(TextureData);

  GLint IndexDestination = static_cast<GLint>(
    ::std::distance(Destinations.cbegin(), itValue));
  const auto TexName = Destinations[IndexDestination].second;
  if (TextureData.Destination == uT("diffuse")) IndexDestination = 0;

  return [=](void)
  {
    glActiveTexture(GL_TEXTURE0 + IndexDestination);
    pTexture->Bind();
    m_SamplerState(); // Нужно делать после установки каждой текстуры!

    GLint ProgramId = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &ProgramId);
    glUniform1i(glGetUniformLocation(ProgramId, TexName), IndexDestination);
  };
}

class OpenGLCommonShader::Programs final
{
  class Program final
  {
  public:
    void Link(const GLuint _VsShaderId, const GLuint & _PsShaderId)
    {
      glAttachShader(Id, _VsShaderId);
      glAttachShader(Id, _PsShaderId);
      glLinkProgram(Id);

      GLint Result = GL_FALSE;
      glGetProgramiv(Id, GL_LINK_STATUS, &Result);
      if (Result == GL_FALSE)
      {
        char InfoLog[512] = { 0 };
        glGetProgramInfoLog(Id, sizeof(InfoLog), NULL, InfoLog);

        throw STD_EXCEPTION << "Link program fail: " << InfoLog;
      }
    }

  public:
    const GLuint Id;

  public:
    Program(void) :
      Id{ glCreateProgram() }
    {
    }
    ~Program(void)
    {
      glDeleteProgram(Id);
    }
  };

  class Shader final
  {
  public:
    void Compile(void)
    {
      glCompileShader(Id);

      GLint Result = GL_FALSE;
      glGetShaderiv(Id, GL_COMPILE_STATUS, &Result);
      if (Result == GL_FALSE)
      {
        // Так делать правильно, но это не работает на Android'e:
        //GLint InfoLogLenght = 0;
        //glGetShaderiv(Id, GL_INFO_LOG_LENGTH, &InfoLogLenght);
        //const auto Size = static_cast<::std::size_t>(InfoLogLenght + 1);
        //::std::vector<GLchar> InfoLog(Size, (GLchar)0x00);

        char InfoLog[512] = { 0 };
        glGetShaderInfoLog(Id, sizeof(InfoLog), nullptr, InfoLog);

        throw STD_EXCEPTION << "Compile shader fail: " << InfoLog;
      };
    }

  public:
    Programs & m_Programs;
    const GLenum Type;
    const GLuint Id;

  public:
    Shader(Programs & _Programs, const GLenum _Type, const GLchar * _ShaderCode) :
      m_Programs{ _Programs },
      Type{ _Type },
      Id{ glCreateShader(_Type) }
    {
      glShaderSource(Id, 1, &_ShaderCode, nullptr);
    }
    ~Shader(void)
    {
      m_Programs.Clear(Id);
      glDeleteShader(Id);
    }
  };

  using ProgramPtr_t = ::std::shared_ptr<Program>;
  using Programs_t = ::std::map<GLuint, ::std::map<GLuint, ProgramPtr_t>>;
  using ShaderPtr_t = ::std::shared_ptr<Shader>;

public:
  ShaderPtr_t MakeVertex(const Component::Shader & _ShaderData)
  {
    using namespace ::alicorn::extension::std;

    const auto ShaderText = 
      ::Predefined + ::Data + ::Input + ::std::vector<uint8_t>{
      _ShaderData.pData, _ShaderData.pData + _ShaderData.Count };

    return ::std::make_shared<Shader>(*this, GL_VERTEX_SHADER,
      (m_ShaderHeader +
      "#define COVELLITE_SHADER_VERTEX\r\n" +
      ::std::string{ reinterpret_cast<const char *>(ShaderText.data()), ShaderText.size() } +
      "out Pixel PixelValue;\r\n"
      "void main()\r\n"
      "{\r\n"
      "  Vertex InputData;\r\n"
      "  InputData.Position = CovelliteVertexPosition;\r\n"
      "  InputData.TexCoord = CovellteVertexTexCoord;\r\n"
      "  InputData.Extra = CovelliteVertexExtra;\r\n"
      "  PixelValue = " + _ShaderData.Entry + "(InputData);\r\n"
      "  gl_Position = PixelValue.ScreenPos;\r\n"
      "}\r\n").c_str());
  }

  ShaderPtr_t MakePixel(const Component::Shader & _ShaderData)
  {
    using namespace ::alicorn::extension::std;

    const auto ShaderText = 
      ::Predefined + ::Data + ::Input + ::std::vector<uint8_t>{
      _ShaderData.pData, _ShaderData.pData + _ShaderData.Count };

    return ::std::make_shared<Shader>(*this, GL_FRAGMENT_SHADER,
      (m_ShaderHeader +
      "#define COVELLITE_SHADER_PIXEL\r\n" +
      ::std::string{ reinterpret_cast<const char *>(ShaderText.data()), ShaderText.size() } +
      "in Pixel PixelValue;\r\n"
      "out vec4 OutPixelColor;\r\n"
      "void main()\r\n"
      "{\r\n"
      "  OutPixelColor = " + _ShaderData.Entry + "(PixelValue);\r\n"
      "}\r\n").c_str());
  }

  void Activate(const ShaderPtr_t & _pShader)
  {
    if (_pShader->Type == GL_VERTEX_SHADER) m_VsShaderId = _pShader->Id;
    if (_pShader->Type == GL_FRAGMENT_SHADER) m_PsShaderId = _pShader->Id;
    if (m_VsShaderId == InvalidId || m_PsShaderId == InvalidId) return;

    auto & pProgram = m_Programs[m_VsShaderId][m_PsShaderId];

    if (pProgram == nullptr)
    {
      pProgram = ::std::make_shared<Program>();
      pProgram->Link(m_VsShaderId, m_PsShaderId);
    }

    glUseProgram(pProgram->Id);
  }

  void Clear(const GLuint _ShaderId)
  {
    for (auto & VertexPrograms : m_Programs)
    {
      if (VertexPrograms.first == _ShaderId)
      {
        for (auto & PixelPrograms : VertexPrograms.second)
        {
          PixelPrograms.second.reset();
        }

        return;
      }

      for (auto & PixelPrograms : VertexPrograms.second)
      {
        if (PixelPrograms.first == _ShaderId)
        {
          PixelPrograms.second.reset();
        }
      }
    }
  }

private:
  const ::std::string m_ShaderHeader;
  const GLuint InvalidId = static_cast<GLuint>(-1);
  GLuint m_VsShaderId = InvalidId;
  GLuint m_PsShaderId = InvalidId;
  Programs_t m_Programs;

public:
  explicit Programs(const ::std::string & _ShaderHeader) :
    m_ShaderHeader(_ShaderHeader + "#define COVELLITE_SHADER_GLSL\r\n")
  {

  }
};

auto OpenGLCommonShader::CreateShader(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const auto pShaderDataComponent =
    m_ServiceComponents.Get({ { uT("Shader"), _pComponent } })[0];

  const Component::Shader ShaderData{ pShaderDataComponent, ::Default };

  if (ShaderData.Kind == uT("Vertex"))
  {
    const auto pShader = m_pPrograms->MakeVertex(ShaderData);
    pShader->Compile();

    return [=](void)
    {
      m_pPrograms->Activate(pShader);
    };
  }
  else if (ShaderData.Kind == uT("Pixel"))
  {
    const auto pShader = m_pPrograms->MakePixel(ShaderData);
    pShader->Compile();

    return [=](void)
    {
      m_pPrograms->Activate(pShader);

#     ifndef TEST_CALL_IF
#     define TEST_CALL_IF(n)
#     endif

      TEST_CALL_IF(1) m_pConstants->Update<::Camera>();
      TEST_CALL_IF(2) m_pConstants->Update<::Fog>();
      TEST_CALL_IF(3) m_pConstants->Update<::Lights>();

#     undef TEST_CALL_IF
    };
  }

  return nullptr;
}

class OpenGLCommonShader::Buffer
{
public:
  inline void Bind(void)
  {
    glBindBuffer(m_Type, m_BufferId);
  }

private:
  const GLenum m_Type;
  GLuint m_BufferId;

public:
  Buffer(
    const GLenum _Type, 
    const void * _pData,
    const ::std::size_t _Size,
    const GLenum _Usage) :
    m_Type(_Type)
  {
    glGenBuffers(1, &m_BufferId);
    glBindBuffer(m_Type, m_BufferId);
    glBufferData(m_Type, static_cast<GLsizeiptr>(_Size), _pData, _Usage);
    glBindBuffer(m_Type, 0);

    const auto Error = glGetError();
    if (Error != GL_NO_ERROR)
    {
      throw STD_EXCEPTION << "Create buffer error: " << Error;
    }
  }

  ~Buffer(void)
  {
    glDeleteBuffers(1, &m_BufferId);
  }
};

auto OpenGLCommonShader::CreateBuffer(const ComponentPtr_t & _pBuffer) -> Render_t /*override*/
{
  const auto pBufferData = 
    m_ServiceComponents.Get({ { uT("Buffer"), _pBuffer } })[0];

  auto CreateIndexBuffer = [&](void) -> Render_t
  {
    using Type_t = int;

    if (!pBufferData->IsType<const Type_t *>(uT("data")))
    {
      throw STD_EXCEPTION << "Unexpected buffer format [" << 
        "id: " << _pBuffer->Id << ", " <<
        "type: " << _pBuffer->Type << ", " <<
        "kind: " << _pBuffer->Kind << "].";
    }

    const Component::Buffer<Type_t> Info{ pBufferData };

    const auto pBuffer = ::std::make_shared<Buffer>(GL_ELEMENT_ARRAY_BUFFER,
      Info.pData, Info.Count * sizeof(Type_t), GL_STATIC_DRAW);

    auto * pDrawElements = &m_DrawElements;
    const auto Size = static_cast<GLsizei>(Info.Count);

    return [=](void)
    {
      pBuffer->Bind();

      *pDrawElements = [=](void) 
      { 
        glDrawElements(GL_TRIANGLES, Size, GL_UNSIGNED_INT, nullptr);
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

    const Render_t Render = [=](void)
    {
      GLint ProgramId = 0;
      glGetIntegerv(GL_CURRENT_PROGRAM, &ProgramId);

      const auto hPosition = 
        glGetAttribLocation(ProgramId, "CovelliteVertexPosition");
      glVertexAttribPointer(hPosition, 4, GL_FLOAT, GL_FALSE, sizeof(Type_t), 
        (void*)0);
      glEnableVertexAttribArray(hPosition);

      const auto hTexCoord = 
        glGetAttribLocation(ProgramId, "CovellteVertexTexCoord");
      glVertexAttribPointer(hTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Type_t),
        (void*)(sizeof(float) * 4));
      glEnableVertexAttribArray(hTexCoord);

      const auto hExtra = 
        glGetAttribLocation(ProgramId, "CovelliteVertexExtra");
      glVertexAttribPointer(hExtra, 4, GL_FLOAT, GL_FALSE, sizeof(Type_t),
        (void*)(sizeof(float) * 6));
      glEnableVertexAttribArray(hExtra);
    };

    const Component::Buffer<Type_t> Info{ pBufferData };

    const auto & cbBufferMapper =
      _pBuffer->GetValue<const BufferMapper_t &>(uT("mapper"), nullptr);

    if (cbBufferMapper == nullptr)
    {
      const auto pBuffer = ::std::make_shared<Buffer>(GL_ARRAY_BUFFER,
        Info.pData, Info.Count * sizeof(Type_t), GL_STATIC_DRAW);

      return [=](void)
      {
        pBuffer->Bind();
        Render();
      };
    }

    const auto pBuffer = ::std::make_shared<Buffer>(GL_ARRAY_BUFFER,
      Info.pData, Info.Count * sizeof(Type_t), GL_DYNAMIC_DRAW);

    const auto pData = ::std::make_shared<::std::vector<Type_t>>(
      Info.pData, Info.pData + Info.Count);

    return [=](void)
    {
      pBuffer->Bind();

      const auto IsDirty = cbBufferMapper(nullptr);
      if (IsDirty)
      {
        cbBufferMapper(pData->data());

        glBufferSubData(GL_ARRAY_BUFFER, 0, 
          static_cast<GLsizeiptr>(pData->size() * sizeof(Type_t)), pData->data());
      }

      Render();
    };
  };

  return CreateVertexBuffer();
}

auto OpenGLCommonShader::CreateGeometry(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  using PreRender_t = ::std::function<void(void)>;

  const auto GetPreRenderDefaultGeometry = [&](void) -> PreRender_t
  {
    const auto PreRender = GetPreRenderGeometry();

    return [=](void)
    {
      auto & World = m_pConstants->Get<::Matrices>().World;
      World = ::glm::identity<::glm::mat4>();
      PreRender(World);
    };
  };

  const auto GetPreRenderStaticGeometry = [&](void) -> PreRender_t
  {
    ::glm::mat4 MatrixWorld = ::glm::identity<::glm::mat4>();
    GetPreRenderGeometry()(MatrixWorld);

    return [=](void)
    {
      auto & World = m_pConstants->Get<::Matrices>().World;
      World = MatrixWorld;
    };
  };

  const auto GetPreRenderBillboardGeometry = [&](void) -> PreRender_t
  {
    const auto PreRender = OpenGLCommonShader::GetPreRenderBillboardGeometry();

    return [=](void)
    {
      auto & World = m_pConstants->Get<::Matrices>().World;
      World = m_pConstants->Get<::Camera>().View;
      PreRender(World);
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
    PreRender();

    m_pConstants->Update<::Matrices>();
    m_DrawElements();
    glBindTexture(GL_TEXTURE_2D, 0);
  };
}

auto OpenGLCommonShader::GetCameraCommon(void) -> Render_t
{
  const auto DisableDepthRender = GetDepthRender(false, false, false);

  return [=](void)
  {
    DisableDepthRender();

    glDisable(GL_BLEND);
    glDisable(GL_DITHER);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
  };
}

auto OpenGLCommonShader::GetCameraOrthographic(const ComponentPtr_t &) -> Render_t
{
  const auto CommonRender = GetCameraCommon();

  const auto ServiceComponents = m_ServiceComponents.Get(
    {
      { uT("Position"), api::Component::Make({}) },
    });

  return [=](void)
  {
    CommonRender();

    GLfloat Viewport[4] = { 0 };
    glGetFloatv(GL_VIEWPORT, Viewport);

    // 25 Август 2019 20:19 (unicornum.verum@gmail.com)
    TODO("Тест не проверяет возможность изменения смещения во время работы программы");
    const Component::Position Offset{ ServiceComponents[0] };

    auto & Matrix = m_pConstants->Get<::Camera>();
    Matrix.Projection = ::glm::identity<::glm::mat4>();
    Matrix.View = ::glm::ortho(
      Viewport[0] + Offset.X,
      Viewport[0] + Viewport[2] + Offset.X,
      Viewport[1] + Viewport[3] + Offset.Y,
      Viewport[1] + Offset.Y,
      -1.0f, 1.0f);
    Matrix.ViewInverse = ::glm::inverse(Matrix.View);
  };
}

auto OpenGLCommonShader::GetCameraPerspective(const ComponentPtr_t & _pComponent) -> Render_t
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

    auto & Matrix = m_pConstants->Get<::Camera>();

    // ************************** Матрица проекции ************************** //

    const auto AngleY = _pComponent->GetValue(uT("fov"), 90.0f) *
      static_cast<float>(::alicorn::extension::cpp::math::GreedToRadian);
    const float zFar = 200.0f;

    Matrix.Projection = ::glm::perspectiveFovRH(AngleY,
      Viewport[2], Viewport[3], 0.01f, zFar);

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

    Matrix.View = ::glm::lookAtRH(
      GetEye(),
      ::glm::vec3{ Look.X, Look.Y, Look.Z },
      ::glm::vec3{ 0.0f, 0.0f, 1.0f }); // Up

    Matrix.ViewInverse = ::glm::inverse(Matrix.View);
  };
}

} // namespace renderer

} // namespace api

} // namespace covellite
