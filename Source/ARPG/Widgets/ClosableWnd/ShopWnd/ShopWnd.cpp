#include "ShopWnd.h"

#include "Widgets/ClosableWnd/ShopWnd/SaleItem/SaleItem.h"

#include "Single/GameInstance/ARPGGameInstance.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Structures/ItemInfo/ItemInfo.h"
#include "Structures/ItemSlotInfo/ItemSlotInfo.h"

#include "Actors/Controllers/RPGPlayerController/RPGPlayerController.h"
#include "Actors/Characters/RPGCharacter/RPGCharacter.h"

#include "Components/ScrollBox.h"
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
}

void UShopWnd::NativeConstruct()
{
	Super::NativeConstruct();

	// â ũ�� ����
	UpdateWndSize(800.0f, 900.0f);

	ScrollBox_SaleList = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBox_SaleList")));
	ScrollBox_InventoryItem = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBox_InventoryItem")));

	// ���� ������ �ʱ�ȭ
	InitializeInventoryItem();
}

void UShopWnd::AddItem(EShopItemType shopItemType, FName itemCode, int32 costs, int32 itemCount)
{
	// �Ǹ� ������ ������ �����մϴ�.
	USaleItem* saleItem = CreateWidget<USaleItem>(this, SaleItemClass);

	// �߰��� ScrollBox ������ �����մϴ�.
	UScrollBox* scrollBox = (shopItemType == EShopItemType::SI_InventoryItem) ? 
		ScrollBox_InventoryItem : ScrollBox_SaleList;

	// ScrollBox �� �߰��մϴ�.
	scrollBox->AddChild(saleItem);

	// itemCode �� ��ġ�ϴ� �������� ������ �����մϴ�.
	FString contextString;
	FItemInfo* itemInfo = DT_ItemInfo->FindRow<FItemInfo>(itemCode, contextString);

	// GameInstance
	UARPGGameInstance* gameInst = Cast<UARPGGameInstance>(GetGameInstance());

	// ������ �̹����� �ε��մϴ�.
	UTexture2D* itemSprite = Cast<UTexture2D>(gameInst->GetStreamableManager()->LoadSynchronous(itemInfo->ItemSpritePath));

	// �Ǹ� ������ ������ �ʱ�ȭ�մϴ�.
	saleItem->InitializeSaleItem(shopItemType, itemSprite, itemCount, itemInfo->ItemName, costs);
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
			/*costs			= */ saleIteminfo.Costs, 
			/*itemCount		= */ 1);
	}
}
