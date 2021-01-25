/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.5
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "spine/spine.h"
#include "spine/spine-cocos2dx.h"
#include "json/document.h"

#include "Based/LZBGlobalVariable.h"

#define INIT Vec2(-1000,-1000)
#define NOINITIALIZATION  -1
#define PLANTSNUMBERS 17

using namespace spine;
using namespace cocos2d;
using namespace cocos2d::experimental;

class Zombies;

enum class PlantsType
{
	SunFlower = 0,      /* ���տ� */
	PeaShooter,         /* �㶹���� */
	WallNut,            /* ���ǽ */
	CherryBomb,         /* ӣ��ը�� */
	PotatoMine,         /* ������ */
	CabbagePult,        /* ���Ĳ�Ͷ�� */
	Torchwood,          /* �����׮ */
	Spikeweed,          /* �ش� */
	Garlic,             /* ���� */
	Chomper,            /* ���컨 */
	IcePeaShooter,      /* �����㶹���� */
	FirePeaShooter,     /* �����㶹���� */
	Jalapeno,           /* ������ */
	JalapenoVariation, /* ���������� */
	AcidLemonShooter,   /* ǿ���������� */
	Citron,             /* ����Ե */
	GloomShroom,        /* ������ */
	//...
	None = 19
};  

/**
 *ֲ�￨����Ϣ
 */
class PlantsInformation
{
public:
	struct PlantsCards
	{
		PlantsCards() :
			timeBarIsFinished(false)
		{}
		Button* plantsCards;            /* ���� */
		Text* plantsCardText;           /* �������� */
		ProgressTimer* progressTimer;   /* ����ʱ */
		int plantsNeedSunNumbers;       /* �������� */
		int tag;                        /* ��� */
		bool timeBarIsFinished;         /* ����ʱ�Ƿ���� */
	};

	struct point { int x, y; };
	struct PlantsCardInformation
	{
		string information[3];              // ������Ϣ
		int quality;                        // Ʒ��
		int priority;                       // ���ȼ�
		int plantsNeedSunNumbers;           // ֲ������������
		float plantsCoolTime;               // ֲ����ȴʱ��
		float PlantsSurPlusCoolTime;        // ֲ��ʣ����ȴʱ��
		float PlantsSurPlusPrecent;         // ֲ��ʣ����Ȱٷֱ�
		bool flippedX;                      // ���ҷ�ת
		PlantsType type;                    // ֲ������
		Color3B color;                      // ��ɫ
		point requirement{ 0,0 };           // Ҫ��
	};
};

extern PlantsInformation::PlantsCardInformation plantsCardInformation[];

class  Plants :public Node
{
public:
	/**
	 *��ֲͣ�����ж���
	 */
	static void stopPlantsAllAction();

	/**
	 *����ͼƬ
	*/
	virtual Sprite* createPlantImage() = 0;

	/**
	 *����ֲ�ﶯ��
	 */
	virtual void createPlantAnimation() = 0;

	/**
	 *�жϽ�ʬ��ֲ��֮������λ��
	 */
	virtual void determineRelativePositionPlantsAndZombies();

	/**
	 *���ֲ���������
	 */
	virtual void checkPlantHealthPoint() {}

	/**
	 * �洢ֲ��������Ϣ  
	 * ������д�÷���
	 */
	virtual void cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) {};

	/**
	 * ��ȡֲ��������Ϣ  
	 * ������д�÷���
	 */
	virtual void readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i) {};

	/**
	 * ��ʾֲ�������Ϣ�붯��  
	 */
	virtual SkeletonAnimation* showPlantAnimationAndText() = 0;

	/**
	 * ����ֲ�ﵱǰ��������  
	 */
	//virtual 

	/**
	 *���ýڵ�
	 */
	virtual void setPlantNode(Node* node) { _node = node; }

	/**
	 *����λ��
	 */
	virtual void setPlantPosition(const Vec2& position) { _position = position; }

	/**
	 *���û���˳��
	 */
	virtual void setPlantLocalZOrder(const int& order) { _zOrder = order; }

	/**
	 *����ֲ����������
	 */
	virtual void setPlantRowAndColumn(const Vec2& rowAndColumn) { _rowAndColumn = rowAndColumn; }

	/**
	 *����ֲ���Ƿ�ɼ�
	 */
	virtual void setPlantVisible(const bool visible);

	/**
	 *��ȡֲ������
	 */
	PlantsType getPlantType() const { return _plantsType; }

	/**
	 *����ֲ����
	 */
	virtual void setPlantTag(const int& tag) { _plantTag = tag; }

	/**
	 *����ֲ������ֵ
	 */
	virtual void setPlantHealthPoint(int healthPoint) { _healthPoint = healthPoint; }

	/**
	 *����ֲ���С
	 */
	virtual void setPlantScale();

	/**
	 *��ȡֲ����
	 */
	virtual int getPlantTag() const { return _plantTag; }

	/**
	 *��ȡ����
	 */
	SkeletonAnimation* getPlantAnimation() { return _plantAnimation; }

	/**
	 *�ж�ֲ���Ƿ���
	 */
	virtual bool getPlantIsSurvive() const;

	/**
	 *��ȡֲ������ֵ
	 */
	virtual float getPlantHealthPoint() const { return _healthPoint; }

	/**
	 *@ 1.��ȡֲ����������
	 */
	virtual Vec2 getPlantRowAndColumn() const { return _rowAndColumn; }

	/**
	 *@ 2.��ȡֲ��������
	 */
	virtual int getPlantRow() const { return _rowAndColumn.y; }

	/**
	 *@ 3.��ȡֲ��������
	 */
	virtual int getPlantColumn() const { return _rowAndColumn.x; }

	/**
	 *��ȡ�Ƿ���ʾ
	 */
	virtual bool getPlantVisible();

	/**
	 *��ȡֲ���Ƿ����ɾ��
	 */
	virtual bool* getPlantIsCanDelete();

