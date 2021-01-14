/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.17
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#include "LZSMInputDataScene.h"
#include "../LoadingScene/LZSLLoadingScene.h"
#include "Based/LZBGlobalVariable.h"
#include "Based/LZBUserData.h"
#include "Based/LZBPlayMusic.h"

#define FontSize 14
#define DefaultName  "δ�����浵"

InputDataMenu::InputDataMenu() :
	_caveFileDialog(nullptr),
	_deleteFileDialog(nullptr),
	_inputCursor(nullptr),
	_inputDataDialog(nullptr),
	_editbox(nullptr),
	_textField(nullptr),
	_shieldDialogLayer(nullptr),
	_isNewFileData(false),
	_caveFileNumber(-1)
{
	_dataButton.clear();
}

InputDataMenu::~InputDataMenu()
{
}

bool InputDataMenu::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	Dialog::createShieldLayer(this);

    createDialog();
	inputNewFileDataName();

	return true;
}

void InputDataMenu::deleteDialog()
{
	if (_global->userInformation->getUserName() != DefaultName && !_isNewFileData)
	{
		setMouseListenerEnable();
		this->removeFromParent();
	}
}

void InputDataMenu::createDialog()
{
	_caveFileDialog = Sprite::createWithSpriteFrameName("InputDataDialog.png");
	_caveFileDialog->setPosition(Director::getInstance()->getWinSize() / 2);
	_caveFileDialog->setScale(2.0f);
	_caveFileDialog->setName("CaveFileDialog");
	this->addChild(_caveFileDialog);

	auto question = Text::create(_global->userInformation->getGameText().find("ѡ����Ĵ浵")->second->text, GAME_FONT_NAME_1, 
		_global->userInformation->getGameText().find("ѡ����Ĵ浵")->second->fontsize);
	question->setTextColor(Color4B::YELLOW);
	question->setPosition(Vec2(_caveFileDialog->getContentSize().width / 2, _caveFileDialog->getContentSize().height - 110));
	_caveFileDialog->addChild(question);

	auto SeedPacketSilhouette1 = Sprite::createWithSpriteFrameName("SeedPacketSilhouette1.png");
	SeedPacketSilhouette1->setPosition(Vec2(245, 270));
	SeedPacketSilhouette1->setScale(2.0f);
	_caveFileDialog->addChild(SeedPacketSilhouette1);

	for (int i = 0; i < 8; i++)
	{
		this->createDataButton(_global->userInformation->getUserCaveFileName(i), Vec2(245, 357 - 25 * i), 3.7f, i);
	}

	/* if not read data, read it */
	if (_global->userInformation->getUserCaveFileNumber() == -1)
	{
		_caveFileNumber = UserDefault::getInstance()->getIntegerForKey("USERDATANUMBER");
		_dataButton.find(_caveFileNumber)->second->setColor(Color3B(0, 255, 255));
	}
	else
	{
		_caveFileNumber = _global->userInformation->getUserCaveFileNumber();
		_dataButton.find(_caveFileNumber)->second->setColor(Color3B(0, 255, 255));
	}

	/* ������ť */
	this->createButtons(_caveFileDialog, _global->userInformation->getGameText().find("ȷ��")->second->text, Vec2(150, 55), 1.6f, 3);
	this->createButtons(_caveFileDialog, _global->userInformation->getGameText().find("ȡ��")->second->text, Vec2(350, 55), 1.6f, 4);
	this->createButtons(_caveFileDialog, _global->userInformation->getGameText().find("������")->second->text, Vec2(150, 100), 1.6f, 5);
	this->createButtons(_caveFileDialog, _global->userInformation->getGameText().find("ɾ��")->second->text, Vec2(350, 100), 1.6f, 6);

	/* �������� */
	this->createTouchtListener(_caveFileDialog);
}

