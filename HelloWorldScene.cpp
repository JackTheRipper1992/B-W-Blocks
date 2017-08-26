#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

Scene* HelloWorld::createScene()//静态的场景创建函数，返回一个包含HelloWorld层对象的Scene场景
//这种静态方法用于在类外使用，当类外没有定义该类对象的时候，使用该方法返回一个包涵该类对象的场景
{
    auto scene = Scene::create();//Scene类创建场景对象
    auto layer = HelloWorld::create();//HelloWorld类使用cocos2d风格的创建函数创建层对象。并在这过程中调用HelloWorld::init()
    scene->addChild(layer);//将HelloWorld类的层对象添加到场景中
    return scene;//返回一个包含HelloWorld层对象的Scene场景
}
bool HelloWorld::init()//重写的初始化函数，在HelloWorld::create()方法中被调用
{
    //*******************此函数中this指针由HelloWorld::create()的层对象传入************************
    
    if ( !Layer::init() )//调用父类初始化函数
    {
        return false;//如果父类初始化失败返回false
    }
    visibleSize = Director::getInstance()->getVisibleSize();//初始化
    origin=Director::getInstance()->getVisibleOrigin();//初始化
    
    srand((unsigned)time(NULL));//time()函数返回time_t类型，而void srand(unsigned seed)接受unsigned类型。
    //即srand((unsigned) time(&t))，还有一个经常用法，不需要定义time_t型t变量；
    //即srand((unsigned) time(NULL)); 直接传入一个空指针，因为你的程序中往往并不需要经过参数获得的t数据

    gameLayer=Node::create();//初始化
    this->addChild(gameLayer);//将gameLayer添加到HelloWolrd::create()的层对象

    timeLabel=Label::create();//初始化
    timeLabel->setSystemFontSize(12);//设置标签字体
    timeLabel->setTextColor(Color4B::ORANGE);//设置标签字体颜色
    timeLabel->setString("0.0000");//设置字体显示格式
    timeLabel->setPosition(origin.x+visibleSize.width/2,visibleSize.height-40);//设置标签显示位置
    this->addChild(timeLabel);//添加标签到HelloWolrd::create()的层对象
    
    StartGame();//开始游戏
    
    auto listener=EventListenerTouchOneByOne::create();//初始化事件侦听器
    listener->onTouchBegan=[this](Touch* touch,Event* event){//lambada表达式定义onTouchBegan操作
        auto blocks=Block::GetBlocks();//通过Block::GetBlocks()静态方法获取Vector<Block*>* 的指针。
        for(auto b=blocks->begin();b!=blocks->end();++b){//获取blocks迭代器进行Vector遍历
            //逻辑与（短路求值），当且仅当左侧运算对象为真时，才对右侧对象求值
            if((*b)->getLineIndex()==1&&(*b)->getBoundingBox().containsPoint(touch->getLocation()))
            {
                if ((*b)->getColor()==Color3B::BLACK){//getColor函数返回一个Color3B的引用，第一次点击到黑块确定游戏开始
                    if(!timeRunning){//游戏开始，判断计时器状态后，开始计时
                        this->StartTimer();//开始计时
                    }
                    (*b)->setColor(Color3B::GRAY);//每点击到黑块之后都将其变为灰色，点击互动功能实现
                    this->MoveDown();//每点击一次后执行一次下移操作
                }else if((*b)->getColor()==Color3B::GREEN){//如果点到的是绿色，则执行一次下移操作，并停止计时
                    this->MoveDown();//下移
                    this->StopTimer();//停止计时
                }else{//如果点到的是line_Index是1且没有点到黑块
                MessageBox("Game Over", "游戏失败");//弹出消息盒子
                }
                break;//终止离它最近的for,while,do while,或switch语句。在这里跳出for之后执行return false
            }
        }
        return false;//返回false表示不进行事件传递
    };
    //注册listener事件侦听器，在这之前必须实现onTouchBegan方法
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    return true;//初始化成功返回true
}
//开始游戏
void HelloWorld::StartGame()
{   //初始化
    lineCount=0;
    showEnd=false;
    timeRunning=false;
    //分别添加开始行和line_Index为1、2、3的黑白行
    AddStartLine();
    AddNormalLine(1);
    AddNormalLine(2);
    AddNormalLine(3);
}
//下移
void HelloWorld::MoveDown()
{
    if(lineCount<10){//如果已添加行计数小于10，才新增一行，等于10之后，就停止添加；之后开始出现结束行
    AddNormalLine(4);//先在屏幕之上看不见的区域添加一行
    }else if(!showEnd){//判断结束行是否出现，标记位用于实现结束行只添加一次，因为结束行是一个整个屏幕的block
        AddEndLine();//添加结束行
        showEnd=true;//标记位置位
    }
    auto blocks=Block::GetBlocks();
    for(auto b=blocks->begin();b!=blocks->end();++b)
    {
        (*b)->MoveDown();
    }
}
//开始行函数
void HelloWorld::AddStartLine()
{
    auto start_line=Block::CreateWithArgs(Color3B::YELLOW,Size(visibleSize.width,visibleSize.height/4), "", 20, Color4B::BLACK);
    start_line->setPosition(origin.x,origin.y);
    start_line->setLineIndex(0);
    gameLayer->addChild(start_line);
}
//结束行函数
void HelloWorld::AddEndLine()
{
    auto end_line=Block::CreateWithArgs(Color3B::GREEN, visibleSize, "Game Over", 32, Color4B::WHITE);
    end_line->setPosition(origin);
    end_line->setLineIndex(4);
    gameLayer->addChild(end_line);
}
//添加普通行
void HelloWorld::AddNormalLine(int index)
{
    Block *b;
    int BlackIndex=rand()%4;
    for (int i=0; i<4; ++i) {
        b=Block::CreateWithArgs(BlackIndex==i?Color3B::BLACK:Color3B::WHITE, Size(visibleSize.width/4-1,visibleSize.height/4-1), "", 23, Color4B::YELLOW);
        //虽然b是临时变量，但是CreateWithArgs函数内部的block是赋予了autorelease属性的，所以在这里b被赋予返回的block之后，引用计数0＋1=1；
        //addchild之后，引用计数被1＋1＝2；
        //退出函数后，b被释放，引用计数2-1=1；
        //这样除非是用remove函数，block块会一直被渲染。
        gameLayer->addChild(b);//将block添加至游戏图层
        b->setPosition(origin.x+i*visibleSize.width/4,origin.y+index*visibleSize.height/4);//设置block的位置
        b->setLineIndex(index);//设置行索引
    }
    lineCount++;//已添加的normal行计数自增
}
//重写update函数
void HelloWorld::update(float dt){
    long offset=clock()-startTimePoint;//计算临时时间偏移量
    timeLabel->setString(StringUtils::format("%g",((double)offset)/1000000));//设置标签显示格式
}
//开始计时
void HelloWorld::StartTimer(){
    if(!timeRunning){//监测标记位
        scheduleUpdate();//启动update定时器
        timeRunning=true;//置位标识位
        startTimePoint=clock();//记录开始时间
    }
}
//结束计时
void HelloWorld::StopTimer(){
    if(timeRunning){//监测标记位
        unscheduleUpdate();//停止update定时器
        timeRunning=false;//置位标记位
    }
}


