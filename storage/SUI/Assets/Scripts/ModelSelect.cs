using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ModelSelect : MonoBehaviour {
    public bool GUI_Control = false;
    public bool Scenario_Play = false;
    public bool Debug_Window = true;

    public bool IsOpenSerial = false;
    // Use this for initialization
    void Start () {
        if (GUI_Control) {
            this.GetComponent<RoboMoniter>().enabled = true;
            this.GetComponent<FlowControl>().enabled = false;
            this.GetComponent<DebugWindow>().enabled = false;
        }
        else if (Scenario_Play)
        {
            this.GetComponent<RoboMoniter>().enabled = false;
            this.GetComponent<FlowControl>().enabled = true;
            this.GetComponent<DebugWindow>().enabled = false;
        }
        else
        {
            //Debug
            this.GetComponent<RoboMoniter>().enabled = false;
            this.GetComponent<FlowControl>().enabled = false;
            this.GetComponent<DebugWindow>().enabled = true;
        }


        if (IsOpenSerial)
            this.GetComponent<SerialListener>().enabled = true;
        else
            this.GetComponent<SerialListener>().enabled = false;
    }
	
	// Update is called once per frame
	void Update () {
		
	}
}
