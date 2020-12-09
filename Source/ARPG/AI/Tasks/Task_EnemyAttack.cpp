#include "Task_EnemyAttack.h"

#include "../../Actors/Controllers/EnemyController/EnemyController.h"
#include "../../Actors/Characters/EnemyCharacter/EnemyCharacter.h"
#include "../../Components/EnemyAttack/EnemyAttackComponent.h"

EBTNodeResult::Type UTask_EnemyAttack::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyController* enemyController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* enemyCharacter = enemyController->GetEnemyCharacter();

	enemyCharacter->GetEnemyAttack()->PlayAttackAnimation();

	return EBTNodeResult::Type::Succeeded;
}