void InputDataMenu::createInputDialog()
{
	_inputDataDialog = Sprite::createWithSpriteFrameName("QuitDialog.png");
	_inputDataDialog->setPosition(Director::getInstance()->getWinSize() / 2);
	_inputDataDialog->setScale(3.0f);
	_inputDataDialog->setName("InputDataDialog");
	_shieldDialogLayer->addChild(_inputDataDialog);

	auto editbox = Sprite::createWithSpriteFrameName("editbox.png");
	editbox->setPosition(Vec2(155, 140));
	editbox->setScale(0.7f);
	_inputDataDialog->addChild(editbox);

	_inputCursor = Sprite::create("resources/images/System/InputCursor.png");
	_inputCursor->setScale(0.7f);
	_inputCursor->setPosition(Vec2(155, 140));
	_inputCursor->setVisible(false);
	_inputDataDialog->addChild(_inputCursor);
	_inputCursor->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.25f), FadeIn::create(0.25f), nullptr)));

	_textField = TextField::create(_global->userInformation->getGameText().find("����˴���������浵����")->second->text,GAME_FONT_NAME_1, FontSize);
	_inputDataDialog->addChild(_textField);
	_textField->setPosition(Vec2(155, 137));
	_textField->setFontSize(_global->userInformation->getGameText().find("����˴���������浵����")->second->fontsize);
	_textField->setMaxLengthEnabled(true);
	_textField->setMaxLength(15);
	_textField->addEventListener([=](Ref* ref, TextField::EventType type)
		{
			switch (type)
			{
			case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
				_textField->setPlaceHolder(" ");
				_inputCursor->setVisible(true);
				break;
			case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
				_textField->setPlaceHolder(_global->userInformation->getGameText().find("����˴���������浵����")->second->text);
				_inputCursor->setVisible(false);
				break;
			case cocos2d::ui::TextField::EventType::INSERT_TEXT:
			{
				/* ��ȡ�����ӵ��ַ� */
				_newInputstring = _textField->getString().substr(_inputString.size());

				/* �����ַ��� */
				_inputString = _textField->getString();

				/* �����ַ����� */
				auto textLabel = onTextFieldInsertText();
				
				_inputCursor->setPositionX(_inputCursor->getPositionX() + textLabel->getContentSize().width / 2);
			}
				
				break;
			case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
			{
				/* ��ȡɾ�����ַ� */
				_newInputstring = _inputString.substr(_textField->getString().size());

				/* �����ַ��� */
				_inputString = _textField->getString();

				/* ɾ������ */
				auto textLabel = onTextFieldDeleteBackward();

				_inputCursor->setPositionX(_inputCursor->getPositionX() - textLabel->getContentSize().width / 2);
			}
				break;
			default:
				break;
			}
		});

	/* ������ť */
	this->createButtons(_inputDataDialog, _global->userInformation->getGameText().find("ȷ��")->second->text, Vec2(90, 65),1.0f, 1);
	this->createButtons(_inputDataDialog, _global->userInformation->getGameText().find("ȡ��")->second->text, Vec2(225, 65),1.0f, 2);
	
	/* �������� */
	this->createTouchtListener(_inputDataDialog);
}

void InputDataMenu::createButtons(Sprite* sprite, const std::string &Label, Vec2 &vec2,const float& scale, const int& ID)
{
	auto button = Button::create("button.png", "button_down.png", "", TextureResType::PLIST);
	button->setPosition(vec2);
	button->setScaleX(scale);
	button->setTitleLabel(label(Label, 20, Vec2(0, 0), 0, Color3B::GREEN, 1.0f/scale));
	sprite->addChild(button);

	button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
	{
		switch (type)
		{
		case Widget::TouchEventType::BEGAN:
			PlayMusic::playMusic("gravebutton");
			break;
		case Widget::TouchEventType::ENDED:
			switch (ID)
			{
			case 1: specifiedInput();    break;
			case 2: cancelInput();       break;
			case 3: selectFileData();    break;
			case 4: deleteDialog();      break;
			case 5: renameFileData();    break;
			case 6: deleteFileData();    break;
			case 7: specifiedDelete();   break;
			case 8: cancelDelete();      break;
			}
		}
	});
}

void InputDataMenu::createDataButton(const std::string& Labels, Vec2& vec2, const float& scale, const int& ID)
{
	auto button = Button::create("CaveFileButton.png", "CaveFileButtonDown.png", "", TextureResType::PLIST);
	button->setPosition(vec2);
	button->setScale9Enabled(true);
	button->setScaleX(scale);
	
	setButtonText(button, Labels, scale);
	_caveFileDialog->addChild(button);

	_dataButton.insert(pair<int, Button*>(ID, button));

	button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("gravebutton");
				break;
			case Widget::TouchEventType::ENDED:
				_caveFileNumber = ID;
				switch (ID)
				{
				case 0: setButtonColor(button); break;
				case 1: setButtonColor(button); break;
				case 2: setButtonColor(button); break;
				case 3: setButtonColor(button); break;
				case 4: setButtonColor(button); break;
				case 5: setButtonColor(button); break;
				case 6: setButtonColor(button); break;
				case 7: setButtonColor(button); break;
				}
			}
		});
}

void InputDataMenu::createShieldLayer()
{
	_global->userInformation->setUserCaveFileNumber(_caveFileNumber);

	_shieldDialogLayer = LayerColor::create(Color4B(0, 0, 0, 180));
	Dialog::createShieldLayer(_shieldDialogLayer);
	this->addChild(_shieldDialogLayer, 1, "_shieldDialogLayer");

	createInputDialog();
}

