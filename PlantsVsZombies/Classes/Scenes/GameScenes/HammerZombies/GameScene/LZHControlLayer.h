/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.09
 *Email: 2117610943@qq.com
 */

#pragma once
#include "spine/spine.h"
#include "spine/spine-cocos2dx.h"

#include "Scenes/GameScenes/Adventure/GameScene/LZAGSControlLayer.h"

using namespace spine;
using namespace cocos2d;

class Zombies;
class HControlLayer :public GSControlLayer
{
public:
	CREATE_FUNC(HControlLayer);
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }
	void updateHammerInformation();

CC_CONSTRUCTOR_ACCESS:
	HControlLayer();
	~HControlLayer();
	virtual bool init();

protected:
	virtual void initData() override;
	virtual void createSchedule() override;
	virtual void createMouseListener() override;
	virtual void createZombies() override;
	virtual bool controlRefurbishZombies() override;
	virtual void controlRefurbishMusicAndText() override;
	virtual void judgeLevelIsFinished() override;

private:
	void createHammerAnimation();
	void judgeHammerZombies(const Vec2& postion);
    void changeCursor();
	void showSelectedButtonHoverEffect();
	void attackZombies(Zombies* zombie);
	void judgeZombiesWin();
	void showPowImage(Zombies* zombie, const Vec2& position);
	void calculateZombiesData();
	void showHammerButton();
	void hideHammerButton();
	
protected:
	SkeletonAnimation* _hammer;
	unsigned int _attack;
	unsigned int _frequencyZombiesNumbers;  // ÿһ����ʬ����
	unsigned int _maxFrequencyNumbers;      // �����
	unsigned int _currentLevelNumber;       // ��ǰ�ؿ���
	unsigned int _zombiesTypeNumbers;       // ��ʬ������
	float _currentLevelZombiesSpeed;        // ��ǰ��ʬ�˶��ٶ�
	bool _isShowHammerButton;
};