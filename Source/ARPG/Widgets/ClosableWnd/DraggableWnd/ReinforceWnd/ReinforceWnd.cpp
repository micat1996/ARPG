#include "ReinforceWnd.h"

#include "Widgets/ClosableWnd/DraggableWnd/ReinforceWnd/ReinforceWndRow.h"


void UReinforceWnd::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateWndSize(700.0f, 600.0f);

	Cast<UReinforceWndRow>(GetWidgetFromName(TEXT("BP_Reinforce_Head")))->SetReinforceWnd(this);
	Cast<UReinforceWndRow>(GetWidgetFromName(TEXT("BP_Reinforce_Mask")))->SetReinforceWnd(this);
	Cast<UReinforceWndRow>(GetWidgetFromName(TEXT("BP_Reinforce_Top")))->SetReinforceWnd(this);
	Cast<UReinforceWndRow>(GetWidgetFromName(TEXT("BP_Reinforce_Bottom")))->SetReinforceWnd(this);
	Cast<UReinforceWndRow>(GetWidgetFromName(TEXT("BP_Reinforce_Hand")))->SetReinforceWnd(this);
	Cast<UReinforceWndRow>(GetWidgetFromName(TEXT("BP_Reinforce_Foot")))->SetReinforceWnd(this);


}