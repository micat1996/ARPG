#include "ClosableWnd.h"

#include "Components/ClosableWndController/ClosableWndControllerComponent.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"


void UClosableWnd::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UClosableWnd::NativeConstruct()
{

	Super::NativeConstruct();

	// â ũ�⸦ �����մϴ�.
	WndSize = FVector2D(1920.0f, 1080.0f);
	UpdateWndSize();

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
	// �θ� â�� �����Ѵٸ�
	if (IsValid(ParentWnd))
	{
		// �ڽ��� �θ� â���� ���ܽ�ŵ�ϴ�.
		ParentWnd->RemoveFromParentWnd(this);
	}

	// ��� �ڽ� â�� �����մϴ�.
	for (auto childWnd : ChildWnds)
	{
		ClosableWndController->CloseWnd(
			/*bAllClose :					*/	false,
			/*closableWndInstanceToClose :	*/	childWnd);
	}

	ClosableWndController->CloseWnd(
		/*bAllClose :					*/	false, 
		/*closableWndInstanceToClose :	*/	this);
}

void UClosableWnd::UpdateWndSize()
{
	Cast<UCanvasPanelSlot>(Slot)->SetSize(WndSize);
}