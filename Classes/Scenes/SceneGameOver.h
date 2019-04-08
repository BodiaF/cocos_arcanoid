#ifndef __ENDGAMESCENE_H_
#define __ENDGAMESCENE_H_

#include "cocos2d.h"

class SceneGameOver : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene(int score = 0);

	virtual bool init();
	
	void setScore(int score);

	CREATE_FUNC(SceneGameOver);

private:
	cocos2d::Label *m_scoreCounter;
};

#endif // !__ENDGAMESCENE_H_