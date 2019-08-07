
#pragma once
#include <DirectXMath.h>

namespace DirectX
{

inline bool operator== (
  const ::DirectX::XMFLOAT4X4 & _Left,
  const ::DirectX::XMFLOAT4X4 & _Right)
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      const auto dValue = 1.0f - _Left.m[i][j] / _Right.m[i][j];
      if (abs(dValue) > 1e-3f)
      {
        return false;
      }
    }
  }

  return true;
}

} // namespace DirectX

#define GL_TRUE                           1
#define GL_FALSE                          0

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
#define GL_TRIANGLES                      0x0004
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_LINEAR                         0x2601
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_REPEAT                         0x2901
#define GL_RGBA                           0x1908
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
using GLboolean = bool;

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
  MOCK_METHOD4(TexCoordPointer, void (GLint, GLenum, GLsizei, Floats_t));
  MOCK_METHOD4(DrawElements, void (GLenum, GLsizei, GLenum, Ints_t));
  MOCK_METHOD1(GenTextures, GLuint(GLsizei));
  MOCK_METHOD3(TexParameteri, void (GLenum, GLenum, GLfixed));
  MOCK_METHOD9(TexImage2D, void (GLenum, GLint, GLint, GLsizei, GLsizei, GLint, 
    GLenum, GLenum, const GLvoid *));
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
  MOCK_METHOD1(GetIntegerv, const GLint * (GLenum));
  MOCK_METHOD1(GetFloatv, const GLfloat * (GLenum));
  MOCK_METHOD3(Materialfv, void (GLenum, GLenum, Floats_t));
  MOCK_METHOD3(Lightfv, void(GLenum, GLenum, Floats_t));
  MOCK_METHOD3(Lightf, void(GLenum, GLenum, GLfloat));
  MOCK_METHOD2(LightModelfv, void(GLenum, Floats_t));
  MOCK_METHOD1(LoadMatrixf, void(::DirectX::XMFLOAT4X4));
  MOCK_METHOD2(AlphaFunc, void (GLenum, GLclampf));
  MOCK_METHOD2(Fogf, void(GLenum, GLfloat));
  MOCK_METHOD2(Fogfv, void(GLenum, Floats_t));
  MOCK_METHOD2(Fogi, void(GLenum, GLint));

public:
  template<class T>
  static ::std::vector<T> GetData(const GLint & _Count, const GLsizei & _Stride,
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
  if (_Type == GL_UNSIGNED_INT)
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
  GLProxy::GetInstance()->TexImage2D(_Param1, _Param2, _Param3, _Param4, 
    _Param5, _Param6, _Param7, _Param8, _Param9);
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

void glGetIntegerv(GLenum _Name, GLint * _pParams)
{
  auto * pParams = GLProxy::GetInstance()->GetIntegerv(_Name);

  if (_Name == GL_VIEWPORT)
  {
    _pParams[0] = pParams[0];
    _pParams[1] = pParams[1];
    _pParams[2] = pParams[2];
    _pParams[3] = pParams[3];
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
  ::DirectX::XMFLOAT4X4 Matrix4x4;

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      Matrix4x4.m[j][i] = _pMatrix[4 * j + i];
    }
  }

  GLProxy::GetInstance()->LoadMatrixf(Matrix4x4);
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

namespace covellite
{

namespace api
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

} // namespace api

} // namespace covellite
