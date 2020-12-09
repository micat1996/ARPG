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

	// �� ���� Ŭ����
	UPROPERTY()
	TSubclassOf<class UHpableCharacterWidget> EnemyCharacterWidgetClass;

	// �� ���� ��ü
	UPROPERTY()
	class UHpableCharacterWidget * EnemyWidgetInstance;

	// �� ĳ����
	UPROPERTY()
	class AEnemyCharacter * EnemyCharacter;

public :
	UEnemyWidgetComponent();

protected :
	virtual void BeginPlay() override;
public :
	class UHpableCharacterWidget* GetEnemyWidgetInstance();



};
