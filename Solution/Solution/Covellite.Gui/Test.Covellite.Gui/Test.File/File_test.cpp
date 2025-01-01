
#include "stdafx.h"

// Примеры макросов библиотеки Google Test
#include <alicorn\google\test\example.hpp>

// Примеры макросов библиотеки Google Mock
#include <alicorn\google\mock\example.hpp>

/**
* \file
*  Тесты класса File.
*/

// Расположение класса File
#include <Covellite\Gui\File.hpp>

#include <alicorn\std\vector.hpp>

// Общий тестовый класс класса File
class File_test :
  public ::testing::Test
{
protected:
  using Tested_t = ::covellite::gui::File;
  using ITested_t = ::mock::CovelliteGui::FileInterface;
  using Data_t = ::alicorn::extension::std::memory::BinaryData_t;
  using Handle_t = ::mock::CovelliteGui::FileHandle;

  // Вызывается ПЕРЕД запуском каждого теста
  void SetUp(void) override
  {
  }

  // Вызывается ПОСЛЕ запуска каждого теста
  void TearDown(void) override
  {
  }
};

// Образец макроса для подстановки в класс File 
// для доступа тестовой функции к закрытым функциям класса (чтобы это сработало, 
// нужно чтобы тестовая функция была расположена В ТОМ ЖЕ ПРОСТРАНСТВЕ ИМЕН, 
// что и тестируемый класс).
// FRIEND_TEST(File_test, Test_Function);

// ************************************************************************** //
TEST_F(File_test, /*DISABLED_*/Test_Destructor)
{
  EXPECT_TRUE(::std::has_virtual_destructor<Tested_t>::value);
  EXPECT_TRUE(::std::is_nothrow_destructible<Tested_t>::value);
}

// ************************************************************************** //
TEST_F(File_test, /*DISABLED_*/Test_Read_UnknownHandle)
{
  Tested_t Example;
  ITested_t & IExample = Example;

  Data_t Data(5, 0x00);
  const auto Expected = Data;

  const auto File = static_cast<Handle_t>(1710041333);

  const auto Result = IExample.Read(Data.data(), Data.size(), File);
  EXPECT_EQ(0, Result);
  EXPECT_EQ(Expected, Data);
  EXPECT_EQ(0, IExample.Tell(File));
}

// ************************************************************************** //
TEST_F(File_test, /*DISABLED_*/Test_Open_NonZeroFirstHandle)
{
  Tested_t Example;
  ITested_t & IExample = Example;

  const auto File = 
    IExample.Open((THIS_DIRECTORY / L"1234567890.txt").string().c_str());
  EXPECT_TRUE(File != 0);
  IExample.Close(File);
}

// ************************************************************************** //
TEST_F(File_test, /*DISABLED_*/Test_OpenNotExistFile)
{
  Tested_t Example;
  ITested_t & IExample = Example;

  const auto File = 
    IExample.Open((THIS_DIRECTORY / L"NotExists.txt").string().c_str());
  EXPECT_TRUE(File == 0);
}

// ************************************************************************** //
TEST_F(File_test, /*DISABLED_*/Test_ReadExcessData)
{
  const Data_t ExpectedData = 
  { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };

  Tested_t Example;
  ITested_t & IExample = Example;

  const auto File = IExample.Open("1234567890.txt");
  EXPECT_TRUE(File != 0);
  EXPECT_EQ(0, IExample.Tell(File));

  {
    using namespace ::alicorn::extension::std;

    Data_t Data(20, 0x00);
    const auto Expected = ExpectedData + 
      Data_t(Data.size() - ExpectedData.size(), 0x00);

    const auto Result = IExample.Read(Data.data(), Data.size(), File);
    EXPECT_EQ(ExpectedData.size(), Result);
    EXPECT_EQ(Expected, Data);
    EXPECT_EQ(ExpectedData.size(), IExample.Tell(File));
  }

  IExample.Close(File);
}

// ************************************************************************** //
TEST_F(File_test, /*DISABLED_*/Test_ReadFile)
{
  const Data_t ExpectedData = { '1', '2', '3', '4', '5', '6', '7' };

  Tested_t Example;
  ITested_t & IExample = Example;

  const auto File = IExample.Open("1234567890.txt");
  EXPECT_TRUE(File != 0);
  EXPECT_EQ(0, IExample.Tell(File));

  {
    Data_t Data(ExpectedData.size(), 0x00);

    const auto Result = IExample.Read(Data.data(), Data.size(), File);
    EXPECT_EQ(ExpectedData.size(), Result);
    EXPECT_EQ(ExpectedData, Data);
    EXPECT_EQ(ExpectedData.size(), IExample.Tell(File));
  }

  IExample.Close(File);

  {
    Data_t Data(2, 0x00);
    const auto Expected = Data;

    const auto Result = IExample.Read(Data.data(), Data.size(), File);
    EXPECT_EQ(0, Result);
    EXPECT_EQ(Expected, Data);
    EXPECT_EQ(0, IExample.Tell(File));
  }
}

