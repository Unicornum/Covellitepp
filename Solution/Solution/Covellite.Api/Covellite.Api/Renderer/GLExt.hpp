
#pragma once

#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_CURRENT_PROGRAM                0x8B8D
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_NUM_EXTENSIONS                 0x821D

/* TextureUnit */
#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_TEXTURE2                       0x84C2
#define GL_TEXTURE3                       0x84C3
#define GL_TEXTURE4                       0x84C4
#define GL_TEXTURE5                       0x84C5
#define GL_TEXTURE6                       0x84C6
#define GL_TEXTURE7                       0x84C7
#define GL_TEXTURE8                       0x84C8
#define GL_TEXTURE9                       0x84C9
#define GL_TEXTURE10                      0x84CA
#define GL_TEXTURE11                      0x84CB
#define GL_TEXTURE12                      0x84CC
#define GL_TEXTURE13                      0x84CD
#define GL_TEXTURE14                      0x84CE
#define GL_TEXTURE15                      0x84CF
#define GL_TEXTURE16                      0x84D0
#define GL_TEXTURE17                      0x84D1
#define GL_TEXTURE18                      0x84D2
#define GL_TEXTURE19                      0x84D3
#define GL_TEXTURE20                      0x84D4
#define GL_TEXTURE21                      0x84D5
#define GL_TEXTURE22                      0x84D6
#define GL_TEXTURE23                      0x84D7
#define GL_TEXTURE24                      0x84D8
#define GL_TEXTURE25                      0x84D9
#define GL_TEXTURE26                      0x84DA
#define GL_TEXTURE27                      0x84DB
#define GL_TEXTURE28                      0x84DC
#define GL_TEXTURE29                      0x84DD
#define GL_TEXTURE30                      0x84DE
#define GL_TEXTURE31                      0x84DF
#define GL_ACTIVE_TEXTURE                 0x84E0

/* Buffer Objects */
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_ARRAY_BUFFER_BINDING           0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING   0x8895

#define GL_STREAM_DRAW                    0x88E0
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8

using GLchar = char;
typedef signed   long  int     khronos_ssize_t;
typedef khronos_ssize_t  GLsizeiptr;
typedef signed   long  int     khronos_intptr_t;
typedef khronos_intptr_t GLintptr;

inline void glOrtho(GLfloat _Left, GLfloat _Right, GLfloat _Bottom,
  GLfloat _Top, GLfloat _zNear, GLfloat _zFar)
{
  glOrtho((GLdouble)_Left, (GLdouble)_Right, (GLdouble)_Bottom, (GLdouble)_Top,
    (GLdouble)_zNear, (GLdouble)_zFar);
}

template<class TCall>
inline TCall GetProcAddress(LPSTR _Name)
{
  auto Result = wglGetProcAddress(_Name);
  if (Result == nullptr) throw ::std::exception{ _Name };

  return reinterpret_cast<TCall>(Result);
}

inline GLuint glCreateShader(GLenum shaderType)
{
  using Call_t = GLuint(*)(GLenum);
  static auto glCall = GetProcAddress<Call_t>("glCreateShader");

  return glCall(shaderType);
}

inline void glShaderSource(GLuint shader, GLsizei count, const GLchar ** string,
  const GLint * length)
{
  using Call_t = void(*)(GLuint, GLsizei, const GLchar **, const GLint *);
  static auto glCall = GetProcAddress<Call_t>("glShaderSource");

  glCall(shader, count, string, length);
}

inline void glCompileShader(GLuint shader)
{
  using Call_t = void(*)(GLuint);
  static auto glCall = GetProcAddress<Call_t>("glCompileShader");

  glCall(shader);
}

inline void glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
  using Call_t = void(*)(GLuint, GLenum, GLint *);
  static auto glCall = GetProcAddress<Call_t>("glGetShaderiv");

  glCall(shader, pname, params);
}

inline void glGetShaderInfoLog(GLuint shader, GLsizei maxLength,
  GLsizei *length, GLchar *infoLog)
{
  using Call_t = void(*)(GLuint, GLsizei, GLsizei *, GLchar *);
  static auto glCall = GetProcAddress<Call_t>("glGetShaderInfoLog");

  glCall(shader, maxLength, length, infoLog);
}

inline void glDeleteShader(GLuint shader)
{
  using Call_t = void(*)(GLuint);
  static auto glCall = GetProcAddress<Call_t>("glDeleteShader");

  glCall(shader);
}

inline GLuint glCreateProgram(void)
{
  using Call_t = GLuint(*)(void);
  static auto glCall = GetProcAddress<Call_t>("glCreateProgram");

  return glCall();
}

inline void glDeleteProgram(GLuint program)
{
  using Call_t = void(*)(GLuint);
  static auto glCall = GetProcAddress<Call_t>("glDeleteProgram");

  glCall(program);
}

inline void glAttachShader(GLuint program, GLuint shader)
{
  using Call_t = void(*)(GLuint, GLuint);
  static auto glCall = GetProcAddress<Call_t>("glAttachShader");

  glCall(program, shader);
}

