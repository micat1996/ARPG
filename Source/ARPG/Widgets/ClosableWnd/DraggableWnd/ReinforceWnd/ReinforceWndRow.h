// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ARPG.h"
#include "Blueprint/UserWidget.h"
#include "Enums/EquipitemType.h"
#include "ReinforceWndRow.generated.h"

UCLASS()
class ARPG_API UReinforceWndRow final : public UUserWidget
{
	GENERATED_BODY()

public :
	// �θ� â�� ��Ÿ���ϴ�.
	UPROPERTY()
	class UReinforceWnd* ReinforceWnd;

	UPROPERTY()
	class ARPGCharacter* PlayerCharacter;

	UPROPERTY()
	class UPlayerInventoryComponent* PlayerInventory;

	// ǥ���� ������ Ÿ���� ��Ÿ���ϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipItemType ItemType;

private :
	class UTextBlock* Text_Type;
	class UTextBlock* Text_Level;
	class UTextBlock* Text_Probability;
	class UTextBlock* Text_Costs;
	class UButton* Button_Reinforce;

	class UDataTable* DTEquipItemInfo;

public :
	UReinforceWndRow(const FObjectInitializer& objInitializer);

protected :
	virtual void NativeOnInitialized() override;
	
private :
	// ���� �����մϴ�.
	void UpdateRow();

	// ��ȭ ��ư�� ������ �� ȣ��� �޼���
	UFUNCTION()
	void OnReinforceButtonClicked();

public :
	FORCEINLINE void SetReinforceWnd(class UReinforceWnd* reinfoceWnd)
	{ ReinforceWnd = reinfoceWnd; }


	
};
