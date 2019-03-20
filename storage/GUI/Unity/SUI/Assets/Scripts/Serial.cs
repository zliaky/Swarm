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
    private static extern bool initWrapper();

    [DllImport("serial_wrapper")]
    private static extern void loopWrapper();

    [DllImport("serial_wrapper")]
    private static extern void getInfor(ref Robo_frame f);

    private Robo_frame robo_frame = new Robo_frame();
    private bool isInitial = false;
    static int roboId;
    static Vector2 roboPos;
    static float roboAngle;

    private void Start()
    {
        isInitial = initWrapper();
        if (isInitial)
        {
            Debug.Log("true");
            loopWrapper();
        }
        else
            Debug.Log(isInitial);
    }

    private void Update()
    {
        if (isInitial)
        {
            getInfor(ref robo_frame);
            Debug.Log(robo_frame);
        }
        roboId = robo_frame.id;
        roboPos = new Vector2(robo_frame.x, robo_frame.y);
        roboAngle = robo_frame.angle;
    }


    ///provide to Unity
    static Vector2 getPos(int id, Vector2 prePos)
    {
        Vector2 pos = new Vector2();
        if (roboId == id)
            pos = roboPos;
        else
            pos = prePos;

        return pos;
    }
}
