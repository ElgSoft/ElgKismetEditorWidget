// Copyright 2019-2021 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include <WorkflowOrientedApp/WorkflowTabFactory.h>
#include <ElgKEWSettings.h>


struct FElgKEWTabFactory : public FWorkflowTabFactory
{
public:

	FElgKEWTabFactory(TSharedPtr<class FAssetEditorToolkit> InHostingApp, FName InIdentifier, FS_ElgKEWConfig InWidgetInfo);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

};


