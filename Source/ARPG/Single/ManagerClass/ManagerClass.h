// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG.h"
#include "UObject/NoExportTypes.h"
#include "ManagerClass.generated.h"

#ifndef MANAGER_INSTANCE
#define MANAGER_INSTANCE
#define GetManager(managerClassType) (Cast<UARPGGameInstance>(GetWorld()->GetGameInstance())->GetManagerClass<managerClassType>())
#endif

UCLASS(Abstract)
class ARPG_API UManagerClass : public UObject
{
    GENERATED_BODY()

public:
    virtual void InitManagerClass() PURE_VIRTUAL(UManagerClass::InitManagerClass, );
};
