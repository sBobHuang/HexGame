#include "MapLayer.h"
#include "Global.h"

MapLayer::MapLayer(){}
MapLayer::~MapLayer()
{
	removeAllChildren();
}

bool MapLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(28, 28, 28, 255)))
	{
		return false;
	}
	auto winSize = Director::getInstance()->getWinSize();

	m_color = Color3B(79, 79, 79);

	for (int i = 0; i < 4; ++i)
	{
		m_shadowS[i] = Sprite::create("shadow.png");
		m_shadowS[i]->retain();
		m_shadowV[i] = 0;
	}

	for (int i = 0; i < gMapLineM; ++i)
	{
		int lineLength = gMapLineM - abs(4 - i);
		int j = 0;
		if (i > 4){
			j = i - 4;
		}
		int rowMaxI = j + lineLength;
		for (int k = 0; k < j; ++k)
			m_Map[i][k] = MAP_NOUSE;
        for ( ; j < rowMaxI; ++j)
			m_Map[i][j] = MAP_EMPTY;
		for ( ; j < gMapLineM; ++j)
			m_Map[i][j] = MAP_NOUSE;
	}

	/*
		{0, 0, 0, 0, 0, 2, 2, 2, 2}
		{0, 0, 0, 0, 0, 0, 2, 2, 2}
		{0, 0, 0, 0, 0, 0, 0, 2, 2}
		{0, 0, 0, 0, 0, 0, 0, 0, 2}
		{0, 0, 0, 0, 0, 0, 0, 0, 0}
		{2, 0, 0, 0, 0, 0, 0, 0, 0}
		{2, 2, 0, 0, 0, 0, 0, 0, 0}
		{2, 2, 2, 0, 0, 0, 0, 0, 0}
		{2, 2, 2, 2, 0, 0, 0, 0, 0}
	*/
	float disY = gSpriteH  -(gSpriteW * sqrt(3) / 6)
		+ gSpace / sqrt(3) * 2;
	float disOneLineX = gSpriteW + gSpace;

	float disRowLineX = disOneLineX / 2;

	float flagPosY = winSize.height - (winSize.height - 8 * disY) / 2;
	float flagPosX = (winSize.width - 8 * disOneLineX) / 2;

	for (int i = 0; i < gMapLineM; ++i){
		
		float firstBlockX;
		if (i < 4)
			firstBlockX = flagPosX + (4 - i) * disRowLineX;
		else{
			firstBlockX = flagPosX + (i - 4) * (disRowLineX - disOneLineX);
		}

		for (int j = 0; j < gMapRowM; ++j){
			if (MAP_NOUSE != m_Map[i][j])
			{
				auto block = Block::create();
				block->setLRIndex(i, j);
				block->setSpriteColor(m_color);
				block->setPosition(Point(firstBlockX + j * disOneLineX, flagPosY - i * disY));
				addChild(block);
				m_blocks[i][j] = block;
			}
			else
			{
				m_blocks[i][j] = NULL;
			}
		}
	}
	return true;
}

/*
	为方块添加一个阴影精灵。不直接调用Block类的接口是因为，每次
	添加时都需要创建，移除了被回收。操作频繁。
	如何使用MapLayer的addShadow**，那么也要使用MapLayer的removeShadow**
*/
void MapLayer::addShadowForBlock(Block* target)
{
	assert(NULL != target);
	int index = -1;
	for (int i = 0; i < 4; ++i)
	{
		if (0 == m_shadowV[i])
		{
			index = i;
		}
	}
	if (index < 0)
		return;
	if (target->addShadowSprite(m_shadowS[index], 70))
	{
		m_shadowV[index] = 1;
	}
}

void MapLayer::removeShadowForBlock(Block* target)
{
	if (!target) return;
	for (int i = 0; i < 4; ++i)
	{
		if (target->getShadowSprite() == m_shadowS[i])
		{
			m_shadowV[i] = 0;
			target->removeShadowSprite();
		}
	}
}

void MapLayer::mapClear()
{

	for (int i = 0; i < gMapLineM; ++i)
	{
		for (int j = 0; j < gMapRowM; ++j)
		{
			if (MAP_FILL == m_Map[i][j])
				m_Map[i][j] = MAP_EMPTY;
			if (MAP_EMPTY == m_Map[i][j])
				m_blocks[i][j]->setSpriteColor(m_color);
		}
	}
}

