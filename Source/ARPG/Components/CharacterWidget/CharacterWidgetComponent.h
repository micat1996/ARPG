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
	// ������ ǥ�õǴ� �ִ� �Ÿ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDrawableDistance;
	
	// �÷��̾� ī�޶�
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
	// ī�޶� �Ÿ��� �̿��Ͽ� ������ ���ߵ��� �մϴ�.
	void DistanceToggle();

public :
	// ������ �ʱ�ȭ�մϴ�.
	void InitializeCharaterWidget(
		TSubclassOf<class UCharacterWidget> widgetClass, 
		FVector2D drawSize, 
		EWidgetSpace newWidgetSpace = EWidgetSpace::Screen);

	// ������ ���̸� �����մϴ�.
	void SetWidgetHeight(float widgetHeight);

};
