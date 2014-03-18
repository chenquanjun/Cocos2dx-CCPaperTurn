//
//  CCPaperTurn.cpp
//  testPaperTurn
//
//  Created by chenquanjun on 14-1-24.
//
//

#include "CCPaperTurn.h"

CCPaperTurn::CCPaperTurn(void)
{
    m_fMarginL = 0, m_fMarginR = 1, m_fMarginD = 0, m_fMarginT = 1;
}

CCPaperTurn* CCPaperTurn::create(float duration, bool isRight){
    CCPaperTurn *pAction = new CCPaperTurn();
    
    if (pAction)
    {
        if (pAction->initWithDirection(duration, isRight))
        {
            pAction->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(pAction);
        }
    }
    
    return pAction;
}

bool CCPaperTurn::initWithDirection(float duration, bool isRight){
    CCSize gridSize = CCSizeMake(32, 24);
    //CCSize gridSize = CCSizeMake(100, 100);
    if (CCGrid3DAction::initWithDuration(duration, gridSize))
    {
        m_bIsRight = isRight;
        
        //线性坐标点
        //        m_point1 = CCPointMake(0.0f, 0.3f);
        //        m_point2 = CCPointMake(0.3f, 0.05f);
        //        m_point3 = CCPointMake(1.0f, 0.0322f);
        
        m_point1 = CCPointMake(0.0f, 0.3f);
        m_point2 = CCPointMake(0.5, 0.135);
        m_point3 = CCPointMake(1.0f, 0.0322f);
        
        //线性参数1
        m_fParameterK1 = (m_point2.y - m_point1.y) / (m_point2.x - m_point1.x);
        m_fParameterB1= m_point1.y - m_fParameterK1 * m_point1.x;
        
        //线性参数2
        m_fParameterK2 = (m_point2.y - m_point3.y) / (m_point2.x - m_point3.x);
        m_fParameterB2= m_point3.y - m_fParameterK2 * m_point3.x;
        
        m_winSize = CCDirector::sharedDirector()->getWinSize();
        
        return true;
    }
    return false;
    
}

void CCPaperTurn::setPaperMargin(float marginL, float marginR, float marginD, float marginT){
    CCAssert(marginL < marginR && marginD < marginT, "action margin out of range");
    m_fMarginL = marginL;
    m_fMarginR = marginR;
    m_fMarginT = marginT;
    m_fMarginD = marginD;
}

void CCPaperTurn::update(float time){
    //主要受到 ay 与 theta 影响
    double tt = time + 0.8;// + 0.8f;
    double deltaAy = (tt * tt * 1000);
    double ay = -10000 - deltaAy; //ay值趋向无穷大时候翻转体从圆锥体退化成圆柱体
    
    //    float deltaTheta = - (float) M_PI_2 * sqrtf( tt) ;
    //    float theta = /*0.01f */ + (float) M_PI_2 +deltaTheta;
    //    float theta = 0.9 / (time + 0.8) - 0.50;//从（0，0.7）到（1，0）
    //    float theta = (-0.1 * time + 0.1);
    
    //    c(x) = 2^(-10x) ln(2)
    float theta;
    if (time < m_point2.x) {
        theta = m_fParameterK1 * time + m_fParameterB1;
        
    }else{
        theta = m_fParameterK2 * time + m_fParameterB2;
    }
    
    //    theta = powf(2, -10 * time)* 0.39;
    
    float sinTheta = sinf(theta);
    float cosTheta = cosf(theta);
    
    GLfloat offsetX = 0.0f;
    GLfloat offsetZ = 0.0f;
    bool flag = true; //第一次循环标志
    
    float marginL = m_fMarginL * m_sGridSize.width;
    float marginR = m_fMarginR * m_sGridSize.width;
    float marginD = m_fMarginD * m_sGridSize.height;
    float marginT = m_fMarginT * m_sGridSize.height;
    
    for (int i = marginL; i <=marginR; ++i)
    {
        for (int j = marginD; j <= marginT; ++j)
        {
            // Get original vertex
            ccVertex3F p = originalVertex(ccp(i ,j));
            
            float R = sqrtf((p.x * p.x) + ((p.y - ay) * (p.y - ay)));
            float r = R * sinTheta;
            float alpha = asinf( p.x / R );
            float beta = alpha / sinTheta;
            float cosBeta = cosf( beta );
            
            GLfloat tempX, tempZ;
            
            if (m_bIsRight) {
                tempX = (m_winSize.width - r * sinf(beta));//左边掀起，往右边翻
                
            }else{
                tempX = (r * sinf(beta));//右边掀起,往左边翻
            }
            
            tempZ  = (r * ( 1 - cosBeta ) * cosTheta) / 7;// "100" didn't work for
            
            //偏移值修正，保持翻转轴不移动
            if (flag) {
                offsetX = p.x - tempX; //防止向左右移动
                offsetZ = p.z - tempZ;//防止向下移动
            }
            
            p.x = tempX + offsetX;
            p.y = ( R + ay - ( r * (1 - cosBeta) * sinTheta)); //y不需要修正偏移值
            p.z = tempZ + offsetZ;
            
            // Set new coords
            setVertex(ccp(i, j), p);
            
        }
        flag = false;
    }
}