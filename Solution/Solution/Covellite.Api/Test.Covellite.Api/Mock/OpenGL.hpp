
#pragma once
#include <glm/glm.force.hpp>
#include <alicorn/std/vector.hpp>

#define GL_NONE                           0
#define GL_TRUE                           1
#define GL_FALSE                          0
#define GL_INVALID_INDEX                  0xFFFFFFFFu
#define GL_FLOAT_VEC2                     0x8B50
#define GL_FLOAT_VEC3                     0x8B51
#define GL_FLOAT_VEC4                     0x8B52
#define GL_INT_VEC2                       0x8B53
#define GL_INT_VEC3                       0x8B54
#define GL_INT_VEC4                       0x8B55
#define GL_INT                            0x1404

#define GL_VENDOR                         0x1F00
#define GL_RENDERER                       0x1F01
#define GL_VERSION                        0x1F02
#define GL_EXTENSIONS                     0x1F03
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_NUM_EXTENSIONS                 0x821D

#define GL_BLEND                          0x0BE2
#define GL_COLOR_BUFFER_BIT               0x00004000
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_MODELVIEW                      0x1700
#define GL_PROJECTION                     0x1701
#define GL_DEPTH_TEST                     0x0B71
#define GL_DITHER                         0x0BD0
#define GL_LIGHTING                       0x0B50
#define GL_PERSPECTIVE_CORRECTION_HINT    0x0C50
#define GL_FASTEST                        0x1101
#define GL_SMOOTH                         0x1D01
#define GL_CCW                            0x0901
#define GL_CULL_FACE                      0x0B44
#define GL_BACK                           0x0405
#define GL_UNSIGNED_INT                   0x1405
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_FLOAT                          0x1406
#define GL_VERTEX_ARRAY                   0x8074
#define GL_NORMAL_ARRAY                   0x8075
#define GL_COLOR_ARRAY                    0x8076
#define GL_TEXTURE_COORD_ARRAY            0x8078
#define GL_TEXTURE_2D                     0x0DE1
#define GL_TEXTURE_2D_ARRAY               0x8C1A
#define GL_TRIANGLES                      0x0004
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_LINEAR                         0x2601
#define GL_LINEAR_MIPMAP_LINEAR           0x2703
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_REPEAT                         0x2901
#define GL_RGBA                           0x1908
#define GL_RGBA8                          0x8058
#define GL_SCISSOR_TEST                   0x0C11
#define GL_VIEWPORT                       0x0BA2
#define GL_MODELVIEW_MATRIX               0x0BA6
#define GL_NO_ERROR                       0
#define GL_VERSION                        0x1F02
#define GL_FRONT_AND_BACK                 0x0408
#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_NORMALIZE                      0x0BA1
#define GL_ALPHA_TEST                     0x0BC0
#define GL_FOG                            0x0B60
#define GL_FOG_HINT                       0x0C54
#define GL_FOG_DENSITY                    0x0B62
#define GL_FOG_START                      0x0B63
#define GL_FOG_END                        0x0B64
#define GL_FOG_MODE                       0x0B65
#define GL_FOG_COLOR                      0x0B66
#define GL_EXP                            0x0800
#define GL_EXP2                           0x0801
#define GL_NICEST                         0x1102

#define GL_LIGHT0                         0x4000
#define GL_AMBIENT                        0x1200
#define GL_DIFFUSE                        0x1201
#define GL_SPECULAR                       0x1202
#define GL_POSITION                       0x1203
#define GL_CONSTANT_ATTENUATION           0x1207
#define GL_LINEAR_ATTENUATION             0x1208
#define GL_QUADRATIC_ATTENUATION          0x1209
#define GL_EMISSION                       0x1600
#define GL_SHININESS                      0x1601
#define GL_LIGHT_MODEL_AMBIENT            0x0B53

#define GL_GREATER                        0x0204

#define GL_TEXTURE0                       0x84C0

#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_CURRENT_PROGRAM                0x8B8D
#define GL_INFO_LOG_LENGTH                0x8B84

#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_ARRAY_BUFFER_BINDING           0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING   0x8895
#define GL_UNIFORM_BUFFER                 0x8A11
#define GL_ACTIVE_ATTRIBUTES              0x8B89

#define GL_STREAM_DRAW                    0x88E0
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8

#define GL_FRAMEBUFFER                    0x8D40
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FRAMEBUFFER_BINDING            0x8CA6
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_STENCIL_ATTACHMENT             0x8D20
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#define GL_DEPTH_STENCIL                  0x84F9
#define GL_UNSIGNED_INT_24_8              0x84FA
#define GL_DEPTH24_STENCIL8               0x88F0

#define GL_RGBA32F                        0x8814
#define GL_RGBA16F                        0x881A
#define GL_HALF_FLOAT                     0x140B

