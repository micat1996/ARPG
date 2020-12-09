#include "ItemSlot.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Components/PlayerInventory/PlayerInventoryComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "Widgets/ClosableWnd/InventoryWnd/InventoryWnd.h"
#include "Widgets/ClosableWnd/InventoryWnd/ItemSlot/ItemSlotDragDropOperation.h"

#include "Single/GameInstance/ARPGGameInstance.h"

#include "Structures/ItemInfo/ItemInfo.h"



UItemSlot::UItemSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> BP_ITEM_SLOT(
		TEXT("WidgetBlueprint'/Game/Resources/Blueprints/Widgets/ClosableWnd/DraggableWidget/InventoryWnd/BP_DragItemImage.BP_DragItemImage_C'"));
	if (BP_ITEM_SLOT.Succeeded()) ItemSlotWidgetClass = BP_ITEM_SLOT.Class;
	else { UE_LOG(LogTemp, Error, TEXT("ItemSlot.cpp :: %d LINE :: BP_ITEM_SLOT is not loaded"), __LINE__); }

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ITEM_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_ItemInfo.DT_ItemInfo'"));
	if (DT_ITEM_INFO.Succeeded()) DT_ItemInfo = DT_ITEM_INFO.Object;
	else { UE_LOG(LogTemp, Error, TEXT("ItemSlot.cpp :: %d LINE :: DT_ITEM_INFO is not loaded"), __LINE__); }
}


void UItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Image_ItemSprite = Cast<UImage>(GetWidgetFromName(TEXT("Image_ItemSprite")));
	Text_ItemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ItemCount")));
}

FReply UItemSlot::NativeOnMouseButtonDown(
	const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);


	// �巡�� �� ��� �۾��� �����ϸ�, �۾� ����� ��ȯ�մϴ�.
	return UWidgetBlueprintLibrary::DetectDragIfPressed(
		InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

void UItemSlot::NativeOnDragDetected(
	const FGeometry& InGeometry, 
	const FPointerEvent& InMouseEvent, 
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	// �巡�� �� ��� �۾� ��ü�� �����մϴ�.
	UItemSlotDragDropOperation* dragDropOp = Cast<UItemSlotDragDropOperation>(
		UWidgetBlueprintLibrary::CreateDragDropOperation(UItemSlotDragDropOperation::StaticClass()));

	if (IsValid(dragDropOp))
	{
		// �ǹ��� �����մϴ�.
		dragDropOp->Pivot = EDragPivot::CenterCenter;

		// �巡�� �� ����� ������ ������ �����մϴ�.
		dragDropOp->DefaultDragVisual = CreateWidget<UUserWidget>(this, ItemSlotWidgetClass);

		// �巡���� ������ ������ �����մϴ�.
		dragDropOp->DraggingSlot = this;
	}

	// ��¿� �Ű� ������ �巡�� �� ��� �۾� ��ü�� �����մϴ�.
	OutOperation = dragDropOp;
}

void UItemSlot::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
}

void UItemSlot::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
}

bool UItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool retVal = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UItemSlotDragDropOperation* dragDropOp = Cast<UItemSlotDragDropOperation>(InOperation);
	UPlayerInventoryComponent* playerInventory = InventoryWnd->GetPlayerInventoryComponent();

	playerInventory->SwapItem(this, dragDropOp->DraggingSlot);

	return retVal;
}

void UItemSlot::UpdateItemSlot()
{
	FItemSlotInfo itemSlotInfo = GetItemSlotInfo();

	if (itemSlotInfo.IsEmpty())
	{
		Image_ItemSprite->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));
		Text_ItemCount->SetText(FText::FromString(TEXT("")));
	}
	else
	{
		FString contextString;
		FItemInfo * itemInfo = DT_ItemInfo->FindRow<FItemInfo>(
			itemSlotInfo.ItemCode, contextString);

		UARPGGameInstance* gameInst = Cast<UARPGGameInstance>(GetGameInstance());
		UTexture2D * itemImage = Cast<UTexture2D>(gameInst->GetStreamableManager()->
			LoadSynchronous(itemInfo->ItemSpritePath));

		Image_ItemSprite->SetBrushFromTexture(itemImage);
		Image_ItemSprite->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));

		Text_ItemCount->SetText(FText::FromString(
			FString::FromInt(itemSlotInfo.ItemCount) ));
	}
}

FItemSlotInfo UItemSlot::GetItemSlotInfo()
{
	FItemSlotInfo slotInfo = InventoryWnd->GetPlayerInventoryComponent()->
		GetInventoryItems()[InventorySlotIndex];

	return slotInfo;
}

void UItemSlot::SetItemSlotInfo(FItemSlotInfo itemInfo)
{
	InventoryWnd->GetPlayerInventoryComponent()->
		SetItemSlotInfo(InventorySlotIndex, itemInfo);
}
