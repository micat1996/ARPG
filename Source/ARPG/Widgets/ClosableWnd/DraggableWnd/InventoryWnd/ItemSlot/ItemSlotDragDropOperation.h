// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemSlot.h"
#include "ItemSlotDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UItemSlotDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public :
	// 드래깅을 시작한 슬롯을 나타냅니다.
	UItemSlot* DraggingSlot;
	
};
