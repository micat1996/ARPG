// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARPG.h"
#include "Widgets/ClosableWnd/DraggableWnd/DraggableWnd.h"
#include "Enums/ShopItemType.h"
#include "ShopWnd.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UShopWnd : public UDraggableWnd
{
	GENERATED_BODY()
	
private :
	class UDataTable * DT_ShopInfo;

	TSubclassOf<class USaleItem> SaleItemClass;

private :
	class UScrollBox* ScrollBox_SaleList;


protected :
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ShopID;

public :
	UShopWnd(const FObjectInitializer& ObjectInitializer);

protected :
	virtual void NativeConstruct() override;

private :
	void InitializeSaleList();

	void AddItem(EShopItemType shopItemType, FName itemCode, int32 itemCount = 1);


};