namespace mock
{

using GLenum = unsigned int;
using GLclampf = float;
using GLbitfield = unsigned int;
using GLint = int;
using GLsizei = int;
using GLfloat = float;
using GLdouble = double;
using GLvoid = void;
using GLuint = size_t;
using GLfixed = int32_t;
using GLubyte = unsigned char;
using GLboolean = unsigned char;
using GLchar = char;
typedef double GLclampd;
typedef signed   long  int GLsizeiptr;
typedef signed   long  int GLintptr;

class GLProxy :
  public ::alicorn::extension::testing::Proxy<GLProxy>
{
public:
  using Uints_t = ::std::vector<uint32_t>;
  using Ints_t = ::std::vector<int>;
  using Floats_t = ::std::vector<GLfloat>;

public:
  MOCK_METHOD0(GetError, GLenum (void));
  MOCK_METHOD1(DepthMask, void(GLboolean));
  MOCK_METHOD1(Enable, void(GLenum));
  MOCK_METHOD1(IsEnabled, GLboolean(GLenum));
  MOCK_METHOD1(Disable, void(GLenum));
  MOCK_METHOD2(Hint, void(GLenum, GLenum));
  MOCK_METHOD1(ShadeModel, void(GLenum));
  MOCK_METHOD1(FrontFace, void(GLenum));
  MOCK_METHOD1(CullFace, void(GLenum));
  MOCK_METHOD0(PushMatrix, void (void));
  MOCK_METHOD0(PopMatrix, void (void));
  MOCK_METHOD1(EnableClientState, void (GLenum));
  MOCK_METHOD1(DisableClientState, void (GLenum));
  MOCK_METHOD2(BindTexture, void (GLenum, GLuint));
  MOCK_METHOD3(Translatef, void (GLfloat, GLfloat, GLfloat));
  MOCK_METHOD4(Rotatef, void(GLfloat, GLfloat, GLfloat, GLfloat));
  MOCK_METHOD3(Scalef, void(GLfloat, GLfloat, GLfloat));
  MOCK_METHOD4(VertexPointer, void (GLint, GLenum, GLsizei, Floats_t));
  MOCK_METHOD3(NormalPointer, void(GLenum, GLsizei, Floats_t));
  MOCK_METHOD4(ColorPointer, void (GLint, GLenum, GLsizei, Uints_t));
  MOCK_METHOD4(ColorPointer, void(GLint, GLenum, GLsizei, Floats_t));
  MOCK_METHOD4(TexCoordPointer, void (GLint, GLenum, GLsizei, Floats_t));
  MOCK_METHOD4(DrawElements, void (GLenum, GLsizei, GLenum, Ints_t));
  MOCK_METHOD4(DrawElements, void(GLenum, GLsizei, GLenum, const void *));
  MOCK_METHOD1(GenTextures, GLuint(GLsizei));
  MOCK_METHOD3(TexParameteri, void (GLenum, GLenum, GLfixed));
  MOCK_METHOD9(TexImage2D, void (GLenum, GLint, GLint, GLsizei, GLsizei, GLint, 
    GLenum, GLenum, ::std::vector<uint8_t>));
  MOCK_METHOD6(TexStorage3D, void(GLenum, GLsizei, GLenum, GLsizei, GLsizei, GLsizei));
  MOCK_METHOD7(TexSubImage3D_1, void(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei));
  MOCK_METHOD4(TexSubImage3D_2, void(GLsizei, GLenum, GLenum, ::std::vector<uint8_t>));
  MOCK_METHOD2(DeleteTextures, void(GLsizei, GLuint));
  MOCK_METHOD2(BlendFunc, void(GLenum, GLenum));
  MOCK_METHOD4(Viewport, void (GLint, GLint, GLsizei, GLsizei));
  MOCK_METHOD1(MatrixMode, void (GLenum));
  MOCK_METHOD0(LoadIdentity, void (void));
  MOCK_METHOD6(Ortho, void(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat));
  MOCK_METHOD4(Scissor, void (GLint, GLint, GLsizei, GLsizei));
  MOCK_METHOD4(ClearColor, void(GLclampf, GLclampf, GLclampf, GLclampf));
  MOCK_METHOD1(Clear, void(GLbitfield));
  MOCK_METHOD1(GetString, const GLubyte * (GLenum));
  MOCK_METHOD2(GetStringi, const GLubyte * (GLenum, GLuint));
  MOCK_METHOD1(GetIntegerv, const GLint * (GLenum));
  MOCK_METHOD1(GetFloatv, const GLfloat * (GLenum));
  MOCK_METHOD3(Materialfv, void (GLenum, GLenum, Floats_t));
  MOCK_METHOD3(Lightfv, void(GLenum, GLenum, Floats_t));
  MOCK_METHOD3(Lightf, void(GLenum, GLenum, GLfloat));
  MOCK_METHOD2(LightModelfv, void(GLenum, Floats_t));
  MOCK_METHOD1(LoadMatrixf, void(::glm::mat4));
  MOCK_METHOD2(AlphaFunc, void (GLenum, GLclampf));
  MOCK_METHOD2(Fogf, void(GLenum, GLfloat));
  MOCK_METHOD2(Fogfv, void(GLenum, Floats_t));
  MOCK_METHOD2(Fogi, void(GLenum, GLint));
  MOCK_METHOD1(ActiveTexture, void (GLenum));
  MOCK_METHOD2(GetUniformLocation, GLint(GLuint, ::std::string));
  MOCK_METHOD1(GenBuffers, GLuint(GLsizei));
  MOCK_METHOD2(DeleteBuffers, void(GLsizei, GLuint));
  MOCK_METHOD2(BindBuffer, void(GLenum, GLuint));
  MOCK_METHOD4(BufferData, void(GLenum, GLsizeiptr, ::std::vector<uint8_t>, GLenum));
  MOCK_METHOD2(GetAttribLocation, GLint(GLuint, ::std::string));
  MOCK_METHOD6(VertexAttribPointer, void(GLuint, GLint, GLenum, GLboolean, 
    GLsizei, const GLvoid *));
  MOCK_METHOD1(EnableVertexAttribArray, void(GLuint));
  MOCK_METHOD4(BufferSubData, void(GLenum, GLintptr, GLsizeiptr, Floats_t));
  MOCK_METHOD3(UniformBufferSubData, void(GLintptr, GLsizeiptr, const void *));
  MOCK_METHOD4(BufferSubDataRaw, void(GLenum, GLintptr, GLsizeiptr, const void *));
  MOCK_METHOD1(CreateShader, GLuint(GLenum));
  MOCK_METHOD1(DeleteShader, void(GLuint));
  MOCK_METHOD4(ShaderSource, void(GLuint, GLsizei, ::std::string, const GLint *));
  MOCK_METHOD1(CompileShader, void(GLuint));
  MOCK_METHOD2(GetShaderiv, GLint(GLuint, GLenum));
  MOCK_METHOD2(GetProgramiv, GLint(GLuint, GLenum));
  MOCK_METHOD3(GetShaderInfoLog, const GLchar *(GLuint, GLsizei, GLsizei *));
  MOCK_METHOD3(GetProgramInfoLog, const GLchar *(GLuint, GLsizei, GLsizei *));
  MOCK_METHOD0(CreateProgram, GLuint(void));
  MOCK_METHOD2(AttachShader, void(GLuint, GLuint));
  MOCK_METHOD1(LinkProgram, void(GLuint));
  MOCK_METHOD1(UseProgram, void(GLuint));
  MOCK_METHOD1(DeleteProgram, void(GLuint));

  MOCK_METHOD2(Uniform1i, void(GLuint, GLint));
  MOCK_METHOD2(Uniform1f, void(GLuint, GLfloat));
  MOCK_METHOD3(Uniform4fv, void (GLint, GLsizei, ::glm::vec4));
  MOCK_METHOD4(UniformMatrix4fv, void(GLint, GLsizei, GLboolean, ::glm::mat4));
  MOCK_METHOD2(GetUniformBlockIndex, GLuint(GLuint, ::std::string));
  MOCK_METHOD3(BindBufferBase, void(GLenum, GLuint, GLuint));
  MOCK_METHOD3(UniformBlockBinding, void(GLuint, GLuint, GLuint));
  MOCK_METHOD5(DrawElementsInstanced, void(GLenum, GLsizei, GLenum,
    const void *, GLsizei));
  MOCK_METHOD2(VertexAttribDivisor, void(GLuint, GLuint));
  MOCK_METHOD2(GetActiveAttribType, GLenum(GLuint, GLuint));
  MOCK_METHOD3(GetActiveAttribName, ::std::string(GLuint, GLuint, GLsizei));
  MOCK_METHOD1(ClearDepth, void(GLclampd));
  MOCK_METHOD1(DepthFunc, void(GLenum));
  MOCK_METHOD1(GenFramebuffers, GLuint(GLsizei));
  MOCK_METHOD2(DeleteFramebuffers, void(GLsizei, GLuint));
  MOCK_METHOD2(BindFramebuffer, void(GLenum, GLuint));
  MOCK_METHOD5(FramebufferTexture2D, void(GLenum, GLenum, GLenum, GLuint, GLint));
  MOCK_METHOD1(CheckFramebufferStatus, GLenum(GLenum));
  MOCK_METHOD1(DrawBuffers, void(::std::vector<GLenum>));
  MOCK_METHOD1(GenerateMipmap, void(GLenum));
  MOCK_METHOD0(GetReadPixelsRawData, ::std::vector<uint32_t>(void));
  MOCK_METHOD7(ReadPixels, void(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, 
    GLvoid *));

public:
  template<class T>
  static ::std::vector<T> GetData(const GLint _Count, const GLsizei _Stride,
    const GLvoid * _pRawData)
  {
    ::std::vector<T> Result;

    auto * pRawData = reinterpret_cast<const GLubyte *>(_pRawData);

    while (true)
    {
      auto * pData = reinterpret_cast<const T *>(pRawData);
      //if (*pData == (T)0) break;

      auto IsEnd = [=](void)
      {
        for (GLint i = 0; i < _Count; ++i)
        {
          if (pData[i] != static_cast<T>(0)) return false;
        }

        return true;
      };

      if (IsEnd()) break;

      for (GLint i = 0; i < _Count; ++i)
      {
        Result.push_back(pData[i]);
      }

      pRawData += _Stride;
    }

    return Result;
  }
};

namespace
{

void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, 
  GLenum format, GLenum type, GLvoid * data)
{
  const auto RawData = GLProxy::GetInstance()->GetReadPixelsRawData();
  memcpy(data, RawData.data(), RawData.size() * sizeof(RawData[0]));

  GLProxy::GetInstance()->ReadPixels(x, y, width, height, format, type, data);
}

void glGenerateMipmap(GLenum target)
{
  GLProxy::GetInstance()->GenerateMipmap(target);
}

void glGenFramebuffers(GLsizei n, GLuint * ids)
{
  *ids = GLProxy::GetInstance()->GenFramebuffers(n);
}

void glDeleteFramebuffers(GLsizei n, const GLuint * framebuffers)
{
  GLProxy::GetInstance()->DeleteFramebuffers(n, *framebuffers);
}

void glBindFramebuffer(GLenum target, GLuint framebuffer)
{
  GLProxy::GetInstance()->BindFramebuffer(target, framebuffer);
}

void glFramebufferTexture2D(GLenum target, GLenum attachment,
  GLenum textarget, GLuint texture, GLint level)
{
  GLProxy::GetInstance()->FramebufferTexture2D(target, attachment,
    textarget, texture, level);
}

GLenum glCheckFramebufferStatus(GLenum target)
{
  return GLProxy::GetInstance()->CheckFramebufferStatus(target);
}

void glDrawBuffers(GLsizei n, const GLenum * bufs)
{
  GLProxy::GetInstance()->DrawBuffers(::std::vector<GLenum>{ bufs, bufs + n });
}

void glClearDepth(GLclampd depth)
{
  GLProxy::GetInstance()->ClearDepth(depth);
}

void glDepthFunc(GLenum func)
{
  GLProxy::GetInstance()->DepthFunc(func);
}

void glGetActiveAttrib(
  GLuint program,
  GLuint index,
  GLsizei bufSize,
  GLsizei * length,
  GLint * /*size*/,
  GLenum * type,
  GLchar * name)
{
  *type = 
    GLProxy::GetInstance()->GetActiveAttribType(program, index);

  const auto Name = 
    GLProxy::GetInstance()->GetActiveAttribName(program, index, bufSize);
  *length = Name.length();
  strncpy(name, Name.c_str(), ::std::min(bufSize , *length + 1));
}

void glVertexAttribDivisor(GLuint index, GLuint divisor)
{
  GLProxy::GetInstance()->VertexAttribDivisor(index, divisor);
}

void glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type,
  const void * indices, GLsizei instancecount)
{
  GLProxy::GetInstance()->DrawElementsInstanced(mode, count, type, indices, 
    instancecount);
}

void glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex,
  GLuint uniformBlockBinding)
{
  GLProxy::GetInstance()->UniformBlockBinding(program, uniformBlockIndex,
    uniformBlockBinding);
}

void glBindBufferBase(GLenum target, GLuint index, GLuint buffer)
{
  GLProxy::GetInstance()->BindBufferBase(target, index, buffer);
}

GLuint glGetUniformBlockIndex(GLuint program, const GLchar *uniformBlockName)
{
  return GLProxy::GetInstance()->GetUniformBlockIndex(program, uniformBlockName);
}

void glUniform1i(GLint location, GLint v0)
{
  GLProxy::GetInstance()->Uniform1i(location, v0);
}

void glUniform1f(GLint location, GLfloat v0)
{
  GLProxy::GetInstance()->Uniform1f(location, v0);
}

void glUniform4fv(GLint location, GLsizei count, const GLfloat * value)
{
  GLProxy::GetInstance()->Uniform4fv(location, count, ::glm::make_vec4(value));
}

void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose,
  const GLfloat * value)
{
  GLProxy::GetInstance()->UniformMatrix4fv(location, count, transpose,
    ::glm::make_mat4(value));
}

GLuint glCreateProgram(void)
{
  return GLProxy::GetInstance()->CreateProgram();
}

