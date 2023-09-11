// Copyright 2019-2023 ElgSoft. All rights reserved. 

#pragma once
		
#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ElgKEWDragDrop.generated.h"

/**
 * 
 */
UCLASS()
class ELGKISMETEDITORWIDGET_API UElgKEWDragDropBase : public UDragDropOperation
{
	GENERATED_BODY()

public:	

	/* Name of the object you want to drag and drop */
	UPROPERTY(BlueprintReadWrite, Category="KEWDragDrop", meta = (ExposeOnSpawn = "true"))
		FName NameOfDragDropObject;

	virtual void DragCancelled_Implementation(const FPointerEvent& PointerEvent) override;

	virtual void Dragged_Implementation(const FPointerEvent& PointerEvent) override;
};


/**
*	Base Variables dragDropOperation
 */
UCLASS()
class ELGKISMETEDITORWIDGET_API UElgKEWDragDropVariableBase : public UElgKEWDragDropBase
{
	GENERATED_BODY()
};


/**
 * Drag and drop a function
 * NameOfDragDropObject is the name of the function you want to drag and drop.
 * Make sure to pass in Self to Payload
 */
UCLASS()
class ELGKISMETEDITORWIDGET_API UElgKEWDragDropFunction : public UElgKEWDragDropBase
{
	GENERATED_BODY()
};

/**
 * Drag and drop a macro
 * NameOfDragDropObject is the name of the macro you want to drag and drop.
 * Make sure to pass in Self to Payload
 */
UCLASS()
class ELGKISMETEDITORWIDGET_API UElgKEWDragDropMacro : public UElgKEWDragDropBase
{
	GENERATED_BODY()
};


/**
 * NameOfDragDropObject is the name of the local variable you want to drag and drop.
 * Make sure to pass in Self to Payload
 */
UCLASS()
class ELGKISMETEDITORWIDGET_API UElgKEWDragDropLocalVariable : public UElgKEWDragDropVariableBase
{
	GENERATED_BODY()
};


/**
 * NameOfDragDropObject is the name of the variable you want to drag and drop.
 * Make sure to pass in Self to Payload
 */
UCLASS()
class ELGKISMETEDITORWIDGET_API UElgKEWDragDropVariable : public UElgKEWDragDropVariableBase
{
	GENERATED_BODY()
};

/**
 * NameOfDragDropObject is the name of the Input Pin variable you want to drag and drop.
 * Make sure to pass in Self to Payload
 */
UCLASS()
class ELGKISMETEDITORWIDGET_API UElgKEWDragDropVariablePin : public UElgKEWDragDropVariableBase
{
	GENERATED_BODY()
};


/**
 * NameOfDragDropObject is the name of the EventDispatcher you want to drag and drop.
 * Make sure to pass in Self to Payload
 */
UCLASS()
class ELGKISMETEDITORWIDGET_API UElgKEWDragDropEventDispatcher : public UElgKEWDragDropBase
{
	GENERATED_BODY()
};


/**
 * NameOfDragDropObject is the name of the InterfaceFunction you want to drag and drop.
 * Make sure to pass in Self to Payload
 */
UCLASS()
class ELGKISMETEDITORWIDGET_API UElgKEWDragDropInterfaceFunction : public UElgKEWDragDropBase
{
	GENERATED_BODY()
};


/**
 * NameOfDragDropObject is the name of the WidgetVariable you want to drag and drop.
 * Make sure to pass in Self to Payload
 */
UCLASS()
class ELGKISMETEDITORWIDGET_API UElgKEWDragDropWidgetVariable : public UElgKEWDragDropVariableBase
{
	GENERATED_BODY()
};