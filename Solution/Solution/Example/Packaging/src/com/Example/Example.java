
package com.Covellitepp;

import android.app.NativeActivity;
import android.view.KeyEvent;

public class Example extends NativeActivity
{
	@Override
    public boolean dispatchKeyEvent(KeyEvent event) 
	{
		int keyaction = event.getAction();
		final int ACTION_KEY_PRESSED = 32;

		if(keyaction == KeyEvent.ACTION_UP) 
		{
			int keyCode = event.getUnicodeChar();
			super.dispatchKeyEvent(new KeyEvent(ACTION_KEY_PRESSED, keyCode));
		} 
		else if(keyaction == KeyEvent.ACTION_MULTIPLE) 
		{
			if (event.getCharacters().length() == 1)
			{
				int keyCode = event.getCharacters().charAt(0);
				return super.dispatchKeyEvent(new KeyEvent(ACTION_KEY_PRESSED, keyCode));
			}
		}

		return super.dispatchKeyEvent(event);
    }
}
