package com.andrej.breakoutcpp;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class BreakoutEngine extends SurfaceView implements Runnable
{
    private Thread gameThread = null;

    private SurfaceHolder ourHolder;

    private volatile boolean playing;

    private boolean paused = true;

    private Canvas canvas;
    private Paint paint;

    private int screenX;
    private int screenY;

    private long fps;

    private long timeThisFrame;

    // Our constructor
    public BreakoutEngine(Context context, int x, int y)
    {
        super(context);

        // Initialize ourHolder and paint objects
        ourHolder = getHolder();
        paint = new Paint();

        // Initialize screenX and screenY because x and y are local
        screenX = x;
        screenY = y;

        Init(screenX,screenY);

    }

    public void pause()
    {
        playing = false;
        try
        {
            gameThread.join();
        }
        catch (InterruptedException e)
        {
            Log.e("Error:", "joining thread");
        }
    }

    public void resume()
    {
        playing = true;
        gameThread = new Thread(this);
        gameThread.start();
    }

    @Override
    public void run()
    {
        while (playing)
        {
            long startFrameTime = System.currentTimeMillis();

            if (!paused)
            {
                update(); // should probably replace with cpp function rather than calling Java
            }

            draw();

            timeThisFrame = System.currentTimeMillis() - startFrameTime;
            if (timeThisFrame >= 1)
            {
                fps = 1000 / timeThisFrame;
            }
        }
    }

    private void update() // probably better to replace
    {
        // this method will be calling c++ calculation methods
    }

    private void draw()
    {
        if (ourHolder.getSurface().isValid())
        {
            canvas = ourHolder.lockCanvas();

            // Set canvas color to blue
            canvas.drawColor(Color.argb(255,26,128,182));

            // Draw the bat
            paint.setColor(Color.argb(255, 0, 0, 0));
            canvas.drawRect(GetPlayerLeft(), GetPlayerTop(), GetPlayerRight(), GetPlayerBottom() ,paint);

            paint.setColor(Color.argb(255, 255, 255, 255));
            paint.setTextSize(70);
            canvas.drawText("PlayerRight: " + GetPlayerRight() + " PlayerTop: " + GetPlayerTop(), 10,80, paint);

            ourHolder.unlockCanvasAndPost(canvas);
        }
    }

    public native void Init(int screenX, int screenY);
    public native float GetPlayerTop();
    public native float GetPlayerLeft();
    public native float GetPlayerRight();
    public native float GetPlayerBottom();

}
