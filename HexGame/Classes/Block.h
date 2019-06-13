#ifndef BLOCK_H_
#define BLOCK_H_

#include "cocos2d.h"
using namespace cocos2d;

class Block : public Node
{
public:

	CREATE_FUNC(Block);
	Block();
	~Block();
	virtual bool init();
	void setSpriteColor(Color3B color);
	void setLRIndex(int lI, int rI);
	bool addShadowSprite(Sprite* sprite, int opacity);
	void removeShadowSprite();
	Sprite* getShadowSprite();


	int m_lineI;
	int m_rowI;
private:

	Sprite* m_sprite;

	Sprite* m_shadowSprite;
};
#endif 
