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
	FItemInfo ItemInfo;
	EShopItemType ShopItemType;
	int32 ItemPrice;

	class UImage* Image_ItemSprite;
	class UTextBlock* Text_ItemCount;

	class UTextBlock* Text_ItemName;
	class UTextBlock* Text_Costs;

	class UButton* Button_SaleOrBuy;
	class UTextBlock* Text_SaleOrBuy;

protected :
	virtual void NativeConstruct() override;

public :
	// 상점 아이템을 초기화합니다.
	void InitializeSaleItem(EShopItemType shopItemType, FItemInfo itemInfo, class UTexture2D * itemImage,
		int32 itemCount, int32 costs);

public :
	FORCEINLINE void SetShopWnd(class UShopWnd* shopWnd)
	{ ShopWnd = shopWnd; }

private :
	UFUNCTION()
	void OnSaleOrBuyButtonClicked();

	
};