void InputDataMenu::setButtonColor(Button* button)
{
	for (auto& sp : _dataButton)
	{
		sp.second->setColor(Color3B::WHITE);
	}
	button->setColor(Color3B(0, 255, 255));
}

void InputDataMenu::setButtonText(Button* button, const string& text, const float& scale)
{
	auto label = Label::createWithTTF(text, GAME_FONT_NAME_1, 17);
	label->setColor(Color3B::BLACK);
	label->setScaleX(1.0f / scale);
	button->setTitleText("");
	button->setTitleLabel(label);
}

void InputDataMenu::caveData()
{
	if (strlen(_textField->getString().c_str()))
	{
		_global->userInformation->setUserName(_textField->getString());
		UserDefault::getInstance()->setStringForKey(
			_global->userInformation->getUserCaveFileNameKey(_caveFileNumber).c_str(),
			_global->userInformation->getUserName());
		_global->userInformation->setIsUpdate(true);
		_global->userInformation->setUserCaveFileName(_caveFileNumber, _global->userInformation->getUserName());
		UserDefault::getInstance()->setIntegerForKey("USERDATANUMBER", _caveFileNumber); /* ��¼��ѡ�浵 */

		/* ���� */
		if (_textField->getString() == "gitee.io/lz")unlock();
	}
}

void InputDataMenu::updateButtonText()
{
	int i = 0;
	for (auto& sp : _dataButton)
	{
		setButtonText(sp.second, _global->userInformation->getUserCaveFileName(i++), 3.7f);
	}
}

void InputDataMenu::inputNewFileDataName()
{
	if (_global->userInformation->getUserName() == DefaultName)
	{
		createShieldLayer();
	}
}

void InputDataMenu::renameFileData()
{
	if (_global->userInformation->getUserCaveFileName(_caveFileNumber) != DefaultName)
	{
		createShieldLayer();
	}
}

void InputDataMenu::specifiedInput()
{
	if (!_textField->getString().empty() && _textField->getString() != DefaultName)
	{
		caveData();
		updateButtonText();
		this->removeChildByName("_shieldDialogLayer");
		_inputString.clear();
		_isNewFileData = true;
	}
}

void InputDataMenu::cancelInput()
{
	if (_global->userInformation->getUserName() != DefaultName)
	{
		this->removeChildByName("_shieldDialogLayer");
		_inputString.clear();
	}
}

void InputDataMenu::specifiedDelete()
{
	auto file = FileUtils::getInstance();
	remove((file->getWritablePath() + "UserDataFile_" + to_string(_global->userInformation->getUserCaveFileNumber()) + ".dat").c_str());
	remove((file->getWritablePath() + "UserDataFile_" + to_string(_global->userInformation->getUserCaveFileNumber()) + ".sav").c_str());

	UserDefault::getInstance()->deleteValueForKey(_global->userInformation->getUserCaveFileNameKey(_caveFileNumber).c_str());
	_global->userInformation->setUserCaveFileName(_caveFileNumber, DefaultName);
	_global->userInformation->setUserName(_global->userInformation->getUserCaveFileName(_caveFileNumber));
	updateButtonText();

	this->removeChildByName("_shieldDialogLayer");
}

void InputDataMenu::cancelDelete()
{
	this->removeChildByName("_shieldDialogLayer");
}

void InputDataMenu::selectFileData()
{
	UserDefault::getInstance()->setIntegerForKey("USERDATANUMBER", _caveFileNumber); /* ��¼��ѡ�浵 */
	_global->userInformation->setUserCaveFileNumber(_caveFileNumber);
	UserData::getInstance()->createNewUserDataDocument();
	LoadingScene::loadUserFileData();

	/* ��ȡ��ѡ�浵�����ֲ����� */
	if (UserDefault::getInstance()->getStringForKey(
		_global->userInformation->getUserCaveFileNameKey(
			_global->userInformation->getUserCaveFileNumber()).c_str()) != "")
	{
		_global->userInformation->setUserName(
			UserDefault::getInstance()->getStringForKey(
				_global->userInformation->getUserCaveFileNameKey(
					_global->userInformation->getUserCaveFileNumber()).c_str()));
	}
	else
	{
		_global->userInformation->setUserName(_global->userInformation->getUserCaveFileName(_caveFileNumber));
	}
	_global->userInformation->setIsUpdate(true);

	deleteDialog();

	Director::getInstance()->replaceScene(MainMenu::createScene());
}

