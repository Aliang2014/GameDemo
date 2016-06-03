#pragma once
#include "cocos2d.h"
USING_NS_CC;

class ButtonHelper;
class GameLayer;
class MenuLayer:public LayerColor
{
public:
	static MenuLayer* create(GameLayer* layer);

private:
	bool init(GameLayer* layer);

	void createButtonBack();
	void createButtonSave();
	void createButtonLoad();
	void createButtonExit(); 

	void touchButtonBack(ButtonHelper*);
	void touchButtonSave(ButtonHelper*);
	void touchButtonLoad(ButtonHelper*);
	void touchButtonExit(ButtonHelper*);

	void removetext();
	void touchButtonYes(ButtonHelper*);
	void touchButtonNo(ButtonHelper*);

private:
	GameLayer* _layer;
	ButtonHelper* _back;
	ButtonHelper* _save;
	ButtonHelper* _load;
	ButtonHelper* _exit;
	
	ValueMap _button_map;
	ValueMap _tips_map;

	Label* _tips;
	Layer* _tip_layer;
	bool _isShowing;

};