/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#pragma once
#include <random>

#include "cocos2d.h"
#include "Based/LZBGlobalVariable.h"

using namespace cocos2d;

class ZombiesAppearControl;
class GSGameEndLayer;
class OpenLevelData;
enum class ZombiesType;
enum class PlantsType;


/* ��Ϸ��ͼ */
struct GameMapInformation
{
	GameMapInformation();
	int plantsMap[6][10];              /* ֲ����ֲ��ͼ */
	unsigned int rowNumbers;           /* ���� */
	unsigned int columnNumbers;        /* ���� */
};

class GSControlLayer :public Layer
{
public:
	CREATE_FUNC(GSControlLayer);
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }
	static void setPlantMapCanPlant(const unsigned int colum, const unsigned int row);
	void updateFlag();

CC_CONSTRUCTOR_ACCESS:
	GSControlLayer();
	~GSControlLayer();
	virtual bool init();

protected:
	virtual void initData();                                                            /* ��ʼ������ */
	virtual void judgeLevelIsFinished();                                                /* �жϹؿ��Ƿ���� */
	virtual void zombiesComeTiming();                                                   /* ��ʬ���ּ�ʱ */
	virtual void createZombies();                                                       /* ������ʬ */
	virtual bool controlRefurbishZombies();                                             /* ���ƽ�ʬˢ�� */
	virtual void controlRefurbishMusicAndText();                                        /* �������������ֵ�ˢ��*/
	virtual void createSchedule();                                                      /* ������ʱ�� */
	virtual void controlCardEnabled();                                                  /* ���ƿ����Ƿ���� */
	virtual void calculatePlantPosition();                                              /* ����ֲ����ֲλ�� */
	virtual void createMouseListener();                                                 /* ������� */
	virtual void mouseMoveControl();                                                    /* ����ƶ����� */
	virtual void mouseDownControl(EventMouse* eventmouse = nullptr);                    /* ��갴�¿��� */
	virtual void createPlantsCardListener();                                            /* ����ֲ�￨�Ƽ��� */
	virtual void showSelectedButtonHoverEffect();                                       /* ��ʾ���������ͣЧ�� */
	virtual void selectPlantsPreviewImage();                                            /* ѡ��ֲ��Ԥ��ͼƬ */
	virtual void createPreviewPlants();                                                 /* ����Ԥ��ֲ�� */
	virtual bool judgeMousePositionIsInMap();                                           /* �ж��������λ�Ƿ��ڵ�ͼ�� */
	virtual bool judgeMousePositionIsCanPlant();                                        /* �ж��������λ�Ƿ������ֲֲ�� */
	virtual bool judgeMousePositionHavePlant();                                         /* �ж��������λ�Ƿ���ֲ�� */
	virtual void removePreviewPlant();                                                  /* �Ƴ�Ԥ��ֲ�� */
	virtual void removeShovel();                                                        /* �Ƴ����� */
	virtual void removeMouseListener();                                                 /* �Ƴ������� */
	virtual void recoveryPlantsColor();                                                 /* �ָ�ֲ����ɫ */
	virtual void setGameEnd();                                                          /* ������Ϸ���� */
	
public:
	Vec2 _cur,_touch;                                    // ������� 
	Vec2 _plantsPosition;                                // ֲ����ֲ����
	EventMouse* _touchMouse;                             // �������
	PlantsType _selectPlantsTag;                         // ��ѡֲ����
	GameMapInformation* _gameMapInformation;             // ��Ϸ��ͼ��Ϣ
	ZombiesAppearControl* _zombiesAppearControl;         // ��ʬ���ּ���

protected:
	Sprite* _plantPreviewImage;                          // ֲ��Ԥ��ͼƬ
	Sprite* _plantCurImage;                              // ֲ�����ͼƬ
	Global* _global;                                     // ȫ�ֵ���
	Node* _animationLayer;                               // ��Ϸ��
	GSGameEndLayer* _gameEndShieldLayer;                 // ��Ϸ�������β�
	OpenLevelData* _openLevelData;                       // �ؿ����ݵ���
	vector<int> _levelData;                              // ��ʱ�ؿ�����

private:
	EventListenerMouse* _listener;                       // ������ 
};