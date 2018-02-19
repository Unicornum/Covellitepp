
#pragma once

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
#define GL_COLOR_ARRAY                    0x8076
#define GL_TEXTURE_COORD_ARRAY            0x8078
#define GL_TEXTURE_2D                     0x0DE1
#define GL_TRIANGLES                      0x0004
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_LINEAR                         0x2601
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_RGBA                           0x1908
#define GL_SCISSOR_TEST                   0x0C11
#define GL_VIEWPORT                       0x0BA2
#define GL_NO_ERROR                       0
#define GL_VERSION                        0x1F02

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

class GLProxy :
  public ::alicorn::extension::testing::Proxy<GLProxy>
{
public:
  MOCK_METHOD0(GetError, GLenum (void));
  MOCK_METHOD1(Enable, void(GLenum));
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
  MOCK_METHOD4(VertexPointer, void (GLint, GLenum, GLsizei, const GLvoid *));
  MOCK_METHOD4(ColorPointer, void (GLint, GLenum, GLsizei, const GLvoid *));
  MOCK_METHOD4(TexCoordPointer, void (GLint, GLenum, GLsizei, const GLvoid *));
  MOCK_METHOD4(DrawElements, void (GLenum, GLsizei, GLenum, const GLvoid *));
  MOCK_METHOD1(GenTextures, GLuint(GLsizei));
  MOCK_METHOD3(TexParameteri, void (GLenum, GLenum, GLfixed));
  MOCK_METHOD9(TexImage2D, void (GLenum, GLint, GLint, GLsizei, GLsizei, GLint, 
    GLenum, GLenum, const GLvoid *));
  MOCK_METHOD2(DeleteTextures, void(GLsizei, GLuint));
  MOCK_METHOD2(BlendFunc, void(GLenum, GLenum));
  MOCK_METHOD4(Viewport, void (GLint, GLint, GLsizei, GLsizei));
  MOCK_METHOD1(MatrixMode, void (GLenum));
  MOCK_METHOD0(LoadIdentity, void (void));
  MOCK_METHOD6(Orthof, void (GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat));
  MOCK_METHOD6(Ortho, void(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble));
  MOCK_METHOD1(GetIntegerv, void(GLenum));
  MOCK_METHOD1(GetIntValue, GLint(::std::string));
  MOCK_METHOD4(Scissor, void (GLint, GLint, GLsizei, GLsizei));
  MOCK_METHOD4(ClearColor, void(GLclampf, GLclampf, GLclampf, GLclampf));
  MOCK_METHOD1(Clear, void(GLbitfield));
  MOCK_METHOD1(GetString, const GLubyte * (GLenum _Name));
};

namespace
{

GLenum glGetError(void) 
{ 
  return GLProxy::GetInstance()->GetError();
}

void glEnable(GLenum _Param)
{
  GLProxy::GetInstance()->Enable(_Param);
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

void glVertexPointer(GLint _Param1, GLenum _Param2, GLsizei _Param3, 
  const GLvoid * _Param4)
{
  GLProxy::GetInstance()->VertexPointer(_Param1, _Param2, _Param3, _Param4);
}

void glColorPointer(GLint _Param1, GLenum _Param2, GLsizei _Param3, 
  const GLvoid * _Param4)
{
  GLProxy::GetInstance()->ColorPointer(_Param1, _Param2, _Param3, _Param4);
}

void glTexCoordPointer(GLint _Param1, GLenum _Param2, GLsizei _Param3, 
  const GLvoid * _Param4)
{
  GLProxy::GetInstance()->TexCoordPointer(_Param1, _Param2, _Param3, _Param4);
}

void glDrawElements(GLenum _Param1, GLsizei _Param2, GLenum _Param3, 
  const GLvoid * _Param4)
{
  GLProxy::GetInstance()->DrawElements(_Param1, _Param2, _Param3, _Param4);
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

void glOrthof(GLfloat _Param1, GLfloat _Param2, GLfloat _Param3, 
  GLfloat _Param4, GLfloat _Param5, GLfloat _Param6)
{
  GLProxy::GetInstance()->Orthof(
    _Param1, _Param2, _Param3, _Param4, _Param5, _Param6);
}

void glOrtho(GLdouble _Param1, GLdouble _Param2, GLdouble _Param3, 
  GLdouble _Param4, GLdouble _Param5, GLdouble _Param6)
{
  GLProxy::GetInstance()->Ortho(
    _Param1, _Param2, _Param3, _Param4, _Param5, _Param6);
}

void glGetIntegerv(GLenum _Name, GLint * _pParams)
{
  GLProxy::GetInstance()->GetIntegerv(_Name);

  if (_Name == GL_VIEWPORT)
  {
    _pParams[0] = GLProxy::GetInstance()->GetIntValue("x");
    _pParams[1] = GLProxy::GetInstance()->GetIntValue("y");
    _pParams[2] = GLProxy::GetInstance()->GetIntValue("width");
    _pParams[3] = GLProxy::GetInstance()->GetIntValue("height");
  }
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
using ::mock::glEnable;
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
using ::mock::glVertexPointer;
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
using ::mock::glOrthof;
using ::mock::glOrtho;
using ::mock::glGetIntegerv;
using ::mock::glScissor;
using ::mock::glClearColor;
using ::mock::glClear;
using ::mock::glGetString;

} // namespace api

} // namespace covellite
