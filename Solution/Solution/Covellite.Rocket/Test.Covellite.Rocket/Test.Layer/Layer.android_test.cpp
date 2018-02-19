
#include "stdafx.h"

// ������� �������� ���������� Google Test
#include <alicorn\google\test\example.hpp>

// ������� �������� ���������� Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  ����� ������ Layer.
*/

#undef BOOST_OS_WINDOWS
#undef BOOST_OS_ANDROID
#define BOOST_OS_ANDROID 1
#define Layer_test Layer_android_test
#define Layer Layer_android

// ������������ ������ Layer
#include "..\..\Covellite.Rocket\Layer.cpp"
#include "..\..\Covellite.Rocket\Layer.android.cpp"

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

  const char * SourcePathToFile = "Path\\To\\File/1710301246";
  const char * ExpectedPathToFile = "Path/To/File/1710301246";

  using namespace ::testing;

  EXPECT_CALL(Context, LoadDocument(Eq(ExpectedPathToFile)))
    .Times(1)
    .WillOnce(Return(&Document));

  const Tested Example{ &Context, Path_t{ SourcePathToFile } };
}

// ************************************************************************** //
TEST_F(Layer_test, /*DISABLED_*/Test_Constructor_Title)
{
  Context_t Context;
  Document_t Document;
  ::mock::Rocket::Core::Element Title;

  const char * SourcePathToFile = "Path\\To\\File/1710301248";
  const char * ExpectedPathToFile = "Path/To/File/1710301248";

  using namespace ::testing;

  EXPECT_CALL(Context, LoadDocument(Eq(ExpectedPathToFile)))
    .Times(1)
    .WillOnce(Return(&Document));

  EXPECT_CALL(Document, GetElementById(_))
    .Times(1)
    .WillOnce(Return(&Title));

  const Tested Example{ &Context, Path_t{ SourcePathToFile }, "" };
}
