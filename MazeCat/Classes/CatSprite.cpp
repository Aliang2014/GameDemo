#include "CatSprite.h"
#include "GameLayer.h"
#include "AStarPathHelper.h"
using namespace cocos2d;

CatSprite * CatSprite::createWithLayer(GameLayer * layer)
{
	CatSprite* ret = new CatSprite();
	if (ret && ret->initWithLayer(layer))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

CatSprite::CatSprite()
	: _layer(nullptr)
	, _moveUpAnimation(nullptr)
	, _moveDownAnimation(nullptr)
	, _moveLeftAnimation(nullptr)
	, _moveRightAnimation(nullptr)
	, _pathMoveController(nullptr)
	, _enableMoveAnimate(false)
	, _curAnimate(nullptr)
	, _curAnimation(nullptr)
	, _nextAnimation(nullptr)
{
}

CatSprite::~CatSprite()
{
	CC_SAFE_RELEASE(_moveDownAnimation);
	CC_SAFE_RELEASE(_moveUpAnimation);
	CC_SAFE_RELEASE(_moveRightAnimation);
	CC_SAFE_RELEASE(_moveLeftAnimation);
}

bool CatSprite::initWithLayer(GameLayer * layer)
{
	if (!initWithSpriteFrameName("cat_forward_1.png"))
		return false;

	_layer = layer;

	createPathMoveController();

	_moveDownAnimation = createMoveAnimation("forward");
	_moveDownAnimation->retain();
	_moveLeftAnimation = createMoveAnimation("left");
	_moveLeftAnimation->retain();
	_moveRightAnimation = createMoveAnimation("right");
	_moveRightAnimation->retain();
	_moveUpAnimation = createMoveAnimation("back");
	_moveUpAnimation->retain();

	this->scheduleUpdate();
	return true;
}


void CatSprite::towardToDestination(const cocos2d::Point & origin, const cocos2d::Point & dest)
{
	AStarPathHelper helper(origin, dest, _layer);
	if (helper.getFoundPath())
	{
		Vector<AStarPathUnit*> vec = helper.getPathVec();
		helper.printPathInfo();
		_pathMoveController->startMove(vec);
	}
}

void CatSprite::onStartMoving()
{
	_enableMoveAnimate = true;
}

void CatSprite::onArrivedTiledCenter()
{
	MapHelper* helper = _layer->getMapHelper();
	Point tileCoord = helper->getTileCoordFromPosition(getPosition());
	// exit
	if (helper->isExitAtTileCoord(tileCoord))
		_layer->catMeetExit(tileCoord);
	// dog
	else if (helper->isDogAtTileCoord(tileCoord))
		_layer->catMeetDog(tileCoord);
	// bone
	else if (helper->isBoneAtTileCoord(tileCoord))
		_layer->catMeetBone(tileCoord);
	else
		_layer->catMeetNothing(tileCoord);
}

void CatSprite::onChangeDirection(PathDirection dir)
{
	switch (dir)
	{
	case en_Direction_Up:
		_nextAnimation = _moveUpAnimation;
		break;
	case en_Direction_Down:
		_nextAnimation = _moveDownAnimation;
		break;
	case en_Direction_Left:
		_nextAnimation = _moveLeftAnimation;
		break;
	case en_Direction_Right:
		_nextAnimation = _moveRightAnimation;
		break;
	case en_Direction_None:
		break;
	default:
		break;
	}
}

void CatSprite::onArrived()
{
	_enableMoveAnimate = false;
}

cocos2d::Point CatSprite::getCurTileCoord() const
{
	return _layer->getMapHelper()->getTileCoordFromPosition(this->getPosition());
}

void CatSprite::createPathMoveController()
{
	_pathMoveController = PathMoveController::create(this, _layer->getMapHelper());
	addChild(_pathMoveController);

	_pathMoveController->setPathMoveControllerListener(this);
}

cocos2d::Animation* CatSprite::createMoveAnimation(const char* type)
{
	Animation* animation = Animation::create();
	for (int i = 1; i <= 2; ++i)
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("cat_%s_%d.png", type, i)));
	animation->setDelayPerUnit(0.2f);

	return animation;
}

void CatSprite::runMoveAnimate(Animation * animation)
{
	if (_curAnimation == animation)
		return;

	_curAnimation = animation;
	if (_curAnimate != nullptr)
	{
		this->stopAction(_curAnimate);
	}
	if (_curAnimation != nullptr)
	{
		_curAnimate = RepeatForever::create(Animate::create(animation));
		this->runAction(_curAnimate);
	}
}

void CatSprite::update(float dt)
{
	if (_enableMoveAnimate)
	{
		runMoveAnimate(_nextAnimation);
	}
	else
	{
		if (_curAnimation != nullptr)
		{
			this->stopAction(_curAnimate);
			_curAnimation = nullptr;
		}
	}
}
