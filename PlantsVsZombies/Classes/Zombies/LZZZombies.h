/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.26
 *Email: 2117610943@qq.com
 */

#pragma once
#include <random>

#include "cocos2d.h"
#include "spine/spine.h"
#include "spine/spine-cocos2dx.h"

#include "Based/LZBLevelData.h"
#include "Based/LZBGlobalVariable.h"

using namespace spine;
using namespace cocos2d;
using namespace cocos2d::experimental;

class Plants;
class GSGameResultJudgement;
class GSGameEndLayer;

/* ��ʬ���� */
enum class ZombiesType
{
	None = 0,
	CommonZombies,
	ConeZombies,
	BucketZombies,
	CommonDoorZombies,
	ConeDoorZombies,
	BucketDoorZombies,
	LmpZombies,
	CommonFlagZombies,
	ConeFlagZombies,
	BucketFlagZombies,
	CommonDoorFlagZombies,
	ConeDoorFlagZombies,
	BucketDoorFlagZombies,
	SnowZombies,
	GargantuarZombies
};

enum class ShieldType
{
	none = 0,
	IronHeadShield,
	IronBodyShield,
	PlasticsHeadShield
};

class Zombies :public Node
{
public:
	static bool compare (Zombies* a, Zombies* b) {
		return a->_zombieRow < b->_zombieRow;
	}
	/**
	 *��ȡ���ֽ�ʬ���ද������
	 */
	static string getZombieAniamtionName(ZombiesType zombiestype);

	/**
	 *ɾ����ʬ
	 */
	static void zombiesDeleteUpdate(list<Zombies*>::iterator& zombie);
	static void zombiesDeleteUpdateNotRecordDieNumbers(list<Zombies*>::iterator& zombie);

	/**
	 *��ȡ��ʬ����
	 */
	static unsigned int getZombiesNumbers();

	/**
	 *���ý�ʬ����
	 */
	static void setZombiesNumbers(const unsigned int numbers);

	/**
	 *��ʬ��������/�Լ�һ
	 *@ ++ ����1
	 *@ -- �Լ�1
	 */
	static void zombiesNumbersChange(const string&);

	/**
	 *�жϽ�ʬӮ
	 */
	static void judgeZombieWin(list<Zombies*>::iterator zombie);

	/**
	 *��ʬ��Ӯ��ʼ��
	 */
	static void zombiesWinOrLoseInit();

	/**
	 *��ȡ����
	 */
	static GLProgram* getHighLight();

	/**
	 *�������
	 */
	static void rewardCoin(SkeletonAnimation* zombies);

	/**
	 *������ʬ
	 */
	virtual void createZombie() = 0;

	/**
	 *����Ԥ����ʬ
	 */
	virtual void createPreviewZombie() = 0;

	/**
	 *��ʬ�����¼�����
	 */
	virtual void zombieInjuredEventUpdate() = 0;

	/**
	 * ��ȡ��ʬ������Ϣ
	 * ���������д�÷���
	 */
	virtual void readZombieInformation();

	/**
	 *������ʬӰ��
	 */
	virtual void createZombieShadow();

	/**
	 * ������ʬ��ʱ��  
	 */
	virtual void createZombieTimer();

	/**
	 * ���ý�ʬ������������֮һ 
	 */
	void setZombieActionSlow();

	/**
	 * ���ý�ʬ�ָ��˶��ٶ� 
	 */
	void setZombieActionRecovery(bool slow = false);

	/**
	 * ���ý�ʬ����ֹͣ
	 */
	void setZombieActionStop();

	/**
	 *���ý�ʬ��С
	 */
	virtual void setZombieScale(const Size& scale = Size(40, 40)) const;

	/**
	 *���ý�ʬ��С
	 */
	virtual void setZombieScale(const int& scale) const;

	/**
	 * ���ý�ʬ������ 
	 */
	virtual void setZombieInRow(const int row);

