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

	// Npc 대화창 애셋 파일명
	FString closableNpcDialogAssetName = TEXT("ClosableDialogWnd_");
	closableNpcDialogAssetName.Append(NpcCode.ToString());

	// Npc 대화창 애셋 경로
	FString closableNpcWndPath = TEXT("/Game/Resources/Blueprints/Widgets/ClosableWnd/ClosableDialogWnd/");
	closableNpcWndPath.Append(closableNpcDialogAssetName);
	closableNpcWndPath.Append(TEXT("."));
	closableNpcWndPath.Append(closableNpcDialogAssetName);
	/// - /Game/Resources/Blueprints/Widgets/ClosableWnd/ClosableDialogWnd/ClosableDialogWnd_90000.ClosableDialogWnd_90000

	// 위젯 블루프린트 애셋 로드
	UBlueprint* closableDialogWndAsset = Cast<UBlueprint>(
			GameInst->GetStreamableManager()->LoadSynchronous(
				FSoftObjectPath(closableNpcWndPath)));

	// 위젯 블루프린트 애셋이 유효한 경우
	if (IsValid(closableDialogWndAsset))
	{
		// 로드한 블루프린트 애셋을 이용하여 UClass 를 얻습니다.
		TSubclassOf<UClosableDialogWnd> bpClosableDialog = 
			static_cast< TSubclassOf<UClosableDialogWnd> >(closableDialogWndAsset->GeneratedClass);


			// 창 객체를 생성하여 화면에 표시합니다.
		UClosableDialogWnd* dialogWnd = 
			PlayerManager->GetPlayerController()->GetClosableWndControllerComponent()->
			AddWnd<UClosableDialogWnd>(bpClosableDialog);

		// 해당 위젯을 소유하는 객체를 자신으로 설정합니다.
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
	// shopID 와 일치하는 상점 정보를 저장합니다.
	FString contextString;
	FShopInfo * shopInfo = DT_ShopInfo->FindRow<FShopInfo>(shopID, contextString);

	UShopWnd* shopWnd = Cast<UShopWnd>(parentWnd->CreateChildClosableWnd(ShopWndClass));

	shopWnd->InitializeSaleList(shopInfo->SaleItems);

	return shopWnd;
}