#include "PlayerCharacterWidget.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

#include "Widgets/ClosableWnd/ClosableWnd.h"


void UPlayerCharacterWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Canvas_ClosableWnds = Cast<UCanvasPanel>(GetWidgetFromName("Canvas_ClosableWnds"));
	ProgressBar_HP = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_HP")));


	// Widget 크기를 설정합니다.
	SetDesiredSizeInViewport(FVector2D(1920.0f, 1080.0f));
	/// - 드래그 앤 드롭시 정확한 위치 계산을 위하여 설정
}

void UPlayerCharacterWidget::AddClosableWnd(UClosableWnd* closableWnd)
{
	Canvas_ClosableWnds->AddChild(closableWnd);

	Cast<UCanvasPanelSlot>(closableWnd->Slot)->SetSize(closableWnd->WndSize);

	if (closableWnd->onWndOpened.IsBound())
		closableWnd->onWndOpened.Broadcast();
}

void UPlayerCharacterWidget::RemoveClosableWnd(UClosableWnd* closableWnd)
{
	closableWnd->RemoveFromParent();

	if (closableWnd->onWndClosed.IsBound())
		closableWnd->onWndClosed.Broadcast();
}
