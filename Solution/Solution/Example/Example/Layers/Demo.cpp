
#include "stdafx.h"
#include "Demo.hpp"
#include <alicorn/std/string.hpp>
#include "Description.hpp"

using namespace layers;

const auto LayerDescription =
  uT("Здесь будет текст описания демонстрационного режима.<br/>");

Demo::Demo(IWindowGui_t & _Window) :
  Layer(_Window, "demo.rml", uT("Демонстрационный режим"), LayerDescription)
{
}
