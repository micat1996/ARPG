// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ClosableWnd/ClosableWnd.h"
#include "ClosableDialogWnd.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UClosableDialogWnd : public UClosableWnd
{
	GENERATED_BODY()

private :
	// 창을 소유하는 객체입니다.
	UPROPERTY()
	class AInteractableNpc* OwnerNpc;

public :
	// 창 소유자를 설정합니다.
	void SetOwnerNpc(class AInteractableNpc* ownerNpc);
	
};
