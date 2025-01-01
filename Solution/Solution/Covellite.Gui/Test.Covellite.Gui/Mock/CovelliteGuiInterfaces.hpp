
#pragma once

namespace mock
{

namespace covellite
{

class InterfacesProxy :
  public ::alicorn::extension::testing::Proxy<InterfacesProxy>
{
public:
  MOCK_METHOD0(FileConstructor, Id_t(void));
  MOCK_METHOD1(SystemConstructor, Id_t(Id_t));
  MOCK_METHOD1(RenderConstructor, Id_t(int));
};

namespace gui
{

class File :
  public ::mock::CovelliteGui::FileInterface
{
  using FileHandle_t = ::mock::CovelliteGui::FileHandle;
  using String_t = ::mock::CovelliteGui::String;

public:
  FileHandle_t Open(const String_t &) override { throw ::std::exception(); }
  void Close(FileHandle_t) override { throw ::std::exception(); }
  size_t Read(void *, size_t, FileHandle_t) override { throw ::std::exception(); }
  bool Seek(FileHandle_t, long, int) override { throw ::std::exception(); }
  size_t Tell(FileHandle_t) override { throw ::std::exception(); }
  size_t Length(FileHandle_t) override { throw ::std::exception(); }

public:
  File(void) :
    FileInterface(InterfacesProxy::GetInstance()->FileConstructor())
  {
  }
};

class System :
  public ::mock::CovelliteGui::SystemInterface
{
  using StringTranslatorPtr_t = ::std::shared_ptr<IStringTranslator>;

public:
  ::covellite::gui::CovelliteGuiTime_t GetElapsedTime(void) override { throw ::std::exception(); }

public:
  explicit System(const StringTranslatorPtr_t & _pStringTranslator) :
    SystemInterface(InterfacesProxy::GetInstance()->SystemConstructor(_pStringTranslator->m_Id))
  {
  }
};

} // namespace gui

namespace api
{

class RenderOpenGL :
  public ::mock::CovelliteGui::RenderInterface
{
  using Vertex_t = ::mock::CovelliteGui::Vertex;
  using TextureHandle_t = ::mock::CovelliteGui::TextureHandle;
  using Vector2f_t = ::mock::CovelliteGui::Vector2f;

public:
  void RenderGeometry(Vertex_t *, int, int *, int, TextureHandle_t,
    const Vector2f_t &) override { throw ::std::exception(); }
  void EnableScissorRegion(bool) override { throw ::std::exception(); }
  void SetScissorRegion(int, int, int, int) override { throw ::std::exception(); }

public:
  explicit RenderOpenGL(int _StatusBarHeight) :
    RenderInterface(InterfacesProxy::GetInstance()->RenderConstructor(_StatusBarHeight))
  {
  }
};

} // namespace api

} // namespace covellite

} // namespace mock
