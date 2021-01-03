// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RootItemSlot.generated.h"

UCLASS()
class ARPG_API URootItemSlot : public UUserWidget
{
	GENERATED_BODY()

private :
	class UDataTable* DT_ItemInfo;

private :
	class UImage* Image_ItemSprite;
	class UTextBlock* Text_ItemCount;

public :
	URootItemSlot(const FObjectInitializer& ObjectInitializer);

protected :
	virtual void NativeConstruct() override;

public :
	void InitializeRootItem(FName itemCode, int32 itemCount);
	
};
