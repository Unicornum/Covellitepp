
#include "stdafx.h"
#include "OpenGLCommonShader.hpp"
#include <glm/glm.force.hpp>
#include <alicorn/std/vector.hpp>
#include <alicorn/std.fast/unordered-map.hpp>

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

namespace std
{

template<class T>
istream & operator>>(istream &, shared_ptr<T> &)
{
  throw STD_EXCEPTION << "Это не должно вызываться, нужно для компилируемости";
}

} // namespace std

namespace covellite
{

namespace api
{

namespace renderer
{

inline void CheckError(const int _Line)
{
  auto Error = glGetError();
  if (Error != GL_NO_ERROR)
  {
    throw EXCEPTION_NO_FILE_LINE(::std::runtime_error) << 
      _Line << ": error " << Error;
  }
}

#define GL_CHECK \
  CheckError(__LINE__)

template<class>
class Support; // Not implement!!!

template<>
class Support<::Camera>
{
public:
  static const GLuint Index = COVELLITE_BUFFER_INDEX_CAMERA;

private:
  static void Update(
    const GLint _ProgramId, 
    const ::Camera & _Matrices,
    const ::std::string & _BufferName)
  {
    const auto MatrixProjectionId =
      glGetUniformLocation(_ProgramId, (_BufferName + ".Projection").c_str());
    const auto MatrixViewId =
      glGetUniformLocation(_ProgramId, (_BufferName + ".View").c_str());
    const auto MatrixViewInverseId =
      glGetUniformLocation(_ProgramId, (_BufferName + ".ViewInverse").c_str());

    glUniformMatrix4fv(MatrixProjectionId, 1, GL_FALSE,
      ::glm::value_ptr(_Matrices.Projection));
    glUniformMatrix4fv(MatrixViewId, 1, GL_FALSE,
      ::glm::value_ptr(_Matrices.View));
    glUniformMatrix4fv(MatrixViewInverseId, 1, GL_FALSE,
      ::glm::value_ptr(_Matrices.ViewInverse));
  }

public:
  static void Update(const GLint _ProgramId, const ::Camera & _Matrices)
  {
    Update(_ProgramId, _Matrices, "MatricesData");
    Update(_ProgramId, _Matrices, "CameraData");
  }
};

template<>
class Support<::Fog>
{
public:
  static const GLuint Index = COVELLITE_BUFFER_INDEX_FOG;

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

template<>
class Support<::Object>
{
public:
  static const GLuint Index = COVELLITE_BUFFER_INDEX_OBJECT;

  static void Update(const GLint _ProgramId, const ::Ambient_t & _Light)
  {
    const auto IsValidId =
      glGetUniformLocation(_ProgramId, "ObjectData.Lights.Ambient.IsValid");
    const auto ColorId =
      glGetUniformLocation(_ProgramId, "ObjectData.Lights.Ambient.Color");

    glUniform1i(IsValidId, _Light.IsValid);
    glUniform4fv(ColorId, 1, ::glm::value_ptr(_Light.Color));
  }

  static void Update(const GLint _ProgramId, const ::Direction_t & _Light)
  {
    const auto IsValidId =
      glGetUniformLocation(_ProgramId, "ObjectData.Lights.Direction.IsValid");
    const auto ColorId =
      glGetUniformLocation(_ProgramId, "ObjectData.Lights.Direction.Color");
    const auto DirectionId =
      glGetUniformLocation(_ProgramId, "ObjectData.Lights.Direction.Direction");

    glUniform1i(IsValidId, _Light.IsValid);
    glUniform4fv(ColorId, 1, ::glm::value_ptr(_Light.Color));
    glUniform4fv(DirectionId, 1, ::glm::value_ptr(_Light.Direction));
  }

