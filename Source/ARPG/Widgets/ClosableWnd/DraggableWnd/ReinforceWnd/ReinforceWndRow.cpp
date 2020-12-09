#include "ReinforceWndRow.h"

#include "Actors/Controllers/RPGPlayerController/RPGPlayerController.h"
#include "Actors/Characters/RPGCharacter/RPGCharacter.h"

#include "Single/GameInstance/ARPGGameInstance.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Components/PlayerInventory/PlayerInventoryComponent.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Enums/ReinforceResult.h"

#include "Structures/EquipItemInfo/EquipItemInfo.h"

#include "Widgets/ClosableWnd/DraggableWnd/ReinforceWnd/ReinforceWnd.h"

#include "Engine/DataTable.h"

UReinforceWndRow::UReinforceWndRow(const FObjectInitializer& objInitializer) :
	Super(objInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_EQUIP_ITEM_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_EquipItemInfo.DT_EquipItemInfo'"));

	if (DT_EQUIP_ITEM_INFO.Succeeded())
		DTEquipItemInfo = DT_EQUIP_ITEM_INFO.Object;
	else 
	{ UE_LOG(LogTemp, Error, TEXT("ReinforceWndRow.cpp :: %d LINE :: DT_EQUIP_ITEM_INFO is not loaded!"), __LINE__); }
}


void UReinforceWndRow::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Text_Type = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Type")));
	Text_Level = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Level")));
	Text_Probability = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Probability")));
	Text_Costs = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_Costs")));
	Button_Reinforce = Cast<UButton>(GetWidgetFromName(TEXT("Button_Reinforce")));
	Button_Reinforce->OnClicked.AddDynamic(this, &UReinforceWndRow::OnReinforceButtonClicked);

	UPlayerManager* playerManager = Cast<UARPGGameInstance>(GetGameInstance())->GetManagerClass<UPlayerManager>();
	PlayerCharacter = Cast<ARPGCharacter>(playerManager->GetPlayerController()->GetPawn());
	PlayerInventory = PlayerCharacter->GetPlayerInventory();

	UpdateRow();
}

void UReinforceWndRow::UpdateRow()
{
	FName itemCode = PlayerInventory->GetEquippedItemCode(ItemType);


	FString contextString;
	FEquipItemInfo equipItemInfo = *DTEquipItemInfo->FindRow<FEquipItemInfo>(
		itemCode, contextString);

	FEquipItemInfo nextItemInfo = (equipItemInfo.bMaximumLevel) ? 
		FEquipItemInfo() :
		*DTEquipItemInfo->FindRow<FEquipItemInfo>(
			equipItemInfo.NextReinforcementItemCode, contextString);





#pragma region 아이템 타입 텍스트 설정
	FText itemTypeText = FText();

	switch (equipItemInfo.ItemType)
	{
	case EEquipItemType::EI_Head:	itemTypeText = FText::FromString(TEXT("머리"));		break;
	case EEquipItemType::EI_Mask:	itemTypeText = FText::FromString(TEXT("마스크"));	break;
	case EEquipItemType::EI_Top:	itemTypeText = FText::FromString(TEXT("상의"));		break;
	case EEquipItemType::EI_Bottom:	itemTypeText = FText::FromString(TEXT("하의"));		break;
	case EEquipItemType::EI_Hand:	itemTypeText = FText::FromString(TEXT("건틀릿"));	break;
	case EEquipItemType::EI_Foot:	itemTypeText = FText::FromString(TEXT("신발"));		break;
	case EEquipItemType::EI_Weapon:	itemTypeText = FText::FromString(TEXT("무기"));		break;
	}

	Text_Type->SetText(itemTypeText);

#pragma endregion

#pragma region 아이템 레벨 텍스트 설정
	FText itemLevelText = (equipItemInfo.bMaximumLevel) ?
		FText::FromString(TEXT("MAX")) :
		FText::FromString(
			FString(TEXT("LV ")) +
			FString::FromInt(equipItemInfo.ItemLevel) +
			FString(TEXT(" -> ")) +
			FString::FromInt(nextItemInfo.ItemLevel));

	Text_Level->SetText(itemLevelText);
#pragma endregion

#pragma region 아이템 강화 성공 확률 텍스트 설정
	FText itemProbabilityText = (equipItemInfo.bMaximumLevel) ?
		FText() : FText::FromString(
			FString::Printf(TEXT("%.2f%%"), nextItemInfo.ReinforcementProbability));

	Text_Probability->SetText(itemProbabilityText);
#pragma endregion

#pragma region 아이템 강화 비용 텍스트 설정
	FText itemReinforceCostsText = (equipItemInfo.bMaximumLevel) ?
		FText() : FText::FromString(FString::FromInt(nextItemInfo.ReinforcementCosts));

	Text_Costs->SetText(itemReinforceCostsText);
#pragma endregion

#pragma region 버튼 설정
	// 현재 장착중인 아이템이 최대 레벨이라면 버튼을 비활성화 시킵니다.
	Button_Reinforce->SetIsEnabled(!equipItemInfo.bMaximumLevel);
#pragma endregion
}

void UReinforceWndRow::OnReinforceButtonClicked()
{
	EReinforceResult reinforceResult = PlayerInventory->TryReinforce(ItemType);

	switch (reinforceResult)
	{
	case EReinforceResult::RF_Success:
		UpdateRow();
		PlayerCharacter->UpdateEquipItemMesh(false, ItemType);
		break;

	case EReinforceResult::RF_LackOfMoney:
		break;

	case EReinforceResult::RF_Fail:
		break;
	}

	// 강화 끝 블루프린트 이벤트 실행
	ReinforceWnd->OnReinforceFinished(reinforceResult);
}
