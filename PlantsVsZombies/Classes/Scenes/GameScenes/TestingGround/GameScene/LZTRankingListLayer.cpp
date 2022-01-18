/**
 *Copyright (c) 2022 LZ.All Right Reserved
 *Author : LZ
 *Date: 2022.01.18
 *Email: 2117610943@qq.com
 */
#include "LZTRankingListLayer.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZUserData.h"
#include "Based/LZCsvFile.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSPauseQuitLayer.h"

TRankingListLayer::TRankingListLayer():
	_csvFile(nullptr)
{
}

TRankingListLayer::~TRankingListLayer()
{
}

void TRankingListLayer::onShowDifferentTitle()
{
	auto title = cocos2d::ui::Text::create();
	title->setFontName(GAME_FONT_NAME_1);
	title->setFontSize(GAME_TEXT_SIZE("�����ǽ�ʬģʽ�� ��Ҵ��ؼ�¼���а�"));
	title->setString("��ֲ��������ģʽ�� ��Ҵ��ؼ�¼���а�");
	title->setPosition(Vec2(660, 980));
	this->addChild(title);
}

void TRankingListLayer::onDownloadRankingList()
{
	const string sURLList = "https://gitee.com/GITLZ/PVZDownLoader/raw/master/tg.csv";
	_downloader->createDownloadDataTask(sURLList);
	_downloader->onDataTaskSuccess = [this](const cocos2d::network::DownloadTask& task,
		std::vector<unsigned char>& data)
	{
		_loadingText->setVisible(false);

		for (auto s : data)
		{
			_strRankingList += s;
		}

		onParseCsvData();
	};
	_downloader->onTaskError = [this](const cocos2d::network::DownloadTask& task,
		int errorCode,
		int errorCodeInternal,
		const std::string& errorStr)
	{
		_loadingText->setString(GAME_TEXT("���а����ʧ�ܣ�"));
		_loadingText->setColor(Color3B::RED);
	};
}

void TRankingListLayer::onShowBackButton()
{
	_uploadButton = Button::create("SeedChooser_Button2.png", "SeedChooser_Button2_Glow.png", "", cocos2d::ui::Widget::TextureResType::PLIST);
	_uploadButton->setTitleText(GAME_TEXT("�����ϴ�"));
	_uploadButton->setTitleColor(Color3B(0, 255, 255));
	_uploadButton->setTitleFontSize(GAME_TEXT_SIZE("�˳�"));
	_uploadButton->setScale(2.0f);
	_uploadButton->setPosition(Vec2(1440, 70));
	_uploadButton->setEnabled(false);
	this->addChild(_uploadButton);
	_uploadButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap2");
				break;
			case Widget::TouchEventType::ENDED:
				onUploadData();
				break;
			}
		});

	auto button2 = Button::create("SeedChooser_Button2.png", "SeedChooser_Button2_Glow.png", "", cocos2d::ui::Widget::TextureResType::PLIST);
	button2->setTitleText(GAME_TEXT("�˳�"));
	button2->setTitleColor(Color3B(0, 255, 255));
	button2->setTitleFontSize(GAME_TEXT_SIZE("�˳�"));
	button2->setScale(2.0f);
	button2->setPosition(Vec2(1785, 70));
	this->addChild(button2);
	button2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				PlayMusic::playMusic("tap2");
				break;
			case Widget::TouchEventType::ENDED:
				GSPauseQuitLayer::resumeLayer();
				deleteDialog();
				break;
			}
		});
}

