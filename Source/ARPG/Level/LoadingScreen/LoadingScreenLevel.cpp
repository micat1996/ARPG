#include "LoadingScreenLevel.h"

#include "UserWidget.h"
#include "Engine/World.h"


ALoadingScreenLevel::ALoadingScreenLevel()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> BP_LOADING_SCREEN(
		TEXT("WidgetBlueprint'/Game/Resources/Blueprints/Widgets/LoadingScreen/BP_LoadingScreen.BP_LoadingScreen_C'"));
	if (BP_LOADING_SCREEN.Succeeded()) BP_LoadingScreen = BP_LOADING_SCREEN.Class;
}

void ALoadingScreenLevel::BeginPlay()
{
	Super::BeginPlay();

	CreateWidget<UUserWidget>(GetWorld(), BP_LoadingScreen)->AddToViewport();
}