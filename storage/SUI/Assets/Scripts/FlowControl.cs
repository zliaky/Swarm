using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Threading;

public class FlowControl : MonoBehaviour {
    
    public GameObject Scenario;
    public GameObject pan;
    private Vector3 tarPan;
    private int isMovWithRobot = 0;
    private int follow_id;
    private bool isbeeRotate = false;
    private SerialListener sl;
    private Scenario sce;
    private bool IsSerial;
    /// <send packge>
    /// according to Serial.sendMsg(...)
    /// </send packge>
    short s_id;
    float s_x;
    float s_y;
    float s_vx;
    float s_vy;
    short s_dA;
    float s_angV;


    public int robots_number = 5;
    private List<RoboState> RoboStateList;

    private bool IsCurframeStart = false;
    private int cur_frame = 0;
    private int should_finishNum = 5;
    // Use this for initialization
    void Start () {
        if(Scenario == null)
        {
            Scenario = GameObject.Find("Scenario");
        }
        if(pan == null)
        {
            pan = GameObject.Find("frisbee");
        }
        sce = Scenario.GetComponent<Scenario>();
        sl = this.GetComponent<SerialListener>();
        IsSerial = this.GetComponent<ModelSelect>().IsOpenSerial;
        RoboStateList = new List<RoboState>(robots_number);
        for (int i = 0; i < robots_number; i++)
        {
            string rob_name = "Robot" + i.ToString();
            RoboStateList.Add(GameObject.Find(rob_name).GetComponent<RoboState>());
        }

        pan.SetActive(false);
    }
	
	// Update is called once per frame
	void Update () {
        //Debug.Log(Input.mousePosition);

        if (cur_frame < sce.frameNum) {
            if (!IsCurframeStart)
            {
                InitialFrame();
                IsCurframeStart = true;
            }
            else
            {
                CheckFrameProgress();
            }

        }
        else
        {
            Debug.Log("END");
        }
        
    }

