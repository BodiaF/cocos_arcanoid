#include "SceneGameArcanoid.h"
#include "SimpleAudioEngine.h"
#include "SceneGameOver.h"
#include "SceneGameWin.h"

USING_NS_CC;

// =============== //

int   _BLOCK_W = 64;
int   _BLOCK_H = 32;
int   _BOARD_W = 128;
int   _BALL_RAD = 16;
int   _COLLISION_CHECK_RADIUS = 100;
float _BALL_BASE_FLY_SPEED = 800.f; // pixel/sec
float _BOARD_FLY_SPEED = 1000.f; // pixel/sec

// =============== //

Scene* GameArcanoid::createScene()
{
    return GameArcanoid::create();
}

bool GameArcanoid::init()
{
    if(!Scene::init())
    {
        return false;
    }

	auto director = Director::getInstance();
    auto visibleSize = director->getVisibleSize();
	cocos2d::Vec2 origin = director->getVisibleOrigin();

	m_firstTap = false;
	m_score = 0;
	m_blocksDestroyed = 0;
	m_ballSpeedCurrent = _BALL_BASE_FLY_SPEED;
	m_ballSpeedMultiplyer = 1.f;
	m_ballFlyAngle_d = 30.f;
   m_board_move = false;
	m_board_move_targetX = origin.x + visibleSize.width / 2;

	// width: 15 blocks | height: viewHeight - 0.5 wall sprite size
	CreateGameField(_BLOCK_W*15, visibleSize.height - 32);
	CreateBlocks();

	// touch events
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameArcanoid::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GameArcanoid::onTouchMoved, this);
   touchListener->onTouchEnded = CC_CALLBACK_2(GameArcanoid::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

GameArcanoid::~GameArcanoid()
{
	std::for_each(m_activeBlocksVector.begin(), m_activeBlocksVector.end(), [=](Block *block) {
      delete block;
   });
	m_activeBlocksVector.clear();
}

// =============== //

void GameArcanoid::CreateGameField(int w, int h)
{
	auto director = cocos2d::Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	cocos2d::Vec2 origin = director->getVisibleOrigin();

	m_gameFieldRect = cocos2d::Rect(origin.x + visibleSize.width / 2 - w / 2, origin.y, w, h);


	Texture2D::TexParams tileParams;
	tileParams.minFilter = GL_NEAREST;
	tileParams.magFilter = GL_NEAREST;
	tileParams.wrapS = GL_REPEAT;
	tileParams.wrapT = GL_REPEAT;

	// background
	auto bgSprite = cocos2d::Sprite::create("textures/background/bg.png");
	bgSprite->setPosition(cocos2d::Vec2(origin.x, origin.y));
	bgSprite->setAnchorPoint(cocos2d::Vec2(0, 0));
	bgSprite->getTexture()->setTexParameters(tileParams);
	bgSprite->setTextureRect(cocos2d::Rect(0, 0, visibleSize.width, visibleSize.height));
	this->addChild(bgSprite, -2);

	// build walls
	auto wall_left = cocos2d::Sprite::create("textures/background/r4.png");
	wall_left->setPosition(cocos2d::Vec2(m_gameFieldRect.getMinX(), m_gameFieldRect.getMinY()));
	wall_left->setAnchorPoint(cocos2d::Vec2(1,0));
	wall_left->getTexture()->setTexParameters(tileParams);
	wall_left->setTextureRect(cocos2d::Rect(0, 0, wall_left->getTexture()->getPixelsWide(), h));
	this->addChild(wall_left, -1);

	auto wall_right = cocos2d::Sprite::create("textures/background/r4.png");
	wall_right->setPosition(cocos2d::Vec2(m_gameFieldRect.getMaxX(), m_gameFieldRect.getMinY()));
	wall_right->setAnchorPoint(cocos2d::Vec2(0, 0));
	wall_right->getTexture()->setTexParameters(tileParams);
	wall_right->setTextureRect(cocos2d::Rect(0, 0, wall_right->getTexture()->getPixelsWide(), h));
	this->addChild(wall_right, -1);

	auto wall_top = cocos2d::Sprite::create("textures/background/r4.png");
	wall_top->setPosition(cocos2d::Vec2(m_gameFieldRect.getMidX(), m_gameFieldRect.getMaxY()));
	wall_top->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
	wall_top->getTexture()->setTexParameters(tileParams);
	wall_top->setTextureRect(cocos2d::Rect(0, 0, w + wall_top->getTexture()->getPixelsWide()*2, wall_top->getTexture()->getPixelsHigh()));
	this->addChild(wall_top, -1);

	// create board
	m_board = cocos2d::Sprite::create("textures/boards/board_128x16.png");
	m_board->setPosition(cocos2d::Vec2(m_board_move_targetX, origin.y + m_board->getTexture()->getPixelsHigh()*2.f));
	this->addChild(m_board, 3);
	_BOARD_W = m_board->getTexture()->getPixelsWide();

	// create ball
	m_ball = cocos2d::Sprite::create("textures/balls/ball_32.png");
	_BALL_RAD = m_ball->getTexture()->getPixelsHigh() / 2;
	m_ball->setPosition(m_board->getPosition() + cocos2d::Vec2(0, _BALL_RAD + m_board->getTexture()->getPixelsHigh()));
	this->addChild(m_ball, 4);

	// create labels
	m_tapToStartLabel = cocos2d::Label::createWithSystemFont("TAP TO START", "Arial", 48);
	m_tapToStartLabel->setAlignment(cocos2d::TextHAlignment::CENTER);
	m_tapToStartLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + 300);
	this->addChild(m_tapToStartLabel, 10);

	m_scoreLabel = cocos2d::Label::createWithSystemFont("Score:", "Arial", 32);
	m_scoreLabel->setAlignment(cocos2d::TextHAlignment::CENTER);
	m_scoreLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + 300);
	m_scoreLabel->setVisible(false);
	this->addChild(m_scoreLabel, 2);

	m_scoreCounter = cocos2d::Label::createWithSystemFont("0", "Arial", 38);
	m_scoreCounter->setAlignment(cocos2d::TextHAlignment::CENTER);
	m_scoreCounter->setPosition(m_scoreLabel->getPosition() - cocos2d::Vec2(0, 40));
	m_scoreCounter->setVisible(false);
	this->addChild(m_scoreCounter, 2);
}

