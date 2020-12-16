#include "DraggableWnd.h"

#include "Components/ClosableWndController/ClosableWndControllerComponent.h"

#include "Widgets/DraggableWidget/DraggableWndTitle/DraggableWndTitle.h"


void UDraggableWnd::NativeConstruct()
{
	Super::NativeConstruct();

	// 창 크기 설정
	UpdateWndSize(500.0f, 600.0f);

	BP_DraggableWndTitle = Cast<UDraggableWndTitle>(
		GetWidgetFromName(TEXT("BP_DraggableWndTitle")));
	BP_DraggableWndTitle->SetTargetWidget(this);

	BP_DraggableWndTitle->OnDraggingStarted.AddLambda([this]() -> void
		{ ClosableWndController->SetHighestPriorityWnd(this); });

	SetCloseButton(BP_DraggableWndTitle->GetCloseButton());

}
