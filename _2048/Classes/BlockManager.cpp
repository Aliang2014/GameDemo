#include "BlockManager.h"

BlockManager * BlockManager::create(int row, int col, GameLayer * layer)
{
	BlockManager* bm = new BlockManager();
	if (bm && bm->init(row, col, layer))
	{
		bm->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(bm);
	}
	return bm;
}

BlockManager::BlockManager()
	: _row(1)
	, _col(1)
	, _total_score(0)
	, _block_size(Size::ZERO)
	, _manager_size(Size::ZERO)
	, _block_delta(0)
	, _layout(nullptr)
{
	for (int x = 0; x < MAXCOL; ++x)
	{
		for (int y = 0; y < MAXROW; ++y)
			_arr[x][y] = nullptr;
	}
}

BlockManager::~BlockManager()
{
}

void BlockManager::showNewBlock()
{
	if (!gameOver())
	{
		do
		{
			int x = RandomHelper::random_int(0, _col - 1);
			int y = RandomHelper::random_int(0, _row - 1);
			if (_arr[x][y]->getActive() == false)
			{
				_arr[x][y]->setActive(true);
				_arr[x][y]->runShowAnimation();
				break;
			}
		} while (1);
	}
}

bool BlockManager::gameOver()
{
	bool bRet = true;
	for (int x = 0; x < _col; ++x)
	{
		for (int y = 0; y < _row; ++y)
		{
			if (_arr[x][y]->getActive() == false ||
				(x < _col - 1 && _arr[x][y]->getCurScore() == _arr[x + 1][y]->getCurScore()) ||
				(x>0 && _arr[x][y]->getCurScore() == _arr[x - 1][y]->getCurScore()) ||
				(y < _row - 1 && _arr[x][y]->getCurScore() == _arr[x][y + 1]->getCurScore()) ||
				(y>0 && _arr[x][y]->getCurScore() == _arr[x][y - 1]->getCurScore()))
			{
				bRet = false;
				break;
			}
		}
	}
	return bRet;
}

bool BlockManager::moveUp()
{
	bool bRet = false;
	for (int x = 0; x < _col; ++x)
	{
		for (int y = _row - 1; y >= 0; --y)
		{
			for (int y1 = y - 1; y1 >= 0; --y1)
			{
				if (_arr[x][y1]->getActive() == true)
				{
					// swap
					if (_arr[x][y]->getActive() == false)
					{
						_arr[x][y]->swapOtherBlock(_arr[x][y1]);
						++y;
						bRet = true;
					}
					// merge
					else if(_arr[x][y]->getActive() == true && _arr[x][y]->getCurScore() == _arr[x][y1]->getCurScore())
					{
						_arr[x][y]->mergeOtherBlock(_arr[x][y1]);
						_total_score += _arr[x][y]->getCurScore();
						bRet = true;
					}
					break;
				}
			}
		}
	}
	return bRet;
}

bool BlockManager::moveDown()
{
	bool bRet = false;
	for (int x = 0; x < _col; ++x)
	{
		for (int y = 0; y < _row; ++y)
		{
			for (int y1 = y + 1; y1 < _row; ++y1)
			{
				if (_arr[x][y1]->getActive())
				{
					if (_arr[x][y]->getActive() == false)
					{
						_arr[x][y]->swapOtherBlock(_arr[x][y1]);
						--y;
						bRet = true;
					}
					else if (_arr[x][y]->getActive() == true && _arr[x][y]->getCurScore() == _arr[x][y1]->getCurScore())
					{
						_arr[x][y]->mergeOtherBlock(_arr[x][y1]);
						_total_score += _arr[x][y]->getCurScore();
						bRet = true;
					}
					break;
				}
			}
		}
	}
	return bRet;
}

