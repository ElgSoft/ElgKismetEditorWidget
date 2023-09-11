// Copyright 2019-2023 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include <WorkflowOrientedApp/WorkflowTabFactory.h>
#include <ElgKEWSettings.h>


struct FElgKEWTabFactory : FWorkflowTabFactory
{
	FElgKEWTabFactory(TSharedPtr<FAssetEditorToolkit> InHostingApp, FName InIdentifier, FS_ElgKEWConfig InWidgetInfo);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

};