void InputDataMenu::deleteFileData()
{
	if (_global->userInformation->getUserCaveFileName(_caveFileNumber) != DefaultName)
	{
		_global->userInformation->setUserCaveFileNumber(_caveFileNumber);

		_shieldDialogLayer = LayerColor::create(Color4B(0, 0, 0, 180));
		Dialog::createShieldLayer(_shieldDialogLayer);
		this->addChild(_shieldDialogLayer, 1, "_shieldDialogLayer");

		createDeleteDialog();
	}
}

void InputDataMenu::createDeleteDialog()
{
	_deleteFileDialog = Sprite::createWithSpriteFrameName("QuitDialog.png");
	_deleteFileDialog->setPosition(Director::getInstance()->getWinSize() / 2);
	_deleteFileDialog->setScale(3.0f);
	_deleteFileDialog->setName("InputDataDialog");
	_shieldDialogLayer->addChild(_deleteFileDialog);

	auto text = Text::create(_global->userInformation->getGameText().find("ɾ����ʾ")->second->text, GAME_FONT_NAME_1,
		_global->userInformation->getGameText().find("ɾ����ʾ")->second->fontsize);
	text->setColor(Color3B::RED);
	text->setPosition(Vec2(_deleteFileDialog->getContentSize().width / 2.0f, _deleteFileDialog->getContentSize().height / 2.0f + 10));
	text->setTextVerticalAlignment(TextVAlignment::CENTER);
	text->setTextHorizontalAlignment(TextHAlignment::CENTER);
	text->setTextAreaSize(Size(_deleteFileDialog->getContentSize().width - 85, 60));
	_deleteFileDialog->addChild(text);

	/* ������ť */
	this->createButtons(_deleteFileDialog, _global->userInformation->getGameText().find("ȷ��")->second->text, Vec2(90, 65), 1.0f, 7);
	this->createButtons(_deleteFileDialog, _global->userInformation->getGameText().find("ȡ��")->second->text, Vec2(225, 65), 1.0f, 8);

	/* �������� */
	this->createTouchtListener(_deleteFileDialog);
}

void InputDataMenu::unlock()
{
	UserData::getInstance()->caveUserData("KILLALLZOMBIES", 11000);
	UserData::getInstance()->caveUserData("COINNUMBERS", 11000);
	UserData::getInstance()->caveUserData("ISBEGINSHOWEGGS", true);
	UserData::getInstance()->caveUserData("WORLD_1_LEVELS", 53);
	UserData::getInstance()->caveUserData("WORLD_1_LEVELS_DIF", 53);
}

Label* InputDataMenu::onTextFieldInsertText()
{
	auto textLabel = Label::createWithTTF(_newInputstring, GAME_FONT_NAME_1, FontSize);
	textLabel->setColor(Color3B(0, 255, 255));
	textLabel->setName("label");
	_inputDataDialog->addChild(textLabel);

	auto endPos = _inputCursor->getPosition() - Vec2(0, 5);
	auto inputTextSize = textLabel->getContentSize();
	Vec2 beginPos(endPos.x + textLabel->getContentSize().width / 2, 270);

	float duration = 0.5f;
	textLabel->setPosition(beginPos);
	textLabel->setScale(8);

	auto seq = Sequence::create(Spawn::create(MoveTo::create(duration, endPos),ScaleTo::create(duration, 1),FadeOut::create(duration),nullptr),
		CallFunc::create([=]()
			{
				_inputDataDialog->removeChildByName("label");
			}), nullptr);
	textLabel->runAction(seq);

	return textLabel;
}

Label* InputDataMenu::onTextFieldDeleteBackward()
{
	// create a delete text sprite and do some action
	auto label = Label::createWithTTF(_newInputstring, GAME_FONT_NAME_1, FontSize);
	label->setName("label");
	label->setColor(Color3B::YELLOW);
	_inputDataDialog->addChild(label);

	// move the sprite to fly out
	auto beginPos = _inputCursor->getPosition();
	auto textfieldSize = _inputCursor->getContentSize();
	auto labelSize = label->getContentSize();
	beginPos.x += (textfieldSize.width - labelSize.width) / 2.0f;

	auto winSize = Director::getInstance()->getWinSize();
	Vec2 endPos(-winSize.width / 4.0f, winSize.height / 2.0f * (0.25 + (float)rand() / (2.0f * RAND_MAX)));

	float duration = 1;
	float rotateDuration = 0.35f;
	int repeatTime = 5;
	label->setPosition(beginPos);

	auto seq = Sequence::create(Spawn::create(MoveTo::create(duration, endPos)
		,Repeat::create(RotateBy::create(rotateDuration, (rand() % 2) ? 360 : -360),repeatTime),
		FadeOut::create(duration),nullptr),
		CallFunc::create([=]() 
			{
				_inputDataDialog->removeChildByName("label");
			}),
		nullptr);
	label->runAction(seq);

	return label;
}
