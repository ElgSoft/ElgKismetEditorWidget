// Copyright 2019-2023 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include <WorkflowOrientedApp/WorkflowTabFactory.h>


struct	FElgKEWViewerTabFactory : FWorkflowTabFactory
{
	FElgKEWViewerTabFactory(TSharedPtr<FAssetEditorToolkit> InHostingApp);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;

	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

};