void glAttachShader(GLuint program, GLuint shader)
{
  GLProxy::GetInstance()->AttachShader(program, shader);
}

void glLinkProgram(GLuint program)
{
  GLProxy::GetInstance()->LinkProgram(program);
}

void glUseProgram(GLuint program)
{
  GLProxy::GetInstance()->UseProgram(program);
}

void glDeleteProgram(GLuint program)
{
  GLProxy::GetInstance()->DeleteProgram(program);
}

GLuint glCreateShader(GLenum shaderType)
{
  return GLProxy::GetInstance()->CreateShader(shaderType);
}

void glDeleteShader(GLuint shader)
{
  GLProxy::GetInstance()->DeleteShader(shader);
}

void glShaderSource(GLuint shader, GLsizei count, const GLchar **string,
  const GLint *length)
{
  GLProxy::GetInstance()->ShaderSource(shader, count, string[0], length);
}

void glCompileShader(GLuint shader)
{
  GLProxy::GetInstance()->CompileShader(shader);
}

void glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length,
  GLchar *infoLog)
{
  const auto * Log =
    GLProxy::GetInstance()->GetProgramInfoLog(program, maxLength, length);
  if (Log == nullptr) return;

  memcpy(infoLog, Log, ::std::min((::std::size_t)maxLength, strlen(Log) + 1));
}

