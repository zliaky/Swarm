using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Threading;
using UnityEngine.UI;

public class SerialListener : MonoBehaviour {
    public int robo_number = 5;
    //public System.String PortNum;
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
            tmp = s.initSerial("\\\\.\\COM22".ToCharArray());

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
                //GameObject theRobot = GameObject.Find("Robot" + s.robo_frame.id.ToString());
                GameObject theRobot = GameObject.Find("Robot" + (s.robo_frame.id - 1).ToString());
                if(theRobot == null)
                {
                    Debug.Log("Robot" + (s.robo_frame.id - 1).ToString() + " are not found");
                }
                else
                {
                  //  Debug.Log("Received from "+s.robo_frame.id + "  : " + s.robo_frame.x + "," + s.robo_frame.y + "," + s.robo_frame.angle);
                }
                if(theRobot== null)
                    Debug.Log(s.robo_frame.id + "," + s.robo_frame.x + "," + s.robo_frame.y + "," + s.robo_frame.angle);
                if (this.GetComponent<ModelSelect>().GUI_Control)
                {
                    if (theRobot.GetComponent<RoboState>().getState() != 2)
                    {
                        theRobot.GetComponentInChildren<Text>().text
                        = "( " + s.robo_frame.x + ", " + s.robo_frame.y + ", " + s.robo_frame.angle + " )";
                    }
                    if (theRobot.GetComponent<RoboState>().getState() == 4)
                    {
                        Vector3 temp_v = Camera.main.ScreenToWorldPoint(new Vector2(s.robo_frame.x, s.robo_frame.y));
                        theRobot.transform.position = new Vector3(temp_v.x, temp_v.y ,0);
                        
                    }
                    else if (theRobot.GetComponent<RoboState>().getState() == 5)
                    {
                        theRobot.transform.rotation = Quaternion.Euler(0f,0f, theRobot.transform.rotation.eulerAngles.z + s.robo_frame.angle);
                        //theRobot.transform.rotation.eulerAngles.z + s.robo_frame.angle || s.robo_frame.angle
                    }
                }
                else if (this.GetComponent<ModelSelect>().Scenario_Play)
                {
                    if (theRobot.GetComponent<RoboState>().getState() == 4)
                    {
                        float temp_y = Screen.height - s.robo_frame.y * 1.37f - 120f;
                        Vector3 temp_v = Camera.main.ScreenToWorldPoint(new Vector2(s.robo_frame.x * 1.35f, temp_y));
                        theRobot.transform.position = new Vector3(temp_v.x, temp_v.y, 0);
                        //Debug.Log(theRobot.name+"moving  to "+temp_v);
                    }
                    else if (theRobot.GetComponent<RoboState>().getState() == 5)
                    {
                        theRobot.transform.rotation = Quaternion.Euler(0f, 0f, theRobot.transform.rotation.eulerAngles.z + s.robo_frame.angle);
                        //theRobot.transform.rotation.eulerAngles.z + s.robo_frame.angle || s.robo_frame.angle
                    }
                }
                else if (this.GetComponent<ModelSelect>().Debug_Window)
                {
                    if (theRobot.GetComponent<RoboState>().getState() != 2)
                    {
                        theRobot.GetComponentInChildren<Text>().text
                        = "( " + s.robo_frame.x + ", " + s.robo_frame.y + ", " + s.robo_frame.angle + " )";
                    }
                    Vector3 temp_v = Camera.main.ScreenToWorldPoint(new Vector2(s.robo_frame.x, s.robo_frame.y));
                    theRobot.transform.position = new Vector3(temp_v.x, temp_v.y, 0);
                    //theRobot.transform.rotation = Quaternion.Euler(0f,0f, theRobot.transform.rotation.eulerAngles.z + s.robo_frame.angle);
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
            //s.sendMsg(robo_moniter.send_id, robo_moniter.send_x, robo_moniter.send_y, robo_moniter.send_v, 0, 0, 0);
            //s.sendMsg(robo_moniter.send_id, robo_moniter.send_mode, robo_moniter.send_x, robo_moniter.send_y);
        }
        catch (System.Exception ex)
        {
            Debug.Log(ex.Message);
        }
    }

    public void SendData(short id, short mode, float x, float y)
    {
        try
        {
            //s.sendMsg(id, x, y, vx, vy, dA, angV);
            s.sendMsg(id, mode, x, y);
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
                    Debug.Log(s.robo_frame.id + "   : " + s.robo_frame.x + "," + s.robo_frame.y + "," + s.robo_frame.angle);

                    //Update infor on GUI

                    if (s.robo_frame.id-1 < robo_number && s.robo_frame.id-1 > -1)
                    {
                        isUapdatingInfo = true;
                    }
                    Thread.Sleep(10);
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
