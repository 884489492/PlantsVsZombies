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

#include "Based/LZGlobalVariable.h"
#include "Plants/LZPlants.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace spine;

class Plants;
enum class PlantsType;

class SPSSpriteLayer : public Layer
{
public:
	CREATE_FUNC(SPSSpriteLayer);
	static void createButtonHoverEffect(Button* button);
	static void pauseButtonHoverEffect();
	static void resumeButtonHoverEffect();
	static void createPlantsText(const unsigned int& ID, const std::string& name, 
		const Vec2& vec2, const float& fontsize, Color3B color = Color3B::WHITE, bool AnchorPoint = true);
	static string selectRequirementText(map<string, LanguageTextAttribute*>& lta, PlantsType type, string str, string str1);
	virtual Text* showPlantsInformation(Button* button, bool showHeart = false);

protected:
	virtual void controlPlantCanSelect(Button* button, int priority);
	virtual void createSelectPlantsDialog();

private:
	void alreadySelectPlantsDialog();
	void alreadyHavePlantsDialog();
	void createScrollview();
	void createMouseListener();
	void createPlantsCards();
	void showRandPlantsInformation();
	Button* createButtons(const Vec2& vec2,int priority);
	void showSelectedButtonHoverEffect(EventMouse* e);
	void showPopulationButtonHoverEffect(EventMouse* e);
	void preLoadText();
	void createMoveButton(Button* button, const Vec2& vec2);
	void createAnimationAndText(PlantsType type);
	void sortPlantsCard(PlantsType type);
	void createBeginButton();
	void startGame();
	void selectPlantsCallBack(Node* node) { _selectFinished = true; }
	float calculateScrollDistance();
	float calculateScrollPrecent(float distance);
	void popUnlockDialog();
	void plantsCardButtonEvent(Button* button, Vec2 vec2);
	void plantsMoveCardButtonEvent(Button* button, Button* moveCard, Vec2 vec2, float plantCardRollingDistanceLast);
	
CC_CONSTRUCTOR_ACCESS:
	SPSSpriteLayer();
	~SPSSpriteLayer();
	virtual bool init();

public:
	static Text* animationText[PLANTSNUMBERS];        /* ����˵�� */
	static bool isPlantIsCanSelect[PLANTSNUMBERS];    /* �Ƿ����ѡ���ֲ�� */
	static ui::ScrollView* plantCardTextScrollView;   /* ֲ�￨�ƹ�����ͼ */
	bool _selectFinished;                             /* �Ƿ�ѡ����� */
	vector<UserSelectCard> seedBankButton;

protected:
	Global* _global;                           /* ȫ�ֱ������� */
	Sprite* _seedChooser;                      /* ֲ��ѡ��Ի��� */
	Plants* _plant;                            /* ֲ����� */
	SkeletonAnimation* _plantsAnimation;       /* ֲ�ﶯ�� */
	ui::ScrollView* _plantCardScrollView;      /* ֲ�￨�ƹ�����ͼ */
	float _plantCardScrollViewPercent;         /* ֲ�￨�ƹ�����ͼ�������� */
	float _plantCardRollingDistance;           /* �������������� */
	static EventListenerMouse* _listener;      /* ������ */
	static int _listenerMutex;                 /* ���������� */
	map<PlantsType, Button*>_plantsCards;      /* ֲ�￨�� */
};