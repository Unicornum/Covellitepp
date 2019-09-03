
#pragma once
#include <alicorn\std\singleton.hpp>
#include <alicorn\vfs.hpp>

namespace covellite
{

namespace app
{

using VfsCore_t = ::alicorn::modules::vfs::Core;
using VfsPtr_t = ::std::unique_ptr<VfsCore_t>;
using Vfs_t = ::alicorn::extension::std::Singleton<const VfsCore_t &>;

} // namespace app

} // namespace covellite

namespace alicorn
{

namespace extension
{

namespace std
{

using namespace ::covellite::app;

template<>
/*static*/ VfsPtr_t Singleton<VfsCore_t>::Make(void);

} // namespace std

} // namespace extension

} // namespace alicorn
