#include "SaleItem.h"

#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USaleItem::NativeConstruct()
{
	Super::NativeConstruct();

	Image_ItemSprite = Cast<UImage>(GetWidgetFromName(TEXT("Image_ItemSprite")));
	Text_ItemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ItemCount")));
	Text_ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ItemName")));
	Text_Costs = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Costs")));
	Text_SaleOrBuy = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_SaleOrBuy")));
}

void USaleItem::InitializeSaleItem(EShopItemType shopItemType, 
	UTexture2D *itemImage, int32 itemCount, FText itemName, int32 costs)
{
	// 아이템 이미지 설정
	Image_ItemSprite->SetBrushFromTexture(itemImage);

	// 아이템 개수 설정
	Text_ItemCount->SetText(FText::FromString(
		(itemCount == 1 ? TEXT("") : FString::FromInt(itemCount))
		));

	// 아이템 이름 설정
	Text_ItemName->SetText(itemName);

	// 아이템 가격 설정
	Text_Costs->SetText(FText::FromString(FString::FromInt(costs)));

	// 버튼 텍스트 설정
	Text_SaleOrBuy->SetText(FText::FromString(
		(shopItemType == EShopItemType::SI_InventoryItem ? TEXT("판매") : TEXT("구매"))
		));
}
