#include "InteractableActor.h"

#include "Components/ArrowComponent.h"
#include "Components/CharacterWidget/CharacterWidgetComponent.h"
#include "Widgets/CharacterWidget/CharacterWidget.h"

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UCharacterWidget> WIDGET_BP_CHARACTER(
		TEXT("WidgetBlueprint'/Game/Resources/Blueprints/Widgets/WidgetBP_Character.WidgetBP_Character_C'"));



	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE_COMPONENT"));
	CharacterWidget = CreateDefaultSubobject<UCharacterWidgetComponent>(TEXT("INTERACTABLE_ACTOR_WIDGET"));
	SetRootComponent(Capsule);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ARROW_COMPONENT"));
	Arrow->SetupAttachment(GetRootComponent());
	CharacterWidget->SetupAttachment(GetRootComponent());

	Capsule->SetCapsuleHalfHeight(88.0f);
	Capsule->SetCapsuleRadius(34.0f);

	if (WIDGET_BP_CHARACTER.Succeeded())
		CharacterWidget->InitializeCharaterWidget(WIDGET_BP_CHARACTER.Class, FVector2D(100.0f, 30.0f));
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	CharacterWidget->SetWidgetHeight(Capsule->GetScaledCapsuleHalfHeight() + 10.0f);

}

void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableActor::StartInteraction(
	FOnInteractionFinishedSignature interactionFinishedEvent)
{
	// 상호작용 시작
	Interaction();

	// 상호작용 끝 이벤트 설정
	OnInteractionFinished = interactionFinishedEvent;
}

void AInteractableActor::FinishInteraction()
{
	// 상호작용이 끝날 때 실행할 내용이 있다면
	if (OnInteractionFinished.IsBound())
	{
		// 상호작용 끝 이벤트 실행
		OnInteractionFinished.Broadcast();

		// 상호작용 이벤트 내용 초기화
		OnInteractionFinished.Clear();
	}
}