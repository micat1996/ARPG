// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DraggableWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FDraggingEventSignature)

UCLASS()
class ARPG_API UDraggableWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	// 드래깅이 시작될 때 호출되는 대리자
	FDraggingEventSignature OnDraggingStarted;

private :
	// 드래깅을 체크할 버튼
	class UButton* Button_Draggable;

	// 드래깅 시킬 위젯을 나타냅니다.
	UPROPERTY()
	class UUserWidget * TargetWidget;

	// 드래깅 중임을 나타내는 변수
	UPROPERTY()
	bool bIsDragging;

	// 이전 입력 위지를 저장할 변수
	UPROPERTY()
	FVector2D PrevInputPosition;

	// 이전 위젯 위치를 저장할 변수
	FVector2D PrevWidgetPosition;
	
private :
	void DragWidget();

public :
	FORCEINLINE void SetTargetWidget(UUserWidget* targetWidget)
	{ TargetWidget = targetWidget; }

protected :
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void OnWidgetDragStarted();

	UFUNCTION()
	void OnWidgetDragFinished();


};
