
#include "stdafx.h"
#include "OpenGLCommonShader.hpp"
#include <glm/glm.force.hpp>
#include <alicorn/std/vector.hpp>
#include <alicorn/std.fast/unordered-map.hpp>
#include <Covellite/Api/Vertex.hpp>
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

inline GLuint glGetProgramId(void)
{
  GLint iProgramId = 0;
  glGetIntegerv(GL_CURRENT_PROGRAM, &iProgramId);
  return static_cast<GLuint>(iProgramId);
}

inline void CheckError(const int _Line)
{
  const auto Error = glGetError();
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
    const GLuint _ProgramId, 
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
  static void Update(const GLuint _ProgramId, const ::Camera & _Matrices)
  {
    Update(_ProgramId, _Matrices, "MatricesData");
    Update(_ProgramId, _Matrices, "CameraData");
  }
};

template<>
class Support<::Object>
{
public:
  static const GLuint Index = COVELLITE_BUFFER_INDEX_OBJECT;

public:
  static void Update(const GLuint _ProgramId, const ::Object & _Object)
  {
    const auto MatrixWorldId =
      glGetUniformLocation(_ProgramId, "ObjectData.World");
    glUniformMatrix4fv(MatrixWorldId, 1, GL_FALSE,
      ::glm::value_ptr(_Object.World));
  }
};

template<>
class Support<uint8_t>
{
public:
  static const GLuint Index = COVELLITE_BUFFER_INDEX_USER;
};

class OpenGLCommonShader::Buffer
{
public:
  inline void Bind(const bool _IsActivate = true) const
  {
    glBindBuffer(m_Type, _IsActivate ? m_BufferId : 0);
  }

  inline void UpdateData(const void * _pData, const size_t _Size) const
  {
    glBufferSubData(m_Type, 0, static_cast<GLsizeiptr>(_Size), _pData);
  }

  inline void UpdateData(const ::std::string & _UniformBufferName,
    const void * _pData, const size_t _Size) const
  {
    const auto ProgramId = glGetProgramId();

    const auto BlockIndex = glGetUniformBlockIndex(
      ProgramId, _UniformBufferName.c_str());

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

    const auto ProgramId = glGetProgramId();

    const auto hPosition = static_cast<GLuint>(
      glGetAttribLocation(ProgramId, "Covellite_VertexPosition"));
    if (hPosition != -1)
    {
      glEnableVertexAttribArray(hPosition);
      glVertexAttribPointer(hPosition, 4, GL_FLOAT, GL_FALSE,
        sizeof(::covellite::api::Vertex), (void *)0);
      glVertexAttribDivisor(hPosition, 0);
    }

    const auto hTexCoord = static_cast<GLuint>(
      glGetAttribLocation(ProgramId, "Covellite_VertexTexCoord"));
    if (hTexCoord != -1)
    {
      glEnableVertexAttribArray(hTexCoord);
      glVertexAttribPointer(hTexCoord, 2, GL_FLOAT, GL_FALSE,
        sizeof(::covellite::api::Vertex), (void *)(sizeof(float) * 4));
      glVertexAttribDivisor(hTexCoord, 0);
    }

    const auto hExtra = static_cast<GLuint>(
      glGetAttribLocation(ProgramId, "Covellite_VertexExtra"));
    if (hExtra != -1)
    {
      glEnableVertexAttribArray(hExtra);
      glVertexAttribPointer(hExtra, 4, GL_FLOAT, GL_FALSE,
        sizeof(::covellite::api::Vertex), (void *)(sizeof(float) * 6));
      glVertexAttribDivisor(hExtra, 0);
    }
  }

  void SetInstanceData(
    const void * _pData, 
    const size_t _Size,
    const GLsizei _Stride)
  {
    using AttributeTypes_t = ::alicorn::extension::std::fast::unordered_map<
      ::std::string, ::std::pair<GLint, GLenum>>;

    constexpr auto BlockSize = sizeof(float) * 4;
    const auto BlockCount = _Stride / BlockSize;

    Bind(true);
    UpdateData(_pData, _Size);

    const auto ProgramId = glGetProgramId();

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

      for (GLuint i = 0; i < static_cast<GLuint>(AttributeCount); i++)
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

      const auto hInstance = static_cast<GLuint>(
        glGetAttribLocation(ProgramId, Name.c_str()));
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

  Buffer(const Buffer &) = delete;
  Buffer(Buffer &&) = delete;
  Buffer & operator= (const Buffer &) = delete;
  Buffer & operator= (Buffer &&) = delete;

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
    const auto ProgramId = glGetProgramId();

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
      (const char *)glGetStringi(GL_EXTENSIONS, static_cast<GLuint>(i));
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
  };
}

