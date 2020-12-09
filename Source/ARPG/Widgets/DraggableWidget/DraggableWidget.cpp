#include "DraggableWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"

#include "Blueprint/WidgetLayoutLibrary.h"

void UDraggableWidget::DragWidget()
{
	// 드래깅중이 아니라면 실행하지 않습니다.
	if (!bIsDragging) return;

	// 이동시킬 위젯이 존재하지 않는 경우 실행하지 않습니다.
	if (!IsValid(TargetWidget)) return;

	// 마우스 위치를 저장합니다.
	FVector2D mousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	/// - UWidgetLayoutLibrary : 위젯 레이아웃에 관련된 유틸성 정적 메서드들을 제공하는 클래스

	// 드래그 이동시킬 위젯 슬롯을 저장합니다.
	UCanvasPanelSlot* targetSlot = Cast<UCanvasPanelSlot>(TargetWidget->Slot);

	targetSlot->SetPosition(
		PrevWidgetPosition + (mousePosition - PrevInputPosition));

	// 다음 연산을 위하여 현재 위치를 저장합니다.
	PrevInputPosition = mousePosition;
	PrevWidgetPosition = targetSlot->GetPosition();
}

void UDraggableWidget::NativeOnInitialized()
{
	Button_Draggable = Cast<UButton>(GetWidgetFromName(TEXT("Button_Draggable")));

	Button_Draggable->OnPressed.AddDynamic(this, &UDraggableWidget::OnWidgetDragStarted);
	Button_Draggable->OnReleased.AddDynamic(this, &UDraggableWidget::OnWidgetDragFinished);

}

void UDraggableWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	DragWidget();

}

void UDraggableWidget::OnWidgetDragStarted()
{
	// 이동시킬 위젯이 존재하지 않는 경우 실행하지 않습니다.
	if (!IsValid(TargetWidget)) return;

	// 입력된 마우스 위치를 저장합니다.
	PrevInputPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	// 이전 위젯 위치를 저장합니다.
	PrevWidgetPosition = Cast<UCanvasPanelSlot>(TargetWidget->Slot)->GetPosition();

	if (OnDraggingStarted.IsBound())
		OnDraggingStarted.Broadcast();

	bIsDragging = true;
}

void UDraggableWidget::OnWidgetDragFinished()
{
	// 이동시킬 위젯이 존재하지 않는 경우 실행하지 않습니다.
	if (!IsValid(TargetWidget)) return;

	bIsDragging = false;
}
