// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARPG.h"
#include "Widgets/ClosableWnd/ClosableWnd.h"
#include "ItemDetailWnd.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UItemDetailWnd : public UClosableWnd
{
	GENERATED_BODY()

private:
	class UTextBlock* Text_ItemName;
	class UTextBlock* Text_ItemDescription;
	class UTextBlock* Text_ItemType;
	class UImage* Image_ItemSprite;

protected :
	virtual void NativeConstruct() override;

public :
	// ������ ������ â�� �����մϴ�.
	void UpdateDetailWnd(class UDataTable* dt_ItemInfo, FName itemCode);
	
};
