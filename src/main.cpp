#include<iostream>
#include<string>
using namespace std;
#include"speechManager.h"
#include<ctime>

int main()
{
    SpeechManager sm;
    srand((unsigned int)(time(NULL)));
    // for(map<int, Speaker>::iterator it = sm.m_Speaker.begin(); it != sm.m_Speaker.end(); it++)
    // {
    //     cout<<"选手编号："<<it->first<<" 姓名："<<it->second.m_Name<<" 分数："<<it->second.m_Score[0]<<" "<<it->second.m_Score[1]<<endl;
    // }
    int choice = 0;
    while (1)
    {
        sm.show_Menu();
        cout<<"请输入您的选择"<<endl;
        cin>>choice;

        switch(choice)
        {
            case 1: //开始演讲比赛
                sm.startSpeech();
                break;
            case 2: //查看往届记录
                sm.showRecord();
                break;
            case 3: //清空比赛记录
                sm.clearRecord();
                break;
            case 0: //退出比赛程序
                sm.exitSystem();
                break;
            default:
                system("cls");  //清屏
                cout<<"无效的选择，请重新输入"<<endl;
                break;
        }
    }
    


    system("pause");
    return 0;
}

