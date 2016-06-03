#include "GameLayer.h"
#include "CatSprite.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
using namespace CocosDenshion;

cocos2d::Scene* GameLayer::createScene()
{
	Scene* scene = Scene::create();
	GameLayer* layer = GameLayer::createWithMapName("CatMaze.tmx");
	scene->addChild(layer);
	return scene;
}

GameLayer * GameLayer::createWithMapName(const char * name)
{
	GameLayer* ret = new GameLayer();
	if (ret && ret->initWithMapName(name))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

void GameLayer::catMeetDog(const cocos2d::Point& tileCoord)
{
	log("hi, dog!");
	if (_bonesCount == 0)
	{
		loseGame();
	}
	else
	{
		SimpleAudioEngine::getInstance()->playEffect("catAttack.wav");

		--_bonesCount;
		showNumBones(_bonesCount);
		_mapHelper->removeObjectAtTileCoord(tileCoord);
	}
}

void GameLayer::catMeetExit(const cocos2d::Point& tileCoord)
{
	log("yes, i did");
	winGame();
}

void GameLayer::catMeetBone(const cocos2d::Point& tileCoord)
{
	log("wa, good");
	SimpleAudioEngine::getInstance()->playEffect("pickup.wav");

	++_bonesCount;
	showNumBones(_bonesCount);
	_mapHelper->removeObjectAtTileCoord(tileCoord);
}

void GameLayer::catMeetNothing(const cocos2d::Point& tileCoord)
{
	log("i need some luck");
	SimpleAudioEngine::getInstance()->playEffect("step.wav");
}

GameLayer::GameLayer()
	: _visibleSize(Director::getInstance()->getVisibleSize())
	, _mapHelper(nullptr)
	, _cat(nullptr)
	, _gameOver(false)
	, _won(false)
	, _bonesCount(0)
	, _bonesUI(nullptr)
{
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameLayer::touchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameLayer::touchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

GameLayer::~GameLayer()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	SpriteFrameCache::getInstance()->destroyInstance();
}

bool GameLayer::initWithMapName(const char* name)
{
	if (!Layer::init())
		return false;

	loadGameResource();

	createMap(name);
	createBonesUI();
	createCatSprite();

	SimpleAudioEngine::getInstance()->playBackgroundMusic("SuddenDefeat.mp3", true);

	this->scheduleUpdate();

	return true;
}

void GameLayer::loadGameResource()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("CatMaze.plist");

	SimpleAudioEngine::getInstance()->playBackgroundMusic("SuddenDefeat.mp3", true);
	SimpleAudioEngine::getInstance()->preloadEffect("pickup.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("catAttack.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("hitWall.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("lose.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("step.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("win.wav");
}

void GameLayer::createMap(const char * name)
{
	_mapHelper = MapHelper::createWithName(name);
	addChild(_mapHelper);
}

void GameLayer::createBonesUI()
{
	_bonesUI = Label::createWithTTF("Bones: 0", "arial.ttf", 24);
	_bonesUI->setPosition(400, 30);
	addChild(_bonesUI);
}

void GameLayer::createCatSprite()
{
	_cat = CatSprite::createWithLayer(this);
	_mapHelper->addChild(_cat);

	_cat->setPosition(_mapHelper->getPositionFromTileCoord(Point(3, 24)));
}

bool GameLayer::touchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	Point touchInNode = _mapHelper->convertTouchToNodeSpace(touch);
	Point touchInTiledCoord = _mapHelper->getTileCoordFromPosition(touchInNode);

//	log("%f, %f", touchInTiledCoord.x, touchInTiledCoord.y);

	_cat->towardToDestination(_cat->getCurTileCoord(), touchInTiledCoord);

	return true;
}

void GameLayer::touchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	;
}

void GameLayer::update(float dt)
{
	setViewPointOfCenter(_cat->getPosition());
}

void GameLayer::setViewPointOfCenter(const cocos2d::Point & pos)
{
	int x = MAX(pos.x, _visibleSize.width / 2);
	int y = MAX(pos.y, _visibleSize.height / 2);
	x = MIN(x, _mapHelper->getMapSize().width - _visibleSize.width / 2);
	y = MIN(y, _mapHelper->getMapSize().height - _visibleSize.height / 2);

	Point diff = Point(_visibleSize / 2) - Point(x, y);
	_mapHelper->setPosition(diff);
}

void GameLayer::winGame()
{
	SimpleAudioEngine::getInstance()->playEffect("win.wav");
	_gameOver = true;
	_won = true;
	this->endScene();
}

void GameLayer::loseGame()
{
	SimpleAudioEngine::getInstance()->playEffect("lose.wav");
	_gameOver = false;
	_won = false;
	this->endScene();
}

void GameLayer::showNumBones(int numBones)
{
	_bonesUI->setString(StringUtils::format("Bones: %d", numBones));
}

void GameLayer::showRestartMenu()
{
	const char *message;
	if (_won)
	{
		message = "You win!";
	}
	else
	{
		message = "You lose!";
	}
	LabelBMFont *label = LabelBMFont::create(message, "Arial.fnt");
	label->setScale(0.1f);
	label->setPosition(_visibleSize.width / 2, _visibleSize.height * 0.6f);
	addChild(label);

	LabelBMFont *restartLabel = LabelBMFont::create("Restart", "Arial.fnt");
	MenuItemLabel *restartItem = MenuItemLabel::create(restartLabel, [](Ref *sender) {
		Director::getInstance()->replaceScene(TransitionZoomFlipX::create(0.5f, GameLayer::createScene()));
	});
	restartItem->setScale(0.1f);
	restartItem->setPosition(_visibleSize.width / 2, _visibleSize.height * 0.4f);

	Menu *menu = Menu::createWithItem(restartItem);
	menu->setPosition(Point::ZERO);
	addChild(menu);

	restartItem->runAction(ScaleTo::create(0.5f, 1.0f));
	label->runAction(ScaleTo::create(0.5f, 1.0f));
}

void GameLayer::endScene()
{
	_cat->runAction(Sequence::create(ScaleBy::create(0.5f, 3.0f),
		DelayTime::create(1.0f),
		ScaleTo::create(0.5f, 0.0f),
		CallFunc::create(CC_CALLBACK_0(GameLayer::showRestartMenu, this)),
		nullptr));
	_cat->runAction(RepeatForever::create(RotateBy::create(0.5f, 360.0f)));
}