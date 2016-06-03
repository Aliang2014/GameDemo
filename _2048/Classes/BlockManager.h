#pragma once
#include "cocos2d.h"
#include "block.h"
#include "ui\CocosGUI.h"
using namespace cocos2d;
using namespace cocos2d::ui;

#define MAXROW 6
#define MAXCOL 6


class GameLayer;
class BlockManager :public Node
{
public:
	static BlockManager* create(int row, int col, GameLayer* layer);

	BlockManager();
	~BlockManager();

	void showNewBlock();
	int getTotalScore() const { return _total_score; }
	void setTotalScore(int score) { _total_score = score; }

	bool gameOver();
	bool moveUp();
	bool moveDown();
	bool moveLeft();
	bool moveRight();

	void saveState();
	void loadState();
	
private:
	bool init(int row, int col, GameLayer* layer);

	void initSize();
	void createBGImg();
	void createBlock();
	void setBlockPos(Block* block);

private:
	int _row;
	int _col;
	int _total_score;
	Size _block_size;
	Size _manager_size;
	float _block_delta;
	Layout* _layout;
	GameLayer* _layer;
	
	Block* _arr[MAXROW][MAXCOL];
};