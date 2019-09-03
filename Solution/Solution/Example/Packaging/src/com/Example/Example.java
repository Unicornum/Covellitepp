
package com.Covellitepp;

import android.os.Bundle;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;

public class Example extends com.alicorn.MainActivity
{
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
    try 
    {
      // Загрузка библиотеки, заданной в AndroidManifest.xml как параметр
      // android.app.lib_name; это нужно из-за того, что инициализация классов
      // Cpp, Contex и Activity должна происходить до запуска кода С++, которое
      // делает класс NativeActivity, но уже после загрузки главного .so файла.

      ActivityInfo ai = getPackageManager().getActivityInfo(
        getIntent().getComponent(), PackageManager.GET_META_DATA);
      String libName = ai.metaData.getString(META_DATA_LIB_NAME);
      System.loadLibrary(libName);
    } 
    catch (PackageManager.NameNotFoundException e) 
    {
      throw new RuntimeException("Error getting activity info", e);
    }

    super.onCreate(savedInstanceState);
	}
}
