
#include "stdafx.h"
#include <iostream>
#include <alicorn\vfs.hpp>
#include <alicorn\compress.hpp>

int main(int argc, char * argv[])
{
  if (argc != 3) return 1;

  using CompressorType_t = ::alicorn::source::compress::Snappy_t;
  using Compressor_t = ::alicorn::modules::vfs::Compress<CompressorType_t>;
  using ::alicorn::modules::vfs::Pack;

  try
  {
    // argv[1] - путь к папке, которая будет упаковываться.
    // argv[2] - путь к файлу, в который будет записан результат упаковки.
    Pack{ argv[2], { ::std::make_shared<Compressor_t>() } }.Build(argv[1]);
  }
  catch (const ::std::exception & _Ex)
  {
    ::std::cout << "Pack error: " << _Ex.what() << ::std::endl;
    return 1;
  }

  return 0;
}
