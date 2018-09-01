
#include "stdafx.h"
#include <alicorn\std\singleton.hpp>
#include <Covellite\Gui\Vfs.hpp>

/// \cond DoxygenWarningSuppress
  
namespace alicorn
{
  
namespace extension
{
  
namespace std
{
  
using namespace ::covellite::gui;
    
template<>
/*static*/ VfsPtr_t Singleton<VfsCore_t>::Make(void)
{
  using ImplPtr_t = ::std::shared_ptr<::alicorn::modules::vfs::IImplementation>;
    
  return ::std::make_unique<VfsCore_t>(::std::vector<ImplPtr_t>
  {
    ::std::make_shared<::alicorn::modules::vfs::FileSystem>("")
  });
}
  
} // namespace std
  
} // namespace extension
  
} // namespace alicorn

/// \endcond
