/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Scenes/SelectPlantsScene/LZSSControlLayer.h"
#include "Based/LZBUserWinRequirement.h"
#include "Based/LZBGameType.h"
#include "Based/LZBLevelData.h"
#include "Plants/LZPPlants.h"

using namespace cocos2d;
using namespace cocos2d::ui;

struct MouseSelectImage /* ���ѡ���ֲ�� */
{
	MouseSelectImage():Iamge(nullptr), isSelectPlants(false), isSelectShovel(false){}
	Sprite* Iamge;                    /* ͼƬ */
	PlantsType selectPlantsId;        /* ��ѡֲ���� */
	bool isSelectPlants;              /* �Ƿ�ѡ����ֲ�� */
	bool isSelectShovel;              /* �Ƿ�ѡ���˲��� */
};

enum class GSLayerButton
{
	stopButton = 1,
	accelerateButton,
	decelerateButton
};

class GSButtonLayer :public SPSControlLayer
{
public:
	CREATE_FUNC(GSButtonLayer);

	/**
	 *��ӵ���������
	 */
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }

CC_CONSTRUCTOR_ACCESS:
	GSButtonLayer();
	~GSButtonLayer();
	virtual bool init();

private:
	Button* createButton(const string& normal, const string& select, const Vec2& position, GSLayerButton buttonName, const bool isFlippedX = false);
	void createKeyBoardListener();
	void createQuitDialog();
	void createRequirementButton();
	void createRequirementLayer();
	void controlAccelerateScheduler();
	void controlDecelerateScheduler();
	void showSeedBank();
	void showShovelBank();
	void createPlantsCard();
	void createJumpLevelButton();
	ProgressTimer* createProgressTimer(Button* button, const float _time, const int from, const unsigned int& id);
	
public:
	MouseSelectImage* mouseSelectImage;                            // ���ѡ��
	PlantsInformation::PlantsCards plantsCards[PLANTSNUMBERS];     // ֲ�￨Ƭ
	
private:
	Global* _global;                                     // ȫ�ֱ���
	Director* _director;                                 // ���ݵ���
	Layer* _quitLayer;                                   // �˳����β�
	Button* _accelerateButton;                           // ���ٰ�ť
	Button* _decelerateButton;                           // ���ٰ�ť
	OpenLevelData* _openLevelData;
};

