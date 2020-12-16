// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ClosableWnd/ClosableWnd.h"
#include "DraggableWnd.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UDraggableWnd : public UClosableWnd
{
	GENERATED_BODY()

private :
	class UDraggableWndTitle* BP_DraggableWndTitle;

protected :
	virtual void NativeConstruct() override;
	
};
