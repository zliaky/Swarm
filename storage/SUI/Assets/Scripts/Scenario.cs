using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public class Scenario : MonoBehaviour {
    public int frameNum = 23;
    public int chaNum = 6;
    public int conNum = 7;
    public struct Character
    {
        public string name;
        public int[] movingMethod;   //0：静止，1：直线运动，2：旋转，3：其他
        public float[] curX;
        public float[] curY;
        public float[] tarX;
        public float[] tarY;
        public int[] deltaAngle;
        public int[] movingTime;
    }
    public Character[] characters;

    // Use this for initialization
    void Start ()
    {
        characters = new Character[chaNum];
        for (int i = 0; i < chaNum; i++)
        {
            characters[i].movingMethod = new int[frameNum];
            characters[i].curX = new float[frameNum];
            characters[i].curY = new float[frameNum];
            characters[i].tarX = new float[frameNum];
            characters[i].tarY = new float[frameNum];
            characters[i].deltaAngle = new int[frameNum];
            characters[i].movingTime = new int[frameNum];
        }
        readFile();
	}
	

    void readFile()
    {
        string path = "Assets/Data/scenario.txt";
        //string path = "Assets/Data/scenario_editing.txt";
        
        if (!File.Exists(path))
        {
            Debug.Log("file does not exist in " + path);
            return;
        }
        string[] lines = File.ReadAllLines(path);
        string[] split = lines[0].Split(',');
        int cnt = 0;
        foreach (string item in split)
        {
            if (item != "")
            {
                characters[cnt].name = item;
                cnt++;
            }
        }
        for (int i = 2; i < lines.Length; i++)
        {
            split = lines[i].Split(',');
            for (int j = 0; j < split.Length; j++)
            {
                float content;
                if (split[j] == "")
                {
                    content = 0;
                }
                else
                {
                    content = float.Parse(split[j]);
                }
                switch (j % conNum)
                {
                    case 0:
                        characters[j / conNum].movingMethod[i - 2] = (int)content;
                        break;
                    case 1:
                        if(content > 0)
                            characters[j / conNum].curX[i - 2] = content*1.35f;
                        else
                            characters[j / conNum].curX[i - 2] = content;
                        break;
                    case 2:
                        if(content > 0)
                            characters[j / conNum].curY[i - 2] = (Screen.height -  content * 1.37f - 120f);
                        else
                            characters[j / conNum].curY[i - 2] = content;
                        break;
                    case 3:
                        if(content > 0)
                            characters[j / conNum].tarX[i - 2] = content*1.35f;
                        else
                            characters[j / conNum].tarX[i - 2] = content;
                        break;
                    case 4:
                        if(content > 0)
                            characters[j / conNum].tarY[i - 2] = (Screen.height -  content * 1.37f - 120f);
                        else
                            characters[j / conNum].tarY[i - 2] = content;
                        break;
                    case 5:
                        characters[j / conNum].movingTime[i - 2] = (int)content;
                        break;
                    case 6:
                        characters[j / conNum].deltaAngle[i - 2] = (int)content;
                        break;
                    default:
                        break;
                }
            }
        }
    }
}
