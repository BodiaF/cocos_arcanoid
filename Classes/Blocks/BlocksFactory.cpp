#include "BlocksFactory.h"

BlocksFactory::BlocksFactory(cocos2d::Scene* parentScene) : _parentScene(parentScene)
{
}

BlocksFactory::~BlocksFactory()
{
	_parentScene = nullptr;
}

// =================== //

BaseBlock* BlocksFactory::CreateBaseBlock(cocos2d::Vec2 position, int hp)
{
	BaseBlock *block = new BaseBlock(position, hp);
	block->_callback_died = _callback_died;
	_parentScene->addChild(block->GetSprite(), -2);
	return block;
}

