#include "SceneMainMenu.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"

#include "SceneGameArcanoid.h"

USING_NS_CC;

Scene* SceneMainMenu::createScene()
{
	return SceneMainMenu::create();
}

bool SceneMainMenu::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	cocos2d::Vec2 origin = director->getVisibleOrigin();
	cocos2d::Vec2 center = origin + visibleSize / 2;

	auto arcLabel = cocos2d::Label::createWithSystemFont("ARCANOID!", "Arial", 72);
	arcLabel->setAlignment(cocos2d::TextHAlignment::CENTER);
	arcLabel->setPosition(center + cocos2d::Vec2(0, 100.f));
	this->addChild(arcLabel);

	auto startButton = ui::Button::create("textures/ui/buttons/btn_yellow.png", "textures/ui/buttons/btn_yellow_selected.png", "textures/ui/buttons/btn_disabled.png");
	startButton->setPosition(center);
	startButton->setTitleText("START");
	startButton->setTitleFontName("fonts/arial.ttf");
	startButton->setTitleFontSize(36);
	startButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
		if(type == ui::Widget::TouchEventType::ENDED)
			Director::getInstance()->replaceScene(TransitionFade::create(0.5, GameArcanoid::create(), Color3B(255, 255, 255)));
	});
	this->addChild(startButton);

	return true;
}