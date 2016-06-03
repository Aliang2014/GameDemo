#pragma once
#include "cocos2d.h"
USING_NS_CC;


class ButtonHelper :public Node
{
public:
	static ButtonHelper* create(const Size& size, const std::function<void(ButtonHelper*)>& onTrigger);

	void setBgSprite(Sprite* sprite);
	void setNameLabel(Label* label);;
	void setEnable(bool enable);
	bool getEnable() { return _enable; }

	void runHitAnimation(float scale);

private:
	ButtonHelper();
	~ButtonHelper();

private:
	bool init(const Size& size, const std::function<void(ButtonHelper*)>& onTrigger);

	bool touchHit(Touch* touch);
	bool touchBegan(Touch* touch, Event* event);
	void touchEnded(Touch* touch, Event* event);

private:
	Size _button_size;
	bool _enable;
	Label* _name_label;
	Sprite* _bg_sprite;
	std::function<void(ButtonHelper*)> _onTrigger;
};