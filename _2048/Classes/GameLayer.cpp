#include "GameLayer.h"
#include "Block.h"
#include "BlockManager.h"
#include "MenuLayer.h"
#include "ButtonHelper.h"

Scene* GameLayer::createScene()
{
	Scene* scene = Scene::create();
	GameLayer* layer = GameLayer::create();
	scene->addChild(layer);
	return scene;
}

GameLayer * GameLayer::create()
{
	GameLayer* layer = new GameLayer();
	if (layer && layer->init())
	{
		layer->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(layer);
	}
	return layer;
}

GameLayer::GameLayer()
	: _manager(nullptr)
	, _touch_delta(30.0f)
	, _move_succes(false)
	, _visible_size(Size::ZERO)
	, _broken_record(false)
{
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameLayer::touchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameLayer::touchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameLayer::touchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

GameLayer::~GameLayer()
{
	_eventDispatcher->removeEventListenersForTarget(this);
}

void GameLayer::gameOver()
{
	Label* show_text = Label::createWithSystemFont("", "fonts/simsun.ttf", 30);
	addChild(show_text);
	show_text->setPosition(_visible_size.width / 2, 500);
	show_text->setColor(Color3B::BLACK);
	int cur_score = _manager->getTotalScore();
	int high_score = UserDefault::getInstance()->getIntegerForKey("high_score");
	if (_broken_record)
	{
		// 恭喜，您创造了新的纪录？分
		std::string s1 = _text_map.at("broken1").asString();
		std::string s2 = _cur_score->getString();
		std::string s3 = _text_map.at("broken2").asString();
		show_text->setString(s1 + s2 + s3);
	}
	else
	{
		// 很遗憾，您还差？分打破记录
		std::string s1 = _text_map.at("unbroken1").asString();
		std::string s2 = StringUtils::toString(high_score - cur_score);
		std::string s3 = _text_map.at("unbroken2").asString();
		show_text->setString(s1 + s2 + s3);
	}
}

void GameLayer::saveCurState()
{
	UserDefault::getInstance()->setBoolForKey("exist", true);
		// cur_score
		int cur_score = _manager->getTotalScore();
		UserDefault::getInstance()->setIntegerForKey("save_score", cur_score);
		// cur_block
		_manager->saveState();
}

void GameLayer::loadLastState()
{
	_manager->setTotalScore(UserDefault::getInstance()->getIntegerForKey("save_score"));
	updateScore();
	_manager->loadState();
}

void GameLayer::clickNewGameButton(Ref* pSender)
{
	Director::getInstance()->replaceScene(GameLayer::createScene());
}

void GameLayer::clickMenuButton(Ref* pSender)
{
	auto layer = MenuLayer::create(this);
	addChild(layer);	
}

bool GameLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(245, 245, 245, 255)))
		return false;
	_visible_size = Director::getInstance()->getVisibleSize();
	_text_map = FileUtils::getInstance()->getValueMapFromFile("text_map.xml");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bgsp.plist");

	createGameMarkUI();
	createManager(4, 4);
	createCurScoreUI();
	createHighScoreUI();

	createMenuButton();
	createNewGameButton();
	return true;
}

void GameLayer::onEnter()
{
	LayerColor::onEnter();
	startGame(2);
}

void GameLayer::onExit()
{
	LayerColor::onExit();
}

void GameLayer::startGame(int first_show)
{
	for (int i = 0; i < first_show; ++i)
		_manager->showNewBlock();
}

bool GameLayer::checkOver()
{
	if (_manager->gameOver())
	{
		return true;
	}
	return false;
}

void GameLayer::updateScore()
{
	int cur_score = _manager->getTotalScore();
	int high_score = UserDefault::getInstance()->getIntegerForKey("best_score");
	_cur_score->setString(StringUtils::toString(cur_score));
	if (cur_score > high_score)
	{
		_broken_record = true;
		UserDefault::getInstance()->setIntegerForKey("best_score", cur_score);
		_high_score->setString(StringUtils::toString(cur_score));
	}
}

bool GameLayer::touchBegan(Touch * touch, Event * event)
{
	_move_succes = false;
	return true;
}

