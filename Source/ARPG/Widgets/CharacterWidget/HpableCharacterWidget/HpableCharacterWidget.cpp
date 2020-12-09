#include "HpableCharacterWidget.h"
#include "Actors/Characters/BaseCharacter.h"
#include "Components/ProgressBar.h"

void UHpableCharacterWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ProgressBar_HP = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_HP")));


}

void UHpableCharacterWidget::InitializeWidget(ABaseCharacter* ownerCharacter)
{
	OwnerCharacter = ownerCharacter;
}

void UHpableCharacterWidget::UpdateHp()
{
	if (!IsValid(OwnerCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("HpableCharacterWidget.cpp :: %d LINE :: OwnerCharacter is not valid!"), __LINE__);
		return;
	}

	ProgressBar_HP->SetPercent(OwnerCharacter->GetHp() / OwnerCharacter->GetMaxHp());



}
