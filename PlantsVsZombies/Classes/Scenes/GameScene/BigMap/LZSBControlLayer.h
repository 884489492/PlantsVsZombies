/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.14
 *Email: 2117610943@qq.com
 */
#pragma once
#include "../LZSGControlLayer.h"

class Zombies;

class BMControlLayer :public GSControlLayer
{
public:
	CREATE_FUNC(BMControlLayer);
	static void judgeZombiesWin(Zombies* zombie);
	virtual void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }

CC_CONSTRUCTOR_ACCESS:
	BMControlLayer();
	~BMControlLayer();
	virtual bool init();

protected:
	virtual void initData() override;                                                            /* ��ʼ������ */
	virtual void judgeLevelIsFinished() override;                                                /* �жϹؿ��Ƿ���� */
	virtual void createSchedule() override;                                                      /* ������ʱ�� */
	virtual void calculatePlantPosition() override;                                              /* ����ֲ����ֲλ�� */
	virtual void createMouseListener() override;                                                 /* ������� */
	virtual void mouseMoveControl() override;                                                    /* ����ƶ����� */
	virtual void createPreviewPlants() override;                                                 /* ����Ԥ��ֲ�� */
	virtual void mouseDownControl(EventMouse* eventmouse = nullptr) override;                    /* ��갴�¿��� */
	virtual Vec2 addScrollViewOffset(Vec2& vec2);
	virtual void changeScrollViewOffset();
	
public:
	static bool isLose;

protected:
	bool _isFade;
	Vec2 _offset;
};
