// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ClosableWnd/ClosableWnd.h"
#include "ItemDetailWnd.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UItemDetailWnd : public UClosableWnd
{
	GENERATED_BODY()

protected :
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;
	
};
