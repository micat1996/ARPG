// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CharacterWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UCharacterWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected :
	// 위젯이 표시되는 최대 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDrawableDistance;
	
	// 플레이어 카메라
	UPROPERTY()
	class UCameraComponent * PlayerCamera;

public :
	UCharacterWidgetComponent();

public:
	virtual void TickComponent(
		float DeltaTime,
		enum ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// 카메라 거리를 이용하여 위젯을 감추도록 합니다.
	void DistanceToggle();

public :
	// 위젯을 초기화합니다.
	void InitializeCharaterWidget(
		TSubclassOf<class UCharacterWidget> widgetClass, 
		FVector2D drawSize, 
		EWidgetSpace newWidgetSpace = EWidgetSpace::Screen);

	// 위젯의 높이를 설정합니다.
	void SetWidgetHeight(float widgetHeight);

};
