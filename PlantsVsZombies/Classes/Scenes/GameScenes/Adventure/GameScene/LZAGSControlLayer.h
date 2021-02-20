/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#pragma once
#include <random>

#include "cocos2d.h"
#include "Based/LZGlobalVariable.h"

using namespace cocos2d;

class ZombiesAppearControl;
class GSGameEndLayer;
class OpenLevelData;
enum class ZombiesType;
enum class PlantsType;

/* ��Ϸ��ͼ */
struct GameMapInformation
{
	GameMapInformation(unsigned int row = 5, unsigned int column = 9);
	void GameMapInit();
	int plantsMap[10][18];             /* ֲ����ֲ��ͼ */
	unsigned int rowNumbers;           /* ���� */
	unsigned int columnNumbers;        /* ���� */
	unsigned int mapLeft;              /* ��ͼ���λ�� */
	unsigned int mapRight;             /* ��ͼ�ұ�λ�� */
	unsigned int mapTop;               /* ��ͼ�ϱ�λ�� */
	unsigned int mapBottom;            /* ��ͼ�±�λ�� */
};

class GSControlLayer :public Layer
{
public:
	CREATE_FUNC(GSControlLayer);
	virtual void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }
	static void setPlantMapCanPlant(const unsigned int colum, const unsigned int row);
	virtual void updateFlag();

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
	virtual void mouseLeftButtonDownControl();                                          /* ���������¿��� */
	virtual void mouseRightButtonDownControl();                                         /* ����Ҽ������¿��� */
	virtual void mouseMiddleButtonDownControl();                                        /* ����м������¿��� */
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
	Vec2 _cur;                                           // ������� 
	Vec2 _plantsPosition;                                // ֲ����ֲ����
	PlantsType _selectPlantsTag;                         // ��ѡֲ����
	GameMapInformation* gameMapInformation;              // ��Ϸ��ͼ��Ϣ
	ZombiesAppearControl* _zombiesAppearControl;         // ��ʬ���ּ���

protected:
	Sprite* _plantPreviewImage;                          // ֲ��Ԥ��ͼƬ
	Sprite* _plantCurImage;                              // ֲ�����ͼƬ
	Global* _global;                                     // ȫ�ֵ���
	GSGameEndLayer* _gameEndShieldLayer;                 // ��Ϸ�������β�
	OpenLevelData* _openLevelData;                       // �ؿ����ݵ���
	vector<int> _levelData;                              // ��ʱ�ؿ�����
	EventListenerMouse* _listener;                       // ������ 
	bool _isShowEggScene;                                // �Ƿ���ʾ
};