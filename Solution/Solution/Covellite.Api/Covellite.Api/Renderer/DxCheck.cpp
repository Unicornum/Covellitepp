
#include "stdafx.h"
#include "DxCheck.hpp"
#include <comdef.h>
#include <alicorn/std/string.hpp>
#include <alicorn/std/string/encoding.hpp>

using namespace covellite::api::renderer;

// cppcheck-suppress unusedFunction
/*static*/ bool DxCheck::IsSuccessfull(HRESULT _hResult)
{
  return SUCCEEDED(_hResult);
}

// cppcheck-suppress unusedFunction
/*static*/ ::std::string DxCheck::GetMessageText(HRESULT _hResult)
{
  using namespace ::alicorn::extension::std;

  return string_cast<::std::string, Encoding::UTF8>(
    String::GetString(_com_error{ _hResult }.ErrorMessage()));
}
