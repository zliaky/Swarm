using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RoboMoniter : MonoBehaviour {
    public int robots_number = 5;
    public float homeline = -3.11f;
    private Vector3 home_point;
    private List<GameObject> RobotsList;
    private Camera main_cam;
    private RoboState hold_state;
    private GameObject roboClone;
    void Start () {
        RobotsList = new List<GameObject>(robots_number);
        for(int i = 0; i < robots_number; i++)
        {
            string rob_name = "Robot" + i.ToString();
            RobotsList.Add(GameObject.Find(rob_name));
        }

        main_cam = GameObject.Find("Main Camera").GetComponent<Camera>();
        //ray = main_cam.ScreenPointToRay(Input.mousePosition);
    }
	
	// Update is called once per frame
	void Update () {
        if (Input.GetMouseButtonDown(0))
        {
            Vector3 ray = main_cam.ScreenToWorldPoint(Input.mousePosition);
            RaycastHit2D hit = Physics2D.Raycast(ray, Vector2.zero, LayerMask.GetMask("Robot"));
            if (hit.collider!= null && RobotsList.Contains(hit.transform.gameObject))
            {
                hold_state = hit.transform.gameObject.GetComponent<RoboState>();
                //clone a robot
                gettheClone(hit.transform.gameObject);
                hold_state.setState(2);
                print(hold_state.name);
            }
        }
        else if (Input.GetMouseButtonUp(0) && hold_state != null) {
            int num = getRobotNum(hold_state.gameObject);
            home_point = new Vector3(3.01f + num * 0.78f, -3.26f, 0f);
            if (roboClone.transform.position.y < homeline && roboClone.transform.position != home_point)
            {
                roboClone.transform.position = home_point;
            }
            hold_state.setState(4);
            //send command to serial port. moving robot.
            hold_state = null;
        }
	}

    int getRobotNum(GameObject robo)
    {
        char[] my_name = robo.name.ToCharArray();
        int num = my_name[my_name.Length - 1] - 48;
        return num;
    }

    void gettheClone(GameObject the_robot) {
        if(GameObject.Find(the_robot.name+"(Clone)") == null)
        {
            roboClone = (GameObject)Instantiate(the_robot, the_robot.transform.position, the_robot.transform.rotation);
            roboClone.GetComponent<RoboState>().enabled = false;
            roboClone.GetComponent<Collider2D>().enabled = false;
            roboClone.GetComponent<SpriteRenderer>().color = new Color(1f, 1f, 1f, 0.4f);
            roboClone.GetComponent<SpriteRenderer>().sortingOrder = 0;
        }
    }

}
