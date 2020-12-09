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
	// �巡���� ���۵� �� ȣ��Ǵ� �븮��
	FDraggingEventSignature OnDraggingStarted;

private :
	// �巡���� üũ�� ��ư
	class UButton* Button_Draggable;

	// �巡�� ��ų ������ ��Ÿ���ϴ�.
	UPROPERTY()
	class UUserWidget * TargetWidget;

	// �巡�� ������ ��Ÿ���� ����
	UPROPERTY()
	bool bIsDragging;

	// ���� �Է� ������ ������ ����
	UPROPERTY()
	FVector2D PrevInputPosition;

	// ���� ���� ��ġ�� ������ ����
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
