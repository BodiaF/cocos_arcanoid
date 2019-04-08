#include "SceneGameWin.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"

#include "SceneGameArcanoid.h"

USING_NS_CC;

Scene* SceneGameWin::createScene(int score)
{
	auto scene = SceneGameWin::create();
	scene->setScore(score);
	return scene;
}

bool SceneGameWin::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	cocos2d::Vec2 origin = director->getVisibleOrigin();
	cocos2d::Vec2 center = origin + visibleSize / 2;

	auto winText = cocos2d::Label::createWithSystemFont("YOU WIN", "Arial", 36);
	winText->setAlignment(cocos2d::TextHAlignment::CENTER);
	winText->setPosition(center + cocos2d::Vec2(0, 50));
	this->addChild(winText);

	auto replayButton = ui::Button::create("textures/ui/buttons/btn_red.png", "textures/ui/buttons/btn_red_selected.png", "textures/ui/buttons/btn_disabled.png");
	replayButton->setPosition(center - cocos2d::Vec2(0, 80));
	replayButton->setTitleText("REPLAY");
	replayButton->setTitleFontName("fonts/arial.ttf");
	replayButton->setTitleFontSize(24);
	replayButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
			Director::getInstance()->replaceScene(TransitionFade::create(0.5, GameArcanoid::create(), Color3B(255, 255, 255)));
	});
	this->addChild(replayButton);

	m_scoreCounter = cocos2d::Label::createWithSystemFont("Your score: 0", "Arial", 38);
	m_scoreCounter->setAlignment(cocos2d::TextHAlignment::CENTER);
	m_scoreCounter->setPosition(center);
	this->addChild(m_scoreCounter);

	return true;
}

void SceneGameWin::setScore(int score)
{
	m_scoreCounter->setString("Your score: " + std::to_string(score));
}