
#pragma once

namespace mock
{

namespace Rocket
{

namespace Core
{

class String 
{
public:
  bool operator== (const char * _String) const { return (m_String == _String); }
  bool operator!= (const char * _String) const { return (m_String != _String); }
  const char * CString(void) const { return m_String.c_str(); }

private:
  ::std::string m_String;

public:
  // cppcheck-suppress noExplicitConstructor
  String(const char * _String = "") : m_String(_String) {}
};

class ElementDocument;

class Element 
{
public:
  MOCK_METHOD0(RemoveReference, void(void));
  MOCK_METHOD1(GetElementById, Element * (const String &));
  MOCK_METHOD1(SetInnerRML, void(const String &));
  MOCK_CONST_METHOD0(GetInnerRML, String(void));
  MOCK_CONST_METHOD0(GetId, String(void));
  MOCK_METHOD2(SetAttribute, void (String, String));
  MOCK_METHOD2(GetAttribute, String(String, String));
  MOCK_CONST_METHOD0(GetTagName, String (void));
  MOCK_CONST_METHOD2(GetAttributeString, String(String, String));
  MOCK_METHOD0(Focus, bool(void));
  MOCK_METHOD0(GetOwnerDocument, ElementDocument *(void));

  // cppcheck-suppress functionStatic
  void GetInnerRML(String & _Content) const
  {
    _Content = GetInnerRML();
  }

public:
  virtual ~Element(void) = default;
};

class Event 
{
public:
  MOCK_CONST_METHOD0(GetTargetElement, Element * (void));
  MOCK_CONST_METHOD0(GetType, String(void));
};

class EventListener;

class EventListenerProxy
{
public:
  MOCK_METHOD3(AddEventListener, void(String, EventListener *, bool));
  MOCK_METHOD3(RemoveEventListener, void(String, EventListener *, bool));
};

class Context;

class ElementDocument :
  public Element,
  public EventListenerProxy
{
public:
  enum FocusFlags
  {
    NONE = 0,
    FOCUS = (1 << 1),
    MODAL = (1 << 2)
  };

public:
  MOCK_METHOD0(GetTitle, String(void));
  MOCK_METHOD0(Show, void(void));
  MOCK_METHOD0(Hide, void(void));
  MOCK_METHOD0(GetContext, Context *(void));
};

template < typename Type >
class Vector2
{
public:
  Type x;
  Type y;

public:
  bool operator== (const Vector2 & _Vector) const
  {
    if (x != _Vector.x) return false;
    if (y != _Vector.y) return false;
    return true;
  }

public:
  Vector2(void) : x(0), y(0) {}
  Vector2(Type _X, Type _Y) : x(_X), y(_Y) {}
};

template<typename ColourType, int AlphaDefault>
class Colour
{
public:
  uint8_t red, green, blue, alpha;
};

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;
using FileHandle = uintptr_t;
using CompiledGeometryHandle = void *;
using TextureHandle = uintptr_t;
using byte = unsigned char;
using word = unsigned short;
using Colourb = Colour<byte, 255>;

struct Vertex
{
  Vector2f position;
  Colourb colour;
  Vector2f tex_coord;
};

namespace Input
{

enum KeyIdentifier
{
  KI_UNKNOWN = 0,
  KI_BACK = 69,				// Backspace key.
  KI_TAB = 70,				// Tab key.
  KI_RETURN = 72,
  KI_ESCAPE = 81,				// Escape key.
  KI_PRIOR = 86,				// Page Up key.
  KI_NEXT = 87,				// Page Down key.
  KI_END = 88,
  KI_HOME = 89,
  KI_LEFT = 90,				// Left Arrow key.
  KI_UP = 91,					// Up Arrow key.
  KI_RIGHT = 92,				// Right Arrow key.
  KI_DOWN = 93,				// Down Arrow key.
  KI_DELETE = 99,
};

} // namespace Input

class Log
{
public:
  enum Type
  {
    LT_ALWAYS = 0,
    LT_ERROR,
    LT_ASSERT,
    LT_WARNING,
    LT_INFO,
    LT_DEBUG,
    LT_MAX
  };
};

class Context :
  public EventListenerProxy
{
public:
  MOCK_METHOD0(RemoveReference, void(void));
  MOCK_METHOD1(LoadDocument, ElementDocument * (String));
  MOCK_METHOD0(Update, bool(void));
  MOCK_METHOD0(Render, bool(void));
  MOCK_METHOD1(SetDimensions, void (const Vector2i &));
  MOCK_METHOD3(ProcessMouseMove, void (int, int, int));
  MOCK_METHOD2(ProcessMouseButtonDown, void (int, int));
  MOCK_METHOD2(ProcessMouseButtonUp, void (int, int));
  MOCK_METHOD2(ProcessKeyDown, bool (Input::KeyIdentifier, int));
  MOCK_METHOD1(ProcessTextInput, bool (word));
  MOCK_CONST_METHOD0(GetNumDocuments, int (void));
  MOCK_METHOD1(GetDocument, ElementDocument * (int));
  MOCK_CONST_METHOD0(GetDimensions, Vector2i(void));
};

class FileInterface
{
public:
  const ::mock::Id_t m_Id;

public:
  virtual ~FileInterface(void) {}
  virtual FileHandle Open(const String &) = 0;
  virtual void Close(FileHandle) = 0;
  virtual size_t Read(void *, size_t, FileHandle) = 0;
  virtual bool Seek(FileHandle, long, int) = 0;
  virtual size_t Tell(FileHandle) = 0;
  virtual size_t Length(FileHandle) = 0;

public:
  explicit FileInterface(::mock::Id_t _Id = 0) : m_Id(_Id) {}
};

class SystemInterface
{
public:
  const ::mock::Id_t m_Id;

public:
  virtual ~SystemInterface(void) {}
  virtual float GetElapsedTime(void) = 0;
  virtual int TranslateString(String &, const String &) { return 0; }
  virtual bool LogMessage(Log::Type, const String &) { return false; }
  virtual void ActivateKeyboard(void) {}
  virtual void DeactivateKeyboard(void) {};

public:
  explicit SystemInterface(::mock::Id_t _Id = 0) : m_Id(_Id) {}
};

class RenderInterface
{
public:
  const ::mock::Id_t m_Id;

public:
  virtual ~RenderInterface(void) {}
  virtual void RenderGeometry(Vertex *, int, int *, int, TextureHandle, const Vector2f &) {}
  virtual void EnableScissorRegion(bool) {}
  virtual void SetScissorRegion(int, int, int, int) {}
  virtual bool LoadTexture(TextureHandle &, Vector2i &, const String &) { return false; }
  virtual bool GenerateTexture(TextureHandle &, const byte *, const Vector2i &) { return false; }
  virtual void ReleaseTexture(TextureHandle) {}
  virtual float GetVerticalTexelOffset(void) { throw ::std::exception(); }
  virtual CompiledGeometryHandle CompileGeometry(Vertex *, int, int *, int, TextureHandle) { throw ::std::exception{}; }
  virtual void RenderCompiledGeometry(CompiledGeometryHandle, const Vector2f &) {}
  virtual void ReleaseCompiledGeometry(CompiledGeometryHandle) {}

public:
  explicit RenderInterface(::mock::Id_t _Id = 0) : m_Id(_Id) {}
};

class EventListener
{
public:
  virtual ~EventListener() {}
  virtual void ProcessEvent(Event &) = 0;
};

class FontDatabase
{
public:
  class Proxy :
    public ::alicorn::extension::testing::Proxy<Proxy>
  {
  public:
    MOCK_METHOD1(LoadFontFace, bool (String));
  };

public:
  static bool LoadFontFace(const String & _Path)
  {
    return Proxy::GetInstance()->LoadFontFace(_Path);
  }
};

class Proxy :
  public ::alicorn::extension::testing::Proxy<Proxy>
{
public:
  MOCK_METHOD1(SetFileInterface, void(Id_t));
  MOCK_METHOD1(SetSystemInterface, void(Id_t));
  MOCK_METHOD1(SetRenderInterface, void(Id_t));
  MOCK_METHOD0(Initialise, void(void));
  MOCK_METHOD0(Shutdown, void(void));
  MOCK_METHOD3(CreateContext, Context * (String, Vector2i, RenderInterface *));
};

namespace
{

void SetFileInterface(FileInterface * _pInterface)
{
  const auto Id = (_pInterface != nullptr) ? _pInterface->m_Id : 0;
  Proxy::GetInstance()->SetFileInterface(Id);
}

void SetSystemInterface(SystemInterface * _pInterface)
{
  const auto Id = (_pInterface != nullptr) ? _pInterface->m_Id : 0;
  Proxy::GetInstance()->SetSystemInterface(Id);
}

void SetRenderInterface(RenderInterface * _pInterface)
{
  const auto Id = (_pInterface != nullptr) ? _pInterface->m_Id : 0;
  Proxy::GetInstance()->SetRenderInterface(Id);
}

void Initialise(void) 
{
  Proxy::GetInstance()->Initialise();
}

void Shutdown(void) 
{
  Proxy::GetInstance()->Shutdown();
}

Context * CreateContext(const String & _Name, const Vector2i & _Size,
  RenderInterface * _pRenderInterface = nullptr)
{
  return Proxy::GetInstance()->CreateContext(_Name, _Size, _pRenderInterface);
}

FileInterface * GetFileInterface(void) 
{ 
  throw ::std::exception(); 
}

} // unnamed namespace

} // namespace Core

namespace Controls
{

class ElementFormControl :
  public Core::Element
{
public:
  MOCK_CONST_METHOD0(GetValue, Core::String(void));
  MOCK_METHOD1(SetValue, void(const Core::String &));
};

class Proxy :
  public ::alicorn::extension::testing::Proxy<Proxy>
{
public:
  MOCK_METHOD0(Initialise, void(void));
};

namespace
{

void Initialise(void)
{
  Proxy::GetInstance()->Initialise();
}

} // unnamed namespace

} // namespace Controls

namespace Debugger
{

class Proxy :
  public ::alicorn::extension::testing::Proxy<Proxy>
{
public:
  MOCK_METHOD1(Initialise, bool(Core::Context *));
  MOCK_METHOD1(SetContext, bool(Core::Context *));
};

namespace
{

bool Initialise(Core::Context * _pContext)
{ 
  return Proxy::GetInstance()->Initialise(_pContext);
}

bool SetContext(Core::Context * _pContext)
{
  return Proxy::GetInstance()->SetContext(_pContext);
}

} // unnamed namespace

} // namespace Debugger

} // namespace Rocket

} // namespace mock
