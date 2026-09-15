
#include "stdafx.h"
#include <alicorn\std\chrono.mock.hpp>
#include <Tools\Tools_test.hpp>

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

#include "..\..\CheckShaderCompilation.App\Initial.hpp"
#include "..\..\CheckShaderCompilation.App\Serializator.hpp"

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
    if (::boost::filesystem::exists(m_PathToWorkFile))
    {
      ::boost::filesystem::remove(m_PathToWorkFile);
    }
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
  const Path_t m_PathToNotExistsInsideFile =
    THIS_DIRECTORY / L"NotExistsInside.ini";
  const Path_t m_PathToWorkFile =
    THIS_DIRECTORY / L"Work.ini";

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
  const ::std::string ExpectMessage =
    "(): error C0000: unrecognised option '--unknown'\r\n" + AllowedOptions;

  const auto Params = { uT("--unknown") };

  const auto Result = RunProcess(Params);
  EXPECT_EQ(-1, Result.ReturnCode);
  EXPECT_EQ(ExpectMessage, Result.ConsoleOutput);
}

// ************************************************************************** //
TEST_F(Using_test, /*DISABLED_*/Test_FileOnly)
{
  const Path_t m_PathToExampleFile =
    THIS_DIRECTORY / L"Example.ini";

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
    const ::std::string ExpectMessage =
      "NotExists.hpp(): error C0000: not exists file\r\n";

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
    const ::std::string ExpectMessage =
      (THIS_DIRECTORY / "NotExistsFile.fx").string() +
      "(): error C0000: not exists file.\r\n";

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
  using namespace ::game::repository;

  Serializator<initial::Shader_t>::Write(m_PathToWorkFile,
    initial::Shader_t{
      { "Invalid.fx" },
      uT("vsMain"),
      uT("")
    });

  using namespace ::alicorn::extension::std;

  const auto Params =
  {
    uT("--file=") + string_cast<String>(m_PathToWorkFile),
    uT("--hlsl"),
  };

  const auto Result = RunProcess(Params);
  EXPECT_EQ(-1, Result.ReturnCode);
  EXPECT_NE("", Result.ConsoleOutput);
}

// ************************************************************************** //
TEST_F(Using_test, /*DISABLED_*/Test_HLSL_ExtractFileLineFromError_Forward)
{
  const ::std::string ExpectMessage =
    "Example1.fx(4): error X3017: cannot implicitly convert from 'float3' to 'float4'.\r\n";

  using namespace ::game::repository;

  Serializator<initial::Shader_t>::Write(m_PathToWorkFile,
    initial::Shader_t{
      {
        "Example1.fx",
        "Example2.fx",
        "Main.fx"
      },
      uT("vsMain"),
      uT("")
    });

  using namespace ::alicorn::extension::std;

  const auto Params =
  {
    uT("--file=") + string_cast<String>(m_PathToWorkFile),
    uT("--hlsl"),
  };

  const auto Result = RunProcess(Params);
  EXPECT_EQ(-1, Result.ReturnCode);
  EXPECT_EQ(ExpectMessage, Result.ConsoleOutput);
  //P:\projects\Covellitepp\Solution\Solution\Covellite.Api\Covellite.Api\Renderer\DirectX.cpp(29): Failed: -2147467259 [header line: 174, P:\projects\Covellitepp\Debug\Test\x64\[Covellite::Api]: vsMain(178,3-16): error X3017: cannot implicitly convert from 'float3' to 'float4'].
}

// ************************************************************************** //
TEST_F(Using_test, /*DISABLED_*/Test_HLSL_ExtractFileLineFromError_Reverse)
{
  const ::std::string ExpectMessage =
    "Example1.fx(4): error X3017: cannot implicitly convert from 'float3' to 'float4'.\r\n";

  using namespace ::game::repository;

  Serializator<initial::Shader_t>::Write(m_PathToWorkFile,
    initial::Shader_t{
      {
        "Example2.fx",
        "Example1.fx",
        "Main.fx"
      },
      uT("vsMain"),
      uT("")
    });

  using namespace ::alicorn::extension::std;

  const auto Params =
  {
    uT("--file=") + string_cast<String>(m_PathToWorkFile),
    uT("--hlsl"),
  };

  const auto Result = RunProcess(Params);
  EXPECT_EQ(-1, Result.ReturnCode);
  EXPECT_EQ(ExpectMessage, Result.ConsoleOutput);
  // P:\projects\Covellitepp\Solution\Solution\Covellite.Api\Covellite.Api\Renderer\DirectX.cpp(29): Failed: -2147467259 [header line: 174, P:\projects\Covellitepp\Debug\Test\x64\[Covellite::Api]: vsMain(183,3-16): error X3017: cannot implicitly convert from 'float3' to 'float4'].
}

// ************************************************************************** //
TEST_F(Using_test, /*DISABLED_*/Test_HLSL_ValidFile)
{
  using namespace ::game::repository;

  Serializator<initial::Shader_t>::Write(m_PathToWorkFile,
    initial::Shader_t{
      { "Valid.fx" },
      uT("vsMain"),
      uT("")
    });

  using namespace ::alicorn::extension::std;

  const auto Params =
  {
    uT("--file=") + string_cast<String>(m_PathToWorkFile),
    uT("--hlsl"),
  };

  const auto Result = RunProcess(Params);
  EXPECT_EQ(0, Result.ReturnCode);
  EXPECT_EQ("", Result.ConsoleOutput);
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
    const ::std::string ExpectMessage =
      "Path\\To\\NotExists.hpp(): error C0000: not exists file\r\n";

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
    const ::std::string ExpectMessage =
      (THIS_DIRECTORY / "NotExistsFile.fx").string() +
      "(): error C0000: not exists file.\r\n";

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
  using namespace ::game::repository;

  Serializator<initial::Shader_t>::Write(m_PathToWorkFile,
    initial::Shader_t{
      { "Invalid.fx" },
      uT("vsMain"),
      uT("")
    });

  using namespace ::alicorn::extension::std;

  const auto Params =
  {
    uT("--file=") + string_cast<String>(m_PathToWorkFile),
    uT("--glsl"),
  };

  const auto Result = RunProcess(Params);
  EXPECT_EQ(-1, Result.ReturnCode);
  EXPECT_NE("", Result.ConsoleOutput);
}

// ************************************************************************** //
TEST_F(Using_test, /*DISABLED_*/Test_GLSL_ValidFile)
{
  using namespace ::game::repository;

  Serializator<initial::Shader_t>::Write(m_PathToWorkFile,
    initial::Shader_t{
      { "Valid.fx" },
      uT("vsMain"),
      uT("")
    });

  using namespace ::alicorn::extension::std;

  const auto Params =
  {
    uT("--file=") + string_cast<String>(m_PathToWorkFile),
    uT("--glsl"),
  };

  const auto Result = RunProcess(Params);
  EXPECT_EQ(0, Result.ReturnCode);
  EXPECT_EQ("", Result.ConsoleOutput);
}

ALICORN_RESTORE_WARNINGS
