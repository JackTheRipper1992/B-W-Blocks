//
//  Block.cpp
//  NoTouchWhiteBlocks
//
//  Created by 刘颖 on 17/6/29.
//
//

#include "Block.hpp"
Vector<Block*> *Block::vBlocks=new Vector<Block*>();//用堆空间初始化
//创建block的静态方法
Block* Block::CreateWithArgs(Color3B color,Size size,std::string label,float font_size,Color4B font_color)
{
    auto block=new(std::nothrow)Block();//堆内申请一个空间，用栈空间的指针指向它
    if(block&&block->InitWithArgs(color, size, label, font_size, font_color))//判断是否申请空间成功且初始化成功
    {
        block->autorelease();//加入outoReleasePool
        vBlocks->pushBack(block);//把block添加到vector
        return block;//返回一个定义好属性的block给调用点
    }
    else//如果申请堆空间失败或者初始化失败
    {
        delete block;//释放block指针申请的堆空间
        block=nullptr;//将原来的指针置为空
        return nullptr;//返回
    }
}
//用来初始化block的类内部方法
bool Block::InitWithArgs(Color3B color,Size size,std::string label,float font_size,Color4B font_color)
{
    if(!Sprite::init()){//调用父类初始化
        return false;
    }
    line_Index=0;//初始化
    setContentSize(size);//设置block大笑
    setAnchorPoint(Vec2::ZERO);//设置锚dian
    setTextureRect(Rect(0,0,size.width,size.height));//设置矩形大笑
    setColor(color);//设置颜色
    
    auto l=Label::create();//创建标签
    l->setString(label);//设置标签显示字符串
    l->setSystemFontSize(font_size);//设置字体大小
    l->setTextColor(font_color);//设置颜色
    this->addChild(l);//添加至当前显示.这里的this指针由静态方法传入
    l->setPosition(size.width/2,size.height/2);//设置位置
    

    return true;
}
//移除block
void Block::RemoveBlock()
{
    auto c=this->getColor();//返回当前块的颜色
    log("the block is removed,its color is %d %d %d",c.r,c.g,c.b);//在控制台输出颜色r,g,b值
    this->removeFromParent();//移除
    vBlocks->eraseObject(this);//在vector中移除
   
}
//获取行索引
int Block::getLineIndex() const
{
    return line_Index;
}
//设置行索引
void Block::setLineIndex(int var)
{
    this->line_Index=var;
}
//获取到vBlocks静态成员的静态方法
Vector<Block*>* Block::GetBlocks()
{
    return vBlocks;
}
//下移方法
void Block::MoveDown()
{
    this->line_Index--;//下移前行索引自减
    auto visibleSize=Director::getInstance()->getVisibleSize();//获取屏幕可见区域大小
    this->runAction(Sequence::create(MoveTo::create(0.01, Vec2(this->getPositionX(),line_Index*visibleSize.height/4)),//移动并回调CallFunc里关联的lambada函数体
                                     CallFunc::create([this](){
        if(line_Index<0)
        {
            this->RemoveBlock();
        }
    }
                                     ),NULL));
}
