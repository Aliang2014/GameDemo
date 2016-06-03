#include "Block.h"

Block * Block::create(const Vec2 & block_pos, const Size & block_size)
{
	Block* block = new Block();
	if (block && block->init(block_pos, block_size))
	{
		block->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(block);
	}
	return block;
}

Block::Block()
	: _active(false)
	, _block_pos(Vec2::ZERO)
	, _block_size(Size::ZERO)
	, _cur_score(0)
	, _score_label(nullptr)
{
}

Block::~Block()
{
}

void Block::setActive(bool active)
{
	_active = active;
	if (_active)
	{
		_score_label->setVisible(true);
	}
	else
	{
		_score_label->setVisible(false);
	}
	updateBlockUI();
}

void Block::setCurScore(int new_score)
{
	_cur_score = new_score;
	_score_label->setString(StringUtils::toString(_cur_score));
	updateBlockUI();
}

void Block::mergeOtherBlock(Block* other)
{
	// this is active after merge
	runMergeAnimation(other);
	setCurScore(_cur_score * 2);
	other->setActive(false);
	int score = RandomHelper::random_int(0, 2) == 0 ? 4 : 2;
	other->setCurScore(score);
}

void Block::swapOtherBlock(Block* other)
{
	// this is active after merge
	runSwapAnimation(other);
	int score = _cur_score;
	setCurScore(other->getCurScore());
	setActive(true);
	other->setActive(false);
	other->setCurScore(score);
}

bool Block::init(const Vec2& block_pos, const Size& block_size)
{
	bool bRet = false;
	do 
	{
		LayerColor::initWithColor(Color4B::BLACK);

		_block_pos = block_pos;
		_block_size = block_size;
		setContentSize(_block_size);

		initScoreLabel();

		bRet = true;
	} while (0);
	return bRet;
}

void Block::initScoreLabel()
{
	_score_label = Label::createWithTTF("", "fonts/arial.ttf", 32);
	addChild(_score_label);
	_score_label->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	// init score
	_cur_score = RandomHelper::random_int(0, 2) == 0 ? 4 : 2;
	setCurScore(_cur_score);
	setActive(false);
}

void Block::updateBlockUI()
{
	if (!_active)
	{
		setColor(Color3B::GRAY);
		return;
	}
	switch (_cur_score)
	{
	case 2:
		_score_label->setColor(Color3B::BLACK);
		setColor(Color3B(240, 230, 220));
		break;
	case 4:
		_score_label->setColor(Color3B::BLACK);
		setColor(Color3B(240, 220, 200));
		break;
	case 8:
		_score_label->setColor(Color3B::WHITE);
		setColor(Color3B(240, 180, 120));
		break;
	case 16:
		setColor(Color3B(240, 140, 90));
		_score_label->setColor(Color3B::WHITE);
		break;
	case 32:
		setColor(Color3B(240, 120, 90));
		_score_label->setColor(Color3B::WHITE);
		break;
	case 64:
		setColor(Color3B(240, 90, 60));
		_score_label->setColor(Color3B::WHITE);
		break;
	case 128:
		setColor(Color3B(240, 90, 60));
		_score_label->setColor(Color3B::WHITE);
		break;
	case 256:
		setColor(Color3B(240, 200, 70));
		_score_label->setColor(Color3B::WHITE);
		break;
	case 512:
		setColor(Color3B(240, 200, 70));
		_score_label->setColor(Color3B::WHITE);
		break;
	case 1024:
		setColor(Color3B(0, 130, 0));
		_score_label->setColor(Color3B::WHITE);
		break;
	default:
		setColor(Color3B(0, 130, 0));
		_score_label->setColor(Color3B::WHITE);
		break;
	}
}

void Block::runShowAnimation()
{
	ScaleTo* scale = ScaleTo::create(0.1f, 1.1f);
	ScaleTo* scale2 = ScaleTo::create(0.1f, 1.0f);
	Sequence* ac = Sequence::create(scale, scale2, nullptr);
	stopAllActions();
	runAction(ac);
}

void Block::runMergeAnimation(Block* other)
{

}

void Block::runSwapAnimation(Block* other)
{

}