  static void Update(const GLint _ProgramId, const ::Points_t & _Lights)
  {
    static const ::std::string LightsDataPoints[] =
    {
      "ObjectData.Lights.Points.Lights[0]",
      "ObjectData.Lights.Points.Lights[1]",
      "ObjectData.Lights.Points.Lights[2]",
      "ObjectData.Lights.Points.Lights[3]",
      "ObjectData.Lights.Points.Lights[4]",
      "ObjectData.Lights.Points.Lights[5]",
      "ObjectData.Lights.Points.Lights[6]",
      "ObjectData.Lights.Points.Lights[7]",
      "ObjectData.Lights.Points.Lights[8]",
      "ObjectData.Lights.Points.Lights[9]",
      "ObjectData.Lights.Points.Lights[10]",
      "ObjectData.Lights.Points.Lights[11]",
      "ObjectData.Lights.Points.Lights[12]",
      "ObjectData.Lights.Points.Lights[13]",
      "ObjectData.Lights.Points.Lights[14]",
      "ObjectData.Lights.Points.Lights[15]",
    };

    constexpr auto LightCount =
      sizeof(LightsDataPoints) / sizeof(LightsDataPoints[0]);
    static_assert(LightCount >= COVELLITE_MAX_LIGHT_POINT_OBJECT_COUNT,
      "Unexpected LightsDataPoints array size.");

    const auto UsedSlotCountId =
      glGetUniformLocation(_ProgramId, "ObjectData.Lights.Points.UsedSlotCount");
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
  static void Update(const GLint _ProgramId, const ::Object & _Object)
  {
    const auto MatrixWorldId =
      glGetUniformLocation(_ProgramId, "ObjectData.World");
    glUniformMatrix4fv(MatrixWorldId, 1, GL_FALSE,
      ::glm::value_ptr(_Object.World));

    Update(_ProgramId, _Object.Lights.Ambient);
    Update(_ProgramId, _Object.Lights.Direction);
    Update(_ProgramId, _Object.Lights.Points);
  }
};

template<>
class Support<uint8_t>
{
public:
  static const GLuint Index = COVELLITE_BUFFER_INDEX_USER;
};

template<>
class Support<::Matrices>
{
public:
  static const GLuint Index = COVELLITE_BUFFER_INDEX_MATRICES;

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
class Support<::SceneLights>
{
public:
  static const GLuint Index = COVELLITE_BUFFER_INDEX_LIGHTS;

private:
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

  static void Update(const GLint _ProgramId, const ::ScenePoints & _Lights)
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
    static_assert(LightCount >= COVELLITE_MAX_LIGHT_POINT_SCENE_COUNT,
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
  static void Update(const GLint _ProgramId, const ::SceneLights & _Lights)
  {
    Update(_ProgramId, _Lights.Ambient);
    Update(_ProgramId, _Lights.Direction);
    Update(_ProgramId, _Lights.Points);
  }
};

class OpenGLCommonShader::Buffer
{
public:
  inline void Bind(const bool _IsActivate = true) const
  {
    glBindBuffer(m_Type, _IsActivate ? m_BufferId : 0);
  }

  inline void UpdateData(const void * _pData, const GLsizeiptr _Size) const
  {
    glBufferSubData(m_Type, 0, _Size, _pData);
  }

  inline void UpdateData(const ::std::string & _UniformBufferName,
    const void * _pData, const GLsizeiptr _Size) const
  {
    GLint ProgramId = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &ProgramId);

    const auto BlockIndex = 
      glGetUniformBlockIndex(ProgramId, _UniformBufferName.c_str());

    // Вероятно, шейдерная программа выкидывает неиспользуемые uniform-буфера,
    // в результате чего Index становится некорректным и glUniformBlockBinding()
    // завершается с ошибкой.
    // При игнорирование ошибки здесь визуально все работает корректно.
    if (BlockIndex == GL_INVALID_INDEX) return;

    Bind();
    UpdateData(_pData, _Size);
    Bind(false);

    glBindBufferBase(m_Type, m_Index, m_BufferId);
    glUniformBlockBinding(ProgramId, BlockIndex, m_Index);
  }

  void SetVertexInputData(void) const
  {
    // Нужно обязательно проверять валидность идентификаторов, т.к. некоторые
    // реализации драйверов (не все!) удаляют при компиляции шейдера
    // объявленные, но не используемые переменные.

    // Явный вызов glVertexAttribDivisor() требуется по той же причине -
    // из-за удаления в шейдере не используемой переменной вызов этой функции
    // при использовании инстансинга привяжет смещение к идентификатору,
    // который в дальнейшем может быть получен здесь.

    GLint ProgramId = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &ProgramId);

    const auto hPosition =
      glGetAttribLocation(ProgramId, "Covellite_VertexPosition");
    if (hPosition != -1)
    {
      glEnableVertexAttribArray(hPosition);
      glVertexAttribPointer(hPosition, 4, GL_FLOAT, GL_FALSE,
        sizeof(::covellite::api::Vertex), (void*)0);
      glVertexAttribDivisor(hPosition, 0);
    }