    void InitialFrame()
    {
        int i;
        int action;
        should_finishNum = 0;
        //robots 0-4
        for (i = 0; i < sce.chaNum - 1; i++)
        {
            action = sce.characters[i].movingMethod[cur_frame];
            //Debug.Log(action+"   "+ sce.characters[i].name);
            switch (action)
            {
                case 0:
                    //freeze
                    var cur_pos = Camera.main.ScreenToWorldPoint(new Vector2(sce.characters[i].curX[cur_frame], sce.characters[i].curY[cur_frame]));
                    RoboStateList[i].gameObject.transform.position = new Vector3(cur_pos.x, cur_pos.y, 0);
                    //RoboStateList[i].setState(1);
                    should_finishNum++;
                    break;
                case 1:
                    //linear motion
                    if (IsSerial)
                    {
                        //Update send packge
                        s_id = System.Convert.ToInt16(i);
                        s_x = sce.characters[i].tarX[cur_frame];
                        s_y = sce.characters[i].tarY[cur_frame];
                        s_vx = 5f;
                        s_vy = 5f;
                        s_dA = 0;
                        s_angV = 0;
                        //Debug.Log("updated send info");
                    }
                    
                    MoveToPos(RoboStateList[i].gameObject, sce.characters[i].tarX[cur_frame], sce.characters[i].tarY[cur_frame]);
                    should_finishNum++;
                    break;
                case 2:
                    //rotation
                    if (IsSerial)
                    {
                        //Update send packge
                        s_id = System.Convert.ToInt16(i);
                        s_x = 0;
                        s_y = 0;
                        s_vx = 0;
                        s_vy = 0;
                        s_dA = System.Convert.ToInt16(sce.characters[i].deltaAngle[cur_frame]);
                        s_angV = 5f;
                    }
                    RotateToAngle(RoboStateList[i].gameObject, sce.characters[i].deltaAngle[cur_frame]);
                    should_finishNum++;
                    break;
                case 3:
                    //others
                    //
                    if (sce.characters[i].curX[cur_frame] > 0)
                    {
                        //cur_pos = Camera.main.ScreenToWorldPoint(new Vector2(sce.characters[i].curX[cur_frame], sce.characters[i].curY[cur_frame]));
                        if (IsSerial)
                        {
                            //Update send packge
                            s_id = System.Convert.ToInt16(i+1);
                            s_x = sce.characters[i].tarX[cur_frame];
                            s_y = sce.characters[i].tarY[cur_frame];
                            s_vx = 5f;
                            s_vy = 5f;
                            s_dA = 0;
                            s_angV = 0;
                        }
                        MoveToPos(RoboStateList[i].gameObject, sce.characters[i].tarX[cur_frame], sce.characters[i].tarY[cur_frame]);
                    }
                    else if(sce.characters[i].curX[cur_frame] == -1)
                    {
                        //MoveToPos(RoboStateList[i].gameObject, sce.characters[i].tarX[cur_frame], sce.characters[i].tarY[cur_frame]);
                        should_finishNum++;
                    }
                    break;
            }
            
        }

        //Debug.Log(should_finishNum);
        //frisbee
        i = sce.chaNum - 1;
        action = sce.characters[i].movingMethod[cur_frame];
        Vector3 temp_world;
        switch (action)
        {
            case 0:
                //Debug.Log("no frisbee in frame " + cur_frame);
                pan.SetActive(false);
                break;
            case 1:
                temp_world = Camera.main.ScreenToWorldPoint(new Vector3(sce.characters[i].curX[cur_frame], sce.characters[i].curY[cur_frame]));
                pan.transform.position = new Vector3(temp_world.x, temp_world.y, 0);
                pan.SetActive(true);
                break;
            case 2:
                temp_world = Camera.main.ScreenToWorldPoint(new Vector2(sce.characters[i].tarX[cur_frame], sce.characters[i].tarY[cur_frame]));
                tarPan = new Vector3(temp_world.x, temp_world.y, 0);
                isMovWithRobot = (int)sce.characters[i].curY[cur_frame];
                follow_id = (int)sce.characters[i].curX[cur_frame];
                if (sce.characters[i].curY[cur_frame] == -1)
                {
                    pan.transform.position = RoboStateList[follow_id].gameObject.transform.position;
                }
                else if(sce.characters[i].curY[cur_frame] == -2)
                {
                    pan.transform.position = RoboStateList[follow_id].gameObject.transform.position - Vector3.Normalize(RoboStateList[follow_id].gameObject.transform.position) * 0.5f;
                }
                else if (sce.characters[i].curY[cur_frame] == -3)
                {
                    temp_world = Camera.main.ScreenToWorldPoint(new Vector2(sce.characters[i].tarX[cur_frame], sce.characters[i].tarY[cur_frame]));
                    tarPan = new Vector3(temp_world.x, temp_world.y, 0);
                }
                pan.SetActive(true);
                break;
            case 3:
                //Debug.Log(action + "   by " + sce.characters[i].name + " cid: " + i + ". in frame: " +cur_frame);
                if(sce.characters[i].movingTime[cur_frame] == 2f)
                {
                    temp_world = Camera.main.ScreenToWorldPoint(new Vector2(sce.characters[i].tarX[cur_frame+1], sce.characters[i].tarY[1+cur_frame]));
                    tarPan = new Vector3(temp_world.x, temp_world.y, 0);
                }
                else if(sce.characters[i].curX[cur_frame] != 0f)
                {
                    if (sce.characters[i].tarY[cur_frame] == -1)
                    {
                        //moving to a robot pos
                        int temp_roboId = (int)sce.characters[i].tarX[cur_frame];
                        tarPan = RoboStateList[temp_roboId].gameObject.transform.position;
                    }
                    else
                    {
                        //moving from cur_pos(x,y) to tar_pos(x,y)
                        temp_world = Camera.main.ScreenToWorldPoint(new Vector2(sce.characters[i].tarX[cur_frame], sce.characters[i].tarY[cur_frame]));
                        tarPan = new Vector3(temp_world.x, temp_world.y, 0);
                        temp_world = Camera.main.ScreenToWorldPoint(new Vector3(sce.characters[i].curX[cur_frame], sce.characters[i].curY[cur_frame]));
                        pan.transform.position = new Vector3(temp_world.x, temp_world.y, 0);
                    }
                }
                
                isbeeRotate = true;
                pan.SetActive(true);
                break;
        }
    }


