#pragma once
#include "cocos2d.h"
USING_NS_CC;

class Block :public LayerColor
{
public:
	static Block* create(const Vec2& block_pos, const Size& block_size);

	Block();
	~Block();
	
	void setActive(bool active);
	bool getActive() const { return _active; }

	Vec2 getBlockPos() const { return _block_pos; }

	void setCurScore(int new_score);
	int getCurScore() { return _cur_score; }

	void mergeOtherBlock(Block* other);
	void swapOtherBlock(Block* other);

private:
	bool init(const Vec2& block_pos, const Size& block_size);

	void initScoreLabel();
	void updateBlockUI();

public:
	// animation
	void runShowAnimation();
	void runMergeAnimation(Block* other);
	void runSwapAnimation(Block* other);

private:
	bool _active;
	Vec2 _block_pos;
	Size _block_size;
	int _cur_score;
	Label* _score_label;
};