void GameArcanoid::CreateBlocks()
{
	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	cocos2d::Vec2 origin = director->getVisibleOrigin();

	BlocksFactory *f = new BlocksFactory(this);
	f->_callback_died = CC_CALLBACK_1(GameArcanoid::callback_block_died, this);
	
	int startX = origin.x + (visibleSize.width - _BLOCK_W * 15) / 2;
	int startY = origin.y + visibleSize.height - 32 - _BLOCK_H * 2;

	cocos2d::Vec2 pos;
	int startHP;

	for (int row = 0; row < 5; row++) {
		for (int col = 0; col < 15; col++) {
			pos = cocos2d::Vec2(startX + _BLOCK_W * col, startY - _BLOCK_H * row);
			startHP = cocos2d::random(1,3);
			Block *block = f->CreateBaseBlock(pos, startHP);
			m_activeBlocksVector.push_back(block);
		}
	}
}

// =============== //

bool GameArcanoid::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (!m_firstTap) {
		// START
		m_firstTap = true;
		m_tapToStartLabel->setVisible(false);
		m_scoreLabel->setVisible(true);
		m_scoreCounter->setVisible(true);
		scheduleUpdate();
		return true;
	}

   m_board_move = true;
	setBoardMoveTargetX(touch->getLocation().x);
	return true;
}

void GameArcanoid::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	setBoardMoveTargetX(touch->getLocation().x);
}

void GameArcanoid::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
   m_board_move = true;
   setBoardMoveTargetX(m_board->getPosition().x);
}

