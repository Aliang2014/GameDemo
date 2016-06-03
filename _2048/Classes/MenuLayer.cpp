#include "MenuLayer.h"
#include "ButtonHelper.h"
#include "GameLayer.h"

MenuLayer * MenuLayer::create(GameLayer * layer)
{
	MenuLayer* p = new MenuLayer();
	if (p && p->init(layer))
	{
		p->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(p);
	}
	return p;
}

bool MenuLayer::init(GameLayer* layer)
{
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
		return false;
	
	_button_map = FileUtils::getInstance()->getValueMapFromFile("text_button.xml");
	_tips_map = FileUtils::getInstance()->getValueMapFromFile("text_tips.xml");

	_layer = layer;
	_isShowing = false;
	
	createButtonBack();
	createButtonSave();
	createButtonLoad();
	createButtonExit();

	return true;
}

void MenuLayer::createButtonBack()
{
	_back = ButtonHelper::create(Size(240,60), CC_CALLBACK_1(MenuLayer::touchButtonBack, this));
	
	Label* name = Label::createWithSystemFont("", "fonts/simsun.ttf", 30);
	name->setString(_button_map.at("resume").asString());
	_back->setNameLabel(name);

	Sprite* sp = Sprite::createWithSpriteFrameName("6.png");
	sp->setColor(Color3B(200, 200, 200));
	_back->setBgSprite(sp);

	addChild(_back);
	_back->setPosition(198, 500);
}

void MenuLayer::createButtonSave()
{
	_save = ButtonHelper::create(Size(240, 60), CC_CALLBACK_1(MenuLayer::touchButtonSave, this));
	
	Label* name = Label::createWithSystemFont("", "fonts/simsun.ttf", 30);
	name->setString(_button_map.at("save").asString());
	_save->setNameLabel(name);

	Sprite* sp = Sprite::createWithSpriteFrameName("6.png");
	sp->setColor(Color3B(200, 200, 200));
	_save->setBgSprite(sp);

	addChild(_save);
	_save->setPosition(198, 380);
}

void MenuLayer::createButtonLoad()
{
	_load = ButtonHelper::create(Size(240, 60), CC_CALLBACK_1(MenuLayer::touchButtonLoad, this));

	Label* name = Label::createWithSystemFont("", "fonts/simsun.ttf", 30);
	name->setString(_button_map.at("load").asString());
	_load->setNameLabel(name);

	Sprite* sp = Sprite::createWithSpriteFrameName("6.png");
	sp->setColor(Color3B(200, 200, 200));
	_load->setBgSprite(sp);

	addChild(_load);
	_load->setPosition(198, 260);
}

void MenuLayer::createButtonExit()
{
	_exit = ButtonHelper::create(Size(240, 60), CC_CALLBACK_1(MenuLayer::touchButtonExit, this));
	Label* name = Label::createWithSystemFont("", "fonts/simsun.ttf", 30);
	name->setString(_button_map.at("exit").asString());
	_exit->setNameLabel(name);

	Sprite* sp = Sprite::createWithSpriteFrameName("6.png");
	sp->setColor(Color3B(200, 200, 200));
	_exit->setBgSprite(sp);

	addChild(_exit);
	_exit->setPosition(198, 140);
}

void MenuLayer::touchButtonBack(ButtonHelper *)
{
	DelayTime* delay = DelayTime::create(0.05f);
	CallFunc* call = CallFunc::create([&]() {
		removeFromParent();
	});
	runAction(Sequence::create(delay, call, nullptr));
}

void MenuLayer::touchButtonSave(ButtonHelper *)
{
	// 有旧的存档
	if (UserDefault::getInstance()->getBoolForKey("exist", false) == true)
	{
		if (_isShowing == false)
		{
			//
			this->setColor(Color3B::GRAY);

			_tip_layer = Layer::create();
			_tip_layer->setPosition(200, 400);
			_tip_layer->setSwallowsTouches(true);

			addChild(_tip_layer);

			Label* text = Label::createWithSystemFont("", "fonts/simsun.ttf", 28);
			text->setString(_tips_map.at("tips_2").asString());
			_tip_layer->addChild(text);
			text->setColor(Color3B::BLACK);
			text->setPositionY(60);

			ButtonHelper* yes = ButtonHelper::create(Size(100, 100), CC_CALLBACK_1(MenuLayer::touchButtonYes, this));
			Label* name = Label::createWithSystemFont("", "fonts/simsun.ttf", 30);
			name->setColor(Color3B::BLACK);
			name->setString(_tips_map.at("tips_3").asString());
			yes->setNameLabel(name);
			_tip_layer->addChild(yes);
			yes->setPositionX(-50);

			ButtonHelper* no = ButtonHelper::create(Size(100, 100), CC_CALLBACK_1(MenuLayer::touchButtonNo, this));
			Label* name_2 = Label::createWithSystemFont("", "fonts/simsun.ttf", 30);
			name_2->setColor(Color3B::BLACK);
			name_2->setString(_tips_map.at("tips_4").asString());
			no->setNameLabel(name_2);
			_tip_layer->addChild(no);
			no->setPositionX(50);
		}
	}
	// 没有旧的存档
	else
	{
		_layer->saveCurState();
	}
}

void MenuLayer::touchButtonLoad(ButtonHelper *)
{
	// none
	if (UserDefault::getInstance()->getBoolForKey("exist", false) == false)
	{
		if (_isShowing == false)
		{
			_isShowing = true;
			_tips = Label::createWithTTF("", "fonts/simsun.ttf", 28);
			_tips->setPosition(200, 430);
			_tips->setString(_tips_map.at("tips").asString());
			_tips->setColor(Color3B::BLACK);
			addChild(_tips);
			CallFunc* call = CallFunc::create([&]() {
				removetext();
			});
			Sequence* seq = Sequence::create(DelayTime::create(1.0f), call, nullptr);
			runAction(seq);
		}
	}
	else
	{
		_layer->loadLastState();
		DelayTime* delay = DelayTime::create(0.05f);
		CallFunc* call = CallFunc::create([&]() {
			removeFromParent();
		});
		runAction(Sequence::create(delay, call, nullptr));
	}
}

void MenuLayer::touchButtonExit(ButtonHelper *)
{
	Director::getInstance()->end();
}

void MenuLayer::removetext()
{
	_tips->removeFromParent();
	this->_isShowing = false;
}

void MenuLayer::touchButtonYes(ButtonHelper *)
{
	CallFunc* call = CallFunc::create([&]() {_layer->saveCurState();
	_tip_layer->removeFromParent();
	this->_isShowing = false;
	this->setColor(Color3B(255, 255, 255)); 
	});
	runAction(call);
}

void MenuLayer::touchButtonNo(ButtonHelper *)
{
	CallFunc* call = CallFunc::create([&]() {
		_tip_layer->removeFromParent();
		this->_isShowing = false;
		this->setColor(Color3B(255, 255, 255));
	});
	runAction(call);
}
