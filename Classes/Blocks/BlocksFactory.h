#pragma once

#include "cocos2d.h"
#include "Block.h"
#include "BaseBlock.h"

class BlocksFactory
{
public:
	BlocksFactory(cocos2d::Scene* parentScene);
	~BlocksFactory();

	/// Callback for block with hp <= 0
	Block::ccBlockDied _callback_died;

	/// Create base block: set pos, hp, all callbacks, draw sprite on scene
	BaseBlock* CreateBaseBlock(cocos2d::Vec2 position, int hp);

private:
	cocos2d::Scene *_parentScene;
};

