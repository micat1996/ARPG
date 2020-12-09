#include "PlayerInteractionComponent.h"

#include "Actors/InteractableActor/InteractableActor.h"
#include "Actors/Characters/RPGCharacter/RPGCharacter.h"
#include "Actors/Controllers/RPGPlayerController/RPGPlayerController.h"
#include "Components/ClosableWndController/ClosableWndControllerComponent.h"

UPlayerInteractionComponent::UPlayerInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ��ȣ�ۿ� ���� ������ �����մϴ�.
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
	// �̹� ��ȣ�ۿ� ���¶�� �������� �ʽ��ϴ�.
	if (bIsInteracting) return;

	// ��ȣ�ۿ� ������ ���Ͱ� �������� ���� ��� �������� �ʽ��ϴ�.
	if (InteractableActors.Num() == 0) return;

	// ��ȣ�ۿ��� ���� �� ������ ������ �����մϴ�.
	FOnInteractionFinishedSignature interactionFinEvent;
	interactionFinEvent.AddLambda([this]() -> void
		{
			// �̵��� ����ŵ�ϴ�.
			PlayerCharacter->AllowMove();

			// ī�޶� �並 ���� ��� �ǵ����ϴ�.
			PlayerCharacter->GetPlayerController()->ChangeViewTarget(nullptr);

			// ��ȣ�ۿ� ���¸� �����մϴ�.
			bIsInteracting = false;
		});

	// �̵��� ���ߵ��� �մϴ�.
	PlayerCharacter->StopMove();

	// �Է� ��带 UI �� �����մϴ�.
	PlayerCharacter->GetPlayerController()->SetInputMode(FInputModeUIOnly());
	PlayerCharacter->GetPlayerController()->bShowMouseCursor = true;


	// ��ȣ�ۿ��� ���۽�ŵ�ϴ�.
	InteractableActors[0]->StartInteraction(interactionFinEvent);
	bIsInteracting = true;

	// ī�޶� �並 NPC �� ������ ��� �����մϴ�.
	PlayerCharacter->GetPlayerController()->ChangeViewTarget(InteractableActors[0]);
}

void UPlayerInteractionComponent::SortByDistance()
{
	// ���� ����
	/// - ���� ������ �� ��Ҹ� �˻��Ͽ� �����ϴ� ���� �˰���
	/// - ������ 2 ���� ��Ҹ� ���Ͽ� ũ�Ⱑ ū ������� �Ǿ����� ������ ���� ��ȯ�մϴ�.

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

	// ���� ��ģ ���Ͱ� AInteractableActor Ÿ���� �ƴ϶�� �������� �ʽ��ϴ�.
	if (!IsValid(interactableActor)) return;

	// �̹� InteractableActors �� ��ҷ� �����ϴ� ���Ͷ�� �������� �ʽ��ϴ�.
	if (InteractableActors.Contains(interactableActor)) return;

	// �迭�� �߰��մϴ�.
	InteractableActors.Add(interactableActor);
}

void UPlayerInteractionComponent::OnInteractableActorLose(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex)
{
	AInteractableActor* interactableActor = Cast<AInteractableActor>(OtherActor);

	// ���� ��ģ ���Ͱ� AInteractableActor Ÿ���� �ƴ϶�� �������� �ʽ��ϴ�.
	if (!IsValid(interactableActor)) return;

	// InteractableActors �� ��ҷ� �����ϴ� ���Ͱ� �ƴ϶�� �������� �ʽ��ϴ�.
	if (!InteractableActors.Contains(interactableActor)) return;

	// �迭���� �����մϴ�.
	InteractableActors.Remove(interactableActor);
}
