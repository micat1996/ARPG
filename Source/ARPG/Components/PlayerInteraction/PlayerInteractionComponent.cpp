#include "PlayerInteractionComponent.h"

#include "Actors/InteractableActor/InteractableActor.h"
#include "Actors/Characters/RPGCharacter/RPGCharacter.h"
#include "Actors/Controllers/RPGPlayerController/RPGPlayerController.h"
#include "Components/ClosableWndController/ClosableWndControllerComponent.h"

UPlayerInteractionComponent::UPlayerInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 상호작용 감지 범위를 설정합니다.
	SphereRadius = 500.0f;
}

void UPlayerInteractionComponent::BeginPlay()
{
	PlayerCharacter = Cast<ARPGCharacter>(GetOwner());

	OnComponentBeginOverlap.AddDynamic(this, &UPlayerInteractionComponent::OnInteractableActorDetected);
	OnComponentEndOverlap.AddDynamic(this, &UPlayerInteractionComponent::OnInteractableActorLose);
}

void UPlayerInteractionComponent::TickComponent(
	float dt, 
	ELevelTick tickType, 
	FActorComponentTickFunction* thisTickFunc)
{
	Super::TickComponent(dt, tickType, thisTickFunc);

	SortByDistance();
}

void UPlayerInteractionComponent::TryInteraction()
{
	// 이미 상호작용 상태라면 실행하지 않습니다.
	if (bIsInteracting) return;

	// 상호작용 가능한 액터가 존재하지 않을 경우 실행하지 않습니다.
	if (InteractableActors.Num() == 0) return;

	// 상호작용이 끝날 때 실행할 내용을 정의합니다.
	FOnInteractionFinishedSignature interactionFinEvent;
	interactionFinEvent.AddLambda([this]() -> void
		{
			// 이동을 허용시킵니다.
			PlayerCharacter->AllowMove();

			// 카메라 뷰를 원래 뷰로 되돌립니다.
			PlayerCharacter->GetPlayerController()->ChangeViewTarget(nullptr);

			// 상호작용 상태를 해제합니다.
			bIsInteracting = false;
		});

	// 이동을 멈추도록 합니다.
	PlayerCharacter->StopMove();

	// 입력 모드를 UI 로 설정합니다.
	PlayerCharacter->GetPlayerController()->SetInputMode(FInputModeUIOnly());
	PlayerCharacter->GetPlayerController()->bShowMouseCursor = true;


	// 상호작용을 시작시킵니다.
	InteractableActors[0]->StartInteraction(interactionFinEvent);
	bIsInteracting = true;

	// 카메라 뷰를 NPC 에 설정된 뷰로 설정합니다.
	PlayerCharacter->GetPlayerController()->ChangeViewTarget(InteractableActors[0]);
}

void UPlayerInteractionComponent::SortByDistance()
{
	// 버블 정렬
	/// - 서로 인접한 두 요소를 검사하여 정렬하는 정렬 알고리즘
	/// - 인접한 2 개의 요소를 비교하여 크기가 큰 순서대로 되어있지 않으면 서로 교환합니다.

	for (int i = 0; i < InteractableActors.Num(); ++i)
	{
		for (int j = 0; j < InteractableActors.Num() - (i + 1); ++i)
		{
			float current = FVector::Distance(
				InteractableActors[j]->GetActorLocation(),
				GetComponentLocation());

			float next = FVector::Distance(
				InteractableActors[j + 1]->GetActorLocation(),
				GetComponentLocation());

			if (current > next)
			{
				auto temp = InteractableActors[j];
				InteractableActors[j] = InteractableActors[j + 1];
				InteractableActors[j + 1] = temp;
			}
		}
	}
}

void UPlayerInteractionComponent::OnInteractableActorDetected(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	AInteractableActor* interactableActor = Cast<AInteractableActor>(OtherActor);

	// 만약 겹친 액터가 AInteractableActor 타입이 아니라면 실행하지 않습니다.
	if (!IsValid(interactableActor)) return;

	// 이미 InteractableActors 의 요소로 존재하는 액터라면 실행하지 않습니다.
	if (InteractableActors.Contains(interactableActor)) return;

	// 배열에 추가합니다.
	InteractableActors.Add(interactableActor);
}

void UPlayerInteractionComponent::OnInteractableActorLose(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex)
{
	AInteractableActor* interactableActor = Cast<AInteractableActor>(OtherActor);

	// 만약 겹친 액터가 AInteractableActor 타입이 아니라면 실행하지 않습니다.
	if (!IsValid(interactableActor)) return;

	// InteractableActors 의 요소로 존재하는 액터가 아니라면 실행하지 않습니다.
	if (!InteractableActors.Contains(interactableActor)) return;

	// 배열에서 제거합니다.
	InteractableActors.Remove(interactableActor);
}
