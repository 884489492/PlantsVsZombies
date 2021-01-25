/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.1.22
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZPBBattlePlants.h"

class Zombies;

class GloomShroom :public BattlePlants
{
public:
	static GloomShroom* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	GloomShroom(Node* node = nullptr);
	~GloomShroom();

protected:
	virtual SkeletonAnimation* showPlantAnimationAndText() override;
	virtual bool getZombieIsInAttackRange(Zombies* zombie);
	virtual void plantAttack(Zombies* zombie);
	virtual void plantRecovery(const string& name);
	virtual void setListenr();

protected:
	bool _isChangeAnimation;  /* �Ƿ�ı䶯�� */
	bool _isAttack;           /* �Ƿ񹥻� */
	bool _isHaveZombies;      /* �Ƿ��н�ʬ���빥����Χ */
};