    const auto hTexCoord =
      glGetAttribLocation(ProgramId, "Covellite_VertexTexCoord");
    if (hTexCoord != -1)
    {
      glEnableVertexAttribArray(hTexCoord);
      glVertexAttribPointer(hTexCoord, 2, GL_FLOAT, GL_FALSE,
        sizeof(::covellite::api::Vertex), (void*)(sizeof(float) * 4));
      glVertexAttribDivisor(hTexCoord, 0);
    }

    const auto hExtra =
      glGetAttribLocation(ProgramId, "Covellite_VertexExtra");
    if (hExtra != -1)
    {
      glEnableVertexAttribArray(hExtra);
      glVertexAttribPointer(hExtra, 4, GL_FLOAT, GL_FALSE,
        sizeof(::covellite::api::Vertex), (void*)(sizeof(float) * 6));
      glVertexAttribDivisor(hExtra, 0);
    }
  }

  void SetInstanceData(
    const void * _pData, 
    const GLsizeiptr _Size, 
    const GLsizei _Stride)
  {
    using AttributeTypes_t = ::alicorn::extension::std::fast::unordered_map<
      ::std::string, ::std::pair<GLint, GLenum>>;

    constexpr auto BlockSize = sizeof(float) * 4;
    const auto BlockCount = _Stride / BlockSize;

    Bind(true);
    UpdateData(_pData, _Size);

    GLint ProgramId = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &ProgramId);

    const auto GetAttributeTypes = [&](void) -> AttributeTypes_t
    {
      // С одной стороны, список атрибутов достаточно формировать один раз
      // для каждой шейдерной программы, с другой стороны - индексы могут быть
      // изменены при помощи функции glBindAttribLocation().

      constexpr GLsizei NameBufferSize = 255;
      GLsizei NameLength = 0;
      GLint ValueSize = 0;
      GLchar ValueName[NameBufferSize] = { 0 };

      GLint AttributeCount = 0;
      glGetProgramiv(ProgramId, GL_ACTIVE_ATTRIBUTES, &AttributeCount);

      AttributeTypes_t Result;

      for (GLint i = 0; i < AttributeCount; i++)
      {
        auto iType = static_cast<GLenum>(-1);
        glGetActiveAttrib(ProgramId, i, NameBufferSize, &NameLength, 
          &ValueSize, &iType, ValueName);

        Result[ValueName] = { i, // Индексы - для отладочных целей
          (iType == GL_FLOAT_VEC4) ? GL_FLOAT :
          (iType == GL_INT_VEC4) ? GL_INT : iType };
      }

      return Result;
    };

    // Работа через список всех атрибутов понадобилась из-за того, что функция
    // glGetActiveAttrib() в Windows версии ожидает hInstance, а в Android
    // версии - индекс атрибута (в Android версии при компиляции шейдера
    // удаляются все неиспользуемые переменные и hInstance и индекс не совпадают).
    auto AttributeTypes = GetAttributeTypes();

    for (::std::size_t i = 0; i < BlockCount; i++)
    {
      const auto Name = "iValue" + ::std::to_string(i + 1);

      const auto hInstance = glGetAttribLocation(ProgramId, Name.c_str());
      if (hInstance == -1) continue;

      glEnableVertexAttribArray(hInstance);
      glVertexAttribPointer(hInstance, 4, AttributeTypes[Name].second, 
        GL_FALSE, _Stride, (void*)(BlockSize * i));
      glVertexAttribDivisor(hInstance, 1);
    }

    Bind(false);
  }

private:
  const GLenum m_Type;
  const GLuint m_Index;
  GLuint       m_BufferId;

public:
  Buffer(
    const GLenum _Type,
    const void * _pData,
    const ::std::size_t _Size,
    const GLenum _Usage,
    const GLuint _Index = static_cast<GLuint>(-1)) :
    m_Type(_Type),
    m_Index(_Index)
  {
    glGenBuffers(1, &m_BufferId);
    Bind();
    glBufferData(m_Type, static_cast<GLsizeiptr>(_Size), _pData, _Usage);
    Bind(false);

    const auto Error = glGetError();
    if (Error != GL_NO_ERROR)
    {
      throw STD_EXCEPTION << "Create buffer error: " << Error;
    }
  }

  template<class T>
  Buffer(const T * _pData, const ::std::size_t _Size) :
    Buffer(GL_UNIFORM_BUFFER, _pData, _Size, GL_DYNAMIC_DRAW, Support<T>::Index)
  {

  }

