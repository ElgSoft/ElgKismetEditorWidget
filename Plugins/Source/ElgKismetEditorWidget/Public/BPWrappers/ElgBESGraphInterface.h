// Copyright 2019-2021 ElgSoft. All rights reserved. 

#pragma once

#include <ElgKEWStructs.h>
#include "ElgBESBase.h"
#include "ElgBESGraphInterface.generated.h"

/**
 * Blueprint wrapper for an interface a Blueprint can implement.
 */
UCLASS(Blueprintable)
class ELGKISMETEDITORWIDGET_API UElgBESGraphInterface : public UElgBESBase
{
	GENERATED_BODY()

public:

	UPROPERTY()
		FName InterfaceName;
	
	/* if true the interface is inherit from a parent class. */
	UPROPERTY()
		bool bInherit;
	UPROPERTY()
		FName InterfaceClassName;

	/* Return if this interface is inherited from a parent class  */
	UFUNCTION(BlueprintPure, Category = "KismetEditorWidget|Interface", meta = (DisplayName = "GetIsInherited", CompactNodeTitle = "IsInherited?"))
		bool GetIsInherited() { return bInherit; };

	/* Get info about all function this interface has */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Interface", meta = (DisplayName = "GetFunctionsInfo"))
		TArray<FS_ElgFunctionInfo> GetFunctionsInfo();

	/*  Get any function this interface has */
	UFUNCTION(BlueprintCallable, Category = "KismetEditorWidget|Interface", meta = (DisplayName = "GetFunctions"))
		TArray<UElgBESGraphInterfaceFunction*> GetInterfaceFunctions();

	
	FBPInterfaceDescription Description;
	UClass* InterfaceClassPtr;
	UBlueprint* BlueprintPtr;


	/* Get all the interface UFunctions */ 
	TArray<UFunction*> GetFunctions();
	// Get the graph the function is on, if any.
	UEdGraph* GetFunctionsGraph(UFunction* InFunction);

	

	bool ValidateAndUpdate();

	void Setup(UBlueprint* InBlueprint, FBPInterfaceDescription InDescription);
	void Setup(UBlueprint* InBlueprint, UClass* InInterfaceClass);

	static UElgBESGraphInterface* MakeGraphInterface(UBlueprint* InBlueprint, FBPInterfaceDescription InDescription);
	/*  */
	static UElgBESGraphInterface* MakeGraphInterface(UBlueprint* InBlueprint, UClass* InInterfaceClass);
	/* Get all the interfaces the Blueprint has. */
	static TArray<UElgBESGraphInterface*> GetBlueprintInterfaces(UBlueprint* InBlueprint, const bool bIncludeInherent = false);
	static UElgBESGraphInterface* GetBlueprintInterface(UBlueprint* InBlueprint, const FName InName);

	/* will load any Blueprint assets that match */
	static TArray<FName> GetInterfaces(UBlueprint* InBlueprint);


	virtual FName GetNameInternal() override;
	
};
