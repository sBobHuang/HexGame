#include "ShapeLayer.h"
#include "Global.h"
#include <assert.h>

ShapeLayer::ShapeLayer(){}    

ShapeLayer::~ShapeLayer()
{
	removeAllChildren();
}

bool ShapeLayer::init()
{
	if (!Layer::init()) return false;

	auto winSize = Director::getInstance()->getWinSize();
	
    
	for (int i = 0; i < 21; ++i)m_currentShapes[i] = new BlockShape();
	float difH = gSpriteW * sqrt(3) / 6;
	float disY = gSpriteH - difH + gSpace;
	float disOneLineX = gSpriteW + gSpace;
	float ySpace = (winSize.height - 8 * disY) / 2;
	//float xSpace = (winSize.width - 8 * disOneLineX) / 2;

	m_currentShapes[1]->setOriPos(winSize.width / 2, ySpace / 2, true);
	m_currentShapes[0]->setOriPos((winSize.width / 2 - gSpriteH * 3),  ySpace / 2,  true);
	m_currentShapes[2]->setOriPos((winSize.width / 2 + gSpriteH * 3),  ySpace / 2,  true);
	addChild(m_currentShapes[0]);
	addChild(m_currentShapes[1]);
	addChild(m_currentShapes[2]);
    

	return true;
}

void ShapeLayer::updateShape(int oriI, float oriX, float oriY)
{
	m_currentShapes[oriI] = new BlockShape();
	
	auto winSize = Director::getInstance()->getWinSize();
	m_currentShapes[oriI]->setPosition(Vec2(winSize.width - 40, 0 - gSpriteH * 2));
	m_currentShapes[oriI]->setVisible(true);
    
	m_currentShapes[oriI]->runScaleAction();
	addChild(m_currentShapes[oriI]);
	if (oriI == 0)
	{
		// 坐标更新
		auto oriX1 = m_currentShapes[1]->m_oriPosX;
		auto oriY1 = m_currentShapes[1]->m_oriPosY;
		auto oriX2 = m_currentShapes[2]->m_oriPosX;
		auto oriY2 = m_currentShapes[2]->m_oriPosY;
		m_currentShapes[1]->runAction(MoveTo::create(0.3f, Vec2(oriX, oriY)));
		m_currentShapes[1]->setOriPos(oriX, oriY);
		m_currentShapes[2]->runAction(MoveTo::create(0.3f, Vec2(oriX1, oriY1)));
		m_currentShapes[2]->setOriPos(oriX1, oriY1);
		m_currentShapes[oriI]->runAction(MoveTo::create(0.2f, Vec2(oriX2, oriY2)));
		m_currentShapes[oriI]->setOriPos(oriX2, oriY2);
		// m_currentShapes 对应更新
		auto tmp = m_currentShapes[0];
		m_currentShapes[0] = m_currentShapes[1];
		m_currentShapes[1] = m_currentShapes[2];
		m_currentShapes[2] = tmp;
		// m_refer 对应更新
		int tmp1 = m_refer[20];
		m_refer[20] = m_refer[19];
		m_refer[19] = m_refer[18];
		m_refer[18] = tmp1;
	}
	else if (oriI == 1)
	{
		auto oriX2 = m_currentShapes[2]->m_oriPosX;
		auto oriY2 = m_currentShapes[2]->m_oriPosY;
		m_currentShapes[2]->runAction(MoveTo::create(0.3f, Vec2(oriX, oriY)));
		m_currentShapes[2]->setOriPos(oriX, oriY);
		m_currentShapes[oriI]->runAction(MoveTo::create(0.2f, Vec2(oriX2, oriY2)));
		m_currentShapes[oriI]->setOriPos(oriX2, oriY2);
		// m_currentShapes 对应更新
		auto tmp = m_currentShapes[1];
		m_currentShapes[1] = m_currentShapes[2];
		m_currentShapes[2] = tmp;
		// m_refer 对应更新
		int tmp1 = m_refer[19];
		m_refer[19] = m_refer[18];
		m_refer[18] = tmp1;
	}
	else{
		m_currentShapes[oriI]->runAction(MoveTo::create(0.2f, Vec2(oriX, oriY)));
		m_currentShapes[oriI]->setOriPos(oriX, oriY);
	}
}


void ShapeLayer::updateShapes()
{
	auto delayF = CallFunc::create([&](){
		float oriX = m_currentShapes[0]->m_oriPosX;
		float oriY = m_currentShapes[0]->m_oriPosY;
		m_currentShapes[0]->removeFromParent();
		updateShape(0, oriX, oriY);
	});
	auto delayRun = Sequence::create(delayF, DelayTime::create(0.3), nullptr);
	runAction(Sequence::create(delayRun, delayRun->clone(), delayF->clone(), nullptr));
}
