// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARPG.h"
#include "Components/CharacterWidget/CharacterWidgetComponent.h"
#include "EnemyWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UEnemyWidgetComponent : public UCharacterWidgetComponent
{
	GENERATED_BODY()
	
private :

	// 利 困连 努贰胶
	UPROPERTY()
	TSubclassOf<class UHpableCharacterWidget> EnemyCharacterWidgetClass;

	// 利 困连 按眉
	UPROPERTY()
	class UHpableCharacterWidget * EnemyWidgetInstance;

	// 利 某腐磐
	UPROPERTY()
	class AEnemyCharacter * EnemyCharacter;

public :
	UEnemyWidgetComponent();

protected :
	virtual void BeginPlay() override;
public :
	class UHpableCharacterWidget* GetEnemyWidgetInstance();



};
