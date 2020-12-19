#pragma once

#include "ARPG.h"
#include "Blueprint/UserWidget.h"

#include "Enums/ShopItemType.h"

#include "Structures/ItemInfo/ItemInfo.h"

#include "SaleItem.generated.h"

UCLASS()
class ARPG_API USaleItem : public UUserWidget
{
	GENERATED_BODY()

private :
	UDataTable* DT_ItemInfo;
	FItemInfo* ItemInfo;

private :
	class UImage* Image_ItemSprite;
	class UTextBlock* Text_ItemCount;

	class UTextBlock* Text_ItemName;
	class UTextBlock* Text_Costs;
	
	class UTextBock* Text_SaleAndBuy;


public :
	USaleItem(const FObjectInitializer& ObjectInitializer);

protected :
	virtual void NativeConstruct() override;

public :
	void InitializeSaleItem(EShopItemType shopItemType, FName ItemCode, int32 itemCount);
	


	
};
