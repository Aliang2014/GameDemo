#include "ButtonHelper.h"

ButtonHelper * ButtonHelper::create(const Size & size, const std::function<void(ButtonHelper*)>& onTrigger)
{
	ButtonHelper* button = new ButtonHelper();
	if (button && button->init(size, onTrigger))
	{
		button->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(button);
	}
	return button;
}

void ButtonHelper::setBgSprite(Sprite * sprite)
{
	addChild(sprite, 0);
	sprite->setPosition(getContentSize() / 2);
	// 
	float x = getContentSize().width / sprite->getContentSize().width;
	float y = getContentSize().height / sprite->getContentSize().height;
	sprite->setScale(x, y);
}

void ButtonHelper::setNameLabel(Label * label)
{
	_name_label = label;
	addChild(_name_label, 1);
	_name_label->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
}

void ButtonHelper::setEnable(bool enable)
{
	;
}

void ButtonHelper::runHitAnimation(float scale)
{
	if (_name_label)
	{
		ScaleTo* action = ScaleTo::create(0.05f, scale);
		action->setTag(20000);
		stopActionByTag(20000);
		runAction(action);
	}
}

ButtonHelper::ButtonHelper()
	: _button_size(Size::ZERO)
	, _enable(true)
	, _name_label(nullptr)
	, _bg_sprite(nullptr)
	, _onTrigger(nullptr)
{
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(ButtonHelper::touchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(ButtonHelper::touchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
}

ButtonHelper::~ButtonHelper()
{
	_eventDispatcher->removeEventListenersForTarget(this);
}

bool ButtonHelper::init(const Size & size, const std::function<void(ButtonHelper*)>& onTrigger)
{
	_button_size = size;
	setContentSize(_button_size);
	setAnchorPoint(Vec2(0.5, 0.5));
	_onTrigger = onTrigger;

	return true;
}

bool ButtonHelper::touchHit(Touch* touch)
{
	Vec2 touch_in_node = convertTouchToNodeSpace(touch);
	if (touch_in_node.x >= 0 &&
		touch_in_node.x <= getContentSize().width &&
		touch_in_node.y >=0 &&
		touch_in_node.y <= getContentSize().height)
	{
		return true;
	}
	return false;
}

bool ButtonHelper::touchBegan(Touch* touch, Event* event)
{
	if (touchHit(touch))
	{
		runHitAnimation(1.05f);
		return true;
	}
	return false;
}

void ButtonHelper::touchEnded(Touch* touch, Event* event)
{
	if (touchHit(touch))
	{
		_onTrigger(this);
	}
	runHitAnimation(1.0f);
}