void GameArcanoid::setBoardMoveTargetX(int x)
{
	int lDist = x - _BOARD_W / 2 - m_gameFieldRect.getMinX();
	int rDist = x + _BOARD_W / 2 - m_gameFieldRect.getMaxX();
	if (lDist < 0)
		x -= lDist;
	if (rDist > 0)
		x -= rDist;
	m_board_move_targetX = x;
}


// =============== //

void GameArcanoid::update(float delta)
{
	Scene::update(delta);
	
	if (!m_firstTap)
		return;

	updateBoardPos(delta);

	// move
	cocos2d::Rect ballRect = m_ball->getBoundingBox();
	cocos2d::Vec2 pos = m_ball->getPosition();
	float angle_rad = m_ballFlyAngle_d * 3.14159265f / 180.f;
	float s = m_ballSpeedCurrent * delta;
	float dx = s * cos(angle_rad);
	float dy = s * sin(angle_rad);
	pos += cocos2d::Vec2(dx, dy);

	m_ball->setPosition(pos);

	// Floor is lava!
	if (ballRect.getMinY() <= 0) {
		// and u die
		endGame(false);
		return;
	}
	
	checkCollisions();
}

void GameArcanoid::updateBoardPos(float delta)
{
   // no flag = no move
   if (!m_board_move)
      return;

	float currentX = m_board->getPosition().x;
	if (currentX == m_board_move_targetX)
		return;

	// new X pos
	float x;
	// shift
	float s = _BOARD_FLY_SPEED * delta;
	// if shift less than last distance
	if (std::abs(currentX - m_board_move_targetX) < s) {
		x = m_board_move_targetX;
	} else {
		// if move left
		if (m_board_move_targetX < currentX)
			s *= -1.f;
		x = currentX + s;
	}
	m_board->setPositionX(x);
}

