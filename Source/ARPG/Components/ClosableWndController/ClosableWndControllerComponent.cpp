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
	// ���� ���� â�� �������� �ʴ´ٸ� �������� �ʽ��ϴ�.
	if (AllocatedWnds.Num() == 0) return;

	// ��� â�� �ݵ��� �Ͽ��ٸ�
	if (bAllClose)
	{
		// ��� â�� �ݽ��ϴ�.
		for (auto wnd : AllocatedWnds)
		{
			PlayerController->GetPlayerCharacterWidgetInstance()->RemoveClosableWnd(wnd);
		}

		AllocatedWnds.Empty();
	}
	else
	{
		// ���� â�� �������� �ʾҴٸ� ���������� ���� â�� �ݽ��ϴ�.
		closableWndInstanceToClose = (closableWndInstanceToClose != nullptr) ?
			closableWndInstanceToClose : 
			AllocatedWnds[AllocatedWnds.Num() - 1];

		// ������ â�� �ݽ��ϴ�.
		AllocatedWnds.Remove(closableWndInstanceToClose);

		PlayerController->GetPlayerCharacterWidgetInstance()->RemoveClosableWnd(
			closableWndInstanceToClose);
	}


	// ���� â�� �������� �ʴ´ٸ�
	if (GetAllocatedWndCount() == 0)
	{
		// �Է� ��带 Game ���� �����մϴ�.
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
	// �켱������ �����ų ������ ������ �����մϴ�.
	UCanvasPanelSlot* wndSlot = Cast<UCanvasPanelSlot>(closableWndInstance->Slot);

	// �÷��̾� ����
	UPlayerCharacterWidget* playerCharacterWidget = 
		PlayerController->GetPlayerCharacterWidgetInstance();

	// �ֻ������ �����Ϸ��� ������ �ֻ�ܿ� ��ġ�Ǿ� ���� �ʴٸ�
	if (playerCharacterWidget->GetClosableWnds()->GetChildIndex(closableWndInstance) !=
		/// - GetChildIndex(widget) : widget �� �������� ������ ����ϴ�.
		playerCharacterWidget->GetClosableWnds()->GetSlots().Num() - 1)
		/// - GetSlots() : �߰��� �������� ���� ������ �����ϴ� �迭�� ����ϴ�.
	{
		// ������ �����մϴ�.
		playerCharacterWidget->GetClosableWnds()->AddChild(closableWndInstance);
	}
}