//
//  CCPaperTurn.h
//  testPaperTurn
//
//  Created by chenquanjun on 14-1-24.
//
//

#ifndef __testPaperTurn__CCPaperTurn__
#define __testPaperTurn__CCPaperTurn__

#include "cocos2d.h"

USING_NS_CC;

class CCPaperTurn : public CCGrid3DAction{
public:
    virtual void update(float time);
    
public:
    CCPaperTurn(void);
    
    static CCPaperTurn* create(float duration, bool isRight);
    
    void setPaperMargin(float marginL, float marginR, float marginD, float marginT);
    
private:
    bool initWithDirection(float duration, bool isRight);
private:
    bool m_bIsRight;
    double m_fParameterK1, m_fParameterB1, m_fParameterK2, m_fParameterB2;
    
    CCPoint m_point1, m_point2, m_point3;
    
    CCSize m_winSize;
    
    //上下左右操作边界，从左到右为0到1，从下到上为0到1
    float m_fMarginL, m_fMarginR, m_fMarginD, m_fMarginT;

};

#endif /* defined(__testPaperTurn__CCPaperTurn__) */
