#include "BaseBlock.h"

BaseBlock::BaseBlock(cocos2d::Vec2 pos, int hp) : Block()
{
	m_position = pos;
	m_hp = hp;

	m_sprite = cocos2d::Sprite::create();
	m_sprite->setPosition(pos);
	m_sprite->setAnchorPoint(cocos2d::Vec2(0,0));
	updateSprite();
}

BaseBlock::~BaseBlock()
{
}

// ======================= //

void BaseBlock::Striked()
{
	m_hp--;
	updateSprite();

	if (m_hp == 0)
		_callback_died(this);
}

void BaseBlock::SetHP(int hp)
{
	m_hp = hp;
	updateSprite();
}

// ======================= //

void BaseBlock::updateSprite()
{
	if (m_hp <= 0) {
		m_sprite->setVisible(false);
		return;
	}
	m_sprite->setTexture(getTextureStr());
	int tw = m_sprite->getTexture()->getPixelsWide();
	int th = m_sprite->getTexture()->getPixelsHigh();
	m_bound = cocos2d::Rect(m_position, cocos2d::Size(tw, th));
}

const char* BaseBlock::getTextureStr()
{
	switch (m_hp) {
	case 1:
		return "textures/blocks/brick_green.png";
		break;
	case 2:
		return "textures/blocks/brick_red.png";
		break;
	case 3:
	default:
		return "textures/blocks/brick_cyan.png";
		break;
	}
}