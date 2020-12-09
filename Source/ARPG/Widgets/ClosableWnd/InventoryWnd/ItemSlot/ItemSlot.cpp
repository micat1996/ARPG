#include "ItemSlot.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Widgets/ClosableWnd/InventoryWnd/InventoryWnd.h"
#include "Widgets/ClosableWnd/InventoryWnd/ItemSlot/ItemSlotDragDropOperation.h"



UItemSlot::UItemSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (!IsValid(ItemSlotWidgetClass))
	{
		static ConstructorHelpers::FClassFinder<UUserWidget> BP_ITEM_SLOT(
			TEXT("WidgetBlueprint'/Game/Resources/Blueprints/Widgets/ClosableWnd/DraggableWidget/InventoryWnd/BP_DragItemImage.BP_DragItemImage_C'"));
	
		if (BP_ITEM_SLOT.Succeeded())
			ItemSlotWidgetClass = BP_ITEM_SLOT.Class;
	}
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
	UItemSlotDragDropOperation* dragDropOp = Cast<UItemSlotDragDropOperation>(InOperation);



	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
