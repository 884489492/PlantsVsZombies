/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.2.10
 *Email: 2117610943@qq.com
 */
#pragma once
#include "Based/LZBDialog.h"

class UnlockDialogLayer :public Dialog
{
public:
	CREATE_FUNC(UnlockDialogLayer);
	virtual bool init() override;

CC_CONSTRUCTOR_ACCESS:
	UnlockDialogLayer(): _lockDialog(nullptr){}

private:
	void createDiglog();	/* �����Ի��� */
	void createButtons();   /* ������ť */
	void deleteDialog();    /* ɾ���Ի��� */
	void createText();

public:
	static unsigned int unlockNeedNumbers;

private:
	Sprite* _lockDialog;    /* �Ի��� */
};

