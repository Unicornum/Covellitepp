  
package com.Simple;
  
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;
import android.os.SystemClock;
  
public class BackgroundService extends Service
{
  @Override
  public void onCreate() 
  {
    System.loadLibrary("Simple");
      
    Thread BackgroundThread = new Thread(new Runnable() 
    {
      @Override
      public void run() 
      {
        int Index = 0;
		  
        while(true) 
        {
          Log.v("Service", "Index: " + Index);
          Index++;
		    
          SystemClock.sleep(1000);
        }
      }
    });
	  
    BackgroundThread.setPriority(Thread.MAX_PRIORITY);
    BackgroundThread.start();
  }
    
  @Override
  public int onStartCommand(Intent intent, int flags, int startId) 
  {
    return START_STICKY;
  }
    
  @Override
  public IBinder onBind(Intent intent) 
  {
    return null;
  }
}
