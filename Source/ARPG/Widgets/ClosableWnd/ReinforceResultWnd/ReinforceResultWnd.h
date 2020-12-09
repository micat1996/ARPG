// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARPG.h"
#include "Widgets/ClosableWnd/ClosableWnd.h"
#include "Enums/ReinforceResult.h"
#include "ReinforceResultWnd.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UReinforceResultWnd : public UClosableWnd
{
	GENERATED_BODY()

private :
	class UImage* Image_Line;
	class UTextBlock* Text_Result;

protected :
	virtual void NativeOnInitialized() override;

public :
	UFUNCTION(BlueprintCallable)
	void SetReinforceResult(EReinforceResult result);
	
};