void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length,
  GLchar *infoLog)
{
  const auto * Log =
    GLProxy::GetInstance()->GetShaderInfoLog(shader, maxLength, length);
  if (Log == nullptr) return;

  memcpy(infoLog, Log, ::std::min((::std::size_t)maxLength, strlen(Log) + 1));
}

void glGetProgramiv(GLuint program, GLenum pname, GLint *params)
{
  *params = GLProxy::GetInstance()->GetProgramiv(program, pname);
}

void glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
  *params = GLProxy::GetInstance()->GetShaderiv(shader, pname);
}

void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
  const GLvoid * data)
{
  if (target == GL_UNIFORM_BUFFER)
  {
    GLProxy::GetInstance()->UniformBufferSubData(offset, size, data);
  }
  else
  {
    GLProxy::GetInstance()->BufferSubDataRaw(target, offset, size, data);

    GLProxy::GetInstance()->BufferSubData(target, offset, size,
      GLProxy::GetData<GLfloat>(1, sizeof(GLfloat), data));
  }
}

GLint glGetAttribLocation(GLuint program, const GLchar * name)
{
  return GLProxy::GetInstance()->GetAttribLocation(program, name);
}

void glVertexAttribPointer(GLuint index, GLint size, GLenum type, 
  GLboolean normalized, GLsizei stride, const GLvoid * pointer)
{
  GLProxy::GetInstance()->VertexAttribPointer(
    index, size, type, normalized, stride, pointer);
}

void glEnableVertexAttribArray(GLuint index)
{
  GLProxy::GetInstance()->EnableVertexAttribArray(index);
}

void glBufferData(GLenum target, GLsizeiptr size, const GLvoid * data,
  GLenum usage)
{
  ::std::vector<uint8_t> Data;

  if (data != nullptr)
  {
    const auto * const pData = reinterpret_cast<const uint8_t *>(data);

    if (*pData != 0x00)
    {
      const auto Size = *reinterpret_cast<const size_t *>(data);
      Data = ::std::vector<uint8_t>{ pData, pData + Size };
    }
  }

  GLProxy::GetInstance()->BufferData(target, size, Data, usage);
}

void glBindBuffer(GLenum target, GLuint buffer)
{
  GLProxy::GetInstance()->BindBuffer(target, buffer);
}

void glGenBuffers(GLsizei n, GLuint * buffers)
{
  *buffers = GLProxy::GetInstance()->GenBuffers(n);
}

void glDeleteBuffers(GLsizei n, const GLuint * buffers)
{
  GLProxy::GetInstance()->DeleteBuffers(n, *buffers);
}

GLint glGetUniformLocation(GLuint program, const GLchar *name)
{
  return GLProxy::GetInstance()->GetUniformLocation(program, name);
}

void glActiveTexture(GLenum texture)
{
  GLProxy::GetInstance()->ActiveTexture(texture);
}

GLenum glGetError(void) 
{ 
  return GLProxy::GetInstance()->GetError();
}

void glDepthMask(GLboolean _Flag)
{
  GLProxy::GetInstance()->DepthMask(_Flag);
}

void glEnable(GLenum _Param)
{
  GLProxy::GetInstance()->Enable(_Param);
}

GLboolean glIsEnabled(GLenum _Param)
{
  return GLProxy::GetInstance()->IsEnabled(_Param);
}

void glDisable(GLenum _Param)
{
  GLProxy::GetInstance()->Disable(_Param);
}

void glHint(GLenum _Param1, GLenum _Param2)
{
  GLProxy::GetInstance()->Hint(_Param1, _Param2);
}

void glShadeModel(GLenum _Param)
{
  GLProxy::GetInstance()->ShadeModel(_Param);
}

void glFrontFace(GLenum _Param)
{
  GLProxy::GetInstance()->FrontFace(_Param);
}

void glCullFace(GLenum _Param)
{
  GLProxy::GetInstance()->CullFace(_Param);
}

void glPushMatrix(void) 
{
  GLProxy::GetInstance()->PushMatrix();
}

void glPopMatrix(void) 
{
  GLProxy::GetInstance()->PopMatrix();
}

void glEnableClientState(GLenum _Param)
{
  GLProxy::GetInstance()->EnableClientState(_Param);
}

void glDisableClientState(GLenum _Param)
{
  GLProxy::GetInstance()->DisableClientState(_Param);
}

void glBindTexture(GLenum _Param1, GLuint _Param2)
{
  GLProxy::GetInstance()->BindTexture(_Param1, _Param2);
}

