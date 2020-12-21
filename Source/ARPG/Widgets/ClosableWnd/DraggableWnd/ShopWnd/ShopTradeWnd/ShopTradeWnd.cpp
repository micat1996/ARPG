// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopTradeWnd.h"

#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"

#include "Structures/ItemInfo/ItemInfo.h"
#include "Structures/PlayerInfo/PlayerInfo.h"

#include "Single/GameInstance/ARPGGameInstance.h"
#include "Single/PlayerManager/PlayerManager.h"

void UShopTradeWnd::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateWndSize(500.0f, 300.0f);


	Text_ItemName				= Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ItemName")));
	Text_Q						= Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Q")));
	EditableTextBox_TradeCount	= Cast<UEditableTextBox>(GetWidgetFromName(TEXT("EditableTextBox_TradeCount")));
	Text_Costs					= Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Costs")));
	Button_Cancel				= Cast<UButton>(GetWidgetFromName(TEXT("Button_Cancel")));
	Button_Ok					= Cast<UButton>(GetWidgetFromName(TEXT("Button_Ok")));

	EditableTextBox_TradeCount->OnTextChanged.AddDynamic(this, &UShopTradeWnd::OnTradeCountChanged);


}

void UShopTradeWnd::InitializeTradeWnd(EShopItemType itemType, FItemInfo* itemInfo, int32 costs, int32 maxTradeCount)
{
	// 가격 설정
	Price = costs;

	// 인벤토리 아이템이라면
	if (itemType == EShopItemType::SI_InventoryItem)
		// 최대 판매 가능 개수를 소지중인 아이템 개수로 설정합니다.
		MaxTradeCount = maxTradeCount;

	// 상점 아이템이라면
	else
	{
		// 최대 구매 가능 개수를 소지금으로 구매 가능한 최대 개수로 설정합니다.
		MaxTradeCount =
			(GetManager(UPlayerManager)->GetPlayerInfo()->Money) / costs;
	}

	Text_ItemName->SetText(itemInfo->ItemName);

	Text_Q->SetText(FText::FromString(
		FString(TEXT("(을)를 몇 개 ")) +
		(itemType == EShopItemType::SI_InventoryItem ? TEXT("판매") : TEXT("구매")) +
		FString(TEXT("할까요?"))
	));
}

void UShopTradeWnd::OnTradeCountChanged(const FText& text)
{
	FText prevText = text;

	// 만약 입력되어있는 문자열이 숫자가 아니라면 이전 입력 값으로 변경합니다.
	if (!text.IsNumeric())
	{
		// 입력된 문자열이 존재하지 않는다면
		if (prevText.IsEmpty())
		{
			EditableTextBox_TradeCount->SetText(FText());
		}
		else
		{
			EditableTextBox_TradeCount->SetText(FText::FromString(TEXT("0")));
		}
		Text_Costs->SetText(FText::FromString(
			FString(TEXT("합계 0원"))
		));

		return;
	}


	// 입력된 문자열을 숫자로 변경합니다.
	int32 textToInt = FCString::Atoi(*prevText.ToString());
	/// - FCString : C Style 문자열과 함께 사용되는 함수들을 제공하는 구조체입니다.

	if (textToInt > MaxTradeCount)
	{
		textToInt = MaxTradeCount;
	}

	EditableTextBox_TradeCount->SetText(
		FText::FromString(FString::FromInt(textToInt))
	);

	// 가격을 표시합니다.
	Text_Costs->SetText(FText::FromString(
		FString(TEXT("합계 ")) +
		FString::FromInt(textToInt * Price) +
		FString(TEXT("원"))
	));
}
