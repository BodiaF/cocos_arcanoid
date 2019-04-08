#pragma once

#include "cocos2d.h"

enum BlockType {base};

class Block
{
public:
	Block();
	~Block();

	typedef std::function<void(Block*)> ccBlockDied;

	/// Return block type;
	virtual BlockType        GetType() = 0;
	/// Ball hit this block
	virtual void             Striked() {}

	virtual int		         GetHP()       { return m_hp; }
	virtual void	         SetHP(int hp) { m_hp = hp; }

	virtual cocos2d::Sprite* GetSprite()   { return m_sprite; }
	virtual cocos2d::Vec2&   GetPosition() { return m_position; }
	virtual cocos2d::Rect&   GetBounds()   { return m_bound; }

	/// Callback when m_hp <= 0 after Striked()
	ccBlockDied _callback_died;

protected:

	int              m_hp;							///< Block's HP
	cocos2d::Sprite* m_sprite;						///< Block's sprite
	cocos2d::Vec2    m_position;					///< Block's scene posiion
	cocos2d::Rect    m_bound;						///< Block's bound rect
};

