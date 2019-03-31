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
    private static extern void sendWrapper(short id, float x, float y, float vx, float vy, short dA, float angV);

    [DllImport("serial_wrapper")]
    private static extern void recvWrapper();

    [DllImport("serial_wrapper")]
    private static extern void getInfor(ref Robo_frame f);
    

    public Robo_frame robo_frame = new Robo_frame();
    static int roboId;
    static Vector2 roboPos;
    static float roboAngle;

    public bool initSerial(char[] com)
    {
        return initWrapper(com);
    }
    public void sendMsg(short id, float x, float y, float vx, float vy, short dA, float angV)
    {
        sendWrapper(id, x, y, vx, vy, dA, angV);
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
    
}
