//
//  Block.hpp
//  NoTouchWhiteBlocks
//
//  Created by 刘颖 on 17/6/29.
//
//

#ifndef Block_hpp
#define Block_hpp

#include <stdio.h>
#include <cocos2d.h>

USING_NS_CC;

class Block:public Sprite {
private:
    static Vector<Block* > *vBlocks;//创建一个名为vBlocks的，且指向存放着Block*类型的Vector类型的指针，此Vector存放的类型必须是Ref类或者其子类
    
public:
    CC_PROPERTY(int, line_Index,LineIndex);//设置属性
    static Block* CreateWithArgs(Color3B color,Size size,std::string label,float font_size,Color4B font_color);//申明静态创建函数
    virtual bool InitWithArgs(Color3B color,Size size,std::string label,float font_size,Color4B font_color);//静态创建函数的最终类内实现
    void RemoveBlock();//移除节点函数
    static Vector<Block*> * GetBlocks();//静态方法的作用在于，在没有指定对象创建的时候，进行类方法的调用。
    void MoveDown();//当前block下移
};
#endif /* Block_hpp */
