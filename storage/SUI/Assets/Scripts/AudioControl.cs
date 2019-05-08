using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AudioControl : MonoBehaviour {
    public FlowControl events;
    public Scenario sce;
    private AudioSource[] audioList = new AudioSource[7];
    public RoboState Dog1;
    public RoboState Dog4;
    public RoboState shaun;
    public Transform master;
    public Transform frisbee;
    private Quaternion preframe_rotation;
    private bool IsbeeStop = true;
    private bool IsMasterPlay = false;
    // Use this for initialization
    void Start () {
        
        audioList = this.GetComponents<AudioSource>();
        preframe_rotation = frisbee.rotation;

    }
	
	// Update is called once per frame
	void Update () {
        bool IsInter = events.getPerformState();
        int frame = events.getCurFrame();
        if(frame == 0 && events.IsLoop && !audioList[0].isPlaying)
        {
            audioList[0].Play();
            if (audioList[7].isPlaying)
                audioList[7].Stop();
        }else if(frame == sce.frameNum && !audioList[7].isPlaying)
        {
            audioList[7].Play();
        }

        if (IsInter && frame==2)
        {
            //dec op vol, then stop
            if (events.IsFullyOp)
            {
                //if (!audioList[0].isPlaying)
                //    audioList[0].Play();
            }
            else
            {
                audioList[0].volume = Mathf.Lerp(audioList[0].volume, 0f, .01f);
                if (!audioList[1].isPlaying)
                    audioList[1].Play();
                else
                    audioList[1].volume = Mathf.Lerp(audioList[1].volume, 1f, .1f);
            }
        }
        else if (!IsInter && frame == 2)
        {
            audioList[0].Stop();
            audioList[0].volume = 1f;
            //adjust bgm vol, play dog
            if (audioList[1].volume < 1f)
            {
                audioList[1].volume = Mathf.Lerp(audioList[1].volume, 1f, .5f);
            }
            audioList[2].volume = 0f;
            audioList[2].Play();
        }
        if(frame>1)
        {
            //Dog1
            if (Dog1.getState() != 1)
            {
                audioList[2].volume = Mathf.Lerp(audioList[2].volume, 1f, .3f);
            }
            else
            {
                if (audioList[2].volume != 0f)
                {
                    audioList[2].volume = Mathf.Lerp(audioList[2].volume, 0f, .1f);
                }
            }

            //frisbee
            if (frisbee.rotation != preframe_rotation && !audioList[6].isPlaying && IsbeeStop)
            {
                audioList[6].Play();
                audioList[6].loop = false;
                IsbeeStop = false;
            }else if(frisbee.rotation == preframe_rotation)
            {
                IsbeeStop = true;

            }
            preframe_rotation = frisbee.rotation;

            //shaun
            if (shaun.getState() == 4 && !audioList[3].isPlaying)
            {
                audioList[3].Play();
            }else if(shaun.getState() != 4 && audioList[3].isPlaying)
            {
                audioList[3].Stop();
            }

            //master leave
            Vector2 master_pos = Camera.main.WorldToScreenPoint(master.position);
            //Debug.Log(Vector2.Distance(master_pos, new Vector2(Screen.width, master_pos.y)));
            if (Vector2.Distance(master_pos, new Vector2(Screen.width,master_pos.y)) < 110f && !IsMasterPlay)
            {
                audioList[4].Play();
                audioList[4].loop = false;
                IsMasterPlay = true;
            }
            

            //two dog wangwang
            if (Dog4.getState()!=1 && !audioList[5].isPlaying)
            {
                audioList[5].Play();
            }else if(frame>21 && audioList[5].isPlaying)
            {
                if (audioList[5].volume > 0f)
                    audioList[5].volume = Mathf.Lerp(audioList[5].volume, 0f, .2f);
            }
        }
		
	}
}
