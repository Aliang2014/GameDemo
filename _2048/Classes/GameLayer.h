#pragma once
#include "cocos2d.h"
USING_NS_CC;

class BlockManager;
class MenuLayer;
class GameLayer :public LayerColor
{
public:
	static Scene* createScene();
	static GameLayer* create();

	GameLayer();
	~GameLayer();

	void gameOver();

	void saveCurState();
	void loadLastState();
	
	void clickNewGameButton(Ref* pSender);
	void clickMenuButton(Ref* pSender);

private:
	bool init() override;

	void startGame(int first_show);
	bool checkOver();

	void updateScore();

	void onEnter();
	void onExit();

	bool touchBegan(Touch* touch, Event* event);
	void touchMoved(Touch* touch, Event* event);
	void touchEnded(Touch* touch, Event* event);
	
	void createGameMarkUI();
	void createManager(int x, int y);
	void createCurScoreUI();
	void createHighScoreUI();

	void createNewGameButton();
	void createMenuButton();

private:
	Size _visible_size;
	BlockManager* _manager;
	bool _move_succes;
	float _touch_delta;
	Label* _cur_score;
	Label* _high_score;
	bool _broken_record;
	ValueMap _text_map;
};