	/**
	 *���ö��������ٶ�
	 */
	virtual void setZombieTimeScale(const float& timeScale) const;

	/**
	 *���ý�ʬ�������
	 */
	virtual void setZombieAnimation(const int tag, const bool isLoop = true) const;

	/**
	 *���ý�ʬ����
	 */
	virtual void setZombieAnimation(const string& name, bool isLoop = true) const;

	/**
	 *���ý�ʬ�Ĵ�С
	 */
	virtual void setZombieAttributeForGameType();

	/**
	 *�����Ƿ���ʾ��ʬ����֫�嶯��
	 */
	virtual void setZombieIsShowLoseLimbsAnimation(const bool isShow);

	/**
	 *�����Ƿ���ʾ��ʬ���令�ܶ���
	 */
	virtual void setZombieIsShowLoseShieldAnimation(const bool isShow);

	/**
	 *���ý�ʬ����ֲ����
	 */
	virtual void setZombieEatPlantNumber(const int& number);

	/**
	 *���ý�ʬ�ƶ�
	 */
	virtual void setZombieMove(const float delta);

	/**
	 *���ý�ʬ͸����
	 */
	virtual void setZombieOpacity(GLubyte opacity);

	/**
	 *����λ��
	 */
	virtual void setZombiePosition(const Vec2& position);

	/**
	 *���ý�ʬ��������
	 */
	virtual void setZombieAnimationName(const string& name);

	/**
	 *���ý�ʬ���
	 */
	virtual void setZombieTag(const int& tag);

	/**
	 *���ý�ʬ��ǰ�ٶ�
	 */
	virtual void setZombieCurrentSpeed(const int currentSpeed);

	/**
	 *���ý�ʬֹͣ�ƶ�
	 */
	virtual void setZombieStop();

	/**
	 *���ý�ʬ�Ƿ���ʾ
	 */
	virtual void setZombieIsShow(const bool isShow);

	/**
	 *���ý�ʬ�Ƿ��ֲ��
	 */
	virtual void setZombieIsEat(const bool isEat);

	/**
	 *���ý�ʬ����
	 */
	virtual void setZombieDeath(const bool death);

	/**���ý���ʬ�Ƿ���ʾ
	 */
	virtual void setZombieVisible(const bool isVisible);

	/**
	 *���ý�ʬ���廤��Ѫ��
	 */
	virtual void setZombieCurrentBodyShieldVolume(const float currentBodyShieldVolume);

	/**
	 *���ý�ʬͷ������Ѫ��
	 */
	virtual void setZombieCurrentHeadShieldVolume(const float currentHeadShieldVolume);

	/**
	 *���ý�ʬѪ��
	 */
	virtual void setZombieCurrentBloodVolume(const float currentBloodVolume);

	/**
	 *���ý�ʬ�Ƿ񴴽���ʱ�� 
	 */
	virtual void setZombieIsCreateTimer(const bool isCreateTimer);

	/**
	 *���ý�ʬ�Ƿ񱻻���
	 */
	virtual void setZombieIsStrikeFly(const bool isStrikeFly);

	/**
	 *���ý�ʬ�Ƿ񱻶�ס  
	 */
	virtual void setZombieIsFrozen(const int isFrozen);

	/**
	 *���ý�ʬ������˸
	 */
	virtual void setZombieHurtBlink();

	/**
	 *���ý�ʬ��С
	 */
	virtual void setZombieScale();

	/**
	 * ���ý�ʬ�Ƿ�Դ��� 
	 */
	virtual void setZombieIsEatGarlic(const bool isEatGarlic);

	/**
	 * ��ȡ��ʬ������ 
	 */
	virtual int getZombieInRow() const;

	/**
	 *���ý�ʬ�ѱ�ĳһֲ��Ԥ��ɱ��
	 */
	virtual void setZombieReserveKill(bool reserveKill);

