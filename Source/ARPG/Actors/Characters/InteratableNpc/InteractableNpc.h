#pragma once

#include "Actors/InteractableActor/InteractableActor.h"
#include "InteractableNpc.generated.h"

UCLASS()
class ARPG_API AInteractableNpc : public AInteractableActor
{
	GENERATED_BODY()

private:
	class UDataTable* DT_ShopInfo;
	TSubclassOf<class UShopWnd> ShopWndClass;




private :
	UPROPERTY()
	class UARPGGameInstance* GameInst;

	UPROPERTY()
	class UPlayerManager * PlayerManager;

protected :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FName NpcCode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent * InteractCamera;



public :
	AInteractableNpc();

protected :
	virtual void BeginPlay() override;

	void OpenDialogWidget();

public :
	virtual void Interaction() override;

	// 상점을 엽니다.
	void OpenShop(FName shopID);

	
};
