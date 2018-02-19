
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ Layer.
*/

#define Layer_test Layer_windows_test
#define Layer Layer_windows

// ������������ ������ Layer
#include "..\..\Covellite.Rocket\Layer.cpp"
#include "..\..\Covellite.Rocket\Layer.windows.cpp"

// ����� �������� ����� ������ Layer
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

  // ���������� ����� �������� ������� �����
  void SetUp(void) override
  {
    ::testing::DefaultValue<String_t>::Set("DefaultString");
  }

  // ���������� ����� ������� ������� �����
  void TearDown(void) override
  {
    ::testing::DefaultValue<String_t>::Clear();
  }

protected:
  class Tested :
    public Tested_t
  {
  public:
    void Subscribe(const EventHandlerPtr_t &) override {}

  public:
    using Tested_t::Tested_t;
  };
};

// ������� ������� ��� ����������� � ����� Layer 
// ��� ������� �������� ������� � �������� �������� ������ (����� ��� ���������, 
// ����� ����� �������� ������� ���� ����������� � ��� �� ������������ ����, 
// ��� � ����������� �����).
// FRIEND_TEST(Layer_test, Test_Function);

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor)
{
  Context_t Context;
  Document_t Document;

  const char * SourcePathToFile = "Path/To/����\\1710301242";
  const char * ExpectedPathToFile = "Path\\To\\����\\1710301242";

  using namespace ::testing;

  EXPECT_CALL(Context, LoadDocument(Eq(ExpectedPathToFile)))
    .Times(1)
    .WillOnce(Return(&Document));

  using namespace ::alicorn::extension::std;
  Locale::Scope::SetDefault<Locale::Ascii128> Dummy;

  const Tested Example{ &Context, Path_t{ SourcePathToFile } };
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor_Title)
{
  Context_t Context;
  Document_t Document;
  ::mock::Rocket::Core::Element Title;

  const char * SourcePathToFile = "Path/To/����\\1710301244";
  const char * ExpectedPathToFile = "Path\\To\\����\\1710301244";

  using namespace ::testing;

  EXPECT_CALL(Context, LoadDocument(Eq(ExpectedPathToFile)))
    .Times(1)
    .WillOnce(Return(&Document));

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Title));

  using namespace ::alicorn::extension::std;
  Locale::Scope::SetDefault<Locale::Ascii128> Dummy;

  const Tested Example{ &Context, Path_t{ SourcePathToFile }, "" };
}
