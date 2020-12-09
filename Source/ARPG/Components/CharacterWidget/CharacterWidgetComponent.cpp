#include "CharacterWidgetComponent.h"


#include "Actors/Characters/RPGCharacter/RPGCharacter.h"
#include "Actors/Controllers/RPGPlayerController/RPGPlayerController.h"

#include "Widgets/CharacterWidget/CharacterWidget.h"

#include "Single/GameInstance/ARPGGameInstance.h"
#include "Single/PlayerManager/PlayerManager.h"

UCharacterWidgetComponent::UCharacterWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	// ���� ǥ�� �ִ� �Ÿ� ����
	MaxDrawableDistance = 2000.0f;
}

void UCharacterWidgetComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DistanceToggle();
}


void UCharacterWidgetComponent::DistanceToggle()
{
	if (!IsValid(PlayerCamera))
	{
		UARPGGameInstance* gameInst = Cast<UARPGGameInstance>(GetWorld()->GetGameInstance());

		if (!IsValid(gameInst)) return;

		UPlayerManager* playerManager = gameInst->GetManagerClass<UPlayerManager>();

		PlayerCamera = Cast<ARPGCharacter>(playerManager->GetPlayerController()->GetPawn())->GetCameraComponent();
		return;
	}

	// ī�޶�� �� ĳ������ �Ÿ��� ���մϴ�.
	float distance = FVector::Distance(
		PlayerCamera->GetComponentLocation(),
		GetOwner()->GetActorLocation());

	// �Ÿ��� ���� ������ ǥ���մϴ�.
	bVisible = (distance <= MaxDrawableDistance);
}

void UCharacterWidgetComponent::InitializeCharaterWidget(
	TSubclassOf<class UCharacterWidget> widgetClass, 
	FVector2D drawSize, 
	EWidgetSpace newWidgetSpace)
{
	SetWidgetClass(widgetClass);
	SetDrawSize(drawSize);
	SetWidgetSpace(newWidgetSpace);
}

void UCharacterWidgetComponent::SetWidgetHeight(float widgetHeight)
{
	SetRelativeLocation(FVector::UpVector * widgetHeight);
}
