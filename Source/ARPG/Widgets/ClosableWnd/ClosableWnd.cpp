#include "ClosableWnd.h"

#include "Components/ClosableWndController/ClosableWndControllerComponent.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"


void UClosableWnd::NativeConstruct()
{
	Super::NativeConstruct();

	// â ũ�⸦ �����մϴ�.
	UpdateWndSize(1920.0f, 1080.0f);

	// ���� ��ư�� �����մϴ�.
	SetCloseButton(Cast<UButton>(GetWidgetFromName(TEXT("Button_Close"))));
}

void UClosableWnd::SetCloseButton(UButton* closeButton)
{
	if (!IsValid(closeButton)) return;


	CloseButton = closeButton;
	CloseButton->OnClicked.AddDynamic(this, &UClosableWnd::OnCloseButtonClicked);
}

UClosableWnd* UClosableWnd::CreateChildClosableWnd(TSubclassOf<UClosableWnd> closableWnd)
{
	if (ClosableWndController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ClosableWndController is nullptr"));
		return nullptr;
	}

	// �ڽ� â�� �����մϴ�.
	UClosableWnd* childWnd = ClosableWndController->AddWnd<UClosableWnd>(closableWnd);

	// ������ �ڽ� â�� �θ� â�� �ڽ����� �����մϴ�.
	childWnd->ParentWnd = this;

	// �迭�� �߰��մϴ�.
	ChildWnds.Add(childWnd);

	return childWnd;
}

bool UClosableWnd::IsAllocated(UClosableWnd* childwnd)
{
	return ClosableWndController->IsAllocated(childwnd);
}

void UClosableWnd::RemoveFromParentWnd(UClosableWnd* childWnd)
{
	ChildWnds.Remove(childWnd);
}

void UClosableWnd::OnCloseButtonClicked()
{
	// �ڽ� â�� �����Ѵٸ�
	if (ChildWnds.Num() != 0)
	{
		// ��� �ڽ� â�� �����մϴ�.
		//for (auto childWnd : ChildWnds)
		for (int32 i = 0; i < ChildWnds.Num(); ++i)
		{
			UClosableWnd* childWnd = ChildWnds[i];

			if (IsValid(childWnd))
			{
				childWnd->OnCloseButtonClicked();

				ClosableWndController->CloseWnd(
					/*bAllClose :					*/	false,
					/*closableWndInstanceToClose :	*/	childWnd);
			}
		}
		ChildWnds.Empty();
	}


	ClosableWndController->CloseWnd(
		/*bAllClose :					*/	false, 
		/*closableWndInstanceToClose :	*/	this);

	// �θ� â�� �����Ѵٸ�
	if (IsValid(ParentWnd))
	{
		// �ڽ��� �θ� â���� ���ܽ�ŵ�ϴ�.
		ParentWnd->RemoveFromParentWnd(this);
	}
}

void UClosableWnd::UpdateWndSize(float width, float height)
{
	WndSize = FVector2D(width, height);
	Cast<UCanvasPanelSlot>(Slot)->SetSize(WndSize);
}