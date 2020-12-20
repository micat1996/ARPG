#include "ClosableDialogWnd.h"

#include "Actors/Characters/InteratableNpc/InteractableNpc.h"
#include "Components/Button.h"

void UClosableDialogWnd::SetOwnerNpc(AInteractableNpc* ownerNpc)
{
	OwnerNpc = ownerNpc;

	// 창을 닫았을 때 상호작용이 끝나도록 합니다.
	if (IsValid(CloseButton))
		CloseButton->OnClicked.AddDynamic(OwnerNpc, &AInteractableNpc::FinishInteraction);

}


UClosableWnd* UClosableDialogWnd::OpenShop(FName shopID)
{
	return OwnerNpc->OpenShop(this, shopID);
}