class FrameBuffer
{
public:
  inline void Bind(void) const
  {
    GLint CurrentFrameBufferId = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &CurrentFrameBufferId);
    m_CurrentFrameBufferId = static_cast<GLuint>(CurrentFrameBufferId);

    glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
  }

  inline void Unbind(void) const
  {
    glBindFramebuffer(GL_FRAMEBUFFER, m_CurrentFrameBufferId);
  }

private:
  GLuint m_Id = 0;
  mutable GLuint m_CurrentFrameBufferId = 0;

public:
  FrameBuffer(void) noexcept
  {
    glGenFramebuffers(1, &m_Id);
  }
  FrameBuffer(const FrameBuffer &) = delete;
  FrameBuffer(FrameBuffer &&) = delete;
  FrameBuffer & operator= (const FrameBuffer &) = delete;
  FrameBuffer & operator= (FrameBuffer &&) = delete;
  ~FrameBuffer(void) noexcept
  {
    glDeleteFramebuffers(1, &m_Id);
  }
};

auto OpenGLCommonShader::CreateBkSurface(
  const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  // !!!std::tuple не компилируется на Android!!!
  using Size_t = ::std::pair<GLint, GLint>;
  using fnBkSurfaceSize_t = ::std::function<Size_t(void)>;

  const auto pBkSurface = _pComponent;

  const fnBkSurfaceSize_t GetBkSurfaceSize = [=](void)
  {
    GLint Viewport[4] = { 0 };
    glGetIntegerv(GL_VIEWPORT, Viewport);

    const int Width = static_cast<int>(Viewport[2]);
    const int Height = static_cast<int>(Viewport[3]);

    (*pBkSurface)[uT("width")] = Width;
    (*pBkSurface)[uT("height")] = Height;

    return Size_t{ static_cast<GLint>(Width), static_cast<GLint>(Height) };
  };

  const auto Size = GetBkSurfaceSize();
  const auto Width = Size.first;
  const auto Height = Size.second;

  const auto pFrameBuffer = ::std::make_shared<FrameBuffer>();
  pFrameBuffer->Bind();

  ::std::vector<GLenum> AttachmentIndexes;
  ::std::vector<::std::pair<ComponentPtr_t, Texture::Ptr_t>> Textures;

  const auto DoDataTexture = [&](const ComponentPtr_t & _pDataTexture)
  {
    (*_pDataTexture)[uT("width")] = Width;
    (*_pDataTexture)[uT("height")] = Height;

    const Component::Texture TextureData{ *_pDataTexture, uT("diffuse") };

    const auto pTexture = ::std::make_shared<Texture>(TextureData);
    Textures.push_back({ _pDataTexture, pTexture });

    if (pTexture->m_Format != GL_DEPTH_COMPONENT)
    {
      const auto Attachment = GL_COLOR_ATTACHMENT0 +
        static_cast<unsigned int>(AttachmentIndexes.size());
      AttachmentIndexes.push_back(Attachment);
      glFramebufferTexture2D(GL_FRAMEBUFFER, Attachment, GL_TEXTURE_2D,
        pTexture->m_TextureId, 0);
    }
    else
    {
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
        GL_TEXTURE_2D, pTexture->m_TextureId, 0);
    }

    (*_pDataTexture)[uT("entity")].Default(pTexture);
  };

  CapturingServiceComponent::Process(_pComponent,
    { { uT("Texture"), DoDataTexture } });
 
  const auto FramebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  pFrameBuffer->Unbind();

  if (FramebufferStatus != GL_FRAMEBUFFER_COMPLETE)
  {
    throw STD_EXCEPTION << "Create Framebuffer fail: " << _pComponent->Id;
  }

  return [=](void)
  {
    const auto [Width, Height] = GetBkSurfaceSize();

    if (m_IsResizeWindow)
    {
      for (const auto & Texture : Textures)
      {
        (*Texture.first)[uT("width")] = Width;
        (*Texture.first)[uT("height")] = Height;
        Texture.second->MakeContent(Width, Height, nullptr);
      }
    }

    pFrameBuffer->Bind();
    glDrawBuffers(static_cast<GLsizei>(AttachmentIndexes.size()),
      AttachmentIndexes.data());
  };
}

