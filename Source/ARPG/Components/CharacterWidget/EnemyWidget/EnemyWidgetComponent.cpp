#include "EnemyWidgetComponent.h"
#include "Actors/Characters/EnemyCharacter/EnemyCharacter.h"
#include "Widgets/CharacterWidget/HpableCharacterWidget/HpableCharacterWidget.h"


UEnemyWidgetComponent::UEnemyWidgetComponent()
{
	static ConstructorHelpers::FClassFinder<UHpableCharacterWidget> WIDGET_BP_ENEMY(
		TEXT("WidgetBlueprint'/Game/Resources/Blueprints/Widgets/WidgetBP_Enemy.WidgetBP_Enemy_C'"));
	if (WIDGET_BP_ENEMY.Succeeded())
		EnemyCharacterWidgetClass = WIDGET_BP_ENEMY.Class;
	else 
	{ UE_LOG(LogTemp, Error, TEXT("EnemyWidgetComponent.cpp :: %d LINE :: WIDGET_BP_ENEMY is not loaded!"), __LINE__); }

	if (IsValid(EnemyCharacterWidgetClass))
	{
		InitializeCharaterWidget(
			EnemyCharacterWidgetClass,
			FVector2D(100.0f, 40.0f));
	}
}

void UEnemyWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	EnemyCharacter = Cast<AEnemyCharacter>(GetOwner());
}


UHpableCharacterWidget* UEnemyWidgetComponent::GetEnemyWidgetInstance()
{
	return EnemyWidgetInstance = IsValid(EnemyWidgetInstance) ?
		EnemyWidgetInstance : 
		Cast<UHpableCharacterWidget>(GetUserWidgetObject());
}

