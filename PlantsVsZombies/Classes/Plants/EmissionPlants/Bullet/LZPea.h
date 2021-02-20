/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.18
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZBullet.h"

class Pea :public Bullet
{
public:
	enum class PeaDirectionType
	{
		Up,         // �Ϸ�
		Normal,     // ����
		Down        // �·�
	};

public:
	/**
	 *�����㶹
	 */
	virtual void createBullet() override;

	/**
	 *���û����׮���
	 */
	virtual void setTorchwoodTag(const int torchwoodTag);

	/**
	 * �����㶹ƫ�Ʒ������ 
	 */
	virtual void calculateDirectionDistance(Vec2 initPosition, Vec2 position);

	/**
	 * �����㶹��������
	 */
	virtual void setPeaDirectionType(PeaDirectionType type);

	/**
	 * ��ȡ�㶹��������
	 */
	virtual PeaDirectionType getPeaDirectionType();

	/**
	 *��ȡ�����׮���
	 */
	virtual int getTorchwoodTag() const;

protected:
	/**
	 *�㶹�뽩ʬ��ײ�ͼ��
	 */
	virtual void bulletAndZombiesCollision() override;

	/**
	 * ������ʬ 
	 */
	virtual void attackZombies(Zombies* zombie);
	
	/**
	 *  ��ʬ�Ƿ��ڽ��䷶Χ�ж�
	 */
	virtual bool getZombieInExplodeRange(Zombies* zombie);

	/**
	 *�����㶹��ը����
	 */
	virtual void  createPeaExplode();

	/**
	 *������ըЧ�� 
	 */
	virtual void createExplodeEffect();

	/**
	 *�����ӵ�Ӱ��
	 */
	virtual void createShadow(Vec2 position = Vec2::ZERO) override;

	/**
	 * �����ƶ���ʽ
	 */
	virtual void setBulletAction();

	/**
	 * �洢�ӵ�������Ϣ
	 */
	virtual void caveBulletInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) override;

	/**
	 * ��ȡ�ӵ�������Ϣ
	 */
	virtual void readBulletInformation(rapidjson::Document* levelDataDocument, char* key, int i) override;
	virtual void readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i) override;

CC_CONSTRUCTOR_ACCESS:
	Pea(Node* node = nullptr);
	~Pea();

protected:
	int _torchwoodTag;                    // �����׮���
	float _actionTime;                    // ����ʱ��
	Vec2 _initPostion[2];                 // ��ʼʱ��
	string _peaAnimationName;             // �㶹��������
	PeaDirectionType _peaDirectionType;   // �㶹��������
};