auto OpenGLCommonShader::CreateState(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  if (_pComponent->Kind == uT("AlphaTest")) return nullptr;

  return OpenGLCommon::CreateState(_pComponent);
}

auto OpenGLCommonShader::CreateTexture(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  using BufferMapper_t = cbBufferMap_t<const void>;

  const auto pTextureDataComponent = CapturingServiceComponent::Get(_pComponent, 
    { { uT("Texture"), _pComponent } })[0];

  const auto pTextureData = 
    ::std::make_shared<Component::Texture>(*pTextureDataComponent, uT("diffuse"));

  ::std::function<GLint(void)> GetTexMinFilter = 
    [=](void) { return m_TexParameters.MinFilter; };

  Texture::Ptr_t pTexture =
    (*pTextureDataComponent)[uT("entity")].Default(Texture::Ptr_t{});
  if (pTexture == nullptr)
  {
    pTexture = ::std::make_shared<Texture>(*pTextureData);

    if (pTextureData->IsUsingMipmapping)
    {
      GetTexMinFilter = [](void) { return GL_LINEAR_MIPMAP_LINEAR; };

      pTexture->Bind();
      glGenerateMipmap(GL_TEXTURE_2D);
      pTexture->Bind(false);
    }
  }
  else
  {
    (*pTextureDataComponent)[uT("entity")] = Texture::Ptr_t{};
  }

  const Render_t TextureRender = [=](void)
  {
    const auto Index = pTexture->m_Destination.first;

    glActiveTexture(GL_TEXTURE0 + Index);
    pTexture->Bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetTexMinFilter());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_TexParameters.MagFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_TexParameters.WrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_TexParameters.WrapT);

    const auto ProgramId = glGetProgramId();

    const auto LocationId = glGetUniformLocation(ProgramId,
      pTexture->m_Destination.second.c_str());

    // -1 означает, что в шейдере такой текстуры нет, но glGetError() при этом
    // не возвращает ошибку, поэтому просто игнорируем ситуацию.
    if (LocationId == -1) return;

    glUniform1i(LocationId, Index);
  };

  if (!(*pTextureDataComponent)[uT("mapper")].IsType<const BufferMapper_t &>())
  {
    return TextureRender;
  }

  const BufferMapper_t cbBufferMapper =
    (*pTextureDataComponent)[uT("mapper")].Default(BufferMapper_t{});
  const auto pFrameBuffer = ::std::make_shared<FrameBuffer>();

  return [=](void)
  {
    if (cbBufferMapper(nullptr))
    {
      pFrameBuffer->Bind();
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, pTexture->m_TextureId, 0);

      auto * pData = reinterpret_cast<uint32_t *>(pTexture->m_ReadCopyData.data());

      glReadPixels(0, 0, pTextureData->Width, pTextureData->Height, GL_RGBA,
        GL_UNSIGNED_BYTE, pData);

      // Изображение в текстуре OpenGL перевернуто по Y, поэтому... 
      for (int y = 0; y < pTextureData->Height / 2; y++)
      {
        auto * pLineUp = 
          pData + (static_cast<size_t>(y) * pTextureData->Width);
        auto * pLineDown = 
          pData + ((static_cast<size_t>(pTextureData->Height) - y - 1) * pTextureData->Width);

        for (int x = 0; x < pTextureData->Width; x++)
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

auto OpenGLCommonShader::CreateTextureArray(
  const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const auto pTextureArrayData = CapturingServiceComponent::Get(_pComponent,
    { { uT("TextureArray"), _pComponent } })[0];

  const Component::Texture TextureData(*pTextureArrayData, uT("diffuse"));
  auto pTexture = ::std::make_shared<Texture>(TextureData, true);

  ::std::function<GLint(void)> GetTexMinFilter =
    [=](void) { return m_TexParameters.MinFilter; } ;
  if (TextureData.IsUsingMipmapping) GetTexMinFilter =
    [](void) { return GL_LINEAR_MIPMAP_LINEAR; };

  return [=](void)
  {
    const auto Index = pTexture->m_Destination.first;

    glActiveTexture(GL_TEXTURE0 + Index);
    pTexture->Bind();

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GetTexMinFilter());
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, m_TexParameters.MagFilter);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, m_TexParameters.WrapS);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, m_TexParameters.WrapT);

    const auto ProgramId = glGetProgramId();

    const auto LocationId = glGetUniformLocation(ProgramId,
      pTexture->m_Destination.second.c_str());

    // -1 означает, что в шейдере такой текстуры нет, но glGetError() при этом
    // не возвращает ошибку, поэтому просто игнорируем ситуацию.
    if (LocationId == -1) return;

    glUniform1i(LocationId, Index);
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
      if (Result == GL_TRUE)
      {
        Use = [=](void) { glUseProgram(Id); };
      }
      else
      {
        Use = [](void) {};

        char InfoLog[512] = { 0 };
        glGetProgramInfoLog(Id, sizeof(InfoLog), NULL, InfoLog);

        LOGGER(Warning) << "Link program fail: " << InfoLog;
      }
    }

    ::std::function<void(void)> Use;

  private:
    const GLuint Id;

  public:
    Program(void) noexcept :
      Id{ glCreateProgram() }
    {
    }
    Program(const Program &) = delete;
    Program(Program &&) = delete;
    Program & operator= (const Program &) = delete;
    Program & operator= (Program &&) = delete;
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

        ::std::vector<GLchar> InfoLog(512, 0x00);
        glGetShaderInfoLog(Id, InfoLog.size(), nullptr, InfoLog.data());

        throw STD_EXCEPTION << "Compile shader fail "
          << "[header line: " << GetHeaderLines(m_Header) << "]: "
          << InfoLog.data();
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
    const GLuint Id;
    const ::std::string m_Header;

  public:
    Shader(Programs & _Programs, const GLenum _Type, 
      const GLchar * _Header, const GLchar * _Body) :
      m_Programs{ _Programs },
      Type{ _Type },
      Id{ glCreateShader(_Type) },
      m_Header(_Header)
    {
      const auto FullShaderBody = ::std::string{ _Header } +_Body;
      auto pFullShaderBody = FullShaderBody.c_str();
      glShaderSource(Id, 1, &pFullShaderBody, nullptr);
    }
    Shader(const Shader &) = delete;
    Shader(Shader &&) = delete;
    Shader & operator= (const Shader &) = delete;
    Shader & operator= (Shader &&) = delete;
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

    const auto BodyShaderText = _ShaderData.Data;

    return ::std::make_shared<Shader>(*this, GL_VERTEX_SHADER,
      (m_ShaderHeader +
      "#define COVELLITE_SHADER_VERTEX\r\n" +
      ::std::string{ ::std::begin(HeaderShaderText), ::std::end(HeaderShaderText) }).c_str(),
      (::std::string{ ::std::begin(BodyShaderText), ::std::end(BodyShaderText) } +
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
      _ShaderData.Data.cbegin(), _ShaderData.Data.cend() };

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
      ::std::string{ ::std::begin(HeaderShaderText), ::std::end(HeaderShaderText) }).c_str(),
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

    pProgram->Use();
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
  const uint8_t Align[4] = { 0 };
  Programs_t m_Programs;

