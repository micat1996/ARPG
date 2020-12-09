#include "InventoryWnd.h"
#include "Widgets/ClosableWnd/InventoryWnd/ItemSlot/ItemSlot.h"

#include "Components/GridPanel.h"

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

void UInventoryWnd::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UPlayerManager* playerManager = GetManager(UPlayerManager);
	PlayerInfo = playerManager->GetPlayerInfo();

	GridPanel_Content = Cast<UGridPanel>(GetWidgetFromName(TEXT("GridPanel_Content")));

	InitializeInventoryWnd();
}

void UInventoryWnd::InitializeInventoryWnd()
{
	for (int32 i = 0; i < PlayerInfo->InventorySlotCount; ++i)
	{ 
		CreateSlot();
	}
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

	// 생성된 슬롯의 부모 위젯을 자신으로 설정합니다.
	newItemSlot->SetInventoryWnd(this);

	// 배열에 추가합니다.
	InventorySlots.Add(newItemSlot);

}
