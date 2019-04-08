#include "SceneGameOver.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"

#include "SceneGameArcanoid.h"

USING_NS_CC;

Scene* SceneGameOver::createScene(int score)
{
	auto scene = SceneGameOver::create();
	scene->setScore(score);
	return scene;
}

bool SceneGameOver::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	cocos2d::Vec2 origin = director->getVisibleOrigin();
	cocos2d::Vec2 center = origin + visibleSize / 2;

	auto gameoverText = cocos2d::Label::createWithSystemFont("GAME OVER", "Arial", 36);
	gameoverText->setAlignment(cocos2d::TextHAlignment::CENTER);
	gameoverText->setPosition(center + cocos2d::Vec2(0, 50));
	this->addChild(gameoverText);

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

void SceneGameOver::setScore(int score)
{
	m_scoreCounter->setString("Your score: " + std::to_string(score));
}