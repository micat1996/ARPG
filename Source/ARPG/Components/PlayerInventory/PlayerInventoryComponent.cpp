#include "PlayerInventoryComponent.h"
#include "Engine/DataTable.h"

#include "Structures/EquipItemInfo/EquipItemInfo.h"
#include "Structures/ItemInfo/ItemInfo.h"

#include "Actors/Controllers/RPGPlayerController/RPGPlayerController.h"

#include "Components/ClosableWndController/ClosableWndControllerComponent.h"

#include "Widgets/ClosableWnd/InventoryWnd/InventoryWnd.h"
#include "Widgets/ClosableWnd/InventoryWnd/ItemSlot/ItemSlot.h"

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
	else { UE_LOG(LogTemp, Error, TEXT("PlayerInventoryComponent.cpp :: %d LINE :: DT_EQUIP_ITEM_INFO is not loaded!"), __LINE__); }

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ITEM_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_ItemInfo.DT_ItemInfo'"));
	if (DT_ITEM_INFO.Succeeded()) DTItemInfo = DT_ITEM_INFO.Object;
	else { UE_LOG(LogTemp, Error, TEXT("PlayerInventoryComponent.cpp :: %d LINE :: DT_ITEM_INFO is not loaded!"), __LINE__); }

}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();


	auto gameInst = GetGameInst();
	if (!IsValid(gameInst)) return;
	if (!gameInst->bIsLevelLoaded) return;

	InitializeEquipItems();
	InitializeInventoryItems();


	auto playerController = GetManager(UPlayerManager)->GetPlayerController();
	if (!IsValid(playerController)) return;

	ClosableWndController = playerController->GetClosableWndControllerComponent();
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
	FPlayerInfo* playerInfo = playerManager->GetPlayerInfo();



	for (int32 i = 0; i < playerInfo->InventorySlotCount; ++i)
		InventoryItems.Add(FItemSlotInfo());

	// -- TEST CODE --
	AddItem(FName(TEXT("20000")), 11);
	AddItem(FName(TEXT("20001")), 12);
	AddItem(FName(TEXT("20002")), 13);
	AddItem(FName(TEXT("20003")), 14);
	AddItem(FName(TEXT("20004")), 15);
	AddItem(FName(TEXT("20005")), 16);

	RemoveItem(0, 6);
	RemoveItem(1, 100);
}

void UPlayerInventoryComponent::OpenInventory()
{
	if (!IsValid(ClosableWndController)) return;

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

void UPlayerInventoryComponent::SwapItem(
	UItemSlot* firstItemSlot, UItemSlot* secondItemSlot)
{
	auto tempItemInfo = firstItemSlot->GetItemSlotInfo();
	firstItemSlot->SetItemSlotInfo(secondItemSlot->GetItemSlotInfo());
	secondItemSlot->SetItemSlotInfo(tempItemInfo);

	firstItemSlot->UpdateItemSlot();
	secondItemSlot->UpdateItemSlot();
}

bool UPlayerInventoryComponent::AddItem(FName itemCode, int32 itemCount)
{
	// 추가하려는 아이템 정보를 저장합니다.
	FString contextString;
	FItemInfo* itemInfo = DTItemInfo->FindRow<FItemInfo>(itemCode, contextString);

	// 추가하려는 아이템과 동일한 아이템이 담긴 슬롯 인덱스를 저장할 변수
	int32 slotIndex = INDEX_NONE;

	// 새로운 아이템을 저장할 수 있는 빈 슬롯 인덱스를 저장활 변수
	int32 emptySlotIndex = INDEX_NONE;

	// 배열에서 동일한 아이템 슬롯과 빈 슬롯을 탐색합니다.
	for (int32 i = 0; i < InventoryItems.Num(); ++i)
	{
		// 추가하려는 아이템과 동일한 아이템이 존재한다면
		if (InventoryItems[i].ItemCode == itemCode)
		{
			if ((InventoryItems[i].ItemCount + itemCount) <= itemInfo->MaxCount)
			{
				slotIndex = i;

				// 추가할 수 있는 슬롯 인덱스를 찾았으므로, 탐색을 종료
				break;
			}
		}

		// 빈 슬롯을 찾았다면
		if (emptySlotIndex == INDEX_NONE && InventoryItems[i].IsEmpty())
		{
			// 빈 슬롯 인덱스 저장
			emptySlotIndex = i;
		}
	}

	// 아이템을 추가할 수 있는 슬롯이 존재하지 않는다면
	if (slotIndex == INDEX_NONE && emptySlotIndex == INDEX_NONE)
		// 아이템 추가 실패
		return false;

	// 동일한 아이템을 가진 슬롯이 존재한다면
	if (slotIndex != INDEX_NONE)
	{
		// 찾은 슬롯의 아이템 개수를 증가시킵니다.
		InventoryItems[slotIndex].AddItemCount(itemCount);
	}

	// 동일한 아이템을 가진 슬롯은 존재하지 않지만, 빈 슬롯을 찾았다면
	else if (emptySlotIndex != INDEX_NONE)
	{
		// 찾은 슬롯에 아이템을 추가합니다.
		InventoryItems[emptySlotIndex] = FItemSlotInfo(itemCode, itemCount);
	}

	if (OnInventorySlotChanged.IsBound())
		OnInventorySlotChanged.Broadcast();

	// 아이템 추가 성공
	return true;
}

bool UPlayerInventoryComponent::RemoveItem(int32 slotIndex, int32 removeCount)
{
	// slotIndex 가 InventoryItems 배열 범위를 초과한다면
	if (InventoryItems.Num() <= slotIndex)

		// 제거 실패
		return false;

	// 아이템 정보가 비어있다면
	if (InventoryItems[slotIndex].IsEmpty())

		// 제거 실패
		return false;

	// 아이템 개수 감소
	InventoryItems[slotIndex].ItemCount -= removeCount;

	// 만약 아이템의 개수가 0 이하라면 슬롯을 비웁니다.
	if (InventoryItems[slotIndex].ItemCount <= 0)
		InventoryItems[slotIndex] = FItemSlotInfo();

	if (OnInventorySlotChanged.IsBound())
		OnInventorySlotChanged.Broadcast();

	return true;
}


int32 UPlayerInventoryComponent::GetItemIndex(FItemSlotInfo itemSlotInfo)
{
	return InventoryItems.Find(itemSlotInfo);
}