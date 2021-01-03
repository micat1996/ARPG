// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARPG.h"
#include "Widgets/ClosableWnd/DraggableWnd/DraggableWnd.h"
#include "Structures/ItemSlotInfo/ItemSlotInfo.h"
#include "ItemRootWnd.generated.h"

UCLASS()
class ARPG_API UItemRootWnd : public UDraggableWnd
{
	GENERATED_BODY()

private :
	class TSubclassOf<class URootItemSlot> RootItemSlotClass;

private :
	TArray<FItemSlotInfo> Items;
	class AWorldItem* WorldItemInstance;

private :
	class UGridPanel* GridPanel_RootItemList;
	class UButton* Button_Cancel;
	class UButton* Button_Ok;

public :
	UItemRootWnd(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public :
	// 얻을 수 있는 아이템 정보를 초기화합니다.
	void InitializeRootItems(TArray<FItemSlotInfo> items);

	FORCEINLINE void SetWorldItem(class AWorldItem* worldItemInstance)
	{ WorldItemInstance = worldItemInstance; }

private :
	UFUNCTION()
	void RootItem();

};
