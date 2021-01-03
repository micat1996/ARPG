#include "ClosableWnd.h"

#include "Components/ClosableWndController/ClosableWndControllerComponent.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"


void UClosableWnd::NativeConstruct()
{
	Super::NativeConstruct();

	// 창 크기를 설정합니다.
	UpdateWndSize(1920.0f, 1080.0f);

	// 종료 버튼을 설정합니다.
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

	// 자식 창을 생성합니다.
	UClosableWnd* childWnd = ClosableWndController->AddWnd<UClosableWnd>(closableWnd);

	// 생성된 자식 창의 부모 창을 자신으로 설정합니다.
	childWnd->ParentWnd = this;

	// 배열에 추가합니다.
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
	// 자식 창이 존재한다면
	if (ChildWnds.Num() != 0)
	{
		// 모든 자식 창을 제거합니다.
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

	// 부모 창이 존재한다면
	if (IsValid(ParentWnd))
	{
		// 자신을 부모 창에서 제외시킵니다.
		ParentWnd->RemoveFromParentWnd(this);
	}
}

void UClosableWnd::UpdateWndSize(float width, float height)
{
	WndSize = FVector2D(width, height);
	Cast<UCanvasPanelSlot>(Slot)->SetSize(WndSize);
}