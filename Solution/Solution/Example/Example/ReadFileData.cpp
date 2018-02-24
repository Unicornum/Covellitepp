
#include "stdafx.h"
#include <alicorn\std\singleton.hpp>
#include <alicorn\vfs.hpp>
  
namespace alicorn
{
  
namespace extension
{
  
namespace std
{
  
using namespace ::alicorn::modules::vfs;
    
template<>
/*static*/ ::std::unique_ptr<Core> Singleton<Core>::Make(void)
{
  using ImplPtr_t = ::std::shared_ptr<IImplementation>;
    
  return ::std::make_unique<Core>(::std::vector<ImplPtr_t>
  {
    ::std::make_shared<FileSystem>("")
  });
}
  
} // namespace std
  
} // namespace extension
  
} // namespace alicorn