  ~Buffer(void) noexcept
  {
    glDeleteBuffers(1, &m_BufferId);
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

    // 21 Сентябрь 2019 11:58 (unicornum.verum@gmail.com)
    TODO("Использование uniform-буфера");

    //static const char * Name[] =
    //{
    //  "cbCameraData",
    //  "cbFogData",
    //  "cbObjectData",
    //  "cbUserData",
    //  "cbMatricesData",
    //  "cbLightsData",
    //};

    //const auto & Data = Constants::Data<T>::m_Data;

    //if (m_pBuffer == nullptr)
    //{
    //  m_pBuffer = ::std::make_shared<OpenGLCommonShader::Buffer>(
    //    &Data, sizeof(T));
    //}

    //m_pBuffer->Update(Name[Support<T>::Index], &Data, sizeof(T));
  }

private:
  //mutable ::std::shared_ptr<OpenGLCommonShader::Buffer> m_pBuffer;
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

void OpenGLCommonShader::PresentFrame(void) /*override*/
{
  OpenGLCommon::PresentFrame();

  //GL_CHECK;
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

class FrameBuffer
{
public:
  inline void Bind(void) const
  {
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_CurrentFrameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
  }

  inline void Unbind(void) const
  {
    glBindFramebuffer(GL_FRAMEBUFFER, m_CurrentFrameBufferId);
  }

private:
  GLuint m_Id = 0;
  mutable GLint m_CurrentFrameBufferId = 0;

public:
  FrameBuffer(void)
  {
    glGenFramebuffers(1, &m_Id);
  }
  ~FrameBuffer(void) noexcept
  {
    glDeleteFramebuffers(1, &m_Id);
  }
};

auto OpenGLCommonShader::CreateBkSurface(
  const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  GLfloat Viewport[4] = { 0 };
  glGetFloatv(GL_VIEWPORT, Viewport);

  const auto pFrameBuffer = ::std::make_shared<FrameBuffer>();
  pFrameBuffer->Bind();

  ::std::vector<GLenum> AttachmentIndexes;
  ::std::vector<::std::pair<ComponentPtr_t, Texture::Ptr_t>> Textures;

  const auto DoDataTexture = [&](const ComponentPtr_t & _pDataTexture)
  {
    _pDataTexture->SetValue(uT("width"), static_cast<int>(Viewport[2]));
    _pDataTexture->SetValue(uT("height"), static_cast<int>(Viewport[3]));

    const Component::Texture TextureData{ _pDataTexture };

    const auto pTexture = ::std::make_shared<Texture>(TextureData);
    Textures.push_back({ _pDataTexture, pTexture });

    if (pTexture->m_Format != GL_DEPTH_COMPONENT)
    {
      auto Attachment = GL_COLOR_ATTACHMENT0 +
        static_cast<int>(AttachmentIndexes.size());
      AttachmentIndexes.push_back(Attachment);
      glFramebufferTexture2D(GL_FRAMEBUFFER, Attachment, GL_TEXTURE_2D,
        pTexture->m_TextureId, 0);
    }
    else
    {
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
        GL_TEXTURE_2D, pTexture->m_TextureId, 0);
    }

    _pDataTexture->SetValue(uT("entity"), pTexture);
  };

  m_ServiceComponents.Process(
    {
      { uT("Texture"), DoDataTexture },
    });
 
  const auto FramebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  pFrameBuffer->Unbind();

  if (FramebufferStatus != GL_FRAMEBUFFER_COMPLETE)
  {
    throw STD_EXCEPTION << "Create Framebuffer fail: " << _pComponent->Id;
  }

  return [=](void)
  {
    if (m_IsResizeWindow)
    {
      GLfloat Viewport[4] = { 0 };
      glGetFloatv(GL_VIEWPORT, Viewport);

      for (const auto & Texture : Textures)
      {
        Texture.first->SetValue(uT("width"), static_cast<int>(Viewport[2]));
        Texture.first->SetValue(uT("height"), static_cast<int>(Viewport[3]));
        Texture.second->MakeContent(Viewport[2], Viewport[3], nullptr);
      }
    }

    pFrameBuffer->Bind();
    glDrawBuffers(static_cast<GLsizei>(AttachmentIndexes.size()),
      AttachmentIndexes.data());
    //glViewport(0, 0, 1280, 768);
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

auto OpenGLCommonShader::CreateLight(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  return DoCreateLight<::SceneLights>(_pComponent);
}

auto OpenGLCommonShader::CreateTexture(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  using BufferMapper_t = cbBufferMap_t<const void>;

  const auto pTextureDataComponent =
    m_ServiceComponents.Get({ { uT("Texture"), _pComponent } })[0];

  const Component::Texture TextureData{ pTextureDataComponent };

  ::std::function<GLint(void)> GetTexMinFilter = 
    [=](void) { return m_TexParameters.MinFilter; };

  Texture::Ptr_t pTexture =
    pTextureDataComponent->GetValue(uT("entity"), Texture::Ptr_t{});
  if (pTexture == nullptr)
  {
    pTexture = ::std::make_shared<Texture>(TextureData);

    if (TextureData.IsUsingMipmapping)
    {
      GetTexMinFilter = [](void) { return GL_LINEAR_MIPMAP_LINEAR; };

      pTexture->Bind();
      glGenerateMipmap(GL_TEXTURE_2D);
      pTexture->Bind(false);
    }
  }
  else
  {
    pTextureDataComponent->SetValue(uT("entity"), Texture::Ptr_t{});
  }

  const Render_t TextureRender = [=](void)
  {
    auto Index = pTexture->m_Destination.first;

    glActiveTexture(GL_TEXTURE0 + Index);
    pTexture->Bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetTexMinFilter());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_TexParameters.MagFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_TexParameters.WrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_TexParameters.WrapT);

    GLint ProgramId = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &ProgramId);

    const auto LocationId = glGetUniformLocation(ProgramId,
      pTexture->m_Destination.second.c_str());
    if (LocationId == -1)
    {
      // Нужна явная проверка, т.к. это не приведет к генерации ошибки
      // для glGetError().
      throw STD_EXCEPTION << "No texture declared in shader: " <<
        pTexture->m_Destination.second << " [id: " << _pComponent->Id << "].";
    }

    glUniform1i(LocationId, Index);
  };

