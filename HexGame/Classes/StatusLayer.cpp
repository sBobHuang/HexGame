#include "StatusLayer.h"
#include "Global.h"
#include "ScoreInfo.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;


StatusLayer::StatusLayer()
{

}

StatusLayer::~StatusLayer(){
	_eventDispatcher->removeAllEventListeners();
}

bool StatusLayer::init()
{
	if (!Layer::init())
		return false;

	m_score = 0;

	auto winSize = Director::getInstance()->getWinSize();
	m_scoreLabel = LabelAtlas::create();
	m_scoreLabel->initWithString("0", "num.png", 79, 111, 48);
	m_scoreLabel->setPosition(winSize.width / 2, winSize.height *0.85);
	m_scoreLabel->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(m_scoreLabel);
	
	auto listener = EventListenerCustom::create(UPDATESCOR_EVENT, [&](EventCustom* event){
		ScoreInfo* info = static_cast<ScoreInfo*>(event->getUserData());
		updateScore(info);
	});
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

void StatusLayer::updateScore(ScoreInfo* info)
{
	unsigned int goal = info->score + m_score;
	int delv = info->score / 20;
	m_scoreLabel->setScale(0.7);
	if (delv > 0 || delv < 0)
	{
		plusplusScore(delv, goal);
	}

	auto winSize = Director::getInstance()->getWinSize();
	
	switch (info->cleanCountOnce)
	{
	case 0:break;
	case 1:{auto audio = SimpleAudioEngine::getInstance();
		audio->playEffect("sound/jiangli.wav", false, 1.0f, 0.0f, 1.0f); }break;
	case 2:
	{
			  auto sprite = Sprite::create("jixunvli.png");
			  auto audio = SimpleAudioEngine::getInstance();
			  audio->playEffect("sound/dianji.wav", false, 1.0f, 0.0f, 1.0f);

			  auto showA = Sequence::create(Spawn::create(FadeIn::create(0.4), MoveTo::create(1.0, Vec2(winSize.width / 2, winSize.height / 2)), nullptr),
				  MoveTo::create(0.5, Vec2(winSize.width / 2, winSize.height / 5 * 4)),
				  CallFunc::create(CC_CALLBACK_0(StatusLayer::actionCb, this, sprite)), nullptr);
			  sprite->setPosition(Vec2(winSize.width / 2, 0 - 40));
			  addChild(sprite);
			  sprite->runAction(showA);
	}break;
	case 3:
	{
			  auto sprite = Sprite::create("welldone.png");
			  auto audio = SimpleAudioEngine::getInstance();
			  audio->playEffect("sound/sansha.mp3", false, 1.0f, 0.0f, 1.0f);
			  sprite->setPosition(Vec2(winSize.width / 2, 0 - 40));
			  auto showA = Sequence::create(Spawn::create(FadeIn::create(0.4), MoveTo::create(1.0, Vec2(winSize.width / 2, winSize.height / 2)), nullptr),
				  MoveTo::create(0.5, Vec2(winSize.width / 2, winSize.height / 5 * 4)),
				  CallFunc::create(CC_CALLBACK_0(StatusLayer::actionCb, this, sprite)), nullptr);
			  sprite->setPosition(Vec2(winSize.width / 2, 0 - 40));
			  addChild(sprite);
			  sprite->runAction(showA);
	}break;
	case 4:
	{
			  auto sprite = Sprite::create("ku.png");
			  auto audio = SimpleAudioEngine::getInstance();
			  audio->playEffect("sound/sisha.mp3", false, 1.0f, 0.0f, 1.0f);
			  auto showA = Sequence::create(Spawn::create(FadeIn::create(0.4), MoveTo::create(1.0, Vec2(winSize.width / 2, winSize.height / 2)), nullptr),
				  MoveTo::create(0.5, Vec2(winSize.width / 2, winSize.height / 5 * 4)),
				  CallFunc::create(CC_CALLBACK_0(StatusLayer::actionCb, this, sprite)), nullptr);
			  sprite->setPosition(Vec2(winSize.width / 2, 0 - 40));
			  sprite->setPosition(Vec2(winSize.width / 2, 0 - 40));
			  addChild(sprite);
			  sprite->runAction(showA);
	}break;
	default:
	{
			   auto sprite = Sprite::create("waixingren.png");
			   auto audio = SimpleAudioEngine::getInstance();
			   audio->playEffect("sound/wusha.mp3", false, 1.0f, 0.0f, 1.0f);
			   auto showA = Sequence::create(Spawn::create(FadeIn::create(0.4), MoveTo::create(1.0, Vec2(winSize.width / 2, winSize.height / 2)), nullptr),
				   MoveTo::create(0.5, Vec2(winSize.width / 2, winSize.height / 5 * 4)),
				   CallFunc::create([&](){
				   this->removeChildByName("ispire");
			   }), nullptr);
			   sprite->setPosition(Vec2(winSize.width / 2, 0 - 40));
			   sprite->setPosition(Vec2(winSize.width / 2, 0 - 40));
			   sprite->setName("ispire");
			   addChild(sprite);
			   sprite->runAction(showA);
	}
	}
}

void StatusLayer::actionCb(Sprite* target)
{
	target->removeFromParent();
}

void StatusLayer::plusplusScore(int delv, int now)
{
	auto addNumA = Sequence::create(DelayTime::create(0.01f), CallFunc::create([=](){
		m_score += delv;
		if (delv > 0)
		{
			char buf[20];
			if (m_score >= now)
			{
				m_score = now;
				sprintf(buf, "%d", m_score);
				m_scoreLabel->setString(buf);
				m_scoreLabel->setScale(1);
				return;
			}
			else
			{
				sprintf(buf, "%d", m_score);
				m_scoreLabel->setString(buf);
				plusplusScore(delv, now);
			}
		}
		else
		{
			char buf[20];
			if (m_score <= now)
			{
				m_score = now;
				sprintf(buf, "%d", m_score);
				m_scoreLabel->setString(buf);
				m_scoreLabel->setScale(1);
				return;
			}
			else
			{
				sprintf(buf, "%d", m_score);
				m_scoreLabel->setString(buf);
				plusplusScore(delv, now);
			}
		}
	}), nullptr);
	runAction(addNumA);
}

int StatusLayer::getScore()
{
	return m_score;
}

