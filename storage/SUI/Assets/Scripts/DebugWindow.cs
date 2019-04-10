using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Threading;

public class DebugWindow : MonoBehaviour {
    public bool IsMultiSendThread = false;
    Thread TrysThread;
    SerialListener sl;
    string[] pwm1s = new string[5];
    string[] pwm2s = new string[5];
    string[] pwm3s = new string[5];

    bool[] dir1s = new bool[5];
    bool[] dir2s = new bool[5];
    bool[] dir3s = new bool[5];
    int[] send_msg = new int[7];

    private void Start()
    {
        sl = this.GetComponent<SerialListener>();
        for(int i = 0; i < pwm1s.Length; i++)
        {
            if (i == 0)
            {
                pwm1s[i] = "150";
                pwm2s[i] = "150";
                pwm3s[i] = "0";
            }
            else
            {
                pwm1s[i] = "";
                pwm2s[i] = "";
                pwm3s[i] = "";
            }

            dir1s[i] = false;
            dir2s[i] = false;
            dir3s[i] = false;
        }
    }

    private void OnGUI()
    {
        GUI.skin.textField.fontSize = 40;
        GUI.skin.button.fontSize = 45;
        GUI.skin.toggle.fontSize = 40;
        

        for (int i=0;i<5;i++)
        {
            GUILayout.BeginArea(new Rect(25+i*420, 20, 390, 200));
            //GUILayout.BeginVertical();
            GUILayout.BeginHorizontal();
            pwm1s[i] = GUILayout.TextField(pwm1s[i], GUILayout.Height(50), GUILayout.Width(130));
            pwm2s[i] = GUILayout.TextField(pwm2s[i], GUILayout.Height(50), GUILayout.Width(130));
            pwm3s[i] = GUILayout.TextField(pwm3s[i], GUILayout.Height(50), GUILayout.Width(130));
            GUILayout.EndHorizontal();

            GUILayout.BeginHorizontal();
            dir1s[i] = GUILayout.Toggle(dir1s[i], "dir1");
            dir2s[i] = GUILayout.Toggle(dir2s[i], "dir2");
            dir3s[i] = GUILayout.Toggle(dir3s[i], "dir3");
            GUILayout.EndHorizontal();
            //GUILayout.EndVertical();
            string btn_name = "Send to robot" + i.ToString();
            if (GUILayout.Button(btn_name))
            {
                send_msg = InputToInt(i, dir1s[i], dir2s[i],dir3s[i],pwm1s[i],pwm2s[i],pwm3s[i]);
                //call the new send function
                if (IsMultiSendThread)
                {
                    TrySend();
                }
                else
                {
                    try
                    {
                        TrysThread = new Thread(new ThreadStart(TrySend));
                        TrysThread.Start();
                        Thread.Sleep(50);
                        TrysThread.Abort();
                    }
                    catch (System.Exception ex)
                    {
                        Debug.Log(ex.Message);
                    }
                }

            }
            GUILayout.EndArea();
        }
        
    }

    int[] InputToInt(int id, bool d1, bool d2, bool d3, string p1, string p2, string p3)
    {
        int[] msg_pack = new int[7];
        msg_pack[0] = id;
        if (d1)
            msg_pack[1] = 1;
        else
            msg_pack[1] = 0;
        msg_pack[2] = int.Parse(p1.Trim());
        if (d2)
            msg_pack[3] = 1;
        else
            msg_pack[3] = 0;
        msg_pack[4] = int.Parse(p2.Trim());
        if (d3)
            msg_pack[5] = 1;
        else
            msg_pack[5] = 0;
        msg_pack[6] = int.Parse(p3.Trim());

        return msg_pack;
    }

    void TrySend()
    {
        try
        {
            sl.SendDebug(send_msg[0], send_msg[1], send_msg[2], send_msg[3], send_msg[4], send_msg[5], send_msg[6]);
            Debug.Log("sent msg");
        }
        catch (System.Exception ex)
        {
            Debug.Log(ex);
        }
    }

    private void OnApplicationQuit()
    {
        if (TrysThread != null)
        {
            TrysThread.Abort();
        }
    }
}