  if (!pTextureDataComponent->IsType<const BufferMapper_t &>(uT("mapper")))
  {
    return TextureRender;
  }

  const auto cbBufferMapper = 
    pTextureDataComponent->GetValue<const BufferMapper_t &>(uT("mapper"), BufferMapper_t{});
  const auto pFrameBuffer = ::std::make_shared<FrameBuffer>();

  return [=](void)
  {
    if (cbBufferMapper(nullptr))
    {
      pFrameBuffer->Bind();
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, pTexture->m_TextureId, 0);

      auto * pData = reinterpret_cast<uint32_t *>(pTexture->m_ReadCopyData.data());

      glReadPixels(0, 0, TextureData.Width, TextureData.Height, GL_RGBA,
        GL_UNSIGNED_BYTE, pData);

      // Изображение в текстуре OpenGL перевернуто по Y, поэтому... 
      for (int y = 0; y < TextureData.Height / 2; y++)
      {
        auto * pLineUp = 
          pData + (y * TextureData.Width);
        auto * pLineDown = 
          pData + ((TextureData.Height - y - 1) * TextureData.Width);

        for (int x = 0; x < TextureData.Width; x++)
        {
          auto & Up = *(pLineUp + x);
          auto & Down = *(pLineDown + x);

          ::std::swap(Up, Down);
        }
      }

      cbBufferMapper(pData);
      pFrameBuffer->Unbind();
    }

    TextureRender();
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
    ~Program(void) noexcept
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

        throw STD_EXCEPTION << "Compile shader fail: " << InfoLog
          << " [header line: " << GetHeaderLines(m_Header) << "]";
      };
    }

  private:
    static ::std::size_t GetHeaderLines(const ::std::string & _Header)
    {
      ::std::size_t Result = 0;

      auto itPosition = _Header.cbegin();
      while (true)
      {
        itPosition = ::std::find(itPosition, _Header.cend(), '\n');
        if (itPosition == _Header.cend()) break;

        ++itPosition;
        Result++;
      }

      return Result;
    }

  public:
    Programs & m_Programs;
    const GLenum Type;
    const ::std::string m_Header;
    const GLuint Id;

  public:
    Shader(Programs & _Programs, const GLenum _Type, 
      const GLchar * _Header, const GLchar * _Body) :
      m_Programs{ _Programs },
      Type{ _Type },
      m_Header(_Header),
      Id{ glCreateShader(_Type) }
    {
      const auto FullShaderBody = ::std::string{ _Header } +_Body;
      auto pFullShaderBody = FullShaderBody.c_str();
      glShaderSource(Id, 1, &pFullShaderBody, nullptr);
    }
    ~Shader(void) noexcept
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
    
    const auto HeaderShaderText =
      ::Predefined + 
      ::Data + 
      _ShaderData.GetInstanceInput(::Input);

    const auto BodyShaderText =
      _ShaderData.GetBody();

