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
	UnlockDialogLayer();

protected:
	virtual void createDiglog();	/* �����Ի��� */
	virtual void createButtons();   /* ������ť */
	virtual void deleteDialog();    /* ɾ���Ի��� */
	virtual void createText();

public:
	static unsigned int unlockNeedNumbers;

protected:
	Sprite* _lockDialog;    /* �Ի��� */
};