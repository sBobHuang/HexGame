#ifndef STATUS_LAYER_H_
#define STATUS_LAYER_H_

#include "cocos2d.h"
#include "ScoreInfo.h"
using namespace cocos2d;

class StatusLayer : public Layer{
public:
	StatusLayer();
	~StatusLayer();
	virtual bool init();
	CREATE_FUNC(StatusLayer);
	int getScore();
	int m_score;

private:
	void updateScore(ScoreInfo* info);
	void plusplusScore(int delv, int now);
	void actionCb(Sprite* target);

	LabelAtlas* m_scoreLabel;
	
};

#endif
