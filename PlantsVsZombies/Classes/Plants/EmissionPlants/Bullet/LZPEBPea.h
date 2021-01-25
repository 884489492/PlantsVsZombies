/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.18
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZPEBBullet.h"

class Pea :public Bullet
{
public:
	/**
	 *�����Ƿ�ȼ��
	 */
	virtual void setIsFire(const bool isFire);

	/**
	 *�����Ƿ���� 
	 */
	virtual void setIsIce(const bool isIce);

	/**
	 *����ȼ�մ���
	 */
	virtual void setPeaFireNumbers(const int fireNumbers);

	/**
	 *�����ӵ�ȼ�մ���
	 * -1 ��  ��ʾ������
	 * 0  ��  ��ʾû��ȼ��
	 * >=1:   ��ʾ��ȼ�յĴ���
	 */
	virtual void addPeaFireNumbers();

	/**
	 *���û����׮���
	 */
	virtual void setTorchwoodTag(const int torchwoodTag);

	/**
	 *��ȡ�Ƿ�ȼ��
	 */
	virtual bool getIsFire() const;

	/**
	 * ��ȡ�Ƿ����
	 */
	virtual bool getIsIce() const;

	/**
	 *��ȡȼ�մ���
	 */
	virtual int getPeaFireNumbers() const;

	/**
	 *��ȡ�����׮���
	 */
	virtual int getTorchwoodTag() const;

protected:
	/**
	 *�����㶹
	 */
	virtual void createBullet() override;

	/**
	 *�㶹�뽩ʬ��ײ�ͼ��
	 */
	virtual void bulletAndZombiesCollision() override;

	/**
	 *  �жι�����ʽ
	 */
	virtual void judgeAttackMethod(Zombies* zombie);

	/**
	 * ������ʬ 
	 */
	virtual void attackZombies(Zombies* zombie);
	
	/**
	 * �Խ�ʬ�����˺� 
	 */
	virtual void splashDamageZombies(Zombies* exceptZombie);

	/**
	 *  ��ʬ�Ƿ��ڽ��䷶Χ�ж�
	 */
	virtual bool getZombieInExplodeRange(Zombies* zombie);

	/**
	 *�����㶹��ը����
	 */
	virtual void  createPeaExplode();

	/**
	 *�����ӵ�Ӱ��
	 */
	virtual void createShadow() override;

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
	bool _isFire;                         // �Ƿ�ȼ�� 
	bool _isIce;                          // �Ƿ����
	int _fireNumbers;                     // ȼ�մ���
	int _torchwoodTag;                    // �����׮���
	string _peaAnimationName;             // �㶹��������
};
