#include "DraggableWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"

#include "Blueprint/WidgetLayoutLibrary.h"

void UDraggableWidget::DragWidget()
{
	// �巡������ �ƴ϶�� �������� �ʽ��ϴ�.
	if (!bIsDragging) return;

	// �̵���ų ������ �������� �ʴ� ��� �������� �ʽ��ϴ�.
	if (!IsValid(TargetWidget)) return;

	// ���콺 ��ġ�� �����մϴ�.
	FVector2D mousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	/// - UWidgetLayoutLibrary : ���� ���̾ƿ��� ���õ� ��ƿ�� ���� �޼������ �����ϴ� Ŭ����

	// �巡�� �̵���ų ���� ������ �����մϴ�.
	UCanvasPanelSlot* targetSlot = Cast<UCanvasPanelSlot>(TargetWidget->Slot);

	targetSlot->SetPosition(
		PrevWidgetPosition + (mousePosition - PrevInputPosition));

	// ���� ������ ���Ͽ� ���� ��ġ�� �����մϴ�.
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
	// �̵���ų ������ �������� �ʴ� ��� �������� �ʽ��ϴ�.
	if (!IsValid(TargetWidget)) return;

	// �Էµ� ���콺 ��ġ�� �����մϴ�.
	PrevInputPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	// ���� ���� ��ġ�� �����մϴ�.
	PrevWidgetPosition = Cast<UCanvasPanelSlot>(TargetWidget->Slot)->GetPosition();

	if (OnDraggingStarted.IsBound())
		OnDraggingStarted.Broadcast();

	bIsDragging = true;
}

void UDraggableWidget::OnWidgetDragFinished()
{
	// �̵���ų ������ �������� �ʴ� ��� �������� �ʽ��ϴ�.
	if (!IsValid(TargetWidget)) return;

	bIsDragging = false;
}