bool BlockManager::moveLeft()
{
	bool bRet = false;
	for (int y = 0; y < _row; ++y)
	{
		for (int x = 0; x < _col; ++x)
		{
			for (int x1 = x + 1; x1 < _col; ++x1)
			{
				if (_arr[x1][y]->getActive() == true)
				{
					if (_arr[x][y]->getActive() == false)
					{
						_arr[x][y]->swapOtherBlock(_arr[x1][y]);
						--x;
						bRet = true;
					}
					else if (_arr[x][y]->getActive() == true && _arr[x][y]->getCurScore() == _arr[x1][y]->getCurScore())
					{
						_arr[x][y]->mergeOtherBlock(_arr[x1][y]);
						_total_score += _arr[x][y]->getCurScore();
						bRet = true;
					}
					break;
				}
			}
		}
	}
	return bRet;
}

bool BlockManager::moveRight()
{
	bool bRet = false;
	for (int y = 0; y < _row; ++y)
	{
		for (int x = _col-1; x >= 0; --x)
		{
			for (int x1 = x - 1; x1 >= 0; --x1)
			{
				if (_arr[x1][y]->getActive() == true)
				{
					if (_arr[x][y]->getActive() == false)
					{
						_arr[x][y]->swapOtherBlock(_arr[x1][y]);
						++x;
						bRet = true;
					}
					else if (_arr[x][y]->getActive() == true && _arr[x][y]->getCurScore() == _arr[x1][y]->getCurScore())
					{
						_arr[x][y]->mergeOtherBlock(_arr[x1][y]);
						_total_score += _arr[x][y]->getCurScore();
						bRet = true;
					}
					break;
				}
			}
		}
	}
	return bRet;
}

void BlockManager::saveState()
{
	for (int x = 0; x < _col; ++x)
	{
		for (int y = 0; y < _row; ++y)
		{
			// active
			UserDefault::getInstance()->setBoolForKey(StringUtils::format("_arr%d%d_b", x, y).c_str(), _arr[x][y]->getActive());
			// score
			UserDefault::getInstance()->setIntegerForKey(StringUtils::format("_arr%d%d_i", x, y).c_str(), _arr[x][y]->getCurScore());
		}
	}
}

void BlockManager::loadState()
{
	for (int x = 0; x < _col; ++x)
	{
		for (int y = 0; y < _row; ++y)
		{
			// active
			_arr[x][y]->setActive(UserDefault::getInstance()->getBoolForKey(StringUtils::format("_arr%d%d_b", x, y).c_str()));
			// score
			_arr[x][y]->setCurScore(UserDefault::getInstance()->getIntegerForKey(StringUtils::format("_arr%d%d_i", x, y).c_str()));
		}
	}
}

void BlockManager::initSize()
{
	// 
	_block_size = Size(75, 75);
	_block_delta = _block_size.width / 6;
	float x = _block_size.width * _row + _block_delta*(_row + 1);
	float y = _block_size.height * _col + _block_delta*(_col + 1);
	_manager_size = Size(x, y);
}

void BlockManager::createBGImg()
{
	_layout = Layout::create();
	_layout->setBackGroundImage("background.png");
	_layout->setBackGroundImageScale9Enabled(true);
	_layout->setAnchorPoint(Vec2(0.5, 0));
	_layout->setContentSize(_manager_size);
	addChild(_layout);
}

void BlockManager::createBlock()
{
	for (int x = 0; x < _row; ++x)
	{
		for (int y = 0; y < _col; ++y)
		{
			Block* b = Block::create(Vec2(x, y), _block_size);
			_layout->addChild(b);
			setBlockPos(b);
			_arr[x][y] = b;
		}
	}
}

void BlockManager::setBlockPos(Block * block)
{
	Vec2 block_pos = block->getBlockPos();
	float x = _block_delta + block_pos.x * _block_size.width + block_pos.x * _block_delta;
	float y = _block_delta + block_pos.y * _block_size.height + block_pos.y * _block_delta;
	block->setPosition(x, y);
}

bool BlockManager::init(int row, int col, GameLayer * layer)
{
	bool bRet = false;
	do 
	{
		_row = row;
		_col = col;
		_layer = layer;
		initSize();
		createBGImg();
		createBlock();

		bRet = true;
	} while (0);
	return bRet;
}
