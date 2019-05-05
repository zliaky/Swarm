using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

public class Serial : MonoBehaviour {
    [StructLayout(LayoutKind.Sequential)]
    public struct Robo_frame
    {
        public short id;
        public float x;
        public float y;
        public short angle;
    }

    [DllImport("serial_wrapper")]
    private static extern bool initWrapper(char[] com);

    [DllImport("serial_wrapper")]
    private static extern int closeWrapper();

    [DllImport("serial_wrapper")]
    private static extern void sendWrapper(short id, short mode, float x, float y);

    [DllImport("serial_wrapper")]
    private static extern void recvWrapper();

    [DllImport("serial_wrapper")]
    private static extern void getInfor(ref Robo_frame f);

    [DllImport("serial_wrapper")]
    private static extern void sendDebugWrapper(int id, int dir1, int pwm1, int dir2, int pwm2, int dir3, int pwm3);


    public Robo_frame robo_frame = new Robo_frame();
    static int roboId;
    static Vector2 roboPos;
    static float roboAngle;

    public bool initSerial(char[] com)
    {
        return initWrapper(com);
    }
    public void sendMsg(short id, short mode, float x, float y)
    {
        //short send_id = System.Convert.ToInt16(id + 1);
        sendWrapper(id, mode, x, y);
        //sendWrapper(send_id, x, y, vx, vy, dA, angV);
        
    }
    public void recvMsg()
    {
        recvWrapper();
    }

    public void getFrame()
    {
        getInfor(ref robo_frame);
    }

    public int closeSerial()
    {
        return closeWrapper();
    }

    public void sendDebug(int id, int dir1, int pwm1, int dir2, int pwm2, int dir3, int pwm3)
    {
        sendDebugWrapper(id, dir1, pwm1, dir2, pwm2, dir3, pwm3);
    }
    
}
