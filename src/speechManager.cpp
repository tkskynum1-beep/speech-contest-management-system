#include"speechManager.h"
#include"speaker.h"
#include <algorithm>
#include <random>
#include <deque>
#include <numeric>
#include <string>
#include <fstream>

SpeechManager::SpeechManager()
{
    this->initSpeech();
    this->createSpeaker();
    this->loadRecord();
}

void SpeechManager::initSpeech()
{
    this->m_Index = 1; //默认第一轮比赛
    this->v1.clear();
    this->v2.clear();
    this->v3.clear();
    this->m_Speaker.clear();
    this->m_Record.clear();
}

void SpeechManager::show_Menu()
{
    cout<<"**************************************"<<endl;
    cout<<"********** 欢迎参加演讲比赛 **********"<<endl;
    cout<<"********** 1.开始演讲比赛 ************"<<endl;
    cout<<"********** 2.查看往届记录 ************"<<endl;
    cout<<"********** 3.清空比赛记录 ************"<<endl;
    cout<<"********** 0.退出比赛程序 ************"<<endl;
    cout<<"**************************************"<<endl;
}

void SpeechManager::createSpeaker()
{
    string nameseed = "ABCDEFGHIJKL";
    for(int i = 0; i<nameseed.size(); i++)
    {
        string name = "选手";
        name += nameseed[i];
        Speaker sp;
        sp.m_Name = name;
        for(int j=0;j<2;j++)
        {
            sp.m_Score[j] = 0;
        }
        //将选手放入初赛选手容器中
        this->v1.push_back(i+10001);
        //将选手编号和选手信息放入map容器中
        this->m_Speaker.insert(make_pair(i+10001, sp));
    }
}

