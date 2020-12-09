#include "InteractableNpc.h"

#include "Single/GameInstance/ARPGGameInstance.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Widgets/ClosableWnd/ClosableDialogWnd/ClosableDialogWnd.h"

#include "Components/ClosableWndController/ClosableWndControllerComponent.h"


AInteractableNpc::AInteractableNpc()
{
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