public:
  explicit Programs(const ::std::string & _ShaderHeader) :
    m_ShaderHeader(_ShaderHeader + "#define COVELLITE_SHADER_GLSL\r\n")
  {

  }
  Programs(const Programs &) = delete;
  Programs(Programs &&) = delete;
  Programs & operator= (const Programs &) = delete;
  Programs & operator= (Programs &&) = delete;
  ~Programs(void) = default;
};

auto OpenGLCommonShader::CreateShader(const ComponentPtr_t & _pComponent) -> Render_t /*override*/
{
  const auto pShaderDataComponent = CapturingServiceComponent::Get(_pComponent, 
    { { uT("Shader"), _pComponent } })[0];

  const Component::Shader ShaderData{ *pShaderDataComponent, ::Default };

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

#     undef TEST_CALL_IF
    };
  }

  return nullptr;
}

auto OpenGLCommonShader::CreateBuffer(const ComponentPtr_t & _pBuffer) -> Render_t /*override*/
{
  const auto pBufferData = CapturingServiceComponent::Get(_pBuffer, 
    { { uT("Buffer"), _pBuffer } })[0];

  auto CreateConstantUserBuffer = [&](void) -> Render_t
  {
    using Type_t = cbBufferMap_t<void>;

    if (!(*_pBuffer)[uT("mapper")].IsType<const Type_t &>())
    {
      throw STD_EXCEPTION << "Unexpected buffer format [" <<
        "id: " << _pBuffer->Id << ", " <<
        "type: " << _pBuffer->Type << ", " <<
        "kind: " << _pBuffer->Kind << "].";
    }

    const Type_t cbBufferMapper =
      (*_pBuffer)[uT("mapper")].Default(Type_t{});
    if (!cbBufferMapper)
    {
      throw STD_EXCEPTION << "Unexpected empty mapper: " << _pBuffer->Id;
    }

    const ::std::size_t BufferSize =
      (*_pBuffer)[uT("size")].Default((::std::size_t)0);
    if (BufferSize == 0)
    {
      throw STD_EXCEPTION << "Unexpected zero size: " << _pBuffer->Id;
    }

    const ::std::string cbName =
      (*_pBuffer)[uT("name")].Default(::std::string{ "cbUserData" });

    const auto pData =
      ::std::make_shared<BinaryData_t>(BufferSize, (uint8_t)0x00);
    const auto pBuffer = 
      ::std::make_shared<Buffer>(pData->data(), pData->size());

    return [=](void)
    {
      cbBufferMapper(pData->data());
      pBuffer->UpdateData(cbName, pData->data(), pData->size());
    };
  };

  const auto CreateVertexBuffer = [&](void) -> Render_t
  {
    using Type_t = ::covellite::api::Vertex;
    using BufferMapper_t = cbBufferMap_t<Type_t>;

    if (!(*pBufferData)[uT("content")].IsType<Buffer_t<Type_t>>())
    {
      return CreateConstantUserBuffer();
    }

    const Component::Buffer<Type_t> Info{ *pBufferData };

    const BufferMapper_t & cbBufferMapper =
      (*_pBuffer)[uT("mapper")].Default(BufferMapper_t{});

    if (cbBufferMapper == nullptr)
    {
      const auto pBuffer = ::std::make_shared<Buffer>(GL_ARRAY_BUFFER,
        Info.Data.data(), Info.Data.size() * sizeof(Type_t), GL_STATIC_DRAW);

      return [=](void)
      {
        pBuffer->Bind();
        pBuffer->SetVertexInputData();
        pBuffer->Bind(false);
      };
    }

    const auto pBuffer = ::std::make_shared<Buffer>(GL_ARRAY_BUFFER,
      Info.Data.data(), Info.Data.size() * sizeof(Type_t), GL_DYNAMIC_DRAW);

    const auto pData = ::std::make_shared<Buffer_t<Type_t>>(Info.Data);

    return [=](void)
    {
      pBuffer->Bind();

      const auto IsDirty = cbBufferMapper(nullptr);
      if (IsDirty)
      {
        cbBufferMapper(pData->data());

        pBuffer->UpdateData(pData->data(),
          static_cast<size_t>(pData->size() * sizeof(Type_t)));
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
    const auto TransformRender = GetPreRenderGeometry(_pComponent);

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
    GetPreRenderGeometry(_pComponent)(World);
    World = ::glm::transpose(World);

    return [=](void)
    {
      m_pConstants->Get<::Object>().World = World;
    };
  };

  const auto GetPreRenderBillboardGeometry = [&](void) -> TransformRender_t
  {
    const auto TransformRender = 
      OpenGLCommonShader::GetPreRenderBillboardGeometry(_pComponent);

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

  // cppcheck-suppress internalAstError
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
  using BufferMapperMaxCount_t = ::std::function<bool(void *)>;
  using BufferMapperChangeCount_t = ::std::function<bool(void *, ::std::size_t &)>;

  ComponentPtr_t pIndexBufferData = _pComponent;
  ComponentPtr_t pInstanceBufferData = nullptr;

  const auto SaveBuffer = [&](const ComponentPtr_t & _pBufferData)
  {
    if ((*_pBufferData)[uT("content")].IsType<Buffer_t<int>>())
    {
      pIndexBufferData = _pBufferData;
    }
    else if ((*_pBufferData)[uT("mapper")].IsType<const BufferMapperMaxCount_t &>())
    {
      pInstanceBufferData = _pBufferData;
    }
    else if ((*_pBufferData)[uT("mapper")].IsType<const BufferMapperChangeCount_t &>())
    {
      pInstanceBufferData = _pBufferData;
    }
    else
    {
      throw STD_EXCEPTION << "Unexpected Presend data component: id = " <<
        _pComponent->Id;
    }
  };

  CapturingServiceComponent::Process(_pComponent, 
    {
      { uT("Buffer"), SaveBuffer },
    });

  const Component::Buffer<int> IndexBufferData{ *pIndexBufferData };

  const auto pIndexBuffer = ::std::make_shared<Buffer>(GL_ELEMENT_ARRAY_BUFFER,
    IndexBufferData.Data.data(), IndexBufferData.Data.size() * sizeof(int), GL_STATIC_DRAW);
  const auto IndexCount = static_cast<GLsizei>(IndexBufferData.Data.size());

  if (pInstanceBufferData == nullptr)
  {
    return [=](void)
    {
      pIndexBuffer->Bind();
      glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr);
      pIndexBuffer->Bind(false);
    };
  }

  const ::std::size_t BufferSize = (*pInstanceBufferData)[uT("size")];
  if (BufferSize % 16 != 0)
  {
    throw STD_EXCEPTION << _pComponent->Id << ": size % 16 != 0";
  }

  const ::std::size_t MaxInstanceCount = (*pInstanceBufferData)[uT("count")];
  if (BufferSize % MaxInstanceCount != 0)
  {
    throw STD_EXCEPTION << _pComponent->Id << ": size % count != 0";
  }

  const auto Stride = static_cast<GLsizei>(BufferSize / MaxInstanceCount);

  const auto pData =
    ::std::make_shared<BinaryData_t>(BufferSize, (uint8_t)0x00);
  const auto pInstanceBuffer = ::std::make_shared<Buffer>(GL_ARRAY_BUFFER,
    nullptr, BufferSize, GL_DYNAMIC_DRAW);

  if ((*pInstanceBufferData)[uT("mapper")].IsType<BufferMapperMaxCount_t>())
  {
    const BufferMapperMaxCount_t cbBufferMapper =
      (*pInstanceBufferData)[uT("mapper")];

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
        static_cast<GLsizei>(MaxInstanceCount));
      pIndexBuffer->Bind(false);
    };
  }

  const BufferMapperChangeCount_t cbBufferMapper =
    (*pInstanceBufferData)[uT("mapper")];

  return [=](void)
  {
    auto InstanceCount = MaxInstanceCount;

    const auto IsDirty = cbBufferMapper(nullptr, InstanceCount);
    if (IsDirty)
    {
      cbBufferMapper(pData->data(), InstanceCount);
      pInstanceBuffer->SetInstanceData(pData->data(), pData->size(), Stride);
    }

    InstanceCount = ::std::min(InstanceCount, MaxInstanceCount);

    pIndexBuffer->Bind();
    glDrawElementsInstanced(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr,
      static_cast<GLsizei>(InstanceCount));
    pIndexBuffer->Bind(false);
  };
}

