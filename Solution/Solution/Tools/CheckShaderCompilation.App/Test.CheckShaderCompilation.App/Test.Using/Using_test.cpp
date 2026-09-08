
#include "stdafx.h"
#include <alicorn\std\chrono.mock.hpp>
#include <Tools\Tools_test.hpp>

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса Using.
*/

// Общий тестовый класс класса Using
class Using_test :
  public ::testing::Test,
  public Tools_test
{
protected:
  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) noexcept override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) noexcept override
  {
  }

protected:
  const ::std::string AllowedOptions =
    "Allowed options:\r\n"
    "  --help                Produce help message.\r\n"
    "  --file arg            Path to shader pack file.\r\n"
    "  --hlsl                Check shader as HLSL.\r\n"
    "  --glsl                Check shader as GLSL.\r\n"
    ;

protected:
  const Path_t m_PathToExampleFile =
    THIS_DIRECTORY / L"Example.fxdef";
  const Path_t m_PathToNotExistsInsideFile =
    THIS_DIRECTORY / L"NotExistsInside.fxdef";

public:
  Using_test(void) :
    Tools_test("CheckShaderCompilation.exe")
  {

  }
};

ALICORN_DISABLE_GTEST_WARNINGS

// Образец макроса для подстановки в класс Using 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(Using_test, Test_Function);

// ************************************************************************** //
TEST_F(Using_test, /*DISABLED_*/Test_EmptyParameters)
{
  const auto Params = { uT("") };

  const auto Result = RunProcess(Params);
  EXPECT_EQ(-1, Result.ReturnCode);
  EXPECT_EQ(AllowedOptions, Result.ConsoleOutput);
}

// ************************************************************************** //
TEST_F(Using_test, /*DISABLED_*/Test_Help)
{
  const auto Params = { uT("--help") };

  const auto Result = RunProcess(Params);
  EXPECT_EQ(0, Result.ReturnCode);
  EXPECT_EQ(AllowedOptions, Result.ConsoleOutput);
}

// ************************************************************************** //
TEST_F(Using_test, /*DISABLED_*/Test_Help_IgnoreAnotherCommand)
{
  const auto Params =
  {
    uT("--help"),
    uT("--file=version.hpp"),
    uT("--hlsl"),
    uT("--glsl"),
  };

  const auto Result = RunProcess(Params);
  EXPECT_EQ(0, Result.ReturnCode);
  EXPECT_EQ(AllowedOptions, Result.ConsoleOutput);
}

// ************************************************************************** //
TEST_F(Using_test, /*DISABLED_*/Test_UnknownParameters)
{
  const ::std::string ExpectMessage = "(): error C0000: "
    "unrecognised option '--unknown'\r\n" + AllowedOptions;

  const auto Params = { uT("--unknown") };

  const auto Result = RunProcess(Params);
  EXPECT_EQ(-1, Result.ReturnCode);
  EXPECT_EQ(ExpectMessage, Result.ConsoleOutput);
}

// ************************************************************************** //
TEST_F(Using_test, /*DISABLED_*/Test_FileOnly)
{
  using namespace ::alicorn::extension::std;

  const auto Params =
  {
    uT("--file=") + string_cast<String>(m_PathToExampleFile)
  };

  const auto Result = RunProcess(Params);
  EXPECT_EQ(-1, Result.ReturnCode);
  EXPECT_EQ(AllowedOptions, Result.ConsoleOutput);
}

// ************************************************************************** //
TEST_F(Using_test, /*DISABLED_*/Test_HLSL_WithoutFile)
{
  const auto Params = { uT("--hlsl") };

  const auto Result = RunProcess(Params);
  EXPECT_EQ(-1, Result.ReturnCode);
  EXPECT_EQ(AllowedOptions, Result.ConsoleOutput);
}

// ************************************************************************** //
TEST_F(Using_test, /*DISABLED_*/Test_HLSL_NotExistsFiles)
{
  {
    const ::std::string ExpectMessage = "NotExists.hpp(): error C0000: "
      "not exists file\r\n";

    const auto Params =
    {
      uT("--file=NotExists.hpp"),
      uT("--hlsl"),
    };

    const auto Result = RunProcess(Params);
    EXPECT_EQ(-1, Result.ReturnCode);
    EXPECT_EQ(ExpectMessage, Result.ConsoleOutput);
  }

  {
    const ::std::string ExpectMessage = "NotExistsInside.fxdef(): error C0000: "
      "not exists file NotExistsFile.fx\r\n" + AllowedOptions;

    using namespace ::alicorn::extension::std;

    const auto Params =
    {
      uT("--file=") + string_cast<String>(m_PathToNotExistsInsideFile),
      uT("--hlsl"),
    };

    const auto Result = RunProcess(Params);
    EXPECT_EQ(-1, Result.ReturnCode);
    EXPECT_EQ(ExpectMessage, Result.ConsoleOutput);
  }
}

// ************************************************************************** //
TEST_F(Using_test, /*DISABLED_*/Test_HLSL_InvalidFile)
{
  FAIL() << u8"Добавить тесты";
}

// ************************************************************************** //
TEST_F(Using_test, /*DISABLED_*/Test_HLSL_ValidFile)
{
  FAIL() << u8"Добавить тесты";
}

// ************************************************************************** //
TEST_F(Using_test, /*DISABLED_*/Test_GLSL_WithoutFile)
{
  const auto Params = { uT("--glsl") };

  const auto Result = RunProcess(Params);
  EXPECT_EQ(-1, Result.ReturnCode);
  EXPECT_EQ(AllowedOptions, Result.ConsoleOutput);
}

// ************************************************************************** //
TEST_F(Using_test, /*DISABLED_*/Test_GLSL_NotExistsFiles)
{
  {
    const ::std::string ExpectMessage = "Path\\To\\NotExists.hpp(): error C0000: "
      "not exists file\r\n";

    const auto Params =
    {
      uT("--file=Path\\To\\NotExists.hpp"),
      uT("--glsl"),
    };

    const auto Result = RunProcess(Params);
    EXPECT_EQ(-1, Result.ReturnCode);
    EXPECT_EQ(ExpectMessage, Result.ConsoleOutput);
  }

  {
    const ::std::string ExpectMessage = "NotExistsInside.fxdef(): error C0000: "
      "not exists file NotExistsFile.fx\r\n" + AllowedOptions;

    using namespace ::alicorn::extension::std;

    const auto Params =
    {
      uT("--file=") + string_cast<String>(m_PathToNotExistsInsideFile),
      uT("--glsl"),
    };

    const auto Result = RunProcess(Params);
    EXPECT_EQ(-1, Result.ReturnCode);
    EXPECT_EQ(ExpectMessage, Result.ConsoleOutput);
  }
}

// ************************************************************************** //
TEST_F(Using_test, /*DISABLED_*/Test_GLSL_InvalidFile)
{
  FAIL() << u8"Добавить тесты";
}

// ************************************************************************** //
TEST_F(Using_test, /*DISABLED_*/Test_GLSL_ValidFile)
{
  FAIL() << u8"Добавить тесты";
}

ALICORN_RESTORE_WARNINGS