void SpeechManager::startSpeech()
{
    //1.抽签
    this->speechDraw();
    //2.比赛
    this->speechContest();
    //3.显示晋级结果
    this->showScore();
    this->m_Index++; //比赛轮数增加
    //第二轮比赛
     //1.抽签
    this->speechDraw();
     //2.比赛
    this->speechContest();
     //3.显示最终结果
    this->showScore();
     //保存比赛结果
    this->saveRecord();

    //重置比赛数据
    this->initSpeech();
    this->createSpeaker();
    this->loadRecord();
    cout<<"比赛结束，感谢观看！"<<endl;
    system("pause");
    system("cls"); //清屏
}
void SpeechManager::speechDraw()
{
    cout<<"---------------第"<<this->m_Index<<"轮比赛抽签---------------"<<endl;
    cout<<"--------------------------------"<<endl;
    cout<<"抽签后选手的顺序为："<<endl;
    random_device rd;
    mt19937 g(rd());
    if(this->m_Index == 1) //第一轮比赛
    {
        shuffle(this->v1.begin(), this->v1.end(), g); //打乱初赛选手的顺序
        for(vector<int>::iterator it = this->v1.begin(); it != this->v1.end(); it++)
        {
            cout<<*it<<" ";
        }
        cout<<endl;
    }
    else{
        shuffle(this->v2.begin(), this->v2.end(), g); //打乱决赛选手的顺序
        for(vector<int>::iterator it = this->v2.begin(); it != this->v2.end(); it++)
        {
            cout<<*it<<" ";
        }
        cout<<endl;
    }
    cout<<"--------------------------------"<<endl;
    system("pause");
    cout<<endl;
}
void SpeechManager::speechContest()
{
    cout<<"---------------第"<<this->m_Index<<"轮比赛正式开始---------------"<<endl;
    cout<<"-----------------------------------"<<endl;
    vector<int> v_Src; //比赛选手容器
    v_Src = (this->m_Index == 1) ? this->v1 : this->v2; //如果是第一轮比赛，比赛选手就是初赛选手，否则就是决赛选手
    multimap<double, int, greater<double>> groupScore; //临时容器
    int num = 0; //记录晋级人数
    for(vector<int>::iterator it = v_Src.begin(); it != v_Src.end(); it++)
    {
        num++;
        deque<double> d; //存放打分容器>
        for(int i=0; i<10; i++)
        {
            double score = (rand() % 401 + 600) /10.f; //600-1000之间的随机数，作为选手的得分)
            d.push_back(score);
            // cout<<score<<" ";
        }
        // cout<<endl;
        sort(d.begin(),d.end(),greater<double>()); //排序
        d.pop_back(); //去掉最低分
        d.pop_front(); //去掉最高分
        double sum = accumulate(d.begin(), d.end(), 0.0); //求和    
        double avg = sum / (double)d.size(); //求平均分

        this->m_Speaker[*it].m_Score[this->m_Index-1] = avg; //将平均分存储到选手信息中
        groupScore.insert(make_pair(avg, *it)); //将选手的平均分和编号存入临时容器中
        if(num % 6 ==0)
        {
            cout<<"第"<<num/6<<"小组比赛结果："<<endl;
            for(multimap<double, int, greater<double>>::iterator it = groupScore.begin(); it != groupScore.end(); it++)
            {
                cout<<"编号: "<<it->second<<"姓名: "<<this->m_Speaker[it->second].m_Name<<"平均分: "<<it->first<<endl;
            }
            //取出每组的前3名选手，放入晋级容器中
            int count = 0; //记录晋级人数
            for(multimap<double, int, greater<double>>::iterator it = groupScore.begin(); it != groupScore.end() && count < 3; it++, count++)
            {
                if(this->m_Index == 1)
                {
                    this->v2.push_back(it->second);
                }
                else{
                    this->v3.push_back(it->second);
                }
            }
            if(num == 6)
            {
                cout<<"-----------------------------------"<<endl;
            }
            groupScore.clear(); //清空临时容器
        }
    }
    cout<<"---------------第"<<this->m_Index<<"轮比赛结束---------------"<<endl;
    system("pause");
}
void SpeechManager::showScore()
{
    cout<<"---------------第"<<this->m_Index<<"轮比赛晋级选手如下：---------------"<<endl;
    auto v_Src = (this->m_Index == 1) ? this->v2 : this->v3; //如果是第一轮比赛，比赛选手就是初赛选手，否则就是决赛选手
    for(auto it = v_Src.begin(); it != v_Src.end(); it++)
    {
        cout<<"编号: "<<*it<<"姓名: "<<this->m_Speaker[*it].m_Name<<"得分: "<<this->m_Speaker[*it].m_Score[this->m_Index-1]<<endl;
    }
    cout<<"-----------------------------------"<<endl;
    system("pause");
    system("cls"); //清屏
    this->show_Menu();
}
void SpeechManager::saveRecord()
{
    ofstream ofs;
    ofs.open("speech.csv", ios::out | ios::app); //以追加的方式打开文件
    for(auto it = this->v3.begin(); it != this->v3.end(); it++)
    {
        ofs<<*it<<","<<this->m_Speaker[*it].m_Score[1]<<",";
    }
    ofs<<endl;
    ofs.close();
    cout<<"记录已保存"<<endl;
    this->fileIsEmpty = false; //文件不为空
}
void SpeechManager::loadRecord()
{
    ifstream ifs("speech.csv", ios::in);
    if(!ifs.is_open())
    {
        this->fileIsEmpty = true; //默认文件为空
        cout<<"文件不存在!"<<endl;
        ifs.close();
        return;
    }
    char ch;
    ifs >> ch; //读取一个字符
    if(ifs.eof())
    {
        cout<<"文件为空!"<<endl;
        this->fileIsEmpty = true; //文件为空
        ifs.close();
        return;
    }
    this->fileIsEmpty = false; //文件不为空
    ifs.putback(ch); //将读取的字符放回流中

    string data;
    int index = 0;
    while(ifs>>data)
    {
        // cout<<"读取到的数据: "<<data<<endl;
        vector<string> v; //存放每行数据的容器
        int pos = -1;
        int start = 0;
        while (true)
        {
            pos = data.find(",", start);
            if (pos == -1)
            {
                break;
            }
            string temp = data.substr(start, pos - start);
            v.push_back(temp);
            start = pos + 1;
        }
        this->m_Record.insert(make_pair(index, v)); //将每行数据存入map容器中
        index++;
    }
    ifs.close();
    // cout<<"记录加载成功!"<<endl;
}
void SpeechManager::showRecord()
{
    if(this->fileIsEmpty)
    {
        cout<<"文件为空！"<<endl;
        return;
    }
    cout<<"---------------往届比赛记录如下：---------------"<<endl;
    for(auto it = this->m_Record.begin(); it != this->m_Record.end(); it++)
    {
        cout<<"第"<<it->first+1<<"届比赛冠军编号: "<<it->second[0]<<" 得分: "<<it->second[1] <<" "
            <<"亚军编号: "<<it->second[2]<<" 得分: "<<it->second[3]<<" "
            <<"季军编号: "<<it->second[4]<<" 得分: "<<it->second[5]<<endl;
    }
    system("pause");
    system("cls"); //清屏
}
void SpeechManager::clearRecord()
{
    cout<<"确认清空?"<<endl;
    cout<<"1.确认 2.取消"<<endl;
    int choice = 0;
    cin>>choice;
    if(choice == 1)
    {
        ofstream ofs("speech.csv", ios::trunc);
        ofs.close();
        this->initSpeech();
        this->createSpeaker();
        this->loadRecord();
        cout<<"记录已清空！"<<endl;
    }
    else
    {
        cout<<"取消清空！"<<endl;
    }
    system("pause");
    system("cls"); //清屏
}

void SpeechManager::exitSystem()
{
    cout<<"欢迎下次使用"<<endl;
    system("pause");
    exit(0);
}

SpeechManager::~SpeechManager()
{
}