// ************************************************************************** //
TEST_F(File_test, /*DISABLED_*/Test_ReadSameFile)
{
  const Data_t ExpectedData = { '1', '2', '3', '4', '5' };

  Tested_t Example;
  ITested_t & IExample = Example;

  const auto File1 = IExample.Open("1234567890.txt");
  EXPECT_TRUE(File1 != 0);
  EXPECT_EQ(0, IExample.Tell(File1));

  const auto File2 = IExample.Open("1234567890.txt");
  EXPECT_TRUE(File2 != 0);
  EXPECT_EQ(0, IExample.Tell(File2));

  {
    Data_t Data(ExpectedData.size(), 0x00);

    const auto Result = IExample.Read(Data.data(), Data.size(), File1);
    EXPECT_EQ(ExpectedData.size(), Result);
    EXPECT_EQ(ExpectedData, Data);
    EXPECT_EQ(ExpectedData.size(), IExample.Tell(File1));
  }

  IExample.Close(File1);

  {
    Data_t Data(5, 0x00);
    const auto Expected = Data;

    const auto Result = IExample.Read(Data.data(), Data.size(), File1);
    EXPECT_EQ(0, Result);
    EXPECT_EQ(Expected, Data);
    EXPECT_EQ(0, IExample.Tell(File1));
  }

  {
    Data_t Data(ExpectedData.size(), 0x00);

    const auto Result = IExample.Read(Data.data(), Data.size(), File2);
    EXPECT_EQ(ExpectedData.size(), Result);
    EXPECT_EQ(ExpectedData, Data);
    EXPECT_EQ(ExpectedData.size(), IExample.Tell(File2));
  }

  IExample.Close(File2);

  {
    Data_t Data(5, 0x00);
    const auto Expected = Data;

    const auto Result = IExample.Read(Data.data(), Data.size(), File2);
    EXPECT_EQ(0, Result);
    EXPECT_EQ(Expected, Data);
    EXPECT_EQ(0, IExample.Tell(File2));
  }
}

// ************************************************************************** //
TEST_F(File_test, /*DISABLED_*/Test_ReadAnotherFile)
{
  const Data_t ExpectedData1 = { '1', '2', '3', '4', '5' };
  const Data_t ExpectedData2 = { '0', '9', '8', '7', '6', '5', '4', '3' };

  Tested_t Example;
  ITested_t & IExample = Example;

  const auto File1 = IExample.Open("1234567890.txt");
  EXPECT_TRUE(File1 != 0);
  EXPECT_EQ(0, IExample.Tell(File1));

  const auto File2 = IExample.Open("0987654321.txt");
  EXPECT_TRUE(File2 != 0);
  EXPECT_EQ(0, IExample.Tell(File2));

  {
    Data_t Data(ExpectedData1.size(), 0x00);

    const auto Result = IExample.Read(Data.data(), Data.size(), File1);
    EXPECT_EQ(ExpectedData1.size(), Result);
    EXPECT_EQ(ExpectedData1, Data);
    EXPECT_EQ(ExpectedData1.size(), IExample.Tell(File1));
  }

  IExample.Close(File1);

  {
    Data_t Data(5, 0x00);
    const auto Expected = Data;

    const auto Result = IExample.Read(Data.data(), Data.size(), File1);
    EXPECT_EQ(0, Result);
    EXPECT_EQ(Expected, Data);
    EXPECT_EQ(0, IExample.Tell(File1));
  }

  {
    Data_t Data(ExpectedData2.size(), 0x00);

    const auto Result = IExample.Read(Data.data(), Data.size(), File2);
    EXPECT_EQ(ExpectedData2.size(), Result);
    EXPECT_EQ(ExpectedData2, Data);
    EXPECT_EQ(ExpectedData2.size(), IExample.Tell(File2));
  }

  IExample.Close(File2);

  {
    Data_t Data(2, 0x00);
    const auto Expected = Data;

    const auto Result = IExample.Read(Data.data(), Data.size(), File2);
    EXPECT_EQ(0, Result);
    EXPECT_EQ(Expected, Data);
    EXPECT_EQ(0, IExample.Tell(File2));
  }
}