void glTranslatef(GLfloat _Param1, GLfloat _Param2, GLfloat _Param3)
{
  GLProxy::GetInstance()->Translatef(_Param1, _Param2, _Param3);
}

void glRotatef(GLfloat _Param1, GLfloat _Param2, GLfloat _Param3, GLfloat _Param4)
{
  GLProxy::GetInstance()->Rotatef(_Param1, _Param2, _Param3, _Param4);
}

void glScalef(GLfloat _Param1, GLfloat _Param2, GLfloat _Param3)
{
  GLProxy::GetInstance()->Scalef(_Param1, _Param2, _Param3);
}

void glVertexPointer(GLint _Count, GLenum _Type, GLsizei _Stride, 
  const GLvoid * _pRawData)
{
  if (_Type == GL_FLOAT)
  {
    GLProxy::GetInstance()->VertexPointer(_Count, _Type, _Stride, 
      GLProxy::GetData<GLfloat>(_Count, _Stride, _pRawData));
  }
  else
  {
    throw ::std::exception{ "Unexpected type." };
  }
}

void glNormalPointer(GLenum _Type, GLsizei _Stride, const GLvoid * _pRawData)
{
  if (_Type == GL_FLOAT)
  {
    GLProxy::GetInstance()->NormalPointer(_Type, _Stride,
      GLProxy::GetData<GLfloat>(3, _Stride, _pRawData));
  }
  else
  {
    throw ::std::exception{ "Unexpected type." };
  }
}

void glColorPointer(GLint _Count, GLenum _Type, GLsizei _Stride,
  const GLvoid * _pRawData)
{
  if (_Type == GL_UNSIGNED_BYTE && _Count == 4)
  {
    GLProxy::GetInstance()->ColorPointer(_Count, _Type, _Stride,
      GLProxy::GetData<uint32_t>(1, _Stride, _pRawData));
  }
  else if (_Type == GL_FLOAT)
  {
    GLProxy::GetInstance()->ColorPointer(_Count, _Type, _Stride,
      GLProxy::GetData<float>(_Count, _Stride, _pRawData));
  }
  else
  {
    throw ::std::exception{ "Unexpected type." };
  }
}

void glTexCoordPointer(GLint _Count, GLenum _Type, GLsizei _Stride,
  const GLvoid * _pRawData)
{
  if (_Type == GL_FLOAT)
  {
    GLProxy::GetInstance()->TexCoordPointer(_Count, _Type, _Stride,
      GLProxy::GetData<GLfloat>(_Count, _Stride, _pRawData));
  }
  else
  {
    throw ::std::exception{ "Unexpected type." };
  }
}

void glDrawElements(GLenum _Mode, GLsizei _Count, GLenum _Type, 
  const GLvoid * _pRawData)
{
  if (_pRawData == nullptr)
  {
    GLProxy::GetInstance()->DrawElements(_Mode, _Count, _Type, _pRawData);
  }
  else if (_Type == GL_UNSIGNED_INT)
  {
    auto GetData = [&](void)
    {
      auto * const pData = reinterpret_cast<const int *>(_pRawData);

      GLProxy::Ints_t Result;
      for (GLsizei i = 0; i < _Count; ++i) Result.push_back(pData[i]);
      return Result;
    };

    GLProxy::GetInstance()->DrawElements(_Mode, _Count, _Type, GetData());
  }
  else
  {
    throw ::std::exception{ "Unexpected type." };
  }
}

void glGenTextures(GLsizei _Param1, GLuint * _Param2)
{
  *_Param2 = GLProxy::GetInstance()->GenTextures(_Param1);
}

void glTexParameteri(GLenum _Param1, GLenum _Param2, GLfixed _Param3)
{
  GLProxy::GetInstance()->TexParameteri(_Param1, _Param2, _Param3);
}

void glTexImage2D(GLenum _Param1, GLint _Param2, GLint _Param3, GLsizei _Param4, 
  GLsizei _Param5, GLint _Param6, GLenum _Param7, GLenum _Param8, 
  const GLvoid * _Param9)
{
  ::std::vector<uint8_t> Data;

  if (_Param9 != nullptr)
  {
    const auto * const pData = reinterpret_cast<const uint8_t *>(_Param9);
    const auto Size = *reinterpret_cast<const size_t *>(_Param9);
    Data = ::std::vector<uint8_t>{ pData, pData + Size };
  }

  GLProxy::GetInstance()->TexImage2D(_Param1, _Param2, _Param3, _Param4, 
    _Param5, _Param6, _Param7, _Param8, Data);
}

void glTexStorage3D(GLenum _Param1, GLsizei _Param2, GLenum _Param3,
  GLsizei _Param4, GLsizei _Param5, GLsizei _Param6)
{
  GLProxy::GetInstance()->TexStorage3D(
    _Param1, _Param2, _Param3, _Param4, _Param5, _Param6);
}

