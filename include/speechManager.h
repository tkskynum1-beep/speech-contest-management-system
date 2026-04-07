#pragma once

#include<iostream>
using namespace std;
#include<string>
#include<vector>
#include<map>
#include"speaker.h"

class SpeechManager
{
    public:
        SpeechManager();
        void initSpeech();
        void show_Menu();
        void createSpeaker();
        void startSpeech();
        void speechDraw();
        void speechContest();
        void showScore();
        void saveRecord();
        void loadRecord();
        void showRecord();
        void clearRecord();
        
        void exitSystem();
        ~SpeechManager();  
        
        int m_Index; //记录比赛轮数
        bool fileIsEmpty; //文件是否为空
        map<int,vector<string>> m_Record; //存放比赛记录

        vector<int> v1; //存放初赛选手
        vector<int> v2; //存放晋级决赛选手
        vector<int> v3; //存放胜出的三名选手

        map<int, Speaker> m_Speaker; //存放比赛选手
};