package com.andrej.breakoutcpp;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.graphics.Point;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.widget.TextView;

public class MainActivity extends Activity
{
    BreakoutEngine breakoutEngine;

    // Used to load the 'native-lib' library on application startup.
    static
    {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        instance = this;

        Display display = getWindowManager().getDefaultDisplay();
        Point size = new Point();
        display.getSize(size);

        // Example of a call to a native method
        breakoutEngine = new BreakoutEngine(this, size.x, size.y);
        setContentView(breakoutEngine);

    }

    @Override
    protected void onResume()
    {
        super.onResume();

        // Tell the gameView resume method to execute
        breakoutEngine.resume();
    }

    // This method executes when the player quits the game
    @Override
    protected void onPause()
    {
        super.onPause();

        // Tell the gameView pause method to execute
        breakoutEngine.pause();
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();

        breakoutEngine.destroy();
    }

    @Override
    protected void onStop()
    {
        super.onStop();

        breakoutEngine.destroy();
    }

    private static MainActivity instance;

    public static MainActivity getInstance()
    {
        return instance;
    }
}