void glTexSubImage3D(GLenum _Param1, GLint _Param2, GLint _Param3, GLint _Param4,
  GLint _Param5, GLsizei _Param6, GLsizei _Param7, GLsizei _Param8, GLenum _Param9,
  GLenum _Param10, const void * _Param11)
{
  ::std::vector<uint8_t> Data;

  if (_Param11 != nullptr)
  {
    const auto * pData = reinterpret_cast<const uint8_t *>(_Param11);

    while (true)
    {
      using namespace ::alicorn::extension::std;

      const auto Size = *reinterpret_cast<const size_t *>(pData);
      const auto IsLastData = *reinterpret_cast<const bool *>(pData + sizeof(size_t));
      Data += ::std::vector<uint8_t>{ pData, pData + Size };

      pData += Size;
      if (IsLastData) break;
    }
  }

  GLProxy::GetInstance()->TexSubImage3D_1(
    _Param1, _Param2, _Param3, _Param4, _Param5, _Param6, _Param7);
  GLProxy::GetInstance()->TexSubImage3D_2(
    _Param8, _Param9, _Param10, Data);
}

void glDeleteTextures(GLsizei _Param1, const GLuint * _Param2)
{
  GLProxy::GetInstance()->DeleteTextures(_Param1, *_Param2);
}

void glBlendFunc(GLenum _Param1, GLenum _Param2)
{
  GLProxy::GetInstance()->BlendFunc(_Param1, _Param2);
}

void glViewport(GLint _Param1, GLint _Param2, GLsizei _Param3, GLsizei _Param4)
{
  GLProxy::GetInstance()->Viewport(_Param1, _Param2, _Param3, _Param4);
}

void glMatrixMode(GLenum _Param1)
{
  GLProxy::GetInstance()->MatrixMode(_Param1);
}

void glLoadIdentity(void) 
{
  GLProxy::GetInstance()->LoadIdentity();
}

void glOrtho(GLfloat _Param1, GLfloat _Param2, GLfloat _Param3,
  GLfloat _Param4, GLfloat _Param5, GLfloat _Param6)
{
  GLProxy::GetInstance()->Ortho(
    _Param1, _Param2, _Param3, _Param4, _Param5, _Param6);
}

void glScissor(GLint _X, GLint _Y, GLsizei _Width, GLsizei _Height)
{
  GLProxy::GetInstance()->Scissor(_X, _Y, _Width, _Height);
}

void glClearColor(GLclampf _R, GLclampf _G, GLclampf _B, GLclampf _A)
{
  GLProxy::GetInstance()->ClearColor(_R, _G, _B, _A);
}

void glClear(GLbitfield _Mask)
{
  GLProxy::GetInstance()->Clear(_Mask);
}

const GLubyte * glGetString(GLenum _Name)
{
  return GLProxy::GetInstance()->GetString(_Name);
}

const GLubyte * glGetStringi(GLenum name, GLuint index)
{
  return GLProxy::GetInstance()->GetStringi(name, index);
}

void glGetIntegerv(GLenum _Name, GLint * _pParams)
{
  auto * pParams = GLProxy::GetInstance()->GetIntegerv(_Name);
  if (pParams == nullptr) return;

  if (_Name == GL_VIEWPORT)
  {
    _pParams[0] = pParams[0];
    _pParams[1] = pParams[1];
    _pParams[2] = pParams[2];
    _pParams[3] = pParams[3];
  }
  else
  {
    *_pParams = *pParams;
  }
}

void glGetFloatv(GLenum _Name, GLfloat * _pParams)
{
  auto * pParams = GLProxy::GetInstance()->GetFloatv(_Name);

  if (_Name == GL_VIEWPORT)
  {
    memcpy(_pParams, pParams, 4 * sizeof(GLfloat));
  }
  else if (_Name == GL_MODELVIEW_MATRIX)
  {
    memcpy(_pParams, pParams, 16 * sizeof(float));
  }
}

void glMaterialfv(GLenum _Face, GLenum _Name, const GLfloat * _pParams)
{
  const auto Count = (_Name == GL_SHININESS) ? 1 : 4;

  ::std::vector<float> Params;

  for (int i = 0; i < Count; i++)
  {
    Params.push_back(*(_pParams + i));
  }

  GLProxy::GetInstance()->Materialfv(_Face, _Name, Params);
}

void glLightfv(GLenum _Index, GLenum _Name, const GLfloat * _pParams)
{
  ::std::vector<float> Params;

  for (int i = 0; i < 4; i++)
  {
    Params.push_back(*(_pParams + i));
  }

  GLProxy::GetInstance()->Lightfv(_Index, _Name, Params);
}

void glLightf(GLenum _Index, GLenum _Name, GLfloat _Param)
{
  GLProxy::GetInstance()->Lightf(_Index, _Name, _Param);
}

void glLightModelfv(GLenum _Index, const GLfloat * _pParams)
{
  ::std::vector<float> Params;

  for (int i = 0; i < 4; i++)
  {
    Params.push_back(*(_pParams + i));
  }

  GLProxy::GetInstance()->LightModelfv(_Index, Params);
}

