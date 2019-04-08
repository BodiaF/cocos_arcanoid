#pragma once
#include "Block.h"

/// Base 3-color destructive block
class BaseBlock : public Block
{
public:
	BaseBlock(cocos2d::Vec2 pos, int hp = -1);
	~BaseBlock();

	virtual BlockType       GetType() { return BlockType::base; }
	virtual void            Striked();
	virtual void	        SetHP(int hp);

private:
	/// Update texture and bound rect
	void updateSprite();
	/// Return texture name depends on m_hp
	const char* getTextureStr();
};

