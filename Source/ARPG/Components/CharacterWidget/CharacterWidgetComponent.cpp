#include "CharacterWidgetComponent.h"


#include "Actors/Characters/RPGCharacter/RPGCharacter.h"
#include "Actors/Controllers/RPGPlayerController/RPGPlayerController.h"

#include "Widgets/CharacterWidget/CharacterWidget.h"

#include "Single/GameInstance/ARPGGameInstance.h"
#include "Single/PlayerManager/PlayerManager.h"

UCharacterWidgetComponent::UCharacterWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	// 위젯 표시 최대 거리 설정
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

	auto gameInst = GetGameInst();
	if (!IsValid(gameInst)) return;
	if (!gameInst->bIsLevelLoaded) return;


	if (!IsValid(PlayerCamera))
	{
		UPlayerManager* playerManager = GetManager(UPlayerManager);

		if (!IsValid(playerManager->GetPlayerController())) return;
		if (!IsValid(playerManager->GetPlayerController()->GetPawn())) return;

		if (!IsValid(Cast<ARPGCharacter>(playerManager->GetPlayerController()->GetPawn()))) return;

		PlayerCamera = Cast<ARPGCharacter>(playerManager->GetPlayerController()->GetPawn())->GetCameraComponent();
		return;
	}

	// 카메라와 적 캐릭터의 거리를 구합니다.
	float distance = FVector::Distance(
		PlayerCamera->GetComponentLocation(),
		GetOwner()->GetActorLocation());

	// 거리에 따라 위젯을 표시합니다.
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
 