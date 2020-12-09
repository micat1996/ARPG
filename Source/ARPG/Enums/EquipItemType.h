// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "EquipItemType.generated.h"

UENUM(BlueprintType)
enum class EEquipItemType : uint8
{
	EI_Head		UMETA(DisplayName = Head),
	EI_Mask		UMETA(DisplayName = Mask),
	EI_Top		UMETA(DisplayName = Top),
	EI_Bottom	UMETA(DisplayName = Bottom),
	EI_Hand		UMETA(DisplayName = Hand),
	EI_Foot		UMETA(DisplayName = Foot),
	EI_Weapon	UMETA(DisplayName = Weapon),

};
