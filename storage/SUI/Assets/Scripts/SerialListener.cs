﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Threading;
using UnityEngine.UI;

public class SerialListener : MonoBehaviour {
    public int robo_number = 5;
    Serial s;
    private Thread dataRecvThread;
    private Thread sendThread;
    bool isUapdatingInfo = false;
    bool tmp = false;
    float time;
    RoboMoniter robo_moniter;
	// Use this for initialization
	void Start () {
        robo_moniter = GameObject.Find("Moniter").GetComponent<RoboMoniter>();
        try
        {
            s = new Serial();
            tmp = s.initSerial("COM5".ToCharArray());
            Debug.Log("initial: " + tmp);

            if (tmp)
            {
                dataRecvThread = new Thread(new ThreadStart(DataRecv));
                dataRecvThread.Start();
            }
        }
        catch (System.Exception ex)
        {
            Debug.Log(ex.Message);
        }
	}

    private void Update()
    {
        if (isUapdatingInfo)
        {
            try
            {
                GameObject theRobot = GameObject.Find("Robot" + s.robo_frame.id.ToString());
                if (theRobot.GetComponent<RoboState>().getState() != 2)
                {
                    theRobot.GetComponentInChildren<Text>().text
                    = "( " + s.robo_frame.x + ", " + s.robo_frame.y + ", " + s.robo_frame.angle + " )";
                }
                if(theRobot.GetComponent<RoboState>().getState() ==4)
                {
                    //theRobot.transform.position = new Vector3(s.robo_frame.x,s.robo_frame.y,0);
                    Vector3 tar_pos = Camera.main.ScreenToWorldPoint(new Vector2(s.robo_frame.x, s.robo_frame.y)); 
                    theRobot.transform.position = new Vector3(tar_pos.x, tar_pos.y, 0); ;
                }
                else if(theRobot.GetComponent<RoboState>().getState() == 5)
                {
                    theRobot.transform.rotation = Quaternion.Euler(0f,0f, theRobot.transform.rotation.eulerAngles.z + s.robo_frame.angle);
                    //theRobot.transform.rotation.eulerAngles.z + s.robo_frame.angle || s.robo_frame.angle
                }
                isUapdatingInfo = false;
            }
            catch(System.Exception ex)
            {
                Debug.Log(ex.Message);
            }
        }
        //check if send msg
        if (robo_moniter.isSend)
        {
            robo_moniter.isSend = false;
            try
            {
                sendThread = new Thread(new ThreadStart(SendData));
                sendThread.Start();
                Thread.Sleep(100);
                sendThread.Abort();
                Debug.Log("sent msg");
            }
            catch (System.Exception ex)
            {
                Debug.Log(ex.Message);
            }

        }
    }

    public void SendDebug(int id, int dir1, int pwm1, int dir2, int pwm2, int dir3, int pwm3)
    {
        
        try
        {
            s.sendDebug(id, dir1, pwm1, dir2, pwm2, dir3, pwm3);
        }
        catch (System.Exception ex)
        {
            Debug.Log(ex.Message);
        }
    }

    public void SendData()
    {
        try
        {
            s.sendMsg(robo_moniter.send_id, robo_moniter.send_x, robo_moniter.send_y, robo_moniter.send_v, 0, 0, 0);
        }
        catch (System.Exception ex)
        {
            Debug.Log(ex.Message);
        }
    }

    void DataRecv()
    {
        while (true)
        {
            try
            {
                if (!isUapdatingInfo)
                {
                    s.recvMsg();
                    s.getFrame();
                    //Debug.Log(s.robo_frame);
                    //Debug.Log(s.robo_frame.id + "," + s.robo_frame.x + "," + s.robo_frame.y + "," + s.robo_frame.angle);

                    //Update infor on GUI

                    if (s.robo_frame.id < robo_number && s.robo_frame.id > -1)
                    {
                        isUapdatingInfo = true;
                    }
                    Thread.Sleep(50);
                }
            }
            catch (System.Exception ex)
            {
                Debug.Log(ex.Message);
            }
        }
    }
    private void OnApplicationQuit()
    {
        try
        {
            if (Application.platform == RuntimePlatform.WindowsEditor)
            {
                int close = s.closeSerial();
                Debug.Log("close:" + close);
            }
            if (dataRecvThread != null)
            {
//                dataRecvThread.Interrupt();
                dataRecvThread.Abort();
            }
            if (sendThread != null)
            {
                sendThread.Abort();
            }
        }
        catch (System.Exception ex)
        {
            Debug.Log(ex.Message);
        }
    }
}