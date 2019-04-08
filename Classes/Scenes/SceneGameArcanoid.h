#ifndef __GAMEARCANOID_SCENE_H__
#define __GAMEARCANOID_SCENE_H__

#include "cocos2d.h"
#include "Blocks/BlocksFactory.h"

class GameArcanoid : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
	~GameArcanoid();

	virtual bool init();

	/// Frame update : upd ball pos, check collsions.
	virtual void update(float delta);

	/// Some block hp is 0 : remove block to inactive, inc score, check win.
	virtual void callback_block_died(Block*);

	/// Touch callback : start game or upd board pos.
	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	/// Touch move callback : upd board pos.
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
    
    CREATE_FUNC(GameArcanoid);

private:
	
	/// Wich side of block is hitted
	enum CollisionSide { none, left, right, top, bottom };

	/// Create game field: background, walls, board, ball, labels.
	///	@w - field width : distance betwen left and right walls
	///	@h - field height
	void CreateGameField(int w, int h);
	/// Create blocks field 15x5
	void CreateBlocks();

	/// Set position X for board move to (set m_board_move_targetX)
	void setBoardMoveTargetX(int x);
	/// Move board to m_board_move_targetX by delta*speed
	void updateBoardPos(float delta);
	/// Check ball collision with walls, board and each block
	void checkCollisions();

	/// End game handler : change screen to GameWin or GameOver
	/// @isWictory : wictory/lose flag
	void endGame(bool isWictory);

	std::vector<Block*> m_activeBlocksVector;		///< "Alive" blocks vector
	std::vector<Block*> m_inactiveBlocksVector;		///< "Dead" blocks vector
	cocos2d::Rect m_gameFieldRect;					///< Game field rect : need for wall collisions
	cocos2d::Sprite *m_board;						///< Board sprite
	cocos2d::Sprite *m_ball;						///< Ball sprite
	cocos2d::Label *m_tapToStartLabel;				///< Label "Tap to start"
	cocos2d::Label *m_scoreCounter;					///< Label with score num
	cocos2d::Label *m_scoreLabel;					///< Label "Score:"

	float m_ballSpeedCurrent;						///< Current ball speed: _BALL_BASE_FLY_SPEED * m_ballSpeedMultiplyer
	float m_ballSpeedMultiplyer;					///< Current speed multiplyer: 1.f + 0.1f for each 5 dead blocks
	float m_ballFlyAngle_d;							///< Current ball fly angle

	float m_board_move_targetX;						///< Position X for board move to

	int m_score;									///< Current score
	int m_blocksDestroyed;							///< Current cound of destroyed blocks

	bool m_firstTap;								///< First tap flag : need to hide "Tap to start" label and start game
};

#endif // __GAMEARCANOID_SCENE_H__