void TRankingListLayer::onParseCsvData()
{
	auto mostLevelNumber = max(UserData::getInstance()->openIntUserData(const_cast<char*>("TESTINGGROUND")), 1);

	if (!_csvFile)
	{
		_csvFile = new CSVFile();
		_csvFile->openFile(_strRankingList);

		vector<string> data;
		data.push_back(GAME_TEXT("���� �ң�") + _global->userInformation->getUserName());
		data.push_back(to_string(mostLevelNumber));
		_csvFile->addNewData(data);
		_csvFile->sortData(1);
	}

	_rankingListScrollView->removeAllChildren();

	auto row = _csvFile->getRows();
	auto draw = DrawNode::create();

	_rankingListScrollView->setInnerContainerSize(Size(1120.f, max(700, 100 * row)));

	auto h = max(700, 100 * row);
	for (int i = 0; i <= row; ++i)
	{
		runAction(Sequence::create(DelayTime::create(i * 0.02f),
			CallFunc::create([=]()
				{
					draw->drawLine(Vec2(0, h - 100 * i), Vec2(1120, h - 100 * i), Color4F(0.5f, 0.5f, 0.5f, 0.7f));

					if (i != row)
					{
						auto num = cocos2d::ui::Text::create();
						num->setFontName(GAME_FONT_NAME_2);
						num->setFontSize(35);
						num->setString(to_string(i + 1));
						num->setPosition(Vec2(100, h - 50 - 100 * i));
						_rankingListScrollView->addChild(num);

						string nameStr = _csvFile->getData(i, 0);
						auto name = cocos2d::ui::Text::create();
						name->setFontName(GAME_FONT_NAME_1);
						name->setFontSize(35);
						name->setString(nameStr);
						name->setPosition(Vec2(500, h - 50 - 100 * i));
						_rankingListScrollView->addChild(name);

						auto level = cocos2d::ui::Text::create();
						level->setFontName(GAME_FONT_NAME_2);
						level->setFontSize(35);
						level->setString(_csvFile->getData(i, 1));
						level->setPosition(Vec2(960, h - 50 - 100 * i));
						_rankingListScrollView->addChild(level);

						if (nameStr.find(GAME_TEXT("���� �ң�")) != nameStr.npos)
						{
							num->setColor(Color3B(0, 255, 255));
							name->setColor(Color3B(0, 255, 255));
							name->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f, 1.08f), ScaleTo::create(0.5f, 0.92f), nullptr)));
							level->setColor(Color3B(0, 255, 255));
							level->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5f, 1.08f), ScaleTo::create(0.5f, 0.92f), nullptr)));

							if (!this->getChildByName("myList"))
							{
								auto own = cocos2d::ui::Text::create();
								own->setFontName(GAME_FONT_NAME_1);
								own->setFontSize(GAME_TEXT_SIZE("�ҵ��������Ƽ�¼"));
								own->setString(StringUtils::format(GAME_TEXT("�ҵ��������Ƽ�¼").c_str(), i + 1, _global->userInformation->getUserName().c_str(), mostLevelNumber));
								own->setPosition(Vec2(660, 57));
								own->setColor(Color3B(0, 255, 255));
								own->setName("myList");
								this->addChild(own);

								_draw->drawRect(Vec2(100, 22), Vec2(1220, 98), Color4F(0, 1, 1, 0.5f));
								_draw->drawSolidRect(Vec2(100, 22), Vec2(1220, 98), Color4F(0, 1, 1, 0.2f));
							}
							draw->drawSolidRect(Vec2(0, h - 100 * i - 1), Vec2(1120, h - 100 * (i + 1) + 1), Color4F(0, 1, 1, 0.2f));
						}
						if (i == 0)
						{
							num->setColor(Color3B::YELLOW);
							name->setColor(Color3B::YELLOW);
							name->enableGlow(Color4B(0, 255, 255, 255));
							level->setColor(Color3B::YELLOW);
							level->enableGlow(Color4B(0, 255, 255, 255));
							draw->drawSolidRect(Vec2(0, h - 100 * i - 1), Vec2(1120, h - 100 * (i + 1) + 1), Color4F(1, 1, 0, 0.3f));
						}
						else if (i == 1)
						{
							num->setColor(Color3B(192, 192, 192));
							num->enableGlow(Color4B::WHITE);
							name->setColor(Color3B(192, 192, 192));
							name->enableGlow(Color4B::WHITE);
							level->setColor(Color3B(192, 192, 192));
							level->enableGlow(Color4B::WHITE);
						}
						else if (i == 2)
						{
							num->setColor(Color3B(117, 60, 43));
							num->enableGlow(Color4B(187, 124, 104, 255));
							name->setColor(Color3B(117, 60, 43));
							name->enableGlow(Color4B(187, 124, 104, 255));
							level->setColor(Color3B(117, 60, 43));
							level->enableGlow(Color4B(187, 124, 104, 255));
						}
					}
				}), nullptr));
	}

	_rankingListScrollView->addChild(draw);

	onMouseEvent();
}

void TRankingListLayer::onUploadData()
{
}

void TRankingListLayer::onCheckUploadButtonEnable()
{
}
