#include "ShopWnd.h"

#include "Widgets/ClosableWnd/ShopWnd/SaleItem/SaleItem.h"
#include "Widgets/ClosableWnd/DraggableWnd/ShopWnd/ShopTradeWnd/ShopTradeWnd.h"

#include "Single/GameInstance/ARPGGameInstance.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Structures/ItemSlotInfo/ItemSlotInfo.h"

#include "Actors/Controllers/RPGPlayerController/RPGPlayerController.h"
#include "Actors/Characters/RPGCharacter/RPGCharacter.h"

#include "Components/ScrollBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PlayerInventory/PlayerInventoryComponent.h"


UShopWnd::UShopWnd(const FObjectInitializer& ObjInitializer) :
	Super(ObjInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ITEM_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_ItemInfo.DT_ItemInfo'"));
	if (DT_ITEM_INFO.Succeeded()) DT_ItemInfo = DT_ITEM_INFO.Object;
	else { UE_LOG(LogTemp, Error, TEXT("ShopWnd.cpp :: %d LINE :: DT_ITEM_INFO is not loaded!"), __LINE__); }

	static ConstructorHelpers::FClassFinder<USaleItem> BP_SALE_ITEM(
		TEXT("WidgetBlueprint'/Game/Resources/Blueprints/Widgets/ClosableWnd/DraggableWidget/ShopWnd/BP_SaleItem.BP_SaleItem_C'"));
	if (BP_SALE_ITEM.Succeeded()) SaleItemClass = BP_SALE_ITEM.Class;
	else { UE_LOG(LogTemp, Error, TEXT("ShopWnd.cpp :: %d LINE :: BP_SALE_ITEM is not loaded!"), __LINE__); }

	static ConstructorHelpers::FClassFinder<UShopTradeWnd> BP_SHOP_TRADE_WND(
		TEXT("WidgetBlueprint'/Game/Resources/Blueprints/Widgets/ClosableWnd/DraggableWidget/ShopWnd/BP_ShopTradeWnd.BP_ShopTradeWnd_C'"));
	if (BP_SHOP_TRADE_WND.Succeeded()) ShopTradeWndClass = BP_SHOP_TRADE_WND.Class;
	else { UE_LOG(LogTemp, Error, TEXT("ShopWnd.cpp :: %d LINE :: BP_SHOP_TRADE_WND is not loaded!"), __LINE__); }

}

void UShopWnd::NativeConstruct()
{
	Super::NativeConstruct();

	// 창 크기 설정
	UpdateWndSize(800.0f, 900.0f);

	ScrollBox_SaleList = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBox_SaleList")));
	ScrollBox_InventoryItem = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBox_InventoryItem")));

	// 소지 아이템 초기화
	InitializeInventoryItem();
}

void UShopWnd::AddItem(EShopItemType shopItemType, FName itemCode, int32 costs, int32 itemCount)
{
	// 판매 아이템 위젯을 생성합니다.
	USaleItem* saleItem = CreateWidget<USaleItem>(this, SaleItemClass);

	// 추가할 ScrollBox 위젯을 설정합니다.
	UScrollBox* scrollBox = (shopItemType == EShopItemType::SI_InventoryItem) ? 
		ScrollBox_InventoryItem : ScrollBox_SaleList;

	// ScrollBox 에 추가합니다.
	scrollBox->AddChild(saleItem);

	// Shop Wnd 설정
	saleItem->SetShopWnd(this);

	// itemCode 와 일치하는 아이템의 정보를 저장합니다.
	FString contextString;
	FItemInfo* itemInfo = DT_ItemInfo->FindRow<FItemInfo>(itemCode, contextString);

	// GameInstance
	UARPGGameInstance* gameInst = Cast<UARPGGameInstance>(GetGameInstance());

	// 아이템 이미지를 로드합니다.
	UTexture2D* itemSprite = Cast<UTexture2D>(gameInst->GetStreamableManager()->LoadSynchronous(itemInfo->ItemSpritePath));

	// 판매 아이템 위젯을 초기화합니다.
	saleItem->InitializeSaleItem(shopItemType, *itemInfo, itemSprite, itemCount, costs);
}

void UShopWnd::InitializeInventoryItem()
{
	UPlayerInventoryComponent* playerInventory = Cast<ARPGCharacter>(GetManager(UPlayerManager)->
		GetPlayerController()->GetPawn())->GetPlayerInventory();

	for (auto inventoryItem : playerInventory->GetInventoryItems())
	{
		if (!inventoryItem.IsEmpty())
		{ 
			FString contextString;
			FItemInfo* itemInfo = DT_ItemInfo->FindRow<FItemInfo>(inventoryItem.ItemCode, contextString);

			AddItem(
				/*shopItemType	= */ EShopItemType::SI_InventoryItem,
				/*itemCode		= */ itemInfo->ItemCode,
				/*costs			= */ itemInfo->SalePrice,
				/*itemCount		= */ inventoryItem.ItemCount);
		}
	}
}

void UShopWnd::InitializeSaleList(TArray<FShopItemInfo> saleItems)
{
	for (auto saleIteminfo : saleItems)
	{
		AddItem(
			/*shopItemType	= */ EShopItemType::SI_SaleItem, 
			/*itemCode		= */ saleIteminfo.ItemCode, 
			/*costs			= */ saleIteminfo.Costs);
	}
}

void UShopWnd::CreateTradeWnd(EShopItemType shopItemType, FItemInfo* itemInfo, int32 costs)
{
	// 물건 교환 창이 열려있을 경우 실행하지 않습니다.
	if (bIsTradeWndActivated) return;

	bIsTradeWndActivated = true;

	UShopTradeWnd* shopTradeWnd = Cast<UShopTradeWnd>(CreateChildClosableWnd(ShopTradeWndClass));
	shopTradeWnd->onWndClosed.AddLambda([this]() { bIsTradeWndActivated = false; });
	
	Cast<UCanvasPanelSlot>(shopTradeWnd->Slot)->SetPosition(
		Cast<UCanvasPanelSlot>(Slot)->GetPosition() +
		(Cast<UCanvasPanelSlot>(Slot)->GetSize() * 0.5f) - 
		(Cast<UCanvasPanelSlot>(shopTradeWnd->Slot)->GetSize() * 0.5f));

	shopTradeWnd->InitializeTradeWnd(shopItemType, itemInfo, costs);
}