// ************************************************************************** //
TEST_F(File_test, /*DISABLED_*/Test_Tell_UnknownHandle)
{
  Tested_t Example;
  ITested_t & IExample = Example;

  const auto File = static_cast<Handle_t>(1710041436);

  EXPECT_EQ(0, IExample.Tell(File));
}

// ************************************************************************** //
TEST_F(File_test, /*DISABLED_*/Test_Seek_UnknownHandle)
{
  Tested_t Example;
  ITested_t & IExample = Example;

  const auto File = static_cast<Handle_t>(1710041432);

  EXPECT_FALSE(IExample.Seek(File, 0, SEEK_SET));
  EXPECT_FALSE(IExample.Seek(File, 0, SEEK_CUR));
  EXPECT_FALSE(IExample.Seek(File, 0, SEEK_END));
}

// ************************************************************************** //
TEST_F(File_test, /*DISABLED_*/Test_Seek_Set)
{
  const Data_t ExpectedData = { '3', '4', '5', '6', '7' };
  const long BeginPosition = 2;

  Tested_t Example;
  ITested_t & IExample = Example;

  const auto File = IExample.Open("1234567890.txt");
  EXPECT_TRUE(File != 0);
  EXPECT_EQ(0, IExample.Tell(File));

  {
    const auto IsSeek = IExample.Seek(File, 13, SEEK_SET);
    EXPECT_FALSE(IsSeek);
    EXPECT_EQ(0, IExample.Tell(File));
  }

  {
    const auto IsSeek = IExample.Seek(File, BeginPosition, SEEK_SET);
    EXPECT_TRUE(IsSeek);
    EXPECT_EQ(BeginPosition, IExample.Tell(File));

    Data_t Data(ExpectedData.size(), 0x00);

    const auto Result = IExample.Read(Data.data(), Data.size(), File);
    EXPECT_EQ(ExpectedData.size(), Result);
    EXPECT_EQ(ExpectedData, Data);
    EXPECT_EQ(BeginPosition + ExpectedData.size(), IExample.Tell(File));
  }

  {
    const auto IsSeek = IExample.Seek(File, BeginPosition, SEEK_SET);
    EXPECT_TRUE(IsSeek);
    EXPECT_EQ(BeginPosition, IExample.Tell(File));

    Data_t Data(ExpectedData.size(), 0x00);

    const auto Result = IExample.Read(Data.data(), Data.size(), File);
    EXPECT_EQ(ExpectedData.size(), Result);
    EXPECT_EQ(ExpectedData, Data);
    EXPECT_EQ(BeginPosition + ExpectedData.size(), IExample.Tell(File));
  }

  {
    const Data_t ExpectedData2 = { '8', '9', '0' };

    using namespace ::alicorn::extension::std;

    Data_t Data(10, 0x00);
    const auto Expected = ExpectedData2 + 
      Data_t(Data.size() - ExpectedData2.size(), 0x00);

    const auto Result = IExample.Read(Data.data(), Data.size(), File);
    EXPECT_EQ(ExpectedData2.size(), Result);
    EXPECT_EQ(Expected, Data);
    EXPECT_EQ(10, IExample.Tell(File));
  }

  {
    const auto IsSeek = IExample.Seek(File, 10, SEEK_SET);
    EXPECT_TRUE(IsSeek);
    EXPECT_EQ(10, IExample.Tell(File));
  }

  IExample.Close(File);
}

