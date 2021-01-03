#include "RPGPlayerController.h"

#include "Actors/Characters/BaseCharacter.h"

#include "Components/ClosableWndController/ClosableWndControllerComponent.h"


#include "Widgets/CharacterWidget/HpableCharacterWidget/PlayerCharacterWidget/PlayerCharacterWidget.h"

#include "Single/GameInstance/ARPGGameInstance.h"


ARPGPlayerController::ARPGPlayerController()
{
	static ConstructorHelpers::FClassFinder<UPlayerCharacterWidget> WIDGET_BP_GAME(
		TEXT("WidgetBlueprint'/Game/Resources/Blueprints/Widgets/WidgetBP_Player.WidgetBP_Player_C'"));
	if (WIDGET_BP_GAME.Succeeded()) GameWidgetClass = WIDGET_BP_GAME.Class;

	ClosableWndController = CreateDefaultSubobject<UClosableWndControllerComponent>(TEXT("CLOSABLE_WND_CONTROLLER"));
}

void ARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis(TEXT("MouseX"), this, &ARPGPlayerController::InputMouseX);
	InputComponent->BindAxis(TEXT("MouseY"), this, &ARPGPlayerController::InputMouseY);
}

void ARPGPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	GetPlayerCharacterWidgetInstance();

}

void ARPGPlayerController::InputMouseX(float axis)
{
	AddYawInput(axis);
}

void ARPGPlayerController::InputMouseY(float axis)
{
	AddPitchInput(axis);
}

UPlayerCharacterWidget* ARPGPlayerController::GetPlayerCharacterWidgetInstance()
{
	if (!IsValid(PlayerCharacterWidgetInstance))
	{
		PlayerCharacterWidgetInstance = CreateWidget<UPlayerCharacterWidget>(this, GameWidgetClass);
		PlayerCharacterWidgetInstance->InitializeWidget(Cast<ABaseCharacter>(GetPawn()));
		PlayerCharacterWidgetInstance->UpdateHp();
		PlayerCharacterWidgetInstance->AddToViewport();
	}

	return PlayerCharacterWidgetInstance;
}

void ARPGPlayerController::ChangeViewTarget(AActor* newViewTarget)
{

	// newViewTarget 이 nullptr 가 아니라면
	if (IsValid(newViewTarget))
	{
		//SetViewTarget(newViewTarget);
		SetViewTargetWithBlend(newViewTarget, 0.3f);
	}

	// nullptr
	//else SetViewTarget(GetPawn());
	else SetViewTargetWithBlend(GetPawn(), 0.3f);
	
}
