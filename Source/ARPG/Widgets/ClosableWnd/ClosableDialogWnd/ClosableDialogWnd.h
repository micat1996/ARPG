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
	// â�� �����ϴ� ��ü�Դϴ�.
	UPROPERTY()
	class AInteractableNpc* OwnerNpc;

public :
	// â �����ڸ� �����մϴ�.
	void SetOwnerNpc(class AInteractableNpc* ownerNpc);
	
};
