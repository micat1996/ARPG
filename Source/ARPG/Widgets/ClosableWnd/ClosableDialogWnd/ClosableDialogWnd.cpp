#include "ClosableDialogWnd.h"

#include "Actors/Characters/InteratableNpc/InteractableNpc.h"
#include "Components/Button.h"

void UClosableDialogWnd::SetOwnerNpc(AInteractableNpc* ownerNpc)
{
	OwnerNpc = ownerNpc;

	// â�� �ݾ��� �� ��ȣ�ۿ��� �������� �մϴ�.
	if (IsValid(CloseButton))
		CloseButton->OnClicked.AddDynamic(OwnerNpc, &AInteractableNpc::FinishInteraction);

}


UClosableWnd* UClosableDialogWnd::OpenShop(FName shopID)
{
	return OwnerNpc->OpenShop(this, shopID);
}