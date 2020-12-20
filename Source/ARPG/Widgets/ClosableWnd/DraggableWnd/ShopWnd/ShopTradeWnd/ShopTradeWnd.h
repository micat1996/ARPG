// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ClosableWnd/DraggableWnd/DraggableWnd.h"
#include "Enums/ShopItemType.h"
#include "ShopTradeWnd.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UShopTradeWnd : public UDraggableWnd
{
	GENERATED_BODY()

private :
	// �ִ� ���� / �Ǹ� ���� ������ ������ ��Ÿ���ϴ�.
	UPROPERTY()
	int32 MaxTradeCount;

	class UTextBlock* Text_ItemName;
	class UTextBlock* Text_Q;
	class UEditableTextBox* EditableTextBox_TradeCount;
	class UTextBlock* Text_Costs;
	class UButton* Button_Cancel;
	class UButton* Button_Ok;

protected :
	virtual void NativeConstruct() override;

public :
	// ��ȯ â�� �ʱ�ȭ�մϴ�.
	/// - itemType : ��ȯ ������ Ÿ���� ��Ÿ���ϴ�.
	/// - itemInfo : ������ ������ ��Ÿ���ϴ�.
	/// - maxTradeCount : �ִ� ��ȯ ���� ������ ��Ÿ���ϴ�.
	///   - �Ǹ� �������̶�� 0 �� �����մϴ�.
	void InitializeTradeWnd(EShopItemType itemType, struct FItemInfo* itemInfo, int32 costs, int32 maxTradeCount = 0);

private :
	UFUNCTION()
	void OnTradeCountChanged(const FText& text);

	
};
