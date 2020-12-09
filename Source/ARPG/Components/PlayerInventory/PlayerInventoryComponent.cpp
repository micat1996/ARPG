#include "PlayerInventoryComponent.h"
#include "Engine/DataTable.h"

#include "Structures/EquipItemInfo/EquipItemInfo.h"

#include "Actors/Controllers/RPGPlayerController/RPGPlayerController.h"

#include "Components/ClosableWndController/ClosableWndControllerComponent.h"

#include "Widgets/ClosableWnd/InventoryWnd/InventoryWnd.h"
#include "Widgets/CLosableWnd/InventoryWnd/ItemSlot/ItemSlot.h"

#include "Single/GameInstance/ARPGGameInstance.h"
#include "Single/PlayerManager/PlayerManager.h"




UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UInventoryWnd> BP_INVENTORY(
		TEXT("WidgetBlueprint'/Game/Resources/Blueprints/Widgets/ClosableWnd/DraggableWidget/InventoryWnd/BP_Inventory.BP_Inventory_C'"));
	if (BP_INVENTORY.Succeeded()) BPInventoryWndClass = BP_INVENTORY.Class;

	static ConstructorHelpers::FObjectFinder<UDataTable>  DT_EQUIP_ITEM_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_EquipItemInfo.DT_EquipItemInfo'"));
	if (DT_EQUIP_ITEM_INFO.Succeeded()) DTEquipItemInfo = DT_EQUIP_ITEM_INFO.Object;
	else
	{ UE_LOG(LogTemp, Error, TEXT("PlayerInventoryComponent.cpp :: %d LINE :: DT_EQUIP_ITEM_INFO is not loaded!"), __LINE__); }
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeEquipItems();
	InitializeInventoryItems();

	ClosableWndController = GetManager(UPlayerManager)->GetPlayerController()->
		GetClosableWndControllerComponent();
}

void UPlayerInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerInventoryComponent::InitializeEquipItems()
{
	EquipItems.Empty();
	EquipItems.Add(EEquipItemType::EI_Head,		TEXT("10000"));
	EquipItems.Add(EEquipItemType::EI_Mask,		TEXT("11000"));
	EquipItems.Add(EEquipItemType::EI_Top,		TEXT("12000"));
	EquipItems.Add(EEquipItemType::EI_Bottom,	TEXT("13000"));
	EquipItems.Add(EEquipItemType::EI_Hand,		TEXT("14000"));
	EquipItems.Add(EEquipItemType::EI_Foot,		TEXT("15000"));
	EquipItems.Add(EEquipItemType::EI_Weapon,	TEXT("16000"));
}

void UPlayerInventoryComponent::InitializeInventoryItems()
{
	UPlayerManager* playerManager = GetManager(UPlayerManager);
	FPlayerInfo * playerInfo = playerManager->GetPlayerInfo();

	for (int32 i = 0; i < playerInfo->InventorySlotCount; ++i)
		InventoryItems.Add(FItemSlotInfo());


	//  -- TEST CODE --
	InventoryItems[0] = FItemSlotInfo(FName(TEXT("20000")), 11);
	InventoryItems[5] = FItemSlotInfo(FName(TEXT("20005")), 7);
}

void UPlayerInventoryComponent::OpenInventory()
{
	// 인벤토리 창 위젯을 생성합니다.
	InventoryWnd = ClosableWndController->AddWnd<UInventoryWnd>(BPInventoryWndClass);

	// 인벤토리 컴포넌트를 설정합니다.
	InventoryWnd->InitializeInventoryWnd(this);
}

void UPlayerInventoryComponent::CloseInventory()
{
	ClosableWndController->CloseWnd(false, InventoryWnd);

	InventoryWnd = nullptr;
}


// 장비 아이템 Mesh 를 갱신합니다.
void UPlayerInventoryComponent::UpdateEquipItemMesh(EEquipItemType equipitemType,
	class USkeletalMeshComponent* refSkMeshToUpdate,
	class USkeletalMeshComponent* refSetSkMeshToUpdate)
{
	// 장착중인 아이템 코드를 저장합니다.
	FName itemCode = EquipItems[equipitemType];

	// 아이템 코드를 이용하여 아이템 정보를 얻습니다.
	FString contextString;
	FEquipItemInfo* equipItemInfo = DTEquipItemInfo->FindRow<FEquipItemInfo>(
		itemCode, contextString);

	// SkMesh 경로가 비었다면 전달된 SkMesh Component 가 표시하는 Mesh 를 비웁니다.
	if (equipItemInfo->SKMeshPath.ToString().IsEmpty())
	{
		refSkMeshToUpdate->SetSkeletalMesh(nullptr);

		if (equipitemType == EEquipItemType::EI_Hand)
			refSetSkMeshToUpdate->SetSkeletalMesh(nullptr);
	}

	// 아니라면 표시하는 Mesh 를 설정합니다.
	else
	{
		// GameInstance
		UARPGGameInstance* gameInst = Cast<UARPGGameInstance>(GetWorld()->GetGameInstance());
		USkeletalMesh * itemMesh = Cast<USkeletalMesh>(gameInst->GetStreamableManager()->LoadSynchronous(
			equipItemInfo->SKMeshPath));

		// 장비 타입이 팔 타입이라면
		if (equipitemType == EEquipItemType::EI_Hand)
		{
			auto setMesh = Cast<USkeletalMesh>(
				gameInst->GetStreamableManager()->LoadSynchronous(
					equipItemInfo->SKMeshSetPath));

			refSetSkMeshToUpdate->SetSkeletalMesh(setMesh);
		}
		
		refSkMeshToUpdate->SetSkeletalMesh(itemMesh);
	}
}

EReinforceResult UPlayerInventoryComponent::TryReinforce(EEquipItemType itemType)
{
	// 현재 장착되어있는 아이템 정보를 얻습니다.
	FString contextStirng;
	auto equipItemInfo = DTEquipItemInfo->FindRow<FEquipItemInfo>(GetEquippedItemCode(itemType), contextStirng);

	// 다음 아이템 정보를 얻습니다.
	auto nextEquipItemInfo = DTEquipItemInfo->FindRow<FEquipItemInfo>(
		equipItemInfo->NextReinforcementItemCode, contextStirng);


	// 강화에 성공했다면
	if (nextEquipItemInfo->ReinforcementProbability >= FMath::FRandRange(1.0f, 100.0f))
	{
		// 장착중인 아이템을 강화된 아이템으로 설정합니다.
		EquipItems[itemType] = nextEquipItemInfo->ItemCode;
		//EquipItems[itemType] = equipItemInfo->NextReinforcementItemCode;

		// 강화 결과 반환
		return EReinforceResult::RF_Success;
	}


	// 강화 결과 반환
	return EReinforceResult::RF_Fail;
}

void UPlayerInventoryComponent::SwapItem(UItemSlot* firstItemSlot, UItemSlot* secondItemSlot)
{
	auto tempItemInfo = firstItemSlot->GetItemSlotInfo();
	firstItemSlot->SetItemSlotInfo(secondItemSlot->GetItemSlotInfo());
	secondItemSlot->SetItemSlotInfo(tempItemInfo);
}

