
#pragma once

class Layer_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::gui::Layer;
  using ITested_t = ::covellite::gui::ILayer;
  using Context_t = ::mock::CovelliteGui::Context;
  using Document_t = ::mock::CovelliteGui::ElementDocument;
  using DocumentPtr_t = ::std::unique_ptr<Document_t, void(*)(Document_t *)>;
  using String_t = ::mock::CovelliteGui::String;
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
    Tested(::covellite::gui::IWindow & _Window, const Path_t & _Path) :
      Tested_t(_Window, _Path)
    {
      m_Events[Message1].Connect([&]() { DoMessage1(); });
      m_Events[::covellite::events::Error.Exception].Connect([]() { });
    }
    Tested(::covellite::gui::IWindow & _Window, const Path_t & _Path,
      const ::std::string & _TitleId) :
      Tested_t(_Window, _Path, _TitleId)
    {
      m_Events[Message2].Connect([&]() { DoMessage2(); });
    }
  };

  class Window :
    public ::covellite::gui::IWindow
  {
  public:
    operator Events_t (void) const override { return m_Events; }
    DocumentPtr_t LoadDocument(const PathToFile_t & _Path) override
    {
      return DocumentPtr_t{ DoLoadDocument(_Path), [](Document_t *) {} };
    }

    MOCK_METHOD1(DoLoadDocument, Document_t * (const PathToFile_t &));

  public:
    Events_t m_Events;
  };
};
