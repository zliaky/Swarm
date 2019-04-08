using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Threading;

public class FlowControl : MonoBehaviour {
    
    public GameObject Scenario;
    private Scenario sce;

    public int robots_number = 5;
    private List<RoboState> RoboStateList;

    private bool IsCurframeStart = false;
    private int cur_frame = 0;
	// Use this for initialization
	void Start () {
        sce = Scenario.GetComponent<Scenario>();

        RoboStateList = new List<RoboState>(robots_number);
        for (int i = 0; i < robots_number; i++)
        {
            string rob_name = "Robot" + i.ToString();
            RoboStateList.Add(GameObject.Find(rob_name).GetComponent<RoboState>());
        }
    }
	
	// Update is called once per frame
	void Update () {

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
        for (int i = 0; i < sce.chaNum - 1; i++)
        {
            int action = sce.characters[i].movingMethod[cur_frame];
            //Debug.Log(action+"   "+ sce.characters[i].name);
            switch (action)
            {
                case 0:
                    //freeze
                    var cur_pos = Camera.main.ScreenToWorldPoint(new Vector2(sce.characters[i].curX[cur_frame], sce.characters[i].curY[cur_frame]));
                    RoboStateList[i].gameObject.transform.position = new Vector3(cur_pos.x, cur_pos.y, 0);
                    break;
                case 1:
                    //linear motion
                    MoveToPos(RoboStateList[i].gameObject, sce.characters[i].tarX[cur_frame], sce.characters[i].tarY[cur_frame]);
                    break;
                case 2:
                    //rotation
                    RotateToAngle(RoboStateList[i].gameObject, sce.characters[i].deltaAngle[cur_frame]);
                    break;
                case 3:
                    //others
                    break;
            }
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
            else
            {
                break;
            }
        }

        if (finish_count == 5)
        {
            Debug.Log("Frame " + cur_frame + " finished, go to next frame.");
            foreach (RoboState rs in RoboStateList)
            {
                GameObject clone = GameObject.Find(rs.gameObject.name + "(Clone)");
                if (clone != null)
                    Destroy(clone);
            }

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
            Debug.Log(roboClone.transform.rotation.eulerAngles+"  before" +a+1);
            //roboClone.transform.rotation = Quaternion.Euler(0f, 0f, the_robot.transform.eulerAngles.z + a+1);
            roboClone.transform.RotateAround(roboClone.transform.position, roboClone.transform.forward,a+1);
            Debug.Log(roboClone.transform.rotation.eulerAngles + "  after"+ the_robot.transform.eulerAngles.z +"   "+ a+1);
            the_robot.GetComponent<RoboState>().setState(5);
            
        }
    }
}
