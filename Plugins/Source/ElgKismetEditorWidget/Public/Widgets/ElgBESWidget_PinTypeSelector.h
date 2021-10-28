// Copyright 2019-2021 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include <ElgKEWStructs.h>
#include "ElgBESWidget_PinTypeSelector.generated.h"

class SPinTypeSelector;

/**
 * Widget for modifying the type for a variable
 */
UCLASS(meta = (DisplayName = "PinTypeSelector"))
class ELGKISMETEDITORWIDGET_API UElgBESWidget_PinTypeSelector : public UWidget
{
	GENERATED_UCLASS_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnElgBESPinTypeSelectorChanged, FS_ElgGraphPinType, InPinType);


public:

	/* Filter what pin types the Selector should list */
	UPROPERTY(EditAnywhere, Category = Content)
		EBPElgEPinTypeFilter PinTypeFilter;

	UPROPERTY()
		FS_ElgGraphPinType VariablePinType;


	/** Called when the pin is changed interactively by the user */
	UPROPERTY(BlueprintAssignable, Category = "Widget|PinTypeSelector|Events")
		FOnElgBESPinTypeSelectorChanged OnPinTypeChanged;

public:

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

	//~ Begin UObject Interface
	virtual void PostLoad() override;
	//~ End UObject Interface
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
	//~ End UWidget Interface

	UFUNCTION(BlueprintCallable, Category = "Widget|PinTypeSelector")
		void SetPinType(FS_ElgGraphPinType InPinType);

	UFUNCTION(BlueprintPure, Category = "Widget|PinTypeSelector")
		FS_ElgGraphPinType GetPinType();

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget

	FEdGraphPinType HandleOnGetVarType()  const;;
	void HandleOnVarTypeChanged(const FEdGraphPinType& NewPinType);


protected:
	TSharedPtr<SPinTypeSelector> MyPinTypeSelector;

	//PROPERTY_BINDING_IMPLEMENTATION(FName, VariableName);
};