void glLoadMatrixf(const GLfloat * _pMatrix)
{
  GLProxy::GetInstance()->LoadMatrixf(::glm::make_mat4(_pMatrix));
}

void glAlphaFunc(GLenum _Function, GLclampf _Value)
{
  GLProxy::GetInstance()->AlphaFunc(_Function, _Value);
}

void glFogf(GLenum pname, GLfloat param)
{
  GLProxy::GetInstance()->Fogf(pname, param);
}

void glFogfv(GLenum pname, const GLfloat * _pParams)
{
  ::std::vector<float> Params;

  for (int i = 0; i < 4; i++)
  {
    Params.push_back(*(_pParams + i));
  }

  GLProxy::GetInstance()->Fogfv(pname, Params);
}

void glFogi(GLenum pname, GLint param)
{
  GLProxy::GetInstance()->Fogi(pname, param);
}

} // unnamed namespace

} // namespace mock

namespace covellite::api
{

using ::mock::GLenum;
using ::mock::GLclampf;
using ::mock::GLbitfield;
using ::mock::GLint;
using ::mock::GLsizei;
using ::mock::GLfloat;
using ::mock::GLdouble;
using ::mock::GLvoid;
using ::mock::GLuint;
using ::mock::GLfixed;
using ::mock::GLubyte;
using ::mock::GLboolean;
using ::mock::GLchar;
using ::mock::GLsizeiptr;
using ::mock::GLintptr;

using ::mock::glGetError;
using ::mock::glDepthMask;
using ::mock::glEnable;
using ::mock::glIsEnabled;
using ::mock::glDisable;
using ::mock::glHint;
using ::mock::glShadeModel;
using ::mock::glFrontFace;
using ::mock::glCullFace;
using ::mock::glPushMatrix;
using ::mock::glPopMatrix;
using ::mock::glEnableClientState;
using ::mock::glDisableClientState;
using ::mock::glBindTexture;
using ::mock::glTranslatef;
using ::mock::glRotatef;
using ::mock::glScalef;
using ::mock::glVertexPointer;
using ::mock::glNormalPointer;
using ::mock::glColorPointer;
using ::mock::glTexCoordPointer;
using ::mock::glDrawElements;
using ::mock::glGenTextures;
using ::mock::glTexParameteri;
using ::mock::glTexImage2D;
using ::mock::glTexStorage3D;
using ::mock::glTexSubImage3D;
using ::mock::glDeleteTextures;
using ::mock::glBlendFunc;
using ::mock::glViewport;
using ::mock::glMatrixMode;
using ::mock::glLoadIdentity;
using ::mock::glOrtho;
using ::mock::glScissor;
using ::mock::glClearColor;
using ::mock::glClear;
using ::mock::glGetString;
using ::mock::glGetStringi;
using ::mock::glGetIntegerv;
using ::mock::glGetFloatv;
using ::mock::glMaterialfv;
using ::mock::glLightfv;
using ::mock::glLightf;
using ::mock::glLightModelfv;
using ::mock::glLoadMatrixf;
using ::mock::glAlphaFunc;
using ::mock::glFogf;
using ::mock::glFogfv;
using ::mock::glFogi;
using ::mock::glActiveTexture;
using ::mock::glGetUniformLocation;
using ::mock::glGenBuffers;
using ::mock::glDeleteBuffers;
using ::mock::glBindBuffer;
using ::mock::glBufferData;
using ::mock::glGetAttribLocation;
using ::mock::glVertexAttribPointer;
using ::mock::glEnableVertexAttribArray;
using ::mock::glBufferSubData;
using ::mock::glCreateShader;
using ::mock::glDeleteShader;
using ::mock::glShaderSource;
using ::mock::glCompileShader;
using ::mock::glGetShaderiv;
using ::mock::glGetProgramiv;
using ::mock::glGetShaderInfoLog;
using ::mock::glGetProgramInfoLog;
using ::mock::glCreateProgram;
using ::mock::glAttachShader;
using ::mock::glLinkProgram;
using ::mock::glUseProgram;
using ::mock::glDeleteProgram;

using ::mock::glUniform1i;
using ::mock::glUniform1f;
using ::mock::glUniform4fv;
using ::mock::glUniformMatrix4fv;
using ::mock::glGetUniformBlockIndex;
using ::mock::glBindBufferBase;
using ::mock::glUniformBlockBinding;
using ::mock::glDrawElementsInstanced;
using ::mock::glVertexAttribDivisor;
using ::mock::glGetActiveAttrib;
using ::mock::glClearDepth;
using ::mock::glDepthFunc;
using ::mock::glGenFramebuffers;
using ::mock::glBindFramebuffer;
using ::mock::glDeleteFramebuffers;
using ::mock::glFramebufferTexture2D;
using ::mock::glCheckFramebufferStatus;
using ::mock::glDrawBuffers;
using ::mock::glGenerateMipmap;
using ::mock::glReadPixels;

} // namespace covellite::api
