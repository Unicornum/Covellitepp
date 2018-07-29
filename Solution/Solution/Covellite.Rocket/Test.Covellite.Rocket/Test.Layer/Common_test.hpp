
#pragma once

class Layer_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::rocket::Layer;
  using ITested_t = ::covellite::rocket::ILayer;
  using Context_t = ::mock::Rocket::Core::Context;
  using Document_t = ::mock::Rocket::Core::ElementDocument;
  using String_t = ::mock::Rocket::Core::String;
  using Path_t = ::boost::filesystem::path;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
    ::testing::DefaultValue<String_t>::Set("DefaultString");
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
    ::testing::DefaultValue<String_t>::Clear();
  }

protected:
  class Tested :
    public Tested_t
  {
  public:
    static const int Message1 = 1807272031;
    static const int Message2 = 1807272032;

  public:
    MOCK_METHOD0(DoMessage1, void(void));
    MOCK_METHOD0(DoMessage2, void(void));

  public:
    void Subscribe(const EventHandlerPtr_t &) override {}

  public:
    Tested(::covellite::rocket::IWindow & _Window, const Path_t & _Path) :
      Tested_t(_Window, _Path)
    {
      m_Events[Message1].Connect([&]() { DoMessage1(); });
      m_Events[::covellite::events::Error.Exception].Connect([]() { });
    }
    Tested(::covellite::rocket::IWindow & _Window, const Path_t & _Path,
      const ::std::string & _TitleId) :
      Tested_t(_Window, _Path, _TitleId)
    {
      m_Events[Message2].Connect([&]() { DoMessage2(); });
    }
    Tested(Context_t * _pContext, const Path_t & _Path) :
      Tested_t(_pContext, _Path)
    {

    }
    Tested(Context_t * _pContext, const Path_t & _Path,
      const ::std::string & _TitleId) :
      Tested_t(_pContext, _Path, _TitleId)
    {

    }
  };

  class Window :
    public ::covellite::rocket::IWindow
  {
  public:
    operator Events_t (void) const override { return m_Events; }
    MOCK_METHOD1(LoadDocument, Document_t * (const PathToFile_t &));

  public:
    Events_t m_Events;
  };
};