	/**
	 *���ý�ʬ��ʱ��ʱ��  
	 */
	virtual void setZombieTimerTime(const int timerTime, bool slow = false);

	/**
	 * ���� ��ʬ��ͬ�ؿ�������ʧЧ
	 */
	virtual void setZombieAttributeForGameTypeInvalid(const bool invalid);

	/**
	 *��ȡ��ʬ����
	 */
	virtual SkeletonAnimation* getZombieAnimation() const;

	/**
	 *��ʬ�Ƿ���
	 */
	virtual bool getZombieIsSurvive() const;

	/**
	 *��ȡ��ʬ�Ƿ���ʾ
	 */
	virtual bool getZombieIsVisible() const;

	/**
	 *��ȡ��ʬ����
	 */
	virtual string getZombieAnimationName() const;

	/**
	 *��ȡ��ʬ���
	 */
	virtual int getZombieTag() const;

	/**
	 *��ȡ��ʬ����ֲ����
	 */
	virtual int getZombieEatPlantNumber() const;

	/**
	 *��ʬû�в�����������
	 */
	virtual bool getZombieIsPlayDieAnimation() const;

	/**
	 *��ȡ��ʬλ��
	 */
	virtual Vec2 getZombiePosition()  const;

	/**
	 *��ȡ��ʬx����
	 */
	virtual float getZombiePositionX() const;

	/**
	 *��ȡ����y����
	 */
	virtual float getZombiePositionY() const; 
	
	/**
	 *��ȡ��ʬ�Ƿ�����ͼ
	 */
	virtual bool getZombieIsEnterMap() const;

	/**
	 *��ȡ��ʬ���廤��Ѫ��
	 */
	virtual float getZombieCurrentBodyShieldVolume() const;

	/**
	 *��ȡ��ʬͷ������Ѫ��
	 */
	virtual float getZombieCurrentHeadShieldVolume() const;

	/**
	 *��ȡ��ʬѪ��
	 */
	virtual float getZombieCurrentBloodVolume() const;

	/**
	 * ��ȡ��ǰ��Ѫ������Ѫ���İٷֱ� 
	 */
	virtual float getZombieCurrentBloodProportionBloodPrecent() const;

	/**
	 *��ȡ��ʬ�Ƿ��ڳ�ֲ��
	 */
	virtual bool getZombieIsEat() const;

	/**
	 *��ȡ��ʬ�Ƿ�ֹͣ
	 */
	virtual bool getZombieIsStop() const;

	/**
	 *@ 1.��ȡ��ʬ��ǰ���ٶ�
	 */
	virtual float getZombieCurrentSpeed() const;

	/**
	 *@ 2.��ȡ��ʬԭ���ٶ�
	 */
	virtual float getZombieSpeed() const;

	/**
	 *��ȡ��ʬ�Ƿ���ʾ
	 */
	virtual bool getZombieIsShow() const;

	/**
	 *��ȡ��ʬ�Ƿ񴴽���ʱ��  
	 */
	virtual bool getZombieIsCreateTimer() const;

	/**
     *��ȡ��ʱ��ʱ��  
	 */
	virtual int& getZombieTimerTime(bool slow = false);

	/**
	 *��ȡ��ʬ�Ƿ񱻶�ס  
	 */
	virtual int getZombieIsFrozen() const;

	/**
	 *��ȡ��ʬ����
	 */
	virtual ZombiesType getZombieType() const;

	/**
	 *��ȡ��ʬͷ������������Ч 
	 */
	virtual int getZombieHeadAttackSoundEffect() const;

	/**
	 *��ȡ��ʬ���屻������Ч
	 */
	virtual int getZombieBodyAttackSoundEffect() const;

	/**
	 *��ȡ�Ƿ��ɾ��
	 */
	virtual bool* getIsCanDelete();

	/**
	 *��ȡ��ʬ�Ƿ��л���
	 */
	virtual bool getZombieIsHaveShield() const;

