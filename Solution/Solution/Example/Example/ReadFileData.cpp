
#include "stdafx.h"
#include <alicorn/compress.hpp>
#include <Covellite/App/Vfs.hpp>

/// \cond DoxygenWarningSuppress
  
namespace alicorn
{

namespace extension
{

namespace std
{

using namespace ::covellite::app;
  
template<>
/*static*/ VfsPtr_t Singleton<VfsCore_t>::Make(void)
{
  using ImplPtr_t = ::std::shared_ptr<::alicorn::modules::vfs::IImplementation>;
  using ConverterPtr_t = ::std::shared_ptr<::alicorn::modules::vfs::IConverter>;
  using CompressorType_t = ::alicorn::source::compress::Snappy_t;
  using Compressor_t = ::alicorn::modules::vfs::Compress<CompressorType_t>;

  return ::std::make_unique<VfsCore_t>(::std::vector<ImplPtr_t>
  {
    ::std::make_shared<::alicorn::modules::vfs::Pack>("data/Data.pack", // Чтение данных из упакованного файла Data.pack.
      ::std::vector<ConverterPtr_t>{ ::std::make_shared<Compressor_t>() }),
  });
}
  
} // namespace std
  
} // namespace extension
  
} // namespace alicorn

/// \endcond