    return ::std::make_shared<Shader>(*this, GL_VERTEX_SHADER,
      (m_ShaderHeader +
      "#define COVELLITE_SHADER_VERTEX\r\n" +
      ::std::string{ reinterpret_cast<const char *>(
        HeaderShaderText.data()), HeaderShaderText.size() }).c_str(),
      (::std::string{ reinterpret_cast<const char *>(
        BodyShaderText.data()), BodyShaderText.size() } +
      "out Pixel PixelValue;\r\n"
      "void main()\r\n"
      "{\r\n"
      "  Vertex InputData;\r\n"
      "  InputData.Position = Covellite_VertexPosition;\r\n"
      "  InputData.TexCoord = Covellite_VertexTexCoord;\r\n"
      "  InputData.Extra = Covellite_VertexExtra;\r\n" +
      _ShaderData.GetInstanceCopyData() +
      "  PixelValue = " + _ShaderData.Entry + "(InputData);\r\n"
      "  gl_Position = PixelValue.ScreenPos;\r\n"
      "}\r\n").c_str());
  }

  ShaderPtr_t MakePixel(const Component::Shader & _ShaderData)
  {
    using namespace ::alicorn::extension::std;

    const auto HeaderShaderText = ::Predefined + ::Data + ::Input;
    const auto BodyShaderText = ::std::string{
      _ShaderData.pData, _ShaderData.pData + _ShaderData.Count };

    ::std::string Main;

    if (_ShaderData.ReturnType == "float4" ||
      _ShaderData.ReturnType == "vec4")
    {
      Main =
        "out vec4 Covellite_OutPixelColor;\r\n"
        "void main()\r\n"
        "{\r\n"
        "  Covellite_OutPixelColor = " + _ShaderData.Entry + "(PixelValue);\r\n"
        "}\r\n";
    }
    else if (_ShaderData.ReturnType == "void")
    {
      Main =
        "void main()\r\n"
        "{\r\n"
        "  " + _ShaderData.Entry + "(PixelValue);\r\n"
        "}\r\n";
    }
    else
    {
      Main =
        "void main()\r\n"
        "{\r\n"
        "  Covellite_MultiOutPixelColor = " + _ShaderData.Entry + "(PixelValue).Target;\r\n"
        "}\r\n";
    }

    return ::std::make_shared<Shader>(*this, GL_FRAGMENT_SHADER,
      (m_ShaderHeader +
      "#define COVELLITE_SHADER_PIXEL\r\n" +
      ::std::string{ reinterpret_cast<const char *>(
        HeaderShaderText.data()), HeaderShaderText.size() }).c_str(),
      (BodyShaderText +
      "in Pixel PixelValue;\r\n" +
      Main).c_str());
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
      TEST_CALL_IF(3) m_pConstants->Update<::SceneLights>();

#     undef TEST_CALL_IF
    };
  }

  return nullptr;
}

auto OpenGLCommonShader::CreateBuffer(const ComponentPtr_t & _pBuffer) -> Render_t /*override*/
{
  const auto pBufferData = 
    m_ServiceComponents.Get({ { uT("Buffer"), _pBuffer } })[0];

  auto CreateConstantUserBuffer = [&](void) -> Render_t
  {
    using Type_t = cbBufferMap_t<void>;
    using BufferData_t = ::std::vector<uint8_t>;

    if (!_pBuffer->IsType<const Type_t &>(uT("mapper")))
    {
      throw STD_EXCEPTION << "Unexpected buffer format [" <<
        "id: " << _pBuffer->Id << ", " <<
        "type: " << _pBuffer->Type << ", " <<
        "kind: " << _pBuffer->Kind << "].";
    }

    const auto cbBufferMapper =
      _pBuffer->GetValue<const Type_t &>(uT("mapper"), Type_t{});
    if (!cbBufferMapper)
    {
      throw STD_EXCEPTION << "Unexpected empty mapper: " << _pBuffer->Id;
    }

    const auto BufferSize = 
      _pBuffer->GetValue(uT("size"), (::std::size_t)0);
    if (BufferSize == 0)
    {
      throw STD_EXCEPTION << "Unexpected zero size: " << _pBuffer->Id;
    }

    const auto cbName =
      _pBuffer->GetValue(uT("name"), ::std::string{ "cbUserData" });

    const auto pData =
      ::std::make_shared<BufferData_t>(BufferSize, (uint8_t)0x00);
    const auto pBuffer = 
      ::std::make_shared<Buffer>(pData->data(), pData->size());

    return [=](void)
    {
      cbBufferMapper(pData->data());
      pBuffer->UpdateData(cbName, pData->data(), pData->size());
    };
  };

  auto CreateConstantLightsBuffer = [&](void) -> Render_t
  {
    using Type_t = cbBufferMap_t<::Lights_t>;

    if (!_pBuffer->IsType<const Type_t &>(uT("mapper")))
    {
      return CreateConstantUserBuffer();
    }

    const auto cbBufferMapper = _pBuffer->GetValue(uT("mapper"), Type_t{});
    if (!cbBufferMapper)
    {
      throw STD_EXCEPTION << "Unexpected empty mapper: " << _pBuffer->Id;
    }

    return [=](void)
    {
      cbBufferMapper(&m_pConstants->Get<::Object>().Lights);
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

    const auto pBuffer = ::std::make_shared<Buffer>(GL_ELEMENT_ARRAY_BUFFER,
      Info.pData, Info.Count * sizeof(Type_t), GL_STATIC_DRAW);
    const auto Size = static_cast<GLsizei>(Info.Count);

    return [=](void)
    {
      pBuffer->Bind();

      m_DrawElements = [=](void)
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
        pBuffer->SetVertexInputData();
        pBuffer->Bind(false);
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

        pBuffer->UpdateData(pData->data(),
          static_cast<GLsizeiptr>(pData->size() * sizeof(Type_t)));
      }

      pBuffer->SetVertexInputData();
      pBuffer->Bind(false);
    };
  };

  return CreateVertexBuffer();
}