// ************************************************************************** //
TEST_F(File_test, /*DISABLED_*/Test_Seek_Cur)
{
  const Data_t ExpectedData = { '3', '4', '5', '6', '7' };

  Tested_t Example;
  ITested_t & IExample = Example;

  const auto File = IExample.Open("1234567890.txt");
  EXPECT_TRUE(File != 0);
  EXPECT_EQ(0, IExample.Tell(File));

  {
    const auto IsSeek = IExample.Seek(File, 13, SEEK_CUR);
    EXPECT_FALSE(IsSeek);
    EXPECT_EQ(0, IExample.Tell(File));
  }

  {
    const auto IsSeek = IExample.Seek(File, 2, SEEK_CUR);
    EXPECT_TRUE(IsSeek);
    EXPECT_EQ(2, IExample.Tell(File));

    Data_t Data(ExpectedData.size(), 0x00);

    const auto Result = IExample.Read(Data.data(), Data.size(), File);
    EXPECT_EQ(ExpectedData.size(), Result);
    EXPECT_EQ(ExpectedData, Data);
    EXPECT_EQ(2 + ExpectedData.size(), IExample.Tell(File));
  }

  {
    const auto IsSeek = IExample.Seek(File, -5, SEEK_CUR);
    EXPECT_TRUE(IsSeek);
    EXPECT_EQ(2, IExample.Tell(File));

    Data_t Data(ExpectedData.size(), 0x00);

    const auto Result = IExample.Read(Data.data(), Data.size(), File);
    EXPECT_EQ(ExpectedData.size(), Result);
    EXPECT_EQ(ExpectedData, Data);
    EXPECT_EQ(2 + ExpectedData.size(), IExample.Tell(File));
  }

  {
    const Data_t ExpectedData2 = { '8', '9', '0' };

    using namespace ::alicorn::extension::std;

    Data_t Data(10, 0x00);
    const auto Expected = ExpectedData2 +
      Data_t(Data.size() - ExpectedData2.size(), 0x00);

    const auto Result = IExample.Read(Data.data(), Data.size(), File);
    EXPECT_EQ(ExpectedData2.size(), Result);
    EXPECT_EQ(Expected, Data);
    EXPECT_EQ(10, IExample.Tell(File));
  }

  {
    const auto IsSeek = IExample.Seek(File, 0, SEEK_CUR);
    EXPECT_TRUE(IsSeek);
    EXPECT_EQ(10, IExample.Tell(File));
  }

  IExample.Close(File);
}

// ************************************************************************** //
TEST_F(File_test, /*DISABLED_*/Test_Seek_End)
{
  const Data_t ExpectedData = { '3', '4', '5', '6', '7' };

  Tested_t Example;
  ITested_t & IExample = Example;

  const auto File = IExample.Open("1234567890.txt");
  EXPECT_TRUE(File != 0);
  EXPECT_EQ(0, IExample.Tell(File));

  {
    const auto IsSeek = IExample.Seek(File, 1, SEEK_END);
    EXPECT_FALSE(IsSeek);
    EXPECT_EQ(0, IExample.Tell(File));
  }

  {
    const auto IsSeek = IExample.Seek(File, -8, SEEK_END);
    EXPECT_TRUE(IsSeek);
    EXPECT_EQ(2, IExample.Tell(File));

    Data_t Data(ExpectedData.size(), 0x00);

    const auto Result = IExample.Read(Data.data(), Data.size(), File);
    EXPECT_EQ(ExpectedData.size(), Result);
    EXPECT_EQ(ExpectedData, Data);
    EXPECT_EQ(2 + ExpectedData.size(), IExample.Tell(File));
  }

  {
    const auto IsSeek = IExample.Seek(File, -8, SEEK_END);
    EXPECT_TRUE(IsSeek);
    EXPECT_EQ(2, IExample.Tell(File));

    Data_t Data(ExpectedData.size(), 0x00);

    const auto Result = IExample.Read(Data.data(), Data.size(), File);
    EXPECT_EQ(ExpectedData.size(), Result);
    EXPECT_EQ(ExpectedData, Data);
    EXPECT_EQ(2 + ExpectedData.size(), IExample.Tell(File));
  }

  {
    const Data_t ExpectedData2 = { '8', '9', '0', };

    using namespace ::alicorn::extension::std;

    Data_t Data(10, 0x00);
    const auto Expected = ExpectedData2 +
      Data_t(Data.size() - ExpectedData2.size(), 0x00);

    const auto Result = IExample.Read(Data.data(), Data.size(), File);
    EXPECT_EQ(ExpectedData2.size(), Result);
    EXPECT_EQ(Expected, Data);
    EXPECT_EQ(10, IExample.Tell(File));
  }

  {
    const auto IsSeek = IExample.Seek(File, 0, SEEK_END);
    EXPECT_TRUE(IsSeek);
    EXPECT_EQ(10, IExample.Tell(File));
  }

  IExample.Close(File);
}

// ************************************************************************** //
TEST_F(File_test, /*DISABLED_*/Test_Length_UnknownHandle)
{
  Tested_t Example;
  ITested_t & IExample = Example;

  const auto File = static_cast<Handle_t>(1710041432);

  EXPECT_EQ(0, IExample.Length(File));
}

// ************************************************************************** //
TEST_F(File_test, /*DISABLED_*/Test_Length)
{
  Tested_t Example;
  ITested_t & IExample = Example;

  {
    const auto File = IExample.Open("1234567890.txt");
    EXPECT_EQ(10, IExample.Length(File));

    IExample.Close(File);
  }

  {
    const auto File = IExample.Open("3.txt");
    EXPECT_EQ(3, IExample.Length(File));

    IExample.Close(File);
  }
}