auto OpenGLCommonShader::GetCameraCommon(const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto DisableDepthRender = GetDepthRender(false, false, false);

  using Size_t = ::std::tuple<GLint, GLint>;
  using fnBkSurfaceSize_t = ::std::function<Size_t(void)>;

  const auto pCamera = _pComponent;

  const fnBkSurfaceSize_t GetScaleBkSurfaceSize = [=](void)
  {
    const float Scale = (*pCamera)[uT("scale")];

    const auto Width = static_cast<GLint>(Scale * m_Width);
    const auto Height = static_cast<GLint>(Scale * (m_Height - m_Top));

    return Size_t{ Width, Height };
  };

  const fnBkSurfaceSize_t GetWindowBkSurfaceSize = [=](void)
  {
    return Size_t{ m_Width, m_Height - m_Top };
  };

  const fnBkSurfaceSize_t GetUserBkSurfaceSize = [=](void)
  {
    const int Width = (*pCamera)[uT("width")];
    const int Height = (*pCamera)[uT("height")];

    return Size_t{ static_cast<GLint>(Width), static_cast<GLint>(Height) };
  };

  const auto IsScaleBkSurfaceSize =
    (*pCamera)[uT("scale")].IsType<float>();
  const auto IsUserBkSurfaceSize =
    (*pCamera)[uT("width")].IsType<int>() &&
    (*pCamera)[uT("height")].IsType<int>();

  const auto GetBkSurfaceSize =
    (IsScaleBkSurfaceSize) ? GetScaleBkSurfaceSize :
    (IsUserBkSurfaceSize) ? GetUserBkSurfaceSize :
    GetWindowBkSurfaceSize;

  const auto [Width, Height] = GetBkSurfaceSize();

  // Функция должна обязательно вызываться не только при рендеринге, но и при
  // создании камеры, чтобы внеэкранная поверхность создавалась с корректными
  // размерами.
  glViewport(0, 0, Width, Height); // (0, 0) - левый нижний угол!

  return [=](void)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    DisableDepthRender();

    glDisable(GL_BLEND);
    glDisable(GL_DITHER);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    const auto [Width, Height] = GetBkSurfaceSize();

    glViewport(0, 0, Width, Height); // (0, 0) - левый нижний угол!
  };
}

