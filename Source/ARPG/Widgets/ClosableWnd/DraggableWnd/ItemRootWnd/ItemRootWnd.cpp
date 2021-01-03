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

	// ������ ������ �ε����� ������ �迭
	TArray<FItemSlotInfo> destroyItemInfo;

	for (int32 i = 0; i < Items.Num(); ++i)
	{
		// ������ ��⿡ �����ߴٸ�
		if (playerInventory->AddItem(Items[i].ItemCode, Items[i].ItemCount))

			// ������ ������ �ε����� �����մϴ�.
			destroyItemInfo.Add(Items[i]);

		// ������ ��⿡ �����ߴٸ� �ݺ� ����
		else break;
	}

	// ������ �������� �����Ѵٸ�
	if (destroyItemInfo.Num() != 0)
	{
		// ������ ����
		for (int32 i = 0; i < destroyItemInfo.Num(); ++i)
		{
			Items.Remove(destroyItemInfo[i]);
		}
	}

	OnCloseButtonClicked();

	// ���� �� �ִ� �������� �������� �ʴ´ٸ�
	if (Items.Num() == 0)
		WorldItemInstance->Destroy();

}
