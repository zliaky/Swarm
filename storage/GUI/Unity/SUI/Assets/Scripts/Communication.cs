using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Communication : MonoBehaviour {
    
	// Use this for initialization
	void Start () {
		
	}
	

    /// <summary>
    /// maybe implement later
    /// 
    ///  static bool isHoldbyHand()
    ///{
    ///    bool state = false;
    ///    // dll.getRobotHolding()
    ///    return state;
    ///}
    /// </summary>
    /// <returns></returns>


    static Vector3 getRobotPos(int robot_id)
    {
        Vector3 robopos = new Vector3();
        //dll.getrobopos(float x_pos, float y_pos, float angel)
        return robopos;
    }

    static void sendGoalPos(int robot_id, float x_pos, float y_pos)
    {
        //dll.gotoPosition(name, pos)
    }
}
