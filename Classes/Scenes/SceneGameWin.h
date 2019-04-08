#ifndef __SCENEGAMEWIN_H_
#define __SCENEGAMEWIN_H_

#include "cocos2d.h"

class SceneGameWin : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene(int score = 0);

	virtual bool init();

	void setScore(int score);

	CREATE_FUNC(SceneGameWin);

private:
	cocos2d::Label *m_scoreCounter;
};

#endif // !__SCENEGAMEWIN_H_