	/**
	 *��ȡ��ʬ�Ƿ񱻻���
	 */
	virtual bool getZombieIsStrikeFly() const;

	/**
	 *��ȡ��ʬ�Ƿ��ѱ�ĳһֲ��Ԥ��ɱ��
	 */
	virtual bool getZombieReserveKill();

	/**
	 *��ȡ��ʬ���廤������
	 */
	ShieldType getZombieBodyShieldType()const;

	/**
	 *��ȡ��ʬͷ����������
	 */
	ShieldType getZombieHeadShieldType()const;

	/**
	 * ��ȡ��ʬ�Ƿ�Դ���
	 */
	virtual bool getZombieIsEatGarlic() const;

	/**
	 *���Ž�ʬ�ҽ�����
	 */
	virtual void playZombiesFillDownAnimation();

	/**
	 *���Ž�ʬ�ҽ�����
	 */
	virtual void playZombiesAshesAnimation();

	/**
	 *���Ž�ʬ��Ч
	 */
	virtual void playZombieSoundEffect();

protected:
	/*
	 *��ʼ����ʬ
	 */
	virtual void zombieInit(const string& animation_name = "Zombies");

	/**
     *��ȡ��ʬ����ͼ��
     */
	virtual float getZombieLocalZOrder() const;

	/**
	 *���ýڵ�
	 */
	virtual void setNode(Node* node){ _node = node; }

	/**
	 *���ý�ʬһ������
	 */
	virtual void setZombiePrimaryInjure();

	/**
	 *���ý�ʬ��������
	 */
	virtual void setZombieSecondaryInjure();

	/**
	 *��ʬ���첲����
	 */
	virtual void zombieLoseArmAnimation(const std::string& name);

	/**
	 *��ʬ��ͷ����
	 */
	virtual void zombieLoseHeadAnimation(const std::string& name);

	/**
	 *��ʬ�����ܶ���
	 */
	virtual void zombieLoseShieldAnimation(const std::string& name);

	/**
	 *��ʬ��ʧ����
	 */
	virtual void zombieFadeOutAnimation();

	/**
	 *��ʬ��Ч
	 */
	virtual void playZombieSoundEffect(const string& name);

	/**
	 *��ʾ��ʬ֫���뻤��Ӱ��
	 */
	virtual void showZombieShadow(Node* node, const int posy);

	/**
	 *���ý�ʬ��ͬ�ؿ�������
	 */
	virtual void setZombieAttributeForGameType(Node* sprite);

	/**
	 *����ͷ������һ������
	 *����������֮ǰ����
	 *������Ҫ����������
	 *���������˵ȼ�
	 */
	virtual void setZombieHeadShieldPrimaryInjure(const string& oldName, const string& newName);

	/**
	 *����ͷ�����ܶ�������
	 *����������֮ǰ����
	 *������Ҫ����������
	 *���������˵ȼ�
	 */
	virtual void setZombieHeadShieldSecondaryInjure(const string& oldName, const string& newName);

	/**
	 *����ͷ��������������
	 *����������֮ǰ����
	 *������Ҫ����������
	 *���������˵ȼ�
	 */
	virtual void setZombieHeadShieldThirdInjure(const string& oldName, const string& newName);

	/**
	 *�������廤��һ������
	 *����������֮ǰ����
	 *������Ҫ����������
	 *���������˵ȼ�
	 */
	virtual void setZombieBodyShieldPrimaryInjure(const string& oldName, const string& newName);

	/**
	 *�������廤�ܶ�������
	 *����������֮ǰ����
	 *������Ҫ����������
	 *���������˵ȼ�
	 */
	virtual void setZombieBodyShieldSecondaryInjure(const string& oldName, const string& newName);

