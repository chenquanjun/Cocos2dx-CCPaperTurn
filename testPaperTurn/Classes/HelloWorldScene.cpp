#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "CCPaperTurn.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    _testLayer = CCLayer::create();
    this->addChild(_testLayer);
    
    do {//menu btn
        
        CCMenu *menu = CCMenu::create();
        this->addChild(menu);
        menu->setPosition(CCPointZero);
        
        CCLabelTTF *label = CCLabelTTF::create("测试", "Arial", 30);
        
        CCMenuItemLabel *labelBtn = CCMenuItemLabel::create(label, this, menu_selector(HelloWorld::testBtn));
        
        labelBtn->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.2f));
        
        menu->addChild(labelBtn);
        
    } while (0);



    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    pSprite->setFlipX(-1);
    // position the sprite on the center of the screen
    pSprite->setPosition( ccp(winSize.width * 0.8f , winSize.height/2) );

    // add the sprite as a child to this layer
    _testLayer->addChild(pSprite, 0);
    
 
    
    return true;
}

void HelloWorld::testBtn(CCObject* pSender)
{
    do{//翻页动画
        bool isRight = false;
        
        float halfPageTurnTime = 1; //半个翻页时间
        
        CCPaperTurn *paperTurnStepOne = CCPaperTurn::create(halfPageTurnTime, isRight);
        paperTurnStepOne->setPaperMargin(0.504, 1, 0, 1);
        CCPaperTurn *paperTurnStepTwo = CCPaperTurn::create(halfPageTurnTime, !isRight);
        paperTurnStepTwo->setPaperMargin(0.504, 1, 0, 1);
        
        CCPoint pointAnimStart;//翻页坐标点偏移参数
        CCPoint pointAnimMid;//翻页坐标点偏移参数
        
        
        if (isRight) {
            pointAnimStart = ccp(-7, 0);
            pointAnimMid = ccp(7, 0);
        }else {
            pointAnimStart = ccp(0, 0);
            pointAnimMid = ccp(-7, 0);
        }
        
        CCSequence *seqPaperTurn = CCSequence::create(
                                                      paperTurnStepOne,
                                                      CCMoveBy::create(0, pointAnimMid),
                                                      CCReverseTime::create(paperTurnStepTwo),
                                                      NULL
                                                      );
        
        _testLayer->setPosition(pointAnimStart);
        _testLayer->runAction(seqPaperTurn);
    }while (0);
}