auto OpenGLCommonShader::CreateTransform(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  using TransformRender_t = ::std::function<void(void)>;

  const auto GetPreRenderDefaultGeometry = [&](void) -> TransformRender_t
  {
    const auto TransformRender = GetPreRenderGeometry();

    return [=](void)
    {
      auto & World = m_pConstants->Get<::Object>().World;
      World = ::glm::identity<::glm::mat4>();
      TransformRender(World);
      World = ::glm::transpose(World);
    };
  };

  const auto GetPreRenderStaticGeometry = [&](void) -> TransformRender_t
  {
    ::glm::mat4 World = ::glm::identity<::glm::mat4>();
    GetPreRenderGeometry()(World);
    World = ::glm::transpose(World);

    return [=](void)
    {
      m_pConstants->Get<::Object>().World = World;
    };
  };

  const auto GetPreRenderBillboardGeometry = [&](void) -> TransformRender_t
  {
    const auto TransformRender = 
      OpenGLCommonShader::GetPreRenderBillboardGeometry();

    return [=](void)
    {
      auto & World = m_pConstants->Get<::Object>().World;
      World = m_pConstants->Get<::Camera>().View;
      TransformRender(World);
      World = ::glm::transpose(World);
    };
  };

  // 17 Сентябрь 2019 12:58 (unicornum.verum@gmail.com)
  TODO("Тест Transform.Static не проверяет, что матрица меняется у РАЗНЫХ объектов");

  const auto TransformRender =
    (_pComponent->Kind == uT("Unknown")) ? GetPreRenderDefaultGeometry() :
    (_pComponent->Kind == uT("Static")) ? GetPreRenderStaticGeometry() :
    (_pComponent->Kind == uT("Billboard")) ? GetPreRenderBillboardGeometry() :
      throw STD_EXCEPTION << "Unexpected transform component: " <<
        " [id=" << _pComponent->Id << ", kind=" << _pComponent->Kind << "].";

  return [=](void)
  {
    TransformRender();
    m_pConstants->Update<::Object>();
  };
}