	/**
	 *�������廤����������
	 *����������֮ǰ����
	 *������Ҫ����������
	 *���������˵ȼ�
	 */
	virtual void setZombieBodyShieldThirdInjure(const string& oldName, const string& newName);

CC_CONSTRUCTOR_ACCESS:
	Zombies();
	~Zombies();

private:
	void setSmallZombieAttribute();
	void setBigZombieAttribute();
	void setOpacityZombieAttribute();
	void setZombieGLProgram();
	bool getZombieWarning();

protected:
	int _attackHeadSoundEffectType;           // ����ͷ����Ч   
	int _attackBodySoundEffectType;           // ����������Ч
	int _bodyAnimationId;                     // ���嶯�����
	int _bodyShieldAnimationId;               // ���廤�ܶ������
	int _headShieldAnimationId;               // ͷ�����ܶ������
	int _zombieTag;                           // ��ʬ���(��δʹ��)
	int _zombieRow;                           // ��ʬ������
	int _timerTimeSlow;                       // ��¼�����˶�ʱ��
	int _timerTimeStop;                       // ��¼ֹͣ�˶�ʱ��
	int _isFrozen;                            // �Ƿ񱻶�ס���٣���ݫ�ģ��������֣�����Ͷ�֣�
	float _timeScale;                         // �����ٶ�
	float _bloodVolume;                       // Ѫ��
	float _headShieldVolume;                  // ����Ѫ��
	float _bodyShieldVolume;                  // ����1Ѫ��
	float _currentBloodVolume;                // ��ǰѪ��
	float _currentHeadShieldVolume;           // ��ǰ����Ѫ��
	float _currentBodyShieldVolume;           // ��ǰ����1Ѫ��
	float _speed;                             // �洢�ƶ��ٶ�
	float _currentSpeed;                      // �ƶ��ٶ�
	bool _isHaveShield;                       // �Ƿ��л���
	bool _isUseForGameType;                   // �Ƿ�ʹ������Ϸ����
	bool _isEat;                              // �Ƿ��ڳ�ֲ��
	bool _isEatGarlic;                        // �Ƿ�Դ���
	bool _isShow;                             // �Ƿ���ʾ
	bool _isPreview;                          // �Ƿ���Ԥ��ģʽ
	bool _isShowLoseLimbsAnimation;           // �Ƿ���ʾ��ʬ����֫�嶯��
	bool _isShowLoseShieldAnimation;          // �Ƿ���ʾ��ʬ���ܵ��䶯��
	bool _isCanDelete[2];                     // �Ƿ����ɾ��
	bool _isCreateTimer;                      // �Ƿ񴴽���ʱ��
	bool _isStrikeFly;                        // �Ƿ����(����Ե��
	bool _isReserveKill;                      // �Ƿ�Ԥ��ɱ�������컨��
	bool _gameTypeInvalid;                    // ��Ϸ�����Ƿ�ʧЧ
	string _zombieAnimationName;              // ��ʬ��������
	Vec2 _position;                           // λ��
	Node* _node;                              // �ڵ�
	Global* _global;                          // ȫ�ֱ���
	SkeletonAnimation* _zombiesAnimation;     // ��ʬ����
	ShieldType _headShieldType;               // ͷ����������
	ShieldType _bodyShieldType;               // ���廤������
	ZombiesType _zombiesType;                 // ��ʬ����
	string _animationName[7];                 // ��������
	default_random_engine _random;            // ���������
	random_device _device;
	static unsigned int _zombiesNumbers;      // ��ʬ����

private:
	int _zombieEatPlantNumber;
	int _zombieHowlNumbers;
	OpenLevelData* _openLevelData;
	GLProgramState* _highLightGLProgramState;
	float _highLightIntensity;                
	bool _highLightFinished;                  // �Ƿ����
	bool _redWarning;                         // �Ƿ񾯸�

	static bool _zombieIsWin;
	static int _zombiesNewDieNumbers;
	static GLProgram* _normalGLProgram;
	static GLProgram* _highLightGLProgram;
	static GSGameEndLayer* _gameEndLayer;
	static GSGameResultJudgement* _gameResultJudgement;
};
