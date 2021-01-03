#pragma once

#include "ARPG.h"
#include "Actors/InteractableActor/InteractableActor.h"
#include "Structures/ItemSlotInfo/ItemSlotInfo.h"
#include "WorldItem.generated.h"

UCLASS()
class ARPG_API AWorldItem : public AInteractableActor
{
	GENERATED_BODY()

private :
	TSubclassOf<class UItemRootWnd> ItemRootWndClass;

private :
	TArray<FItemSlotInfo> Items;

public :
	AWorldItem();

protected :
	virtual void BeginPlay() override;

public :
	// 월드에 아이템 액터를 생성합니다.
	static AWorldItem* SpawnItem(UObject* contextObj, TArray<FItemSlotInfo> items, 
		FVector location, FRotator rotation = FRotator::ZeroRotator);
	
protected :
	virtual void Interaction() override;

};