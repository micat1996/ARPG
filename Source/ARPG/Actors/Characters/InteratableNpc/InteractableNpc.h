#pragma once

#include "Actors/InteractableActor/InteractableActor.h"
#include "InteractableNpc.generated.h"

UCLASS()
class ARPG_API AInteractableNpc : public AInteractableActor
{
	GENERATED_BODY()

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

	
};
