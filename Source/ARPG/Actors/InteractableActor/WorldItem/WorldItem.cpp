#include "WorldItem.h"
#include "Engine/Engine.h"

#include "Actors/Controllers/RPGPlayerController/RPGPlayerController.h"

#include "Single/PlayerManager/PlayerManager.h"
#include "Single/GameInstance/ARPGGameInstance.h"

#include "Widgets/ClosableWnd/DraggableWnd/ItemRootWnd/ItemRootWnd.h"
#include "Widgets/CharacterWidget/CharacterWidget.h"

#include "Components/ClosableWndController/ClosableWndControllerComponent.h"
#include "Components/CharacterWidget/CharacterWidgetComponent.h"

AWorldItem::AWorldItem()
{
	static ConstructorHelpers::FClassFinder<UItemRootWnd> BP_ITEMROOTWND(
		TEXT("WidgetBlueprint'/Game/Resources/Blueprints/Widgets/ClosableWnd/DraggableWidget/ItemRootWnd/BP_ItemRootWnd.BP_ItemRootWnd_C'"));
	if (BP_ITEMROOTWND.Succeeded()) ItemRootWndClass = BP_ITEMROOTWND.Class;
	else { UE_LOG(LogTemp, Error, TEXT("WorldItem.cpp :: %d LINE :: BP_ITEMROOTWND is not loaded!"), __LINE__); }
}

void AWorldItem::BeginPlay()
{
	Super::BeginPlay();

	Capsule->SetHiddenInGame(false);

	Capsule->SetCapsuleRadius(40.0f);
	Capsule->SetCapsuleHalfHeight(40.0f);

	Cast<UCharacterWidget>(CharacterWidget->GetUserWidgetObject())->SetNameText(FText::FromString(TEXT(" ")));
}


AWorldItem* AWorldItem::SpawnItem(UObject * contextObj, TArray<FItemSlotInfo> items, FVector location, FRotator rotation)
{
	AWorldItem* newWorldItem = GEngine->GetWorldFromContextObject(contextObj)->
		SpawnActor<AWorldItem>(AWorldItem::StaticClass(), location, rotation);

	newWorldItem->Items = items;

	return newWorldItem;
}

void AWorldItem::Interaction()
{
	auto itemRootWnd = GetManager(UPlayerManager)->GetPlayerController()->
		GetClosableWndControllerComponent()->AddWnd<UItemRootWnd>(ItemRootWndClass);

	itemRootWnd->InitializeRootItems(Items);
	itemRootWnd->SetWorldItem(this);
	itemRootWnd->onWndClosed.AddLambda([this]() { FinishInteraction(); });
}