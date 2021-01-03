// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARPG.h"
#include "Engine/LevelScriptActor.h"


#include "LoadingScreenLevel.generated.h"

UCLASS()
class ARPG_API ALoadingScreenLevel : public ALevelScriptActor
{
	GENERATED_BODY()

private :
	TSubclassOf<class UUserWidget> BP_LoadingScreen;

public :
	ALoadingScreenLevel();

protected :
	virtual void BeginPlay() override;

	
};
