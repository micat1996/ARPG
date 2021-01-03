// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/ClosableWnd/DraggableWnd/DraggableWnd.h"

#include "Enums/ShopItemType.h"
#include "ShopTradeWnd.generated.h"

DECLARE_DELEGATE_OneParam(FOnOkButtonClickSignature, int32)

UCLASS()
class ARPG_API UShopTradeWnd : public UDraggableWnd
{
	GENERATED_BODY()

public:
	FOnOkButtonClickSignature OnOkButtonClickEvent;

private :
	EShopItemType TradeType;

	// 최대 교환 가능 아이템 개수를 나타냅니다.
	int32 MaxTradeCount;

	int32 Price;

	struct FItemInfo* ItemInfo;

	class UTextBlock* Text_ItemName;
	class UTextBlock* Text_Q;
	class UEditableTextBox* EditableTextBox_TradeCount;
	class UTextBlock* Text_Costs;
	class UButton * Button_Cancel;
	class UButton * Button_Ok;

protected :
	virtual void NativeConstruct() override;
	
public :
	// 교환 창을 초기화합니다.
	/// - itemType : 교환 아이템 타입을 나타냅니다.
	/// - itemInfo : 아이템 정보를 나타냅니다.
	/// - costs : 아이템의 가격을 나타냅니다.
	/// - maxTradeCount : 최대 교환 가능 개수를 나타냅니다.
	///   - 판매 아이템이라면 0 을 전달합니다.
	void InitializeTradeWnd(EShopItemType itemType, 
		struct FItemInfo* itemInfo, int32 costs, int32 maxTradeCount = 0);

private :
	UFUNCTION()
	void OnTradeCountChanged(const FText& text);

	UFUNCTION()
	void OnOkButtonClicked();

};

