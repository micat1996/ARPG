#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Enums/ShopItemType.h"
#include "Structures/ItemInfo/ItemInfo.h"

#include "SaleItem.generated.h"


UCLASS()
class ARPG_API USaleItem : public UUserWidget
{
	GENERATED_BODY()


private :
	class UShopWnd* ShopWnd;
	EShopItemType ShopItemType;
	FItemInfo ItemInfo;
	int32 ItemPrice;
	int32 InventoryItemIndex;

	int32 InventoryItemCount;


	class UImage* Image_ItemSprite;
	class UTextBlock* Text_ItemCount;

	class UTextBlock* Text_ItemName;
	class UTextBlock* Text_Costs;

	class UTextBlock* Text_SaleOrBuy;
	class UButton* Button_SaleOrBuy;


protected :
	virtual void NativeConstruct() override;

public :
	// 상점 아이템을 초기화합니다.
	void InitializeSaleItem(EShopItemType shopItemType, FItemInfo itemInfo, class UTexture2D * itemImage,
		int32 itemCount, FText itemName, int32 costs, int32 inventoryItemIndex);

private :
	UFUNCTION()
	void OnSaleOrBuyButtonClicked();

public :
	FORCEINLINE void SetShopWnd(class UShopWnd* shopWnd)
	{ ShopWnd = shopWnd; }

	FORCEINLINE int32 GetInventoryItemCount() const
	{ return InventoryItemCount; }
	

	

	
};