CC_CONSTRUCTOR_ACCESS:
	Plants(Node* node = nullptr, const Vec2& position = Vec2::ZERO);
	~Plants();
	
protected:
	/**
	 *��ֲֲ��
	 */
	virtual SkeletonAnimation* plantInit(const std::string& plantname, const std::string& animaionname);

	/**
	 *����ֲ��ͼƬ
	 */
	virtual void imageInit(const std::string& name, const Vec2& position);

	/**
	 *����ֲ������ֵ
	 */
	virtual void reducePlantHealthPoint(int number) { _healthPoint -= number; }

	/**
	 *�����ɽ�����
	 */
	virtual void setPlantSoilSplashAnimation(const float& scale);

	/**
	 *����ֲ��Ӱ��
	 */
	virtual void setPlantShadow(const float& scale);

	/**
	 *����ֲ���ܵ��˺���˸
	 */
	virtual void setPlantHurtBlink();
	virtual void setPlantHurtBlink(PlantsType type) const;

	/**
	 *��ȡ��ʬ�Ƿ���ֲ��ǰ��
	 */
	virtual bool getZombieIsTheFrontOfPlant(Zombies* zombie);

	/**
	 *��ȡ��ʬ�Ƿ���ֲ����ͬһ��
	 */
	virtual bool getZombieIsSameLineWithPlant(Zombies* zombie);

	/**
	 *��ȡ��ʬ�Ƿ�����ֲ��
	 */
	virtual bool getZombieIsEncounterPlant(Zombies* zombie);

	/**
	 *��ʬ��ֲ��
	 */
	virtual void zombieEatPlant(Zombies* zombie);

	/**
	 *��ʬ�ӳ�ֲ���лָ�
	 */
	virtual void zombieRecoveryMove(Zombies* zombie);

private:
	void setPlantGLProgram();

protected:
	int _sunNeed;                               // ��������
	int _costGold;                              // ���ѽ��
	int _costMasonry;                           // ����שʯ
	int _plantTag;                              // ֲ���ǩ
	int _zOrder;                                // ����˳��
	int _plantNumber;                           // �洢ֲ���ţ�Ψһ�� ��
	bool _isLoop;                               // �Ƿ�ѭ��
	bool _isCanDelete[2];                       // �Ƿ����ɾ��
	float _combatEffecttiveness;                // ս����
	float _coolDownTime;                        // ��ȴʱ��
	float _healthPoint;                         // ����ֵ
	float _totalHealthPoint;                    // ������ֵ
	Vec2 _position;                             // λ��
	Vec2 _rowAndColumn;                         // ��ϸ��ַ,��������
	SkeletonAnimation* _plantAnimation;         // ֲ�ﶯ��
	string _plantAnimationName;                 // ֲ�ﶯ������
	Node* _node;                                // �ڵ�
	Global* _global;                            // ȫ�ֱ���
	Sprite* _plantImage;                        // ֲ��ͼƬ
	Sprite* _plantShadow;                       // ֲ��Ӱ��
	PlantsType _plantsType;                     // ֲ������
	SkeletonAnimation* _soilSplashAnimation;    // �����ɽ�����
	static int plantNumber;                     // ֲ���ţ�Ψһ�ԣ�

private:
	static GLProgram* _normalGLProgram;
	static GLProgram* _highLightGLProgram;
	GLProgramState* _highLightGLProgramState;
	float _highLightIntensity;
};