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
	// 상호작용을 시작합니다.
	void StartInteraction(FOnInteractionFinishedSignature interactionFinishedEvent);

	// 상호작용을 끝냅니다.
	UFUNCTION()
	void FinishInteraction();

protected :
	// 상호작용시 호출될 메서드
	virtual void Interaction() PURE_VIRTUAL(AInteractableActor::Interaction, );


};