auto OpenGLCommonShader::GetCameraOrthographic(
  const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto CommonRender = GetCameraCommon(_pComponent);

  const auto ServiceComponents = CapturingServiceComponent::Get(_pComponent,
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
    const Component::Position Offset{ *ServiceComponents[0] };

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

    (*_pComponent)[uT("view")] = Matrix.View;
    (*_pComponent)[uT("projection")] = Matrix.Projection;
  };
}

auto OpenGLCommonShader::GetCameraPerspective(
  const ComponentPtr_t & _pComponent) -> Render_t
{
  const auto CommonRender = GetCameraCommon(_pComponent);

  const auto ServiceComponents = CapturingServiceComponent::Get(_pComponent,
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

    const auto AngleY = (float)(*_pComponent)[uT("fov")].Default(90.0f) *
      ::alicorn::extension::cpp::math::Constant<float>::DegreeToRadian;
    const float zNear = (*_pComponent)[uT("znear")].Default(0.01f);
    const float zFar = (*_pComponent)[uT("zfar")].Default(200.0f);

    Matrix.Projection = ::glm::transpose(::glm::perspectiveFovRH(AngleY,
      Viewport[2], Viewport[3], zFar, zNear));

    // **************************** Матрица вида **************************** //

    // Точка, куда смотрит камера - задается как компонент Data.Position.
    const Component::Position Look{ *ServiceComponents[0] };

    const auto GetEye = [&](void) -> ::glm::vec3
    {
      // Расстояние от камеры до Look.
      const float Distance = (*_pComponent)[uT("distance")].Default(0.0f);

      // Точка, где расположена камера - вычисляется на основе Look, Distance и
      // компонента Data.Rotation.

      const Component::Position Rot{ *ServiceComponents[1] };

      ::glm::mat4 Transform = ::glm::identity<::glm::mat4>();

      Transform = ::glm::translate(Transform,
        ::glm::vec3{ Look.X, Look.Y, Look.Z });
      Transform = ::glm::rotate(Transform,
        Rot.Z, ::glm::vec3{ 0.0f, 0.0f, 1.0f });
      Transform = ::glm::rotate(Transform,
        Rot.Y, ::glm::vec3{ 0.0f, 1.0f, 0.0f });
      Transform = ::glm::rotate(Transform,
        Rot.X, ::glm::vec3{ 1.0f, 0.0f, 0.0f });

      return Transform * ::glm::vec4{ Distance + 0.1f, 0.0f, 0.0f, 1.0f };
    };

    const auto View = ::glm::lookAtRH(
      GetEye(),
      ::glm::vec3{ Look.X, Look.Y, Look.Z },
      ::glm::vec3{ 0.0f, 0.0f, 1.0f }); // Up

    Matrix.View = ::glm::transpose(View);
    Matrix.ViewInverse = ::glm::transpose(::glm::inverse(View));

    (*_pComponent)[uT("view")] = Matrix.View;
    (*_pComponent)[uT("projection")] = Matrix.Projection;
  };
}

} // namespace renderer

} // namespace api

} // namespace covellite
