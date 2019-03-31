using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class RoboState : MonoBehaviour {
    private int crt_state;
    /// </crt_state>
    /// 1 == "free"
    /// 2 == "draging following pointer"
    /// 3 == "holding by hand"
    /// 4 == "moving"
    /// </crt_state>
    //Camera main_cam;
    //GameObject Panel;

    // Use this for initialization
    void Start () {
        crt_state = 0;
        //main_cam = GameObject.Find("Main Camera").GetComponent<Camera>();
        //Panel = GameObject.Find("Panel" + getRobotNum(this.name));
        //if (Panel != null)
        //{
        //    textFiled = Panel.GetComponentInChildren<Text>();
        //}
        //else
        //    textFiled = null;
    }
	
	// Update is called once per frame
	void Update () {
        switch (crt_state)
        {
            case 0:
                this.GetComponent<SpriteRenderer>().color = Color.white;
                //Debug.Log("There is " + this.name + ", I still stay in the home...");
                break;
            case 1:
                this.GetComponent<SpriteRenderer>().color = Color.white;
                //Debug.Log("There is " + this.name + ", I'm waiting for your command!");
                break;
            case 2:
                moveWithPointer();
                this.GetComponent<SpriteRenderer>().color = new Color(0.5f, 0.098f, 0f);
                showPosText();
                //Debug.Log("There is " + this.name + ", I'm catching by someone");
                break;
            case 4:
                //check if arriving destination
                //wating for msg from serial port

                this.GetComponent<SpriteRenderer>().color = new Color(0.7f, 1f, 0.7f);
                gotoDestination();
                //Debug.Log("There is " + this.name + ", I'm moving to where you want");
                break;
        }
	}

    private void moveWithPointer() {
        //var ray = main_cam.ScreenToWorldPoint(Input.mousePosition);
        var pos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
        pos = new Vector3(pos.x, pos.y, 0);
        GameObject clone = GameObject.Find(this.name + "(Clone)");
        if (clone != null)
        {
            clone.transform.position = Vector3.Lerp(clone.transform.position, pos, 0.5f);
        }
        else {
            this.transform.position = Vector3.Lerp(this.transform.position, pos, 0.5f);
        }
        //RaycastHit2D hit = Physics2D.Raycast(ray, Vector2.zero, LayerMask.GetMask("UI"));
        //if (hit.collider != null)
        //    //this.transform.position = Vector3.Lerp(this.transform.position, hit.point, 0.1f);
        //    this.transform.position = hit.point;
    }

    void gotoDestination() {
        GameObject clone = GameObject.Find(this.name+"(Clone)");
        if (clone != null && Vector3.Distance(this.transform.position, clone.transform.position)>0.01f)
        {
            //Serial.getPos()
            this.transform.position = Vector3.Lerp(this.transform.position, clone.transform.position, 0.05f);
        }
        else if (clone != null && Vector3.Distance(this.transform.position, clone.transform.position) < 0.01f) {
            this.transform.position = clone.transform.position;
            Destroy(clone);
            crt_state = 1;
        }
    }

    private void showPosText()
    {
        GameObject clone = GameObject.Find(this.name + "(Clone)");
        Vector3 screen_position = Camera.main.WorldToScreenPoint(clone.transform.position);
        this.GetComponentInChildren<Text>().text = "(" + this.name + ", " + screen_position + ")";
        //Panel.transform.position = new Vector3(this.transform.position.x + (Screen.width / 919f), this.transform.position.y - (Screen.height / 1024f), this.transform.position.z);
    }

    private string getRobotNum(string name)
    {
        char[] my_name = name.ToCharArray();
        int num = my_name[my_name.Length - 1] - 48;
        return num.ToString();
    }

    public void setState(int state_idx) {
        crt_state = state_idx;
    }

    public int getState()
    {
        return crt_state;
    }
}
