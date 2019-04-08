#ifndef __SCENEMAINMENU_H_
#define __SCENEMAINMENU_H_

#include "cocos2d.h"

class SceneMainMenu : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(SceneMainMenu);
};

#endif // !__SCENEMAINMENU_H_