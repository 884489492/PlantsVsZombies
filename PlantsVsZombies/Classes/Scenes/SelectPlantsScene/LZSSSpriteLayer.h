/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "spine/spine-cocos2dx.h"

#include "Based/LZBGlobalVariable.h"
#include "Plants/LZPPlants.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace spine;

enum class PlantsType;

class SPSSpriteLayer : public Layer
{
public:
	CREATE_FUNC(SPSSpriteLayer);
	static void createButtonHoverEffect(Button* button);
	static void pauseButtonHoverEffect();
	static void resumeButtonHoverEffect();
	static void createPlantsText(const unsigned int& ID, const std::string& name, const Vec2& vec2, const float& fontsize, Color3B color = Color3B::WHITE, bool AnchorPoint = true);
	static string selectRequirementText(map<string, LanguageTextAttribute*>& lta, PlantsType type, string str, string str1);
	Text* showPlantsInformation(Button* button, PlantsType type);

private:
	void createSelectPlantsDialog();
	void alreadySelectPlantsDialog();
	void alreadyHavePlantsDialog();
	void createScrollview();
	void createMouseListener();
	void createPlantsCards();
	void showRandPlantsInformation();
	Button* createButtons(const Vec2& vec2, PlantsType type);
	void showSelectedButtonHoverEffect(EventMouse* e);
	void showPopulationButtonHoverEffect(EventMouse* e);
	void preLoadText();
	void createPlantsImage(Button* button, PlantsType type, const float& scale = 0.8f) const;
	void createMoveButton(Button* button, const Vec2& vec2, PlantsType type);
	void createAnimationAndText(PlantsType type);
	void sortPlantsCard(PlantsType type);
	void createBeginButton();
	void controlPlantCanSelect(Button* button, PlantsType type);
	void createProhibit(Button* button);
	void startGame();
	void selectPlantsCallBack(Node* node) { _selectFinished = true; }
	float calculateScrollDistance();
	float calculateScrollPrecent(float distance);

CC_CONSTRUCTOR_ACCESS:
	SPSSpriteLayer();
	~SPSSpriteLayer();
	virtual bool init();

public:
	static Text* animationText[PLANTSNUMBERS];        /* ����˵�� */
	static ui::ScrollView* plantCardTextScrollView;   /* ֲ�￨�ƹ�����ͼ */
	static bool isPlantIsCanSelect[PLANTSNUMBERS];    /* �Ƿ����ѡ���ֲ�� */
	bool _selectFinished;                             /* �Ƿ�ѡ����� */
	vector<UserSelectCard> seedBankButton;

private:
	Global* _global;
	Sprite* _seedChooser;
	SkeletonAnimation* _plantsAnimation;       /* ֲ�ﶯ�� */
	ui::ScrollView* _plantCardScrollView;      /* ֲ�￨�ƹ�����ͼ */
	float _plantCardScrollViewPercent;         /* ֲ�￨�ƹ�����ͼ�������� */
	float _plantCardRollingDistance;           /* �������������� */
	unsigned int _plantsNumber;                /* ֲ������ */
	static EventListenerMouse* _listener;      /* ������ */
	static int _listenerMutex;                 /* ���������� */
	vector<Button*>_plantsCards;               /* ֲ�￨�� */
};