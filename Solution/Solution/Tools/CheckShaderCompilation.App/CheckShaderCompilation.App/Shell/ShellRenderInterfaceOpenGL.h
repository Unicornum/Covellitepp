/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef ROCKETSHELLRENDERINTERFACEOPENGL_H
#define ROCKETSHELLRENDERINTERFACEOPENGL_H

#include "ShellRenderInterfaceExtensions.h"
#include <windows.h>

/**
	Low level OpenGL render interface for Rocket
	@author Peter Curry
 */
class ShellRenderInterfaceOpenGL final :
  //public GuiLib::RenderInterface,
  public ShellRenderInterfaceExtensions
{
public:
  /// Called by Rocket when it wants to render geometry that it does not wish 
  /// to optimise.
  //virtual void RenderGeometry(GuiLib::Vertex *, int, int *, int, 
  //  GuiLib::TextureHandle, const GuiLib::Vector2f &);

  /// Called by Rocket when it wants to compile geometry it believes will be 
  /// static for the forseeable future.
  //virtual GuiLib::CompiledGeometryHandle CompileGeometry(
  //  GuiLib::Vertex *, int, int *, int, GuiLib::TextureHandle);

  /// Called by Rocket when it wants to render application-compiled geometry.
  //virtual void RenderCompiledGeometry(GuiLib::CompiledGeometryHandle,
                                      //const GuiLib::Vector2f &);
  /// Called by Rocket when it wants to release application-compiled geometry.
  //virtual void ReleaseCompiledGeometry(GuiLib::CompiledGeometryHandle);

  /// Called by Rocket when it wants to enable or disable scissoring to clip content.
  //virtual void EnableScissorRegion(bool);
  /// Called by Rocket when it wants to change the scissor region.
  //virtual void SetScissorRegion(int, int, int, int);

  /// Called by Rocket when a texture is required by the library.
  //virtual bool LoadTexture(GuiLib::TextureHandle &, 
  //  GuiLib::Vector2i &, const GuiLib::String &);
  /// Called by Rocket when a texture is required to be built from an 
  /// internally-generated sequence of pixels.
  //virtual bool GenerateTexture(GuiLib::TextureHandle &, 
  //  const GuiLib::byte *, const GuiLib::Vector2i &);
  /// Called by Rocket when a loaded texture is no longer required.
  //virtual void ReleaseTexture(GuiLib::TextureHandle);

public:
  // ShellRenderInterfaceExtensions
  virtual void SetViewport(int, int);
  virtual void SetContext(void *);
  virtual bool AttachToNative(void *);
  virtual void DetachFromNative(void);
  virtual void PrepareRenderBuffer(void);
  virtual void PresentRenderBuffer(void);

protected:
  int m_width = 0;
  int m_height = 0;
  void * m_rocket_context = nullptr;
  HWND window_handle = NULL;
  HDC device_context = NULL;
  HGLRC render_context = NULL;

public:
  //ShellRenderInterfaceOpenGL(void);
};

#endif
