// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARPG.h"

#include "Widgets/ClosableWnd/DraggableWnd/DraggableWnd.h"

#include "Structures/ShopItemInfo/ShopItemInfo.h"

#include "Enums/ShopItemType.h"

#include "ShopWnd.generated.h"

UCLASS()
class ARPG_API UShopWnd : public UDraggableWnd
{
	GENERATED_BODY()

private :
	class UDataTable* DT_ItemInfo;
	TSubclassOf<class USaleItem> SaleItemClass;

private :
	class UScrollBox* ScrollBox_SaleList;
	class UScrollBox* ScrollBox_InventoryItem;


public :
	UShopWnd(const FObjectInitializer& ObjInitializer);

protected :
	virtual void NativeConstruct() override;

private :
	void AddItem(EShopItemType shopItemType, FName itemCode, int32 costs, int32 itemCount = 1);

private :
	// ���� ������ ����� �ʱ�ȭ �մϴ�.
	void InitializeInventoryItem();

public :
	// �������� �Ĵ� ���� ����� �ʱ�ȭ�մϴ�.
	void InitializeSaleList(TArray<FShopItemInfo> saleItems);
	
};
