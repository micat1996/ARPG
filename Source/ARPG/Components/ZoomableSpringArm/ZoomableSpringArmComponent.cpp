#include "ZoomableSpringArmComponent.h"

UZoomableSpringArmComponent::UZoomableSpringArmComponent()
{
	// 해당 컴포넌트에서 Tick 이벤트를 활성화 시킵니다.
	PrimaryComponentTick.bCanEverTick = true;

	// 캐릭터와 카메라의 거리를 설정합니다.
	TargetArmLength = TargetLength = 
		((CAMERA_ZOOM_MAX + CAMERA_ZOOM_MIN) * 0.5f);

	// 폰 컨트롤 회전을 사용하도록 합니다.
	bUsePawnControlRotation = true;

	// 줌 스피드 설정
	ZoomSpeed = 20.0f;
}

void UZoomableSpringArmComponent::TickComponent(float dt, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(dt, tickType, thisTickFunction);

	UpdateLength(dt);
}

void UZoomableSpringArmComponent::UpdateLength(float dt)
{
	TargetArmLength = FMath::Lerp(
		TargetArmLength,
		TargetLength,
		10.0f * dt);
}

void UZoomableSpringArmComponent::ZoomCamera(float axis)
{
	TargetLength = FMath::Clamp(
		TargetLength + (axis * ZoomSpeed),
		CAMERA_ZOOM_MIN,
		CAMERA_ZOOM_MAX);
}
