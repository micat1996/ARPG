// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DraggableWidget/DraggableWidget.h"
#include "DraggableWndTitle.generated.h"


UCLASS()
class ARPG_API UDraggableWndTitle : public UDraggableWidget
{
	GENERATED_BODY()

private :
	class UButton* Button_Close;
	class UTextBlock* Text_Title;

protected :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Window Title")
	FText TitleText;

protected :
	virtual void NativeOnInitialized() override;
	
private :
	UFUNCTION()
	FORCEINLINE FText UpdateTitleText()
	{ return TitleText; }

public :
	FORCEINLINE class UButton* GetCloseButton() const
	{ return Button_Close; }

};