inline void glLinkProgram(GLuint program)
{
  using Call_t = void(*)(GLuint);
  static auto glCall = GetProcAddress<Call_t>("glLinkProgram");

  glCall(program);
}

inline void glGetProgramiv(GLuint program, GLenum pname, GLint *params)
{
  using Call_t = void(*)(GLuint, GLenum, GLint *);
  static auto glCall = GetProcAddress<Call_t>("glGetProgramiv");

  glCall(program, pname, params);
}

inline void glGetProgramInfoLog(GLuint program, GLsizei maxLength,
  GLsizei *length, GLchar *infoLog)
{
  using Call_t = void(*)(GLuint, GLsizei, GLsizei *, GLchar *);
  static auto glCall = GetProcAddress<Call_t>("glGetProgramInfoLog");

  glCall(program, maxLength, length, infoLog);
}

inline void glUseProgram(GLuint program)
{
  using Call_t = void(*)(GLuint);
  static auto glCall = GetProcAddress<Call_t>("glUseProgram");

  glCall(program);
}

inline void glActiveTexture(GLenum texture)
{
  using Call_t = void(*)(GLenum);
  static auto glCall = GetProcAddress<Call_t>("glActiveTexture");

  glCall(texture);
}

inline void glBindBuffer(GLenum target, GLuint buffer)
{
  using Call_t = void(*)(GLenum, GLuint);
  static auto glCall = GetProcAddress<Call_t>("glBindBuffer");

  glCall(target, buffer);
}

inline void glGenBuffers(GLsizei n, GLuint * buffers)
{
  using Call_t = void(*)(GLsizei, GLuint *);
  static auto glCall = GetProcAddress<Call_t>("glGenBuffers");

  glCall(n, buffers);
}

inline void glDeleteBuffers(GLsizei n, const GLuint * buffers)
{
  using Call_t = void(*)(GLsizei, const GLuint *);
  static auto glCall = GetProcAddress<Call_t>("glDeleteBuffers");

  glCall(n, buffers);
}

inline void glBufferData(GLenum target, GLsizeiptr size, const GLvoid * data,
  GLenum usage)
{
  using Call_t = void(*)(GLenum, GLsizeiptr, const GLvoid *, GLenum);
  static auto glCall = GetProcAddress<Call_t>("glBufferData");

  glCall(target, size, data, usage);
}

inline void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
  const GLvoid* data)
{
  using Call_t = void(*)(GLenum, GLintptr, GLsizeiptr, const GLvoid *);
  static auto glCall = GetProcAddress<Call_t>("glBufferSubData");

  glCall(target, offset, size, data);
}

inline GLint glGetAttribLocation(GLuint program, const GLchar *name)
{
  using Call_t = GLint(*)(GLuint, const GLchar *);
  static auto glCall = GetProcAddress<Call_t>("glGetAttribLocation");

  return glCall(program, name);
}

inline void glVertexAttribPointer(GLuint index, GLint size, GLenum type,
  GLboolean normalized, GLsizei stride, const GLvoid * pointer)
{
  using Call_t = void(*)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid *);
  static auto glCall = GetProcAddress<Call_t>("glVertexAttribPointer");

  glCall(index, size, type, normalized, stride, pointer);
}

inline void glEnableVertexAttribArray(GLuint index)
{
  using Call_t = void(*)(GLuint);
  static auto glCall = GetProcAddress<Call_t>("glEnableVertexAttribArray");

  glCall(index);
}

inline GLint glGetUniformLocation(GLuint program, const GLchar *name)
{
  using Call_t = GLint(*)(GLuint, const GLchar *);
  static auto glCall = GetProcAddress<Call_t>("glGetUniformLocation");

  return glCall(program, name);
}

inline void glUniform1i(GLint location, GLint v0)
{
  using Call_t = void(*)(GLint, GLint);
  static auto glCall = GetProcAddress<Call_t>("glUniform1i");

  glCall(location, v0);
}

inline void glUniform1f(GLint location, GLfloat x)
{
  using Call_t = void(*)(GLint, GLfloat);
  static auto glCall = GetProcAddress<Call_t>("glUniform1f");

  glCall(location, x);
}

inline void glUniform4fv(GLint location, GLsizei count, const GLfloat *value)
{
  using Call_t = void(*)(GLint, GLsizei, const GLfloat *);
  static auto glCall = GetProcAddress<Call_t>("glUniform4fv");

  glCall(location, count, value);
}

inline void glUniformMatrix4fv(GLint location, GLsizei count,
  GLboolean transpose, const GLfloat *value)
{
  using Call_t = void(*)(GLint, GLsizei, GLboolean, const GLfloat *);
  static auto glCall = GetProcAddress<Call_t>("glUniformMatrix4fv");

  glCall(location, count, transpose, value);
}

inline const GLubyte * glGetStringi(GLenum name, GLuint index)
{
  using Call_t = const GLubyte *(*)(GLenum, GLuint);
  static auto glCall = GetProcAddress<Call_t>("glGetStringi");

  return glCall(name, index);
}
