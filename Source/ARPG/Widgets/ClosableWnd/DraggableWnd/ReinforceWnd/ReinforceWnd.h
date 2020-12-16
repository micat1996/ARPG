// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ClosableWnd/DraggableWnd/DraggableWnd.h"
#include "Enums/ReinforceResult.h"
#include "ReinforceWnd.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UReinforceWnd : public UDraggableWnd
{
	GENERATED_BODY()

protected :
	virtual void NativeConstruct() override;

public :
	// 강화를 끝냈을 때 호출되는 블루프린트 이벤트입니다.
	/// - 블루프린트 이벤트는 cpp 에 구현하지 않습니다.
	UFUNCTION(BlueprintImplementableEvent, Category = "Reinforce Event")
	void OnReinforceFinished(EReinforceResult result);
};