auto OpenGLCommonShader::CreatePresentBuffer(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  using BufferMapper_t = cbBufferMap_t<void>;
  using BufferData_t = ::std::vector<uint8_t>;

  ComponentPtr_t pIndexBufferData = _pComponent;
  ComponentPtr_t pInstanceBufferData = nullptr;

  const auto SaveBuffer = [&](const ComponentPtr_t & _pBufferData)
  {
    if (_pBufferData->IsType<const int *>(uT("data")))
    {
      pIndexBufferData = _pBufferData;
    }
    else if (_pBufferData->IsType<const BufferMapper_t &>(uT("mapper")))
    {
      pInstanceBufferData = _pBufferData;
    }
    else
    {
      // 23 Сентябрь 2019 13:26 (unicornum.verum@gmail.com)
      TODO("Здесь требуется диагностика (warning) компонента буфера, содержащего данные неожиданного типа");
    }
  };

  m_ServiceComponents.Process(
    {
      { uT("Buffer"), SaveBuffer },
    });

  const Component::Buffer<int> IndexBufferData{ pIndexBufferData };

  const auto pIndexBuffer = ::std::make_shared<Buffer>(GL_ELEMENT_ARRAY_BUFFER,
    IndexBufferData.pData, IndexBufferData.Count * sizeof(int), GL_STATIC_DRAW);
  const auto IndexCount = static_cast<GLsizei>(IndexBufferData.Count);

  if (pInstanceBufferData == nullptr)
  {
    return [=](void)
    {
      pIndexBuffer->Bind();
      glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr);
      pIndexBuffer->Bind(false);
    };
  }

  const auto cbBufferMapper =
    pInstanceBufferData->GetValue<const BufferMapper_t &>(uT("mapper"), nullptr);
  const auto BufferSize =
    pInstanceBufferData->GetValue(uT("size"), (::std::size_t)16);
  const auto InstanceCount = static_cast<GLint>(
    pInstanceBufferData->GetValue(uT("count"), (::std::size_t)1));
  const auto Stride = static_cast<GLsizei>(
    BufferSize / InstanceCount);

  // 23 Сентябрь 2019 18:53 (unicornum.verum@gmail.com)
  TODO("Проверка делимости BufferSize / InstanceCount без остатка");

  // 23 Сентябрь 2019 18:53 (unicornum.verum@gmail.com)
  TODO("Проверка того, что Stride кратно 16");

  const auto pData =
    ::std::make_shared<BufferData_t>(BufferSize, (uint8_t)0x00);
  const auto pInstanceBuffer = ::std::make_shared<Buffer>(GL_ARRAY_BUFFER,
    nullptr, BufferSize, GL_DYNAMIC_DRAW);

  return [=](void)
  {
    const auto IsDirty = cbBufferMapper(nullptr);
    if (IsDirty)
    {
      cbBufferMapper(pData->data());
      pInstanceBuffer->SetInstanceData(pData->data(), pData->size(), Stride);
    }

    pIndexBuffer->Bind();
    glDrawElementsInstanced(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr, 
      InstanceCount);
    pIndexBuffer->Bind(false);
  };
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
      World = ::glm::transpose(World);
    };
  };

  const auto GetPreRenderStaticGeometry = [&](void) -> PreRender_t
  {
    ::glm::mat4 World = ::glm::identity<::glm::mat4>();
    GetPreRenderGeometry()(World);
    World = ::glm::transpose(World);

    return [=](void)
    {
      m_pConstants->Get<::Matrices>().World = World;
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
      World = ::glm::transpose(World);
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
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    DisableDepthRender();

    glDisable(GL_BLEND);
    glDisable(GL_DITHER);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
  };
}

auto OpenGLCommonShader::GetCameraOrthographic(
  const ComponentPtr_t & _pComponent) -> Render_t
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

    Matrix.Projection = ::glm::transpose(::glm::ortho(
      Viewport[0] + Offset.X,
      Viewport[0] + Viewport[2] + Offset.X,
      Viewport[1] + Viewport[3] + Offset.Y,
      Viewport[1] + Offset.Y,
      1.0f, -1.0f));
    Matrix.View = ::glm::transpose(::glm::identity<::glm::mat4>());
    Matrix.ViewInverse = ::glm::transpose(
      ::glm::inverse(::glm::identity<::glm::mat4>()));

    _pComponent->SetValue(uT("view"), Matrix.View);
    _pComponent->SetValue(uT("projection"), Matrix.Projection);
  };
}

auto OpenGLCommonShader::GetCameraPerspective(
  const ComponentPtr_t & _pComponent) -> Render_t
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
    const auto zNear = _pComponent->GetValue(uT("znear"), 0.01f);
    const auto zFar = _pComponent->GetValue(uT("zfar"), 200.0f);

    Matrix.Projection = ::glm::transpose(::glm::perspectiveFovRH(AngleY,
      Viewport[2], Viewport[3], zFar, zNear));

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

    const auto View = ::glm::lookAtRH(
      GetEye(),
      ::glm::vec3{ Look.X, Look.Y, Look.Z },
      ::glm::vec3{ 0.0f, 0.0f, 1.0f }); // Up

    Matrix.View = ::glm::transpose(View);
    Matrix.ViewInverse = ::glm::transpose(::glm::inverse(View));

    _pComponent->SetValue(uT("view"), Matrix.View);
    _pComponent->SetValue(uT("projection"), Matrix.Projection);
  };
}

} // namespace renderer

} // namespace api

} // namespace covellite
