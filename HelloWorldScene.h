#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Block.hpp"

USING_NS_CC;
class HelloWorld : public cocos2d::Layer
{
private:
    Size visibleSize;//获取可视区域尺寸
    Vec2 origin;//获取可视区域出发点坐标
    bool showEnd;//标记结束画面是否出现
    int lineCount;//记录游戏开始后添加的normal行的行数
    Label *timeLabel;//现实实时消耗时间的标签
    Node *gameLayer;//作为所有normal行，start行，end行的父节点
    bool timeRunning;//标记计时器是否在运行
    long startTimePoint;//记录游戏开始时间
public:
    static cocos2d::Scene* createScene();//cocos2dx风格的静态场景创建函数
    virtual bool init();//重写父类（Layer）初始化函数
    CREATE_FUNC(HelloWorld);//为HelloWorld类添加cocos2dx风格的create功能
    void AddStartLine();//添加游戏开始行
    void AddEndLine();//添加游戏结束行
    void AddNormalLine(int index);//添加普通行
    void StartGame();//开始游戏
    void MoveDown();//下移
    virtual void update(float dt);//重写update函数
    void StartTimer(); //开始计时
    void StopTimer();//结束计时
};

#endif // __HELLOWORLD_SCENE_H__
