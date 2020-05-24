
package com.Covellitepp;

public class Example extends com.alicorn.MainActivity
{
  static
  {
    System.loadLibrary("c++_shared");
  }

  @Override
  public void onUserLeaveHint() 
  {
    finish();
  }
}
