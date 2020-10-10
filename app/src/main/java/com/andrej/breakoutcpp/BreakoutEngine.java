package com.andrej.breakoutcpp;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.Log;
import android.view.MotionEvent;
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

    private long holdTime;
    private long startHoldTime;

    private float touchX;
    private float touchY;

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

        holdTime = 0;
        startHoldTime = 0;

        touchX = 0;
        touchY = 0;

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
                Update(fps);
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

            // Draw player paddle
            paint.setColor(Color.argb(255, 0, 0, 0));
            canvas.drawRect(GetPlayerLeft(), GetPlayerTop(), GetPlayerRight(), GetPlayerBottom() ,paint);
            canvas.drawCircle(GetPlayerLeft(), GetPlayerTop() - GetPlayerRadius(), GetPlayerRadius(), paint);
            canvas.drawCircle(GetPlayerRight(), GetPlayerTop() - GetPlayerRadius(), GetPlayerRadius(), paint);

            // Draw ball
            paint.setColor(Color.argb(255, 255, 255, 255));
            canvas.drawCircle(GetBallXPos(), GetBallYPos(), GetBallRadius(), paint);

            paint.setColor(Color.argb(255, 0, 0, 0));
            paint.setTextSize(70);
            canvas.drawText("ballX: " + GetBallXPos() + " debug value: " + DebugValue(1), 10,80, paint);

            ourHolder.unlockCanvasAndPost(canvas);
        }
    }

    public void HoldTime(boolean hold)
    {
        if(hold)
        {
            startHoldTime = System.currentTimeMillis();
        }
        else
        {
            holdTime = System.currentTimeMillis() - startHoldTime;
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent motionEvent)
    {
        switch (motionEvent.getAction() & MotionEvent.ACTION_MASK)
        {

            case MotionEvent.ACTION_DOWN:
                paused = false;
                SetOffsetSet(false);
                HoldTime(true);

                break;

            case MotionEvent.ACTION_MOVE:
                touchX = motionEvent.getX();
                touchY = motionEvent.getY();
                TouchListener(motionEvent.getX(), motionEvent.getY());
                break;

            case MotionEvent.ACTION_UP:
                SetPaddleIsTouched(false);
                HoldTime(false);
                LaunchBall(holdTime);
                break;
        }
        return true;
    }

    // Native methods

    // Engine methods
    public native void Init(int screenX, int screenY);
    public native void Update(long fps);
    public native void SetOffsetSet(boolean value);
    public native void TouchListener(float touchPointX, float touchPointY);
    public native void SetPaddleIsTouched(boolean value);
    public native float DebugValue(float value);

    // player methods
    public native float GetPlayerTop();
    public native float GetPlayerLeft();
    public native float GetPlayerRight();
    public native float GetPlayerBottom();
    public native float GetPlayerRadius();
    public native void LaunchBall(long holdTime);

    // ball methods
    public native float GetBallXPos();
    public native float GetBallYPos();
    public native float GetBallRadius();

}
