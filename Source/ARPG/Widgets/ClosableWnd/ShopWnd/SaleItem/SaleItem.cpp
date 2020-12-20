#include "SaleItem.h"

#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Widgets/ClosableWnd/ShopWnd/ShopWnd.h"

void USaleItem::NativeConstruct()
{
	Super::NativeConstruct();

	Image_ItemSprite = Cast<UImage>(GetWidgetFromName(TEXT("Image_ItemSprite")));
	Text_ItemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ItemCount")));
	Text_ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ItemName")));
	Text_Costs = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Costs")));
	Button_SaleOrBuy = Cast<UButton>(GetWidgetFromName(TEXT("Button_SaleOrBuy")));
	Text_SaleOrBuy = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_SaleOrBuy")));

	Button_SaleOrBuy->OnClicked.AddDynamic(this, &USaleItem::OnSaleOrBuyButtonClicked);
}

void USaleItem::InitializeSaleItem(EShopItemType shopItemType, FItemInfo itemInfo,
	UTexture2D *itemImage, int32 itemCount, int32 costs)
{
	// 아이템 정보 설정
	ItemInfo = itemInfo;

	// 상점 아이템 타입 설정
	ShopItemType = shopItemType;

	// 상점 아이템 가격 설정
	ItemCosts = costs;

	// 아이템 이미지 설정
	Image_ItemSprite->SetBrushFromTexture(itemImage);

	// 아이템 개수 설정
	Text_ItemCount->SetText(FText::FromString(
		(itemCount == 1 ? TEXT("") : FString::FromInt(itemCount))
		));

	// 아이템 이름 설정
	Text_ItemName->SetText(ItemInfo.ItemName);

	// 아이템 가격 설정
	Text_Costs->SetText(FText::FromString(FString::FromInt(costs)));

	// 버튼 텍스트 설정
	Text_SaleOrBuy->SetText(FText::FromString(
		(ShopItemType == EShopItemType::SI_InventoryItem ? TEXT("판매") : TEXT("구매"))
		));
}

void USaleItem::OnSaleOrBuyButtonClicked()
{

	// 물건 교환 창이 열려있지 않다면
	if (!ShopWnd->IsTradeWndActivated())
	{
		ShopWnd->CreateTradeWnd(ShopItemType , ItemInfo, ItemCosts);
	}

}
