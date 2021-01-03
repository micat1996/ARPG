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

	// â ũ�� ����
	UpdateWndSize(800.0f, 900.0f);

	ScrollBox_SaleList = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBox_SaleList")));
	ScrollBox_InventoryItem = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBox_InventoryItem")));


	UPlayerInventoryComponent* playerInventory = Cast<ARPGCharacter>(GetManager(UPlayerManager)->
		GetPlayerController()->GetPawn())->GetPlayerInventory();

	auto hDelegate = playerInventory->OnInventorySlotChanged.AddUObject(
		this, &UShopWnd::OnInventorySlotChanged);
	onWndClosed.AddLambda([=]() { playerInventory->OnInventorySlotChanged.Remove(hDelegate); });

	// ���� ������ �ʱ�ȭ
	InitializeInventoryItem();
}

void UShopWnd::AddItem(EShopItemType shopItemType, FName itemCode, 
	int32 costs, int32 itemCount, int32 inventorySlotIndex)
{
	// �Ǹ� ������ ������ �����մϴ�.
	USaleItem* saleItem = CreateWidget<USaleItem>(this, SaleItemClass);

	// �߰��� ScrollBox ������ �����մϴ�.
	UScrollBox* scrollBox = (shopItemType == EShopItemType::SI_InventoryItem) ? 
		ScrollBox_InventoryItem : ScrollBox_SaleList;

	// ScrollBox �� �߰��մϴ�.
	scrollBox->AddChild(saleItem);

	if (shopItemType == EShopItemType::SI_InventoryItem)
		InventoryItemSlots.Add(saleItem);

	// Shop Wnd ����
	saleItem->SetShopWnd(this);

	// itemCode �� ��ġ�ϴ� �������� ������ �����մϴ�.
	FString contextString;
	FItemInfo* itemInfo = DT_ItemInfo->FindRow<FItemInfo>(itemCode, contextString);

	// GameInstance
	UARPGGameInstance* gameInst = Cast<UARPGGameInstance>(GetGameInstance());

	// ������ �̹����� �ε��մϴ�.
	UTexture2D* itemSprite = Cast<UTexture2D>(gameInst->GetStreamableManager()->LoadSynchronous(itemInfo->ItemSpritePath));

	// �Ǹ� ������ ������ �ʱ�ȭ�մϴ�.
	saleItem->InitializeSaleItem(shopItemType, *itemInfo, itemSprite, itemCount, itemInfo->ItemName, costs, inventorySlotIndex);
}

void UShopWnd::InitializeInventoryItem()
{
	UPlayerInventoryComponent* playerInventory = Cast<ARPGCharacter>(GetManager(UPlayerManager)->
		GetPlayerController()->GetPawn())->GetPlayerInventory();

	for (int32 i = 0; i < playerInventory->GetInventoryItems().Num(); ++i)
	{
		// �κ��丮 ������ ������ ������ ����
		FItemSlotInfo inventoryItem = playerInventory->GetInventoryItems()[i];

		if (!inventoryItem.IsEmpty())
		{ 
			FString contextString;
			FItemInfo* itemInfo = DT_ItemInfo->FindRow<FItemInfo>(inventoryItem.ItemCode, contextString);

			AddItem(
				/*shopItemType			= */ EShopItemType::SI_InventoryItem,
				/*itemCode				= */ itemInfo->ItemCode,
				/*costs					= */ itemInfo->SalePrice,
				/*itemCount				= */ inventoryItem.ItemCount,
				/*inventorySlotIndex	= */ i);
		}
	}
}

void UShopWnd::OnInventorySlotChanged()
{
	for (auto inventoryItemSlot : InventoryItemSlots)
	{
		ScrollBox_InventoryItem->RemoveChild(inventoryItemSlot);
	}

	InventoryItemSlots.Empty();

	InitializeInventoryItem();
}

void UShopWnd::InitializeSaleList(TArray<FShopItemInfo> saleItems)
{
	for (auto saleIteminfo : saleItems)
	{
		AddItem(
			/*shopItemType	= */ EShopItemType::SI_SaleItem, 
			/*itemCode		= */ saleIteminfo.ItemCode, 
			/*costs			= */ saleIteminfo.Costs, 
			/*itemCount		= */ 1);
	}
}

UShopTradeWnd* UShopWnd::CreateTradeWnd(USaleItem* saleItemWidget, 
	EShopItemType shopItemType, FItemInfo* itemInfo, int32 costs)
{
	// ���� ��ȯ â�� �������� ��� �������� �ʽ��ϴ�.
	if (bIsTradeWndActivated) return nullptr;

	bIsTradeWndActivated = true;

	UShopTradeWnd* shopTradeWnd = Cast<UShopTradeWnd>(CreateChildClosableWnd(ShopTradeWndClass));
	shopTradeWnd->onWndClosed.AddLambda([this]() { bIsTradeWndActivated = false; });


	Cast<UCanvasPanelSlot>(shopTradeWnd->Slot)->SetPosition(
		Cast<UCanvasPanelSlot>(Slot)->GetPosition() +
		(Cast<UCanvasPanelSlot>(Slot)->GetSize() * 0.5f) -
		(Cast<UCanvasPanelSlot>(shopTradeWnd->Slot)->GetSize() * 0.5f));

	if (shopItemType == EShopItemType::SI_SaleItem)
		shopTradeWnd->InitializeTradeWnd(shopItemType, itemInfo, costs);
	else
		shopTradeWnd->InitializeTradeWnd(shopItemType, itemInfo, costs, 
			saleItemWidget->GetInventoryItemCount());

	return shopTradeWnd;
}
