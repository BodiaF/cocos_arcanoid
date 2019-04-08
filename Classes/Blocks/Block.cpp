#include "Block.h"

Block::Block() :
	m_hp(-1),
	m_sprite(nullptr)
{
}

Block::~Block()
{
	if (m_sprite) {
		delete m_sprite;
		m_sprite = nullptr;
	}
}