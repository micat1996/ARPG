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


	// 드래그 앤 드롭 작업을 생성하며, 작업 결과를 반환합니다.
	return UWidgetBlueprintLibrary::DetectDragIfPressed(
		InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

void UItemSlot::NativeOnDragDetected(
	const FGeometry& InGeometry, 
	const FPointerEvent& InMouseEvent, 
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	// 드래그 앤 드롭 작업 객체를 생성합니다.
	UItemSlotDragDropOperation* dragDropOp = Cast<UItemSlotDragDropOperation>(
		UWidgetBlueprintLibrary::CreateDragDropOperation(UItemSlotDragDropOperation::StaticClass()));

	if (IsValid(dragDropOp))
	{
		// 피벗을 설정합니다.
		dragDropOp->Pivot = EDragPivot::CenterCenter;

		// 드래그 앤 드랍시 보여질 위젯을 설정합니다.
		dragDropOp->DefaultDragVisual = CreateWidget<UUserWidget>(this, ItemSlotWidgetClass);

		// 드래깅을 시작한 슬롯을 설정합니다.
		dragDropOp->DraggingSlot = this;
	}

	// 출력용 매개 변수에 드래그 앤 드롭 작업 객체를 설정합니다.
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