void GameArcanoid::checkCollisions()
{
	cocos2d::Rect ballRect = m_ball->getBoundingBox();
	cocos2d::Vec2 pos = m_ball->getPosition();
	float angle_rad = m_ballFlyAngle_d * 3.14159265f / 180.f;

	bool vertical_collision = false;
	bool horizontal_collision = false;

	// == checker
	auto collisionChecker = [&](cocos2d::Rect targetRect) -> CollisionSide
	{
		CollisionSide side = none;
		if (targetRect.intersectsCircle(pos, _BALL_RAD)) {
			cocos2d::Vec2 p1(std::max(ballRect.getMinX(), targetRect.getMinX()), std::max(ballRect.getMinY(), targetRect.getMinY()));
			cocos2d::Vec2 p2(std::min(ballRect.getMaxX(), targetRect.getMaxX()), std::min(ballRect.getMaxY(), targetRect.getMaxY()));
			float rectW = std::abs(p1.x - p2.x);
			float rectH = std::abs(p1.y - p2.y);

			if (rectW >= rectH) {
				vertical_collision = true;
				if (pos.y + _BALL_RAD >= targetRect.getMinY() && pos.y + _BALL_RAD < targetRect.getMaxY()) {
					side = bottom;
					pos.y = targetRect.getMinY() - _BALL_RAD;
				} else {
					side = top;
					pos.y = targetRect.getMaxY() + _BALL_RAD;
				}
			} else {
				horizontal_collision = true;
				if (pos.x + _BALL_RAD >= targetRect.getMinX() && pos.x + _BALL_RAD < targetRect.getMaxX()) {
					side = left;
					pos.x = targetRect.getMinX() - _BALL_RAD;
				} else {
					side = right;
					pos.x = targetRect.getMaxX() + _BALL_RAD;
				}
			}

			m_ball->setPosition(pos);
		}

		return side;
	};


	// == check collision with walls
	if (ballRect.getMaxX() > m_gameFieldRect.getMaxX())
	{
		pos.x = m_gameFieldRect.getMaxX() - _BALL_RAD;
		horizontal_collision = true;
	}

	if (ballRect.getMinX() < m_gameFieldRect.getMinX())
	{
		pos.x = m_gameFieldRect.getMinX() + _BALL_RAD;
		horizontal_collision = true;
	}

	if (ballRect.getMaxY() > m_gameFieldRect.getMaxY())
	{
		pos.y = m_gameFieldRect.getMaxY() - _BALL_RAD;
		vertical_collision = true;
	}

	// == check collision with board
	cocos2d::Vec2 boardPos = m_board->getPosition();
	cocos2d::Rect boardRect = m_board->getBoundingBox();
	CollisionSide boardCollisionSide = collisionChecker(boardRect);
	switch (boardCollisionSide) {
	case left:
		// fly from left - switch fly direction
		if (cos(angle_rad) > 0)
			horizontal_collision = true;
		break;
	case right:
		// fly from right - switch fly direction
		if (cos(angle_rad) < 0)
			horizontal_collision = true;
		break;
	};
	if (boardCollisionSide != none && boardCollisionSide != bottom) {
		vertical_collision = true;
	}

	// == check collision with each block
	// vector of blocks and distances (for faster sort)
	std::vector<std::pair<Block*, float>> blocksInRange;
	std::for_each(m_activeBlocksVector.begin(), m_activeBlocksVector.end(), [&](Block* block) {
		cocos2d::Vec2 blockPos = block->GetPosition();
		float dist = sqrt(pow(blockPos.x - pos.x, 2) + pow(blockPos.y - pos.y, 2));
		if (dist < _COLLISION_CHECK_RADIUS)
			blocksInRange.push_back(std::pair<Block*, float>(block, dist));
	});
	// sort by distance
	std::sort(blocksInRange.begin(), blocksInRange.end(), [](std::pair<Block*, float> &lp, std::pair<Block*, float> &rp) {
		return lp.second < rp.second;
	});
	// check collision
	std::for_each(blocksInRange.begin(), blocksInRange.end(), [=](std::pair<Block*,float> &blockPair) {
		Block* block = blockPair.first;
		cocos2d::Vec2 blockPos = block->GetPosition();
		float dist = sqrt(pow(blockPos.x - pos.x, 2) + pow(blockPos.y - pos.y, 2));
		if (dist > _COLLISION_CHECK_RADIUS)
			return;
		if (collisionChecker(block->GetBounds()) != CollisionSide::none)
			block->Striked();
	});

	// == change fly dirrection
	if (horizontal_collision)
		m_ballFlyAngle_d = 180 - m_ballFlyAngle_d;
	if (vertical_collision)
		m_ballFlyAngle_d *= -1;
}

// =============== //

void GameArcanoid::callback_block_died(Block* block)
{
	cocos2d::log("block died");

	auto iter = m_activeBlocksVector.cbegin();
	while (iter != m_activeBlocksVector.cend()) {
		if (*iter == block) {
			m_activeBlocksVector.erase(iter);
         delete block;
			break;
		}
		++iter;
	}

	m_score += 10;
	m_scoreCounter->setString(std::to_string(m_score));
	m_blocksDestroyed++;

	cocos2d::log("score: %i", m_score);
	cocos2d::log("destroyed: %i", m_blocksDestroyed);

	// check endgame
	if (m_activeBlocksVector.empty()) {
		endGame(true);
		return;
	}

	// check speed
	m_ballSpeedMultiplyer = 1.f + (m_blocksDestroyed - m_blocksDestroyed % 5) / 50.f ;
	m_ballSpeedCurrent = _BALL_BASE_FLY_SPEED * m_ballSpeedMultiplyer;
	
	cocos2d::log("speed multi: %f", m_ballSpeedMultiplyer);
	cocos2d::log("speed: %f", m_ballSpeedCurrent);
}

// =============== //

void GameArcanoid::endGame(bool isWictory)
{
	unscheduleUpdate();
	auto nextScene = isWictory ? SceneGameWin::createScene(m_score) : SceneGameOver::createScene(m_score);
	Director::getInstance()->replaceScene(TransitionFade::create(0.2, nextScene, Color3B(0, 0, 0)));
}
