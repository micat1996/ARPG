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

	// ���� ����
	for (int32 i = 0; i < PlayerInfo->InventorySlotCount; ++i)
		CreateSlot();

	// ���� ����
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
	// ���� ������ �����մϴ�.
	UItemSlot* newItemSlot = CreateWidget<UItemSlot>(this, BP_ItemSlotClass);

	// ������ ���� ������ GridPanel �� �ڽ� �������� �߰��մϴ�.
	GridPanel_Content->AddChild(newItemSlot);

	// ������ ���� ������ �ε����� �����մϴ�.
	newItemSlot->SetInventorySlotIndex(InventorySlots.Num());

	// ������ ��ġ�� �����մϴ�.
	int32 column = newItemSlot->GetInventorySlotIndex() % 7;
	int32 row = newItemSlot->GetInventorySlotIndex() / 7;

	newItemSlot->GetGridSlot()->SetColumn(column);
	newItemSlot->GetGridSlot()->SetRow(row);

	// ������ ������ �θ� ������ �����մϴ�.
	// ������ ������ �θ� ������ �ڽ����� �����մϴ�.
	newItemSlot->SetInventoryWnd(this);

	// �迭�� �߰��մϴ�.
	InventorySlots.Add(newItemSlot);

}
