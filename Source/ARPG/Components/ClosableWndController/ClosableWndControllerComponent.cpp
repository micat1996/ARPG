#include "ClosableWndControllerComponent.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"

UClosableWndControllerComponent::UClosableWndControllerComponent()
{
}

void UClosableWndControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ARPGPlayerController>(GetOwner());
	
}

void UClosableWndControllerComponent::CloseWnd(bool bAllClose, UClosableWnd* closableWndInstanceToClose)
{
	// 만약 열린 창이 존재하지 않는다면 실행하지 않습니다.
	if (AllocatedWnds.Num() == 0) return;

	// 모든 창을 닫도록 하였다면
	if (bAllClose)
	{
		// 모든 창을 닫습니다.
		for (auto wnd : AllocatedWnds)
		{
			PlayerController->GetPlayerCharacterWidgetInstance()->RemoveClosableWnd(wnd);
		}

		AllocatedWnds.Empty();
	}
	else
	{
		// 닫을 창이 지정되지 않았다면 마지막으로 열린 창을 닫습니다.
		closableWndInstanceToClose = (closableWndInstanceToClose != nullptr) ?
			closableWndInstanceToClose : 
			AllocatedWnds[AllocatedWnds.Num() - 1];

		// 지정된 창을 닫습니다.
		AllocatedWnds.Remove(closableWndInstanceToClose);

		PlayerController->GetPlayerCharacterWidgetInstance()->RemoveClosableWnd(
			closableWndInstanceToClose);
	}


	// 열린 창이 존재하지 않는다면
	if (GetAllocatedWndCount() == 0)
	{
		// 입력 모드를 Game 으로 설정합니다.
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}
}

bool UClosableWndControllerComponent::IsAllocated(UClosableWnd* closableWndInstance)
{
	return AllocatedWnds.Find(closableWndInstance) != INDEX_NONE;
}

void UClosableWndControllerComponent::SetHighestPriorityWnd(UClosableWnd* closableWndInstance)
{
	// 우선순위를 변경시킬 위젯의 슬롯을 저장합니다.
	UCanvasPanelSlot* wndSlot = Cast<UCanvasPanelSlot>(closableWndInstance->Slot);

	// 플레이어 위젯
	UPlayerCharacterWidget* playerCharacterWidget = 
		PlayerController->GetPlayerCharacterWidgetInstance();

	// 최상단으로 설정하려는 위젯이 최상단에 배치되어 있지 않다면
	if (playerCharacterWidget->GetClosableWnds()->GetChildIndex(closableWndInstance) !=
		/// - GetChildIndex(widget) : widget 의 계층구조 순서를 얻습니다.
		playerCharacterWidget->GetClosableWnds()->GetSlots().Num() - 1)
		/// - GetSlots() : 추가된 위젯들의 슬롯 정보를 저장하는 배열을 얻습니다.
	{
		// 위젯을 재등록합니다.
		playerCharacterWidget->GetClosableWnds()->AddChild(closableWndInstance);
	}
}