void GameLayer::touchMoved(Touch * touch, Event * event)
{
	Vec2 delta = touch->getLocation() - touch->getStartLocation();
	if (!_move_succes)
	{
		if (delta.x > _touch_delta)
		{
			_move_succes = _manager->moveRight();
		}
		else if (delta.x < -_touch_delta)
		{
			_move_succes = _manager->moveLeft();
		}
		else if (delta.y > _touch_delta)
		{
			_move_succes = _manager->moveUp();
		}
		else if (delta.y < -_touch_delta)
		{
			_move_succes = _manager->moveDown();
		}
	}
}

void GameLayer::touchEnded(Touch * touch, Event * event)
{
	if (_move_succes)
	{
		_manager->showNewBlock();
	}
	updateScore();
	// check over
	if (checkOver())
	{
		gameOver();
	}
}

void GameLayer::createGameMarkUI()
{
	// block name 
	Sprite* sp = Sprite::createWithSpriteFrameName("5.png");
	sp->setColor(Color3B(255, 220, 0));
	Size s1 = sp->getContentSize();
	Size s2 = Size(130, 130);
	sp->setScale(s2.width / s1.width, s2.height / s1.height);
	addChild(sp);
	sp->setPosition(80, _visible_size.height - 90);
	// text
	Label* text = Label::create("2048", "fonts/arial.ttf", 60);
	sp->addChild(text);
	text->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
}

void GameLayer::createManager(int x, int y)
{
	_manager = BlockManager::create(x, y, this);
	addChild(_manager);
	_manager->setPosition(_visible_size.width / 2, 50);
}

void GameLayer::createCurScoreUI()
{
	Sprite* sp = Sprite::createWithSpriteFrameName("5.png");
	sp->setColor(Color3B(174, 174, 174));
	Size s1 = sp->getContentSize();
	Size s2 = Size(100, 70);
	sp->setScale(s2.width / s1.width, s2.height / s1.height);
	addChild(sp);
	sp->setPosition(210, _visible_size.height - 60);
	// describe
	Label* text = Label::createWithSystemFont("", "fonts/simsun.ttf", 24);
	text->setString(_text_map.at("score").asString());
	addChild(text);
	text->setPosition(210, _visible_size.height - 42);
	// cur_score
	_cur_score = Label::createWithTTF("0", "fonts/arial.ttf", 32);
	addChild(_cur_score);
	_cur_score->setPosition(210, _visible_size.height - 75);
}

void GameLayer::createHighScoreUI()
{
	Sprite* sp = Sprite::createWithSpriteFrameName("5.png");
	sp->setColor(Color3B(170, 170, 170));
	Size s1 = sp->getContentSize();
	Size s2 = Size(105, 70);
	sp->setScale(s2.width / s1.width, s2.height / s1.height);
	addChild(sp);
	sp->setPosition(328, _visible_size.height - 60);
	// describe
	Label* text = Label::createWithSystemFont("", "fonts/simsun.ttf", 20);
	text->setString(_text_map.at("high_score").asString());
	addChild(text);
	text->setPosition(328, _visible_size.height - 42);
	// high_score
	_high_score = Label::createWithTTF("0", "fonts/arial.ttf", 32);
	int high_score = UserDefault::getInstance()->getIntegerForKey("best_score", 0);
	_high_score = Label::createWithTTF(StringUtils::toString(high_score), "fonts/arial.ttf", 30); 
	addChild(_high_score);
	_high_score->setPosition(328, _visible_size.height - 75);
}

void GameLayer::createNewGameButton()
{
	// layer
	auto button = ButtonHelper::create(Size(105, 45), CC_CALLBACK_1(GameLayer::clickNewGameButton, this));
	button->setPosition(328, _visible_size.height - 132.5);
	addChild(button);
	// label
	Label* name = Label::createWithSystemFont("", "fonts/simsun.ttf", 24);
	name->setString(_text_map.at("start").asString());
	button->setNameLabel(name);
	//sprite
	Sprite* sp = Sprite::createWithSpriteFrameName("3.png");
	button->setBgSprite(sp);
}

void GameLayer::createMenuButton()
{
	auto button = ButtonHelper::create(Size(100, 45), CC_CALLBACK_1(GameLayer::clickMenuButton, this));
	button->setPosition(210, _visible_size.height - 132.5);
	addChild(button);
	// label
	Label* name = Label::createWithSystemFont("", "fonts/simsun.ttf", 24);
	name->setString(_text_map.at("menu").asString());
	button->setNameLabel(name);
	// sprite
	Sprite* sp = Sprite::createWithSpriteFrameName("3.png");
	button->setBgSprite(sp);
}
