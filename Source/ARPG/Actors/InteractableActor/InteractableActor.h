#pragma once

#include "ARPG.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInteractionFinishedSignature)

UCLASS(Abstract)
class ARPG_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
protected :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
	class UCapsuleComponent * Capsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
	class UArrowComponent * Arrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
	class UCharacterWidgetComponent * CharacterWidget;

	FOnInteractionFinishedSignature OnInteractionFinished;


public:	
	AInteractableActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public :
	// ��ȣ�ۿ��� �����մϴ�.
	void StartInteraction(FOnInteractionFinishedSignature interactionFinishedEvent);

	// ��ȣ�ۿ��� �����ϴ�.
	UFUNCTION()
	void FinishInteraction();

protected :
	// ��ȣ�ۿ�� ȣ��� �޼���
	virtual void Interaction() PURE_VIRTUAL(AInteractableActor::Interaction, );


};
