// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ClosableWnd/ClosableWnd.h"
#include "ShopWnd.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UShopWnd : public UClosableWnd
{
	GENERATED_BODY()
	
private :
	//TSubclassOf<class USaleItem> SaleItemClass;


protected :
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ShopID;

public :
	UShopWnd(const FObjectInitializer& ObjectInitializer);

protected :
	virtual void NativeConstruct() override;

private :
	void InitializeSaleList();


};
