/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.11
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "spine/spine-cocos2dx.h"
#include "json/document.h"
#include "Based/LZBGlobalVariable.h"
#include "Scenes/GameScene/LZSGAnimationLayer.h"

using namespace spine;
using namespace cocos2d;

class Zombies;

enum class BulletType
{
	None = 0,
	Pea,
	IcePea,
	FirePea,
	Cabbage,
	AcidLemonBullet,
	CitronBullet,
	StarFruitBullet,
	WaterMelonBullet,
	WinterMelonBullet
};

enum class SoundEffectType
{
	kernelpult = 0,
	plastichit,
	shieldhit,
	firepea
};

class Bullet
{
public:
	/**
	 *�����ӵ�
	 */
	virtual void createBullet() = 0;

	/**
	 *�ӵ��뽩ʬ��ײ���
	 */
	virtual void bulletAndZombiesCollision() = 0;

	/**
	 * �洢�ӵ�������Ϣ
	 * ������ʵ��
	 */
	virtual void caveBulletInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) {};

	/**
	 * ��ȡ�ӵ�������Ϣ 
	 * ������ʵ��
	 */
	virtual void readBulletInformation(rapidjson::Document* levelDataDocument, char* key, int i) {};
	virtual void readBulletAnimationInformation(rapidjson::Document* levelDataDocument, char* key, int i) {};

	/**
	 *��ȡ�ӵ�����
	 */
	virtual SkeletonAnimation* getBullet() const;

	/**
	 *�����ӵ���λ��
	 */
	virtual void setBulletPosition(const Vec2& position);
	
	/**
	 * �����ӵ�������  
	 */
	virtual void setBulletInRow(const int row);

	/**
	 *�����ӵ�������
	 */
	virtual void setBulletName(const string& name);

	/**
	 *�����ӵ��Ƿ�ʹ��
	 */
	virtual void setBulletIsUsed(const bool isUsed);

	/**
	 *�����ӵ�������
	 */
	virtual void setBulletAttack(const float attack);

	/**
	 *�����ӵ�͸����
	 */
	virtual void setBulletOpacity(GLubyte opacity);
	virtual void setBulletOpacity();

	/**
	 *��ȡ�ӵ��Ƿ�ʹ��
	 */
	virtual bool getBulletIsUsed() const;

	/**
	 *��ȡ�ӵ�������
	 */
	virtual int getBulletAttack() const;

	/**
	 *��ȡ�ӵ�λ��
	 */
	virtual Vec2 getBulletPosition() const;

	/**
	 * ��ȡ�ӵ���ʼλ�� 
	 */
	virtual Vec2 getBulletInitPosition() const;

	/**
	 *��ȡ�������� 
	 */
	virtual int getBulletInRow() const;

	/**
	 *��ȡ�ӵ�Xλ��
	 */
	virtual float getBulletPositionX() const;

	/**
	 *��ȡ�ӵ�Yλ��
	 */
	virtual float getBulletPositionY() const;

	/**
	 *��ȡ�ӵ�����
	 */
	virtual BulletType getBulletType() const;

	/**
	 *��ȡ�ӵ��Ƿ��뽩ʬ��ͬһ��
	 */
	virtual bool getBulletIsSameLineWithZombie(Zombies* zombie);

	/**
	 *��ȡ�ӵ��Ƿ��뽩ʬ����
	 */
	virtual bool getBulletIsEncounterWithZombie(Zombies* zombie);

	/*
	 *��ȡ�ӵ�������
	 */
	virtual string& getBulletName();

	/**
	 *��ȡ�ӵ��Ƿ���ʾ
	 */
	virtual bool getBulletVisible() const;

	/**
	 *�ӵ�������ʬ
	 */
	virtual void bulletAttackHurtZombies(Zombies* zombie);

	/**
	 *��̬����ɾ���ӵ�
	 */
	static void bulletDeleteUpdate(list<Bullet*>::iterator& bullet);

	/**
	 *�����ӵ���ײ����
	 */
	static void playSoundEffect(SoundEffectType soundEffect);

	/**
	 *ѡ�񲥷���Ч
	 */
	static void selectSoundEffect(const int body, const int head);
	static void selectSoundEffect(const int head);

CC_CONSTRUCTOR_ACCESS:
	Bullet();
	~Bullet();

protected:
	virtual SkeletonAnimation* bulletInit(const std::string& plantname, const std::string& animaionname);
	virtual void createShadow() {};
	virtual void createShadow(Vec2 position) {};
	virtual void createShadow(float scale) {};
	virtual float getZOrder() const;

private:
	void releaseBullet() const;

protected:
	SkeletonAnimation* _bulletAnimation;  // ����
	float _attack;                        // ������
	bool _isUsed;                         // �Ƿ�ʹ��
	int _bulletRow;                       // ������
	int _zombieInExplodeRangeNumbers;     // �����˺���ʬ��
	Node* _node;                          // ���ڵ�
	Vec2 _position;                       // λ��
	string _bulletName;                   // �㶹����
	BulletType _bulletType;               // �ӵ�����
	Global* _global;                      // ȫ�ֵ���
};