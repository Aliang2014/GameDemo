#include "GameLayer.h"
#include "EntityManager.h"
#include "GameOverLayer.h"
USING_NS_CC;

cocos2d::Scene * GameLayer::createScene()
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

void GameLayer::refreshScore(float new_score)
{
	_score_label->setString(StringUtils::format("score:%.f", new_score));
}

void GameLayer::addAvailableBomb()
{
	_avaiable_bomb = true;
	_bomb_sp->setVisible(_avaiable_bomb);
}

void GameLayer::touchPauseButton()
{
	if (_game_pause)
	{
		_pause_button->setSelectedImage(Sprite::createWithSpriteFrameName("game_pause_pressed.png"));
		_pause_button->setNormalImage(Sprite::createWithSpriteFrameName("game_pause_nor.png"));
		Director::getInstance()->resume();
		_eventDispatcher->resumeEventListenersForTarget(_manager, true);
		_eventDispatcher->resumeEventListenersForTarget(this);
		_game_pause = false;	
	}
	else
	{
		_pause_button->setSelectedImage(Sprite::createWithSpriteFrameName("game_resume_pressed.png"));
		_pause_button->setNormalImage(Sprite::createWithSpriteFrameName("game_resume_nor.png"));
		Director::getInstance()->pause();
		_eventDispatcher->pauseEventListenersForTarget(_manager, true);
		_eventDispatcher->pauseEventListenersForTarget(this);
		_game_pause = true;
	}
	_color_layer->setVisible(_game_pause);
}

void GameLayer::gameOver()
{
	Scene* over_scene = GameOverLayer::createScene(_manager->getTotalPoints());
	Director::getInstance()->replaceScene(TransitionSlideInR::create(1.0f, over_scene));
}

GameLayer::GameLayer()
	: _bg_sp1(nullptr)
	, _bg_sp2(nullptr)
	, _score_label(nullptr)
	, _pause_button(nullptr)
	, _bomb_sp(nullptr)
	, _avaiable_bomb(false)
	, _manager(nullptr)
	, _game_pause(false)
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameLayer::touchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameLayer::touchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

GameLayer::~GameLayer()
{
	_eventDispatcher->removeEventListenersForTarget(this);
}

bool GameLayer::init()
{
	if (!Layer::init())
		return false;

	_visible_size = Director::getInstance()->getVisibleSize();
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot_background.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot.plist");

    createBackGround();
	createManager();
	createScoreUI();
	createPauseButton();
	createGoodsUI();
	
	scheduleUpdate();

	return true;
}

void GameLayer::createBackGround()
{
	_bg_sp1 = Sprite::createWithSpriteFrameName("background.png");
	_bg_sp2 = Sprite::createWithSpriteFrameName("background.png");
	addChild(_bg_sp1);
	addChild(_bg_sp2);
	_bg_sp1->setPosition(_visible_size.width / 2, _bg_sp1->getContentSize().height / 2);
	_bg_sp2->setPosition(_visible_size.width / 2, _bg_sp1->getContentSize().height + _bg_sp2->getContentSize().height / 2 - 15.0f);
	MoveBy* bg_move = MoveBy::create(1.0f, Vec2(0, -50.0f));
	RepeatForever* ac = RepeatForever::create(bg_move);
	_bg_sp1->runAction(ac);
	_bg_sp2->runAction(ac->clone());
}

void GameLayer::createManager()
{
	_manager = EntityManager::createWithLayer(this);
	addChild(_manager);
}

void GameLayer::createScoreUI()
{
	_score_label = Label::createWithSystemFont("score:0", "Marker Felt.ttf", 24);
	addChild(_score_label);
	_score_label->setAnchorPoint(Vec2(0, 0));
	_score_label->setPosition(10, 680);
	_score_label->setColor(Color3B::BLACK);
}

void GameLayer::createGoodsUI()
{
	_bomb_sp = Sprite::createWithSpriteFrameName("bomb.png");
	addChild(_bomb_sp);
	_bomb_sp->setPosition(50, 50);
	_bomb_sp->setVisible(_avaiable_bomb);
}

void GameLayer::createPauseButton()
{
	_pause_button = MenuItemSprite::create(Sprite::createWithSpriteFrameName("game_pause_nor.png"), Sprite::createWithSpriteFrameName("game_pause_pressed.png"), CC_CALLBACK_0(GameLayer::touchPauseButton, this));
	_pause_button->setPosition(370, 680);
	auto menu = Menu::create(_pause_button, nullptr);
	addChild(menu);
	menu->setPosition(Vec2::ZERO);

	// create color_layer
	_color_layer = LayerColor::create(Color4B::GRAY);
	_color_layer->setOpacity(120);
	addChild(_color_layer);
	_color_layer->setVisible(_game_pause);
}

void GameLayer::update(float dt)
{
	if (_bg_sp1->getPositionY() < -_bg_sp1->getContentSize().height / 2)
	{
		_bg_sp1->setPositionY(_bg_sp1->getContentSize().height / 2 + _bg_sp2->getContentSize().height - 20.0f);
	}
	if (_bg_sp2->getPositionY() < -_bg_sp2->getContentSize().height / 2)
	{
		_bg_sp2->setPositionY(_bg_sp1->getContentSize().height + _bg_sp2->getContentSize().height / 2 - 20.0f);
	}
}

bool GameLayer::touchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	if (_avaiable_bomb)
	{
		Vec2 touch_pos = touch->getLocation();
		if (_bomb_sp->getBoundingBox().containsPoint(touch_pos))
		{
			_bomb_sp->setScale(1.1f);
			return true;
		}
	}
	return false;
}

void GameLayer::touchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	Vec2 touch_pos = touch->getLocation();
	if (_bomb_sp->getBoundingBox().containsPoint(touch_pos))
	{
		_manager->useGoodsBomb();
		_avaiable_bomb = false;
		_bomb_sp->setVisible(_avaiable_bomb);
	}
	_bomb_sp->setScale(1.0f);
}

