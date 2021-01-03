#include "ItemRootWnd.h"

#include "Widgets/ClosableWnd/DraggableWnd/ItemRootWnd/RootItemSlot/RootItemSlot.h"

#include "Actors/Controllers/RPGPlayerController/RPGPlayerController.h"
#include "Actors/Characters/RPGCharacter/RPGCharacter.h"
#include "Actors/InteractableActor/WorldItem/WorldItem.h"

#include "Single/GameInstance/ARPGGameInstance.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Components/PlayerInventory/PlayerInventoryComponent.h"
#include "Components/GridPanel.h"
#include "Components/Button.h"
#include "Components/GridSlot.h"


UItemRootWnd::UItemRootWnd(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<URootItemSlot> BP_ROOTITEMSLOT(
		TEXT("WidgetBlueprint'/Game/Resources/Blueprints/Widgets/ClosableWnd/DraggableWidget/ItemRootWnd/BP_RootItemSlot.BP_RootItemSlot_C'"));
	if (BP_ROOTITEMSLOT.Succeeded()) RootItemSlotClass = BP_ROOTITEMSLOT.Class;
	else { UE_LOG(LogTemp, Error, TEXT("ItemRootWnd.cpp :: %d LINE :: BP_ROOTITEMSLOT is not loaded!"), __LINE__); }
}

void UItemRootWnd::NativeConstruct()
{
	Super::NativeConstruct();

	GridPanel_RootItemList = Cast<UGridPanel>(GetWidgetFromName(TEXT("GridPanel_RootItemList")));
	Button_Cancel = Cast<UButton>(GetWidgetFromName(TEXT("Button_Cancel")));
	Button_Ok = Cast<UButton>(GetWidgetFromName(TEXT("Button_Ok")));

	Button_Cancel->OnClicked.AddDynamic(this, &UClosableWnd::OnCloseButtonClicked);
	Button_Ok->OnClicked.AddDynamic(this, &UItemRootWnd::RootItem);

	UpdateWndSize(500.0f, 300.0f);
}

void UItemRootWnd::InitializeRootItems(TArray<FItemSlotInfo> items)
{
	Items = items;

	for (int32 i = 0; i < items.Num(); ++i)
	{
		URootItemSlot* rootItemSlot = CreateWidget<URootItemSlot>(this, RootItemSlotClass);
		GridPanel_RootItemList->AddChild(rootItemSlot);

		UGridSlot* gridSlot = Cast<UGridSlot>(rootItemSlot->Slot);
		gridSlot->SetColumn(i % 7);
		gridSlot->SetRow(i / 7);

		rootItemSlot->InitializeRootItem(items[i].ItemCode, items[i].ItemCount);
	}
}

void UItemRootWnd::RootItem()
{
	UPlayerInventoryComponent* playerInventory = Cast<ARPGCharacter>(GetManager(UPlayerManager)->
		GetPlayerController()->GetPawn())->GetPlayerInventory();

	// 제거할 아이템 인덱스를 저장할 배열
	TArray<FItemSlotInfo> destroyItemInfo;

	for (int32 i = 0; i < Items.Num(); ++i)
	{
		// 아이템 얻기에 성공했다면
		if (playerInventory->AddItem(Items[i].ItemCode, Items[i].ItemCount))

			// 제거할 아이템 인덱스를 저장합니다.
			destroyItemInfo.Add(Items[i]);

		// 아이템 얻기에 실패했다면 반복 종료
		else break;
	}

	// 제거할 아이템이 존재한다면
	if (destroyItemInfo.Num() != 0)
	{
		// 아이템 제거
		for (int32 i = 0; i < destroyItemInfo.Num(); ++i)
		{
			Items.Remove(destroyItemInfo[i]);
		}
	}

	OnCloseButtonClicked();

	// 얻을 수 있는 아이템이 존재하지 않는다면
	if (Items.Num() == 0)
		WorldItemInstance->Destroy();

}
