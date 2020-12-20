#include "InteractableNpc.h"

#include "Single/GameInstance/ARPGGameInstance.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Widgets/ClosableWnd/ClosableDialogWnd/ClosableDialogWnd.h"
#include "Widgets/ClosableWnd/ShopWnd/ShopWnd.h"

#include "Structures/ShopInfo/ShopInfo.h"

#include "Components/ClosableWndController/ClosableWndControllerComponent.h"


AInteractableNpc::AInteractableNpc()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ST_SHOP_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_ShopInfo.DT_ShopInfo'"));
	if (ST_SHOP_INFO.Succeeded()) DT_ShopInfo = ST_SHOP_INFO.Object;
	else { UE_LOG(LogTemp, Error, TEXT("IInteractableNpc.cpp :: %d LINE :: ST_SHOP_INFO is not loaded!"), __LINE__); }

	static ConstructorHelpers::FClassFinder<UShopWnd> BP_SHOP_WND(
		TEXT("WidgetBlueprint'/Game/Resources/Blueprints/Widgets/ClosableWnd/DraggableWidget/ShopWnd/BP_ShopWnd.BP_ShopWnd_C'"));
	if (BP_SHOP_WND.Succeeded()) ShopWndClass = BP_SHOP_WND.Class;
	else { UE_LOG(LogTemp, Error, TEXT("IInteractableNpc.cpp :: %d LINE :: BP_SHOP_WND is not loaded!"), __LINE__); }


	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKELETAL_MESH_COMPONENT"));
	InteractCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("INTERACT_CAMERA"));

	Mesh->SetupAttachment(GetRootComponent());
	InteractCamera->SetupAttachment(GetRootComponent());
}

void AInteractableNpc::BeginPlay()
{
	Super::BeginPlay();

	GameInst = Cast<UARPGGameInstance>(GetGameInstance());
	PlayerManager = GameInst->GetManagerClass<UPlayerManager>();
}

void AInteractableNpc::OpenDialogWidget()
{

	// Npc ��ȭâ �ּ� ���ϸ�
	FString closableNpcDialogAssetName = TEXT("ClosableDialogWnd_");
	closableNpcDialogAssetName.Append(NpcCode.ToString());

	// Npc ��ȭâ �ּ� ���
	FString closableNpcWndPath = TEXT("/Game/Resources/Blueprints/Widgets/ClosableWnd/ClosableDialogWnd/");
	closableNpcWndPath.Append(closableNpcDialogAssetName);
	closableNpcWndPath.Append(TEXT("."));
	closableNpcWndPath.Append(closableNpcDialogAssetName);
	/// - /Game/Resources/Blueprints/Widgets/ClosableWnd/ClosableDialogWnd/ClosableDialogWnd_90000.ClosableDialogWnd_90000

	// ���� �������Ʈ �ּ� �ε�
	UBlueprint* closableDialogWndAsset = Cast<UBlueprint>(
			GameInst->GetStreamableManager()->LoadSynchronous(
				FSoftObjectPath(closableNpcWndPath)));

	// ���� �������Ʈ �ּ��� ��ȿ�� ���
	if (IsValid(closableDialogWndAsset))
	{
		// �ε��� �������Ʈ �ּ��� �̿��Ͽ� UClass �� ����ϴ�.
		TSubclassOf<UClosableDialogWnd> bpClosableDialog = 
			static_cast< TSubclassOf<UClosableDialogWnd> >(closableDialogWndAsset->GeneratedClass);


			// â ��ü�� �����Ͽ� ȭ�鿡 ǥ���մϴ�.
		UClosableDialogWnd* dialogWnd = 
			PlayerManager->GetPlayerController()->GetClosableWndControllerComponent()->
			AddWnd<UClosableDialogWnd>(bpClosableDialog);

		// �ش� ������ �����ϴ� ��ü�� �ڽ����� �����մϴ�.
		dialogWnd->SetOwnerNpc(this);
	}
	else
	{ UE_LOG(LogTemp, Error, TEXT("InteractableNpc.cpp :: %d LINE :: closableDialogWndAsset is not loaded!"), __LINE__); }

}

void AInteractableNpc::Interaction()
{
	OpenDialogWidget();
}

UClosableWnd* AInteractableNpc::OpenShop(class UClosableWnd* parentWnd, FName shopID)
{
	// shopID �� ��ġ�ϴ� ���� ������ �����մϴ�.
	FString contextString;
	FShopInfo * shopInfo = DT_ShopInfo->FindRow<FShopInfo>(shopID, contextString);

	UShopWnd* shopWnd = Cast<UShopWnd>(parentWnd->CreateChildClosableWnd(ShopWndClass));

	shopWnd->InitializeSaleList(shopInfo->SaleItems);

	return shopWnd;
}