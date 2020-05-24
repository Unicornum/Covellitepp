
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
#define GL_INVALID_INDEX                  0xFFFFFFFFu
#define GL_FLOAT_VEC2                     0x8B50
#define GL_FLOAT_VEC3                     0x8B51
#define GL_FLOAT_VEC4                     0x8B52
#define GL_INT_VEC2                       0x8B53
#define GL_INT_VEC3                       0x8B54
#define GL_INT_VEC4                       0x8B55

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
#define GL_UNIFORM_BUFFER                 0x8A11
#define GL_ACTIVE_ATTRIBUTES              0x8B89

#define GL_STREAM_DRAW                    0x88E0
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8

#define GL_FRAMEBUFFER                    0x8D40
#define GL_RENDERBUFFER                   0x8D41
#define GL_FRAMEBUFFER_BINDING            0x8CA6
#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_STENCIL_ATTACHMENT             0x8D20
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#define GL_DEPTH_STENCIL                  0x84F9
#define GL_UNSIGNED_INT_24_8              0x84FA
#define GL_DEPTH24_STENCIL8               0x88F0
#define GL_DEPTH_COMPONENT                0x1902
#define GL_DEPTH_COMPONENT16              0x81A5
#define GL_DEPTH_COMPONENT24              0x81A6
#define GL_DEPTH_COMPONENT32F             0x8CAC
#define GL_TEXTURE_COMPARE_MODE           0x884C
#define GL_TEXTURE_COMPARE_FUNC           0x884D
#define GL_COMPARE_REF_TO_TEXTURE         0x884E

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

inline void glBindBufferBase(GLenum target, GLuint index, GLuint buffer)
{
  using Call_t = void(*)(GLenum, GLuint, GLuint);
  static auto glCall = GetProcAddress<Call_t>("glBindBufferBase");

  glCall(target, index, buffer);
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

inline GLuint glGetUniformBlockIndex(GLuint program, const GLchar *uniformBlockName)
{
  using Call_t = GLuint(*)(GLuint, const GLchar *);
  static auto glCall = GetProcAddress<Call_t>("glGetUniformBlockIndex");

  return glCall(program, uniformBlockName);
}

inline void glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex,
  GLuint uniformBlockBinding)
{
  using Call_t = void(*)(GLuint, GLuint, GLuint);
  static auto glCall = GetProcAddress<Call_t>("glUniformBlockBinding");

  glCall(program, uniformBlockIndex, uniformBlockBinding);
}

inline const GLubyte * glGetStringi(GLenum name, GLuint index)
{
  using Call_t = const GLubyte *(*)(GLenum, GLuint);
  static auto glCall = GetProcAddress<Call_t>("glGetStringi");

  return glCall(name, index);
}

inline void glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count,
  GLsizei instancecount)
{
  using Call_t = void(*)(GLenum, GLint, GLsizei, GLsizei);
  static auto glCall = GetProcAddress<Call_t>("glDrawArraysInstanced");

  glCall(mode, first, count, instancecount);
}

inline void glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type,
  const void * indices, GLsizei instancecount)
{
  using Call_t = void(*)(GLenum, GLsizei, GLenum, const void *, GLsizei);
  static auto glCall = GetProcAddress<Call_t>("glDrawElementsInstanced");

  glCall(mode, count, type, indices, instancecount);
}

inline void glVertexAttribDivisor(GLuint index, GLuint divisor)
{
  using Call_t = void(*)(GLuint, GLuint);
  static auto glCall = GetProcAddress<Call_t>("glVertexAttribDivisor");

  glCall(index, divisor);
}

inline void glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize,
  GLsizei *length, GLint *size, GLenum *type, GLchar *name)
{
  using Call_t = void(*)(GLuint, GLuint, GLsizei, GLsizei *, GLint *, 
    GLenum *, GLchar *);
  static auto glCall = GetProcAddress<Call_t>("glGetActiveAttrib");

  glCall(program, index, bufSize, length, size, type, name);
}

