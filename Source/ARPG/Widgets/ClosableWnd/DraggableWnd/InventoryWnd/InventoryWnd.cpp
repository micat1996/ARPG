#include "InventoryWnd.h"
#include "Widgets/ClosableWnd/DraggableWnd/InventoryWnd/ItemSlot/ItemSlot.h"

#include "Components/GridPanel.h"
#include "Components/PlayerInventory/PlayerInventoryComponent.h"



#include "Single/GameInstance/ARPGGameInstance.h"
#include "Single/PlayerManager/PlayerManager.h"

UInventoryWnd::UInventoryWnd(const FObjectInitializer& ObjInitializer) :
	Super(ObjInitializer)
{

	static ConstructorHelpers::FClassFinder<UItemSlot> BP_ITEM_SLOT(
		TEXT("WidgetBlueprint'/Game/Resources/Blueprints/Widgets/ClosableWnd/DraggableWidget/InventoryWnd/BP_ItemSlot.BP_ItemSlot_C'"));
	if (BP_ITEM_SLOT.Succeeded()) BP_ItemSlotClass = BP_ITEM_SLOT.Class;


}

void UInventoryWnd::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UPlayerManager* playerManager = GetManager(UPlayerManager);
	PlayerInfo = playerManager->GetPlayerInfo();

	GridPanel_Content = Cast<UGridPanel>(GetWidgetFromName(TEXT("GridPanel_Content")));


}

void UInventoryWnd::NativeConstruct()
{
	Super::NativeConstruct();

	// â ũ�⸦ �����մϴ�.
	UpdateWndSize(500.0f, 600.0f);
}

void UInventoryWnd::CreateItemSlot()
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

	// ������ ������ �θ� ������ �ڽ����� �����մϴ�.
	newItemSlot->SetInventoryWnd(this);

	// �迭�� �߰��մϴ�.
	InventorySlots.Add(newItemSlot);

}

void UInventoryWnd::InitializeInventoryWnd(UPlayerInventoryComponent* playerInventory)
{
	PlayerInventory = playerInventory;

	for (int32 i = 0; i < PlayerInfo->InventorySlotCount; ++i)
		CreateItemSlot();

	UpdateInventorySlots();
}

void UInventoryWnd::UpdateInventorySlots()
{
	for (auto slot : InventorySlots)
		slot->UpdateItemSlot();
}
