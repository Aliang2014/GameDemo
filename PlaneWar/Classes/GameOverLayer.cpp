#include "GameOverLayer.h"
#include "GameLayer.h"
using namespace cocos2d;

Scene * GameOverLayer::createScene(float points)
{
	Scene* scene = Scene::create();
	auto layer = GameOverLayer::createWithPoints(points);
	scene->addChild(layer);
	return scene;
}

GameOverLayer * GameOverLayer::createWithPoints(float points)
{
	GameOverLayer* ret = new GameOverLayer();
	if (ret && ret->initWithPoints(points))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

void GameOverLayer::touchButtonRetry(Ref*)
{
	Director::getInstance()->replaceScene(GameLayer::createScene());
}

void GameOverLayer::touchButtonExit(Ref*)
{
	Director::getInstance()->end();
}

bool GameOverLayer::initWithPoints(float points)
{
	if (!Layer::init())
		return false;

	createBackGround();
	createMenuUI();
	createPointsUI(points);

	return true;
}

void GameOverLayer::createBackGround()
{
	auto sp = Sprite::createWithSpriteFrameName("gameover.png");
	sp->setPosition(Director::getInstance()->getVisibleSize().width / 2, getContentSize().height / 2);
	addChild(sp);
}

void GameOverLayer::createMenuUI()
{
	auto retry = MenuItemFont::create("Restart", CC_CALLBACK_1(GameOverLayer::touchButtonRetry,this));
	auto exit = MenuItemFont::create("Exit", CC_CALLBACK_1(GameOverLayer::touchButtonExit, this));
	retry->setPosition(200, 300);
	exit->setPosition(200, 200);
	retry->setColor(Color3B::BLACK);
	exit->setColor(Color3B::BLACK);
	Menu* menu = Menu::create(retry, exit, nullptr);
	addChild(menu);
	menu->setPosition(Vec2::ZERO);
}

void GameOverLayer::createPointsUI(float points)
{
	float best = UserDefault::getInstance()->getFloatForKey("best_score", 0);
	if (points > best)
	{
		UserDefault::getInstance()->setFloatForKey("best_score", points);
	}
	Label* best_label = Label::createWithTTF(StringUtils::toString(UserDefault::getInstance()->getFloatForKey("best_score", 0)), "fonts/arial.ttf", 30);
	Label* cur_label = Label::createWithTTF(StringUtils::toString(points), "fonts/arial.ttf", 30);
	addChild(best_label);
	addChild(cur_label);
	best_label->setColor(Color3B::BLACK);
	cur_label->setColor(Color3B::BLACK);
	best_label->setPosition(140, 690);
	cur_label->setPosition(200, 400);
}