inline void glGenFramebuffers(GLsizei n, GLuint *ids)
{
  using Call_t = void(*)(GLsizei, GLuint *);
  static auto glCall = GetProcAddress<Call_t>("glGenFramebuffers");

  glCall(n, ids);
}

inline void glBindFramebuffer(GLenum target, GLuint framebuffer)
{
  using Call_t = void(*)(GLenum, GLuint);
  static auto glCall = GetProcAddress<Call_t>("glBindFramebuffer");

  glCall(target, framebuffer);
}

inline GLenum glCheckFramebufferStatus(GLenum target)
{
  using Call_t = GLenum(*)(GLenum);
  static auto glCall = GetProcAddress<Call_t>("glCheckFramebufferStatus");

  return glCall(target);
}

inline void glDeleteFramebuffers(GLsizei n, const GLuint * framebuffers)
{
  using Call_t = void(*)(GLsizei, const GLuint *);
  static auto glCall = GetProcAddress<Call_t>("glDeleteFramebuffers");

  glCall(n, framebuffers);
}

inline void glFramebufferTexture2D(GLenum target, GLenum attachment,
  GLenum textarget, GLuint texture, GLint level)
{
  using Call_t = void(*)(GLenum, GLenum, GLenum, GLuint, GLint);
  static auto glCall = GetProcAddress<Call_t>("glFramebufferTexture2D");

  glCall(target, attachment, textarget, texture, level);
}

inline void glDepthRangef(GLfloat n, GLfloat f)
{
  using Call_t = void(*)(GLfloat, GLfloat);
  static auto glCall = GetProcAddress<Call_t>("glDepthRangef");

  glCall(n, f);
}

inline void glDrawBuffers(GLsizei n, const GLenum *bufs)
{
  using Call_t = void(*)(GLsizei, const GLenum *);
  static auto glCall = GetProcAddress<Call_t>("glDrawBuffers");

  glCall(n, bufs);
}

inline void glGenerateMipmap(GLenum target)
{
  using Call_t = void(*)(GLenum);
  static auto glCall = GetProcAddress<Call_t>("glGenerateMipmap");

  glCall(target);
}

#define GL_DEBUG_OUTPUT_SYNCHRONOUS       0x8242
#define GL_DEBUG_OUTPUT                   0x92E0

#define GL_DEBUG_TYPE_ERROR               0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR  0x824E
#define GL_DEBUG_TYPE_PORTABILITY         0x824F
#define GL_DEBUG_TYPE_PERFORMANCE         0x8250
#define GL_DEBUG_TYPE_OTHER               0x8251
#define GL_DEBUG_TYPE_MARKER              0x8268
#define GL_DEBUG_TYPE_PUSH_GROUP          0x8269
#define GL_DEBUG_TYPE_POP_GROUP           0x826A

#define GL_DEBUG_SEVERITY_HIGH            0x9146
#define GL_DEBUG_SEVERITY_MEDIUM          0x9147
#define GL_DEBUG_SEVERITY_LOW             0x9148
#define GL_DEBUG_SEVERITY_NOTIFICATION    0x826B

typedef void (APIENTRY * DEBUGPROC)(GLenum source, GLenum type, GLuint id,
  GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

inline void glDebugMessageCallback(DEBUGPROC callback, const void * userParam)
{
  using Call_t = void(*)(DEBUGPROC, const void *);
  static auto glCall = GetProcAddress<Call_t>("glDebugMessageCallback");

  glCall(callback, userParam);
}

inline void glDebugMessageControl(GLenum source, GLenum type, GLenum severity,
  GLsizei count, const GLuint *ids, GLboolean enabled)
{
  using Call_t = void(*)(GLenum, GLenum, GLenum, GLsizei, const GLuint *, GLboolean);
  static auto glCall = GetProcAddress<Call_t>("glDebugMessageControl");

  glCall(source, type, severity, count, ids, enabled);
}
