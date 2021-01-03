#pragma once

#include "ARPG.h"
#include "GameFramework/Actor.h"
#include "DungeonDoor.generated.h"

UCLASS()
class ARPG_API ADungeonDoor : public AActor
{
	GENERATED_BODY()
	
private :
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent * Box;


public:	
	ADungeonDoor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private :
	UFUNCTION()
	void OnDoorOverlap(UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
