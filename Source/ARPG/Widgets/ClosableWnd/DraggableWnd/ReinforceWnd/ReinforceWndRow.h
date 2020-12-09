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
	// 부모 창을 나타냅니다.
	UPROPERTY()
	class UReinforceWnd* ReinforceWnd;

	UPROPERTY()
	class ARPGCharacter* PlayerCharacter;

	UPROPERTY()
	class UPlayerInventoryComponent* PlayerInventory;

	// 표시할 아이템 타입을 나타냅니다.
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
	// 행을 갱신합니다.
	void UpdateRow();

	// 강화 버튼을 눌렀을 때 호출될 메서드
	UFUNCTION()
	void OnReinforceButtonClicked();

public :
	FORCEINLINE void SetReinforceWnd(class UReinforceWnd* reinfoceWnd)
	{ ReinforceWnd = reinfoceWnd; }


	
};
