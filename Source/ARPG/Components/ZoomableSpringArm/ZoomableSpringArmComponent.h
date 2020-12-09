#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "ZoomableSpringArmComponent.generated.h"

#ifndef CAMERA_ZOOM_MINMAX
#define CAMERA_ZOOM_MINMAX

#define CAMERA_ZOOM_MIN		200.0f
#define CAMERA_ZOOM_MAX		1000.0f
#endif


UCLASS()
class ARPG_API UZoomableSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()
	
private :
	// 카메라 줌 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Option", meta = (AllowPrivateAccess = "true"))
	float ZoomSpeed;

	// 목표 길이
	UPROPERTY()
	float TargetLength;


public :
	UZoomableSpringArmComponent();

	virtual void TickComponent(float dt, enum ELevelTick tickType, FActorComponentTickFunction * thisTickFunction) override;

private :
	void UpdateLength(float dt);

public :
	void ZoomCamera(float axis);

};