    void CheckFrameProgress()
    {
        int finish_count = 0;
        foreach (RoboState rs in RoboStateList)
        {
            if (rs.getState() == 1)
            {
                finish_count++;
            }
        }
        

        bool isPanFinish = false;
        if (isMovWithRobot != 0)
        {
            if (isMovWithRobot == -1)
            {
                pan.transform.position = RoboStateList[follow_id].gameObject.transform.position;
                isPanFinish = true;
            }
            else if (isMovWithRobot == -2)
            {
                pan.transform.position = RoboStateList[follow_id].gameObject.transform.position - Vector3.Normalize(RoboStateList[follow_id].gameObject.transform.position) * 0.5f;
                isPanFinish = true;
            }else if(isMovWithRobot == -3)
            {
                if(Vector3.Distance(pan.transform.position, tarPan) > 1f)
                {
                    pan.transform.position = RoboStateList[follow_id].gameObject.transform.position;
                    //Debug.Log("Dis: "+Vector3.Distance(pan.transform.position, tarPan)+"  pos: "+ pan.transform.position);
                    
                    //isPanFinish = false;
                }
                else
                {
                    pan.transform.position = tarPan;
                    isPanFinish = true;
                }
                
            }
            
        }
        else
        {
            Vector2 cur_pan = new Vector2(pan.transform.position.x, pan.transform.position.y);
            if (Vector2.Distance(cur_pan, tarPan) < 0.1f)
            {
                isPanFinish = true;
            }
            else
            {
                //isPanFinish = false;
                pan.transform.position = Vector3.Lerp(pan.transform.position, new Vector3(tarPan.x, tarPan.y, 0), 0.05f);
                if (isbeeRotate)
                {
                    pan.transform.RotateAround(pan.transform.position, pan.transform.forward, pan.transform.rotation.eulerAngles.z + 10f);
                    if (sce.characters[sce.chaNum - 1].movingTime[cur_frame] == 2f)
                    {
                        isPanFinish = true;
                        pan.transform.position = Vector3.Lerp(pan.transform.position, new Vector3(tarPan.x, tarPan.y, 0), 0.05f);
                    }
                }
                    
                
            }
        }
        

        if (finish_count == should_finishNum && isPanFinish)
        {
            Debug.Log("Frame " + cur_frame + " finished, go to next frame.");
            if (should_finishNum == 5)
            {
                foreach (RoboState rs in RoboStateList)
                {
                    GameObject clone = GameObject.Find(rs.gameObject.name + "(Clone)");
                    if (clone != null)
                        Destroy(clone);
                }
            }

            pan.SetActive(false);
            isMovWithRobot = 0;
            isbeeRotate = false;
            cur_frame++;
            IsCurframeStart = false;
        }
    }


    void MoveToPos(GameObject the_robot, float tar_x, float tar_y)
    {
        Vector3 tar_pos = Camera.main.ScreenToWorldPoint(new Vector2(tar_x, tar_y));
        tar_pos = new Vector3(tar_pos.x, tar_pos.y, 0);
        //clone icon to show tar_pos on gui
        GameObject roboClone;
        if (GameObject.Find(the_robot.name + "(Clone)") == null)
        {
            roboClone = (GameObject)Instantiate(the_robot, tar_pos, the_robot.transform.rotation);
            roboClone.GetComponent<RoboState>().enabled = false;
            roboClone.GetComponent<Collider2D>().enabled = false;
            roboClone.GetComponent<SpriteRenderer>().color = new Color(1f, 1f, 1f, 0.4f);
            roboClone.GetComponent<SpriteRenderer>().sortingOrder = 0;
            Destroy(roboClone.transform.GetChild(0).gameObject);

            the_robot.GetComponent<RoboState>().setState(4);

            if (IsSerial)
            {
                try
                {
                    Thread sendThread = new Thread(new ThreadStart(trySend));
                    sendThread.Start();
                    Thread.Sleep(100);
                    sendThread.Abort();
                    Debug.Log("Scenario.sent Move cmd " + s_id +" to " + s_x+", "+s_y);
                }catch(System.Exception e)
                {
                    Debug.Log(e.Message);
                }
            }
        }
    }

    void RotateToAngle(GameObject the_robot, int a)
    {
         //create a clone to save tar_quaternion
        GameObject roboClone;
        if (GameObject.Find(the_robot.name + "(Clone)") == null)
        {
            roboClone = (GameObject)Instantiate(the_robot, the_robot.transform.position, the_robot.transform.rotation);
            roboClone.GetComponent<RoboState>().enabled = false;
            roboClone.GetComponent<Collider2D>().enabled = false;
            roboClone.GetComponent<SpriteRenderer>().color = new Color(1f, 1f, 1f, 0.4f);
            roboClone.GetComponent<SpriteRenderer>().sortingOrder = 0;
            Destroy(roboClone.transform.GetChild(0).gameObject);
            //Debug.Log(roboClone.transform.rotation.eulerAngles+"  before" +a+1);
            //roboClone.transform.rotation = Quaternion.Euler(0f, 0f, the_robot.transform.eulerAngles.z + a+1);
            roboClone.transform.RotateAround(roboClone.transform.position, roboClone.transform.forward,a+1);
            //Debug.Log(roboClone.transform.rotation.eulerAngles + "  after"+ the_robot.transform.eulerAngles.z +"   "+ a+1);
            the_robot.GetComponent<RoboState>().setState(5);

            if (IsSerial)
            {
                try
                {
                    Thread sendThread = new Thread(new ThreadStart(trySend));
                    sendThread.Start();
                    Thread.Sleep(100);
                    sendThread.Abort();
                    Debug.Log("Scenario.sent Rotate cmd " + s_id + " rotate to " + s_dA);
                }
                catch (System.Exception e)
                {
                    Debug.Log(e.Message);
                }
            }
        }
    }

    void trySend()
    {
        try
        {
            sl.SendData(s_id, s_x, s_y, s_vx, s_vy, s_dA, s_angV);
        }catch(System.Exception e)
        {
            Debug.Log(e.Message);
        }
    }
}
