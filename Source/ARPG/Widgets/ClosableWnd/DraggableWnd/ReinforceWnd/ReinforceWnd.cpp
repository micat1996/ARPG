#include "ReinforceWnd.h"

#include "Widgets/ClosableWnd/DraggableWnd/ReinforceWnd/ReinforceWndRow.h"


void UReinforceWnd::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Cast<UReinforceWndRow>(GetWidgetFromName(TEXT("BP_Reinforce_Head")))->SetReinforceWnd(this);
	Cast<UReinforceWndRow>(GetWidgetFromName(TEXT("BP_Reinforce_Mask")))->SetReinforceWnd(this);
	Cast<UReinforceWndRow>(GetWidgetFromName(TEXT("BP_Reinforce_Top")))->SetReinforceWnd(this);
	Cast<UReinforceWndRow>(GetWidgetFromName(TEXT("BP_Reinforce_Bottom")))->SetReinforceWnd(this);
	Cast<UReinforceWndRow>(GetWidgetFromName(TEXT("BP_Reinforce_Hand")))->SetReinforceWnd(this);
	Cast<UReinforceWndRow>(GetWidgetFromName(TEXT("BP_Reinforce_Foot")))->SetReinforceWnd(this);


}