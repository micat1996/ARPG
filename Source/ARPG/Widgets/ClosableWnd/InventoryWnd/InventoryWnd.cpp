#include "InventoryWnd.h"
#include "Widgets/ClosableWnd/InventoryWnd/ItemSlot/ItemSlot.h"

#include "Components/GridPanel.h"
#include "Components/PlayerInventory/PlayerInventoryComponent.h"

#include "Single/GameInstance/ARPGGameInstance.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Structures/PlayerInfo/PlayerInfo.h"

UInventoryWnd::UInventoryWnd(const FObjectInitializer& ObjInitializer) :
	Super(ObjInitializer)
{

	static ConstructorHelpers::FClassFinder<UItemSlot> BP_ITEM_SLOT(
		TEXT("WidgetBlueprint'/Game/Resources/Blueprints/Widgets/ClosableWnd/DraggableWidget/InventoryWnd/BP_ItemSlot.BP_ItemSlot_C'"));
	if (BP_ITEM_SLOT.Succeeded()) BP_ItemSlotClass = BP_ITEM_SLOT.Class;


}

void UInventoryWnd::NativeConstruct()
{
	Super::NativeConstruct();

	UPlayerManager* playerManager = GetManager(UPlayerManager);
	PlayerInfo = playerManager->GetPlayerInfo();

	UpdateWndSize(448.0f, 600.0f);

	GridPanel_Content = Cast<UGridPanel>(GetWidgetFromName(TEXT("GridPanel_Content")));
}

void UInventoryWnd::InitializeInventoryWnd(class UPlayerInventoryComponent* playerInventory)
{
	PlayerInventory = playerInventory;

	// 슬롯 생성
	for (int32 i = 0; i < PlayerInfo->InventorySlotCount; ++i)
		CreateSlot();

	// 슬롯 갱신
	UpdateInventorySlots();

	auto hDelegate = PlayerInventory->OnInventorySlotChanged.AddUObject(
		this, &UInventoryWnd::UpdateInventorySlots);
	onWndClosed.AddLambda([=]() { PlayerInventory->OnInventorySlotChanged.Remove(hDelegate); });
}

void UInventoryWnd::UpdateInventorySlots()
{
	for (auto slot : InventorySlots)
		slot->UpdateItemSlot();
}

void UInventoryWnd::CreateSlot()
{
	// 슬롯 위젯을 생성합니다.
	UItemSlot* newItemSlot = CreateWidget<UItemSlot>(this, BP_ItemSlotClass);

	// 생성한 슬롯 위젯을 GridPanel 의 자식 위젯으로 추가합니다.
	GridPanel_Content->AddChild(newItemSlot);

	// 생성한 슬롯 위젯의 인덱스를 설정합니다.
	newItemSlot->SetInventorySlotIndex(InventorySlots.Num());

	// 슬롯의 위치를 설정합니다.
	int32 column = newItemSlot->GetInventorySlotIndex() % 7;
	int32 row = newItemSlot->GetInventorySlotIndex() / 7;

	newItemSlot->GetGridSlot()->SetColumn(column);
	newItemSlot->GetGridSlot()->SetRow(row);

	// 생성된 슬롯의 부모 위젯을 설정합니다.
	// 생성된 슬롯의 부모 위젯을 자신으로 설정합니다.
	newItemSlot->SetInventoryWnd(this);

	// 배열에 추가합니다.
	InventorySlots.Add(newItemSlot);

}
