
#include "stdafx.h"
#include "ExtraWindow.hpp"
#include <Covellite\Api\GL.hpp>

void ExtraWindow::Subscribe(const EventHandlerPtr_t & _pEvents) /*override*/
{
  using namespace ::covellite::core;

  auto DoDrawing = [](const params::Empty &)
  {
    glClearColor(0.4f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
  };
    
  (*_pEvents)[Event::Drawing].connect(DoDrawing);
}
