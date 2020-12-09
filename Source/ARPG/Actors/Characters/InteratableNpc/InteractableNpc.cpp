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
