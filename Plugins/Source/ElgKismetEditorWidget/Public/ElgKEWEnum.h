// Copyright 2019-2023 ElgSoft. All rights reserved. 

#pragma once

#include "CoreMinimal.h"
#include "ElgKEWEnum.generated.h"




UENUM(BlueprintType)
enum class EBPElgKEWFunctionAccess : uint8
{
	Public,
	Protected,
	Private,
};


UENUM(BlueprintType)
enum class EBPElgKEWMessageSeverity : uint8
{
	CriticalError,
	Error,
	PerformanceWarning,
	Warning,
	Info,
};


/*
* BP wrapper for EFunctionFlags
*/
UENUM(BlueprintType)
enum class EElgFunctionFlags : uint8 {
	// Function flags.
	FUNC_None,

	FUNC_Final,				// Function is final (prebindable, non-overridable function).
	FUNC_RequiredAPI,			// Indicates this function is DLL exported/imported.
	FUNC_BlueprintAuthorityOnly,   // Function will only run if the object has network authority
	FUNC_BlueprintCosmetic,   // Function is cosmetic in nature and should not be invoked on dedicated servers
	// FUNC_				= 0x00000010,   // unused.
	// FUNC_				= 0x00000020,   // unused.
	FUNC_Net,   // Function is network-replicated.
	FUNC_NetReliable,   // Function should be sent reliably on the network.
	FUNC_NetRequest,	// Function is sent to a net service
	FUNC_Exec,	// Executable from command line.
	FUNC_Native,	// Native function.
	FUNC_Event,   // Event function.
	FUNC_NetResponse,   // Function response from a net service
	FUNC_Static,   // Static function.
	FUNC_NetMulticast,	// Function is networked multicast Server -> All Clients
	FUNC_UbergraphFunction,   // Function is used as the merge 'ubergraph' for a blueprint, only assigned when using the persistent 'ubergraph' frame
	FUNC_MulticastDelegate,	// Function is a multi-cast delegate signature (also requires FUNC_Delegate to be set!)
	FUNC_Public,	// Function is accessible in all classes (if overridden, parameters must remain unchanged).
	FUNC_Private,	// Function is accessible only in the class it is defined in (cannot be overridden, but function name may be reused in subclasses.  IOW: if overridden, parameters don't need to match, and Super.Func() cannot be accessed since it's private.)
	FUNC_Protected,	// Function is accessible only in the class it is defined in and subclasses (if overridden, parameters much remain unchanged).
	FUNC_Delegate,	// Function is delegate signature (either single-cast or multi-cast, depending on whether FUNC_MulticastDelegate is set.)
	FUNC_NetServer,	// Function is executed on servers (set by replication code if passes check)
	FUNC_HasOutParms,	// function has out (pass by reference) parameters
	FUNC_HasDefaults,	// function has structs that contain defaults
	FUNC_NetClient,	// function is executed on clients
	FUNC_DLLImport,	// function is imported from a DLL
	FUNC_BlueprintCallable,	// function can be called from blueprint code
	FUNC_BlueprintEvent,	// function can be overridden/implemented from a blueprint
	FUNC_BlueprintPure,	// function can be called from blueprint code, and is also pure (produces no side effects). If you set this, you should set FUNC_BlueprintCallable as well.
	FUNC_EditorOnly,	// function can only be called from an editor scrippt.
	FUNC_Const,	// function can be called from blueprint code, and only reads state (never writes state)
	FUNC_NetValidate,	// function must supply a _Validate implementation

	FUNC_AllFlags,
};

/* BP wrapper for UEdGraphSchema_K2 PinType.PinCategory values
* Allowable PinType.PinCategory values
*/
UENUM(BlueprintType)
enum class EBPElgKEWPinCategory : uint8
{
	PC_Exec,
	PC_Boolean,
	PC_Byte,
	// SubCategoryObject is the MetaClass of the Class passed thru this pin, or SubCategory can be 'self'. The DefaultValue string should always be empty, use DefaultObject.
	PC_Class,    
	PC_SoftClass,
	PC_Int,
	PC_Int64,
	PC_Float,
	PC_Name,
	// SubCategoryObject is the UFunction of the delegate signature
	PC_Delegate,    
	// SubCategoryObject is the UFunction of the delegate signature
	PC_MCDelegate,  
	// SubCategoryObject is the Class of the object passed thru this pin, or SubCategory can be 'self'. The DefaultValue string should always be empty, use DefaultObject.
	PC_Object,   
	// SubCategoryObject is the Class of the object passed thru this pin.
	PC_Interface,	
	// SubCategoryObject is the Class of the AssetPtr passed thru this pin.
	PC_SoftObject,		
	PC_String,
	PC_Text,
	// SubCategoryObject is the ScriptStruct of the struct passed thru this pin, 'self' is not a valid SubCategory. DefaultObject should always be empty, the DefaultValue string may be used for supported structs.
	PC_Struct,    
	// Special matching rules are imposed by the node itself
	PC_Wildcard,    
	// SubCategoryObject is the UEnum object passed thru this pin
	PC_Enum,    
	PC_FieldPath,
	// Category=PC_Object or PC_Class, indicates the class being compiled
	PSC_Self,
	// Category=PC_Wildcard, indicates the wildcard will only accept Int, Bool, Byte and Enum pins (used when a pin represents indexing a list)
	PSC_Index,
	// Category=PC_Byte or PC_Int, indicates that the pin represents a bitmask field. SubCategoryObject is either NULL or the UEnum object to which the bitmap is linked for bitflag name specification.
	PSC_Bitmask,
};

/** 
* BP Wrapper of EEdGraphPinDirection
Enum used to define which way data flows into or out of this pin. 
*/
UENUM(BlueprintType)
enum class EBPElgKEWPinDirection : uint8
{
	Input,
	Output,
	MAX,
};

/* 
* BP Wrapper for EPropertyFlags
 ** Flags associated with each property in a class, overriding the
 ** property's default behavior.*
* 
*/
UENUM(BlueprintType)
enum class EBPElgKEWPropertyFlags : uint8
{
	CPF_None,
	CPF_Edit,							///< Property is user-settable in the editor.
	CPF_ConstParm,						///< This is a constant function parameter
	CPF_BlueprintVisible,				///< This property can be read by blueprint code
	CPF_ExportObject,					///< Object can be exported with actor.
	CPF_BlueprintReadOnly,				///< This property cannot be modified by blueprint code
	CPF_Net,							///< Property is relevant to network replication.
	CPF_EditFixedSize,					///< Indicates that elements of an array can be modified, but its size cannot be changed.
	CPF_Parm,							///< Function/When call parameter.
	CPF_OutParm,						///< Value is copied out after function call.
	CPF_ZeroConstructor,				///< memset is fine for construction
	CPF_ReturnParm,						///< Return value.
	CPF_DisableEditOnTemplate,			///< Disable editing of this property on an archetype/sub-blueprint
	CPF_Transient,						///< Property is transient: shouldn't be saved or loaded, except for Blueprint CDOs.
	CPF_Config,							///< Property should be loaded/saved as permanent profile.
	CPF_DisableEditOnInstance,			///< Disable editing on an instance of this class
	CPF_EditConst,						///< Property is uneditable in the editor.
	CPF_GlobalConfig,					///< Load config from base class, not subclass.
	CPF_InstancedReference,				///< Property is a component references.
	CPF_DuplicateTransient,				///< Property should always be reset to the default value during any type of duplication (copy/paste, binary duplication, etc.)
	CPF_SaveGame,						///< Property should be serialized for save games, this is only checked for game-specific archives with ArIsSaveGame
	CPF_NoClear,						///< Hide clear (and browse) button.
	CPF_ReferenceParm,					///< Value is passed by reference; CPF_OutParam and CPF_Param should also be set.
	CPF_BlueprintAssignable,			///< MC Delegates only.  Property should be exposed for assigning in blueprint code
	CPF_Deprecated,						///< Property is deprecated.  Read it from an archive, but don't save it.
	CPF_IsPlainOldData,					///< If this is set, then the property can be memcopied instead of CopyCompleteValue / CopySingleValue
	CPF_RepSkip,						///< Not replicated. For non replicated properties in replicated structs 
	CPF_RepNotify,						///< Notify actors when a property is replicated
	CPF_Interp,							///< interpolatable property for use with matinee
	CPF_NonTransactional,				///< Property isn't transacted
	CPF_EditorOnly,						///< Property should only be loaded in the editor
	CPF_NoDestructor,					///< No destructor
	CPF_AutoWeak,						///< Only used for weak pointers, means the export type is autoweak
	CPF_ContainsInstancedReference,		///< Property contains component references.
	CPF_AssetRegistrySearchable,		///< asset instances will add properties with this flag to the asset registry automatically
	CPF_SimpleDisplay,					///< The property is visible by default in the editor details view
	CPF_AdvancedDisplay,				///< The property is advanced and not visible by default in the editor details view
	CPF_Protected,						///< property is protected from the perspective of script
	CPF_BlueprintCallable,				///< MC Delegates only.  Property should be exposed for calling in blueprint code
	CPF_BlueprintAuthorityOnly,			///< MC Delegates only.  This delegate accepts (only in blueprint) only events with BlueprintAuthorityOnly.
	CPF_TextExportTransient,			///< Property shouldn't be exported to text format (e.g. copy/paste)
	CPF_NonPIEDuplicateTransient,		///< Property should only be copied in PIE
	CPF_ExposeOnSpawn,					///< Property is exposed on spawn
	CPF_PersistentInstance,				///< A object referenced by the property is duplicated like a component. (Each actor should have an own instance.)
	CPF_UObjectWrapper,					///< Property was parsed as a wrapper class like TSubclassOf<T>, FScriptInterface etc., rather than a USomething*
	CPF_HasGetValueTypeHash,			///< This property can generate a meaningful hash value.
	CPF_NativeAccessSpecifierPublic,	///< Public native access specifier
	CPF_NativeAccessSpecifierProtected,	///< Protected native access specifier
	CPF_NativeAccessSpecifierPrivate,	///< Private native access specifier
	CPF_SkipSerialization,				///< Property shouldn't be serialized, can still be exported to text
};


/*
*	BP wrapper for EVariableReplication
*  * Variable network replication options.
* @see https://docs.unrealengine.com/latest/INT/Gameplay/Networking/Replication/
*/
UENUM(BlueprintType)
enum class EBPElgVariableReplication : uint8 {
	/**
	 * Not replicated.
	 */
	None,

	/**
	 * Replicated from server to client.
	 * As values change on the server, client automatically receives new values, if Actor is set to replicate.
	 */
	 Replicated,

	 /**
	  * Replicated from server to client, with a notification function called on clients when a new value arrives.
	  * An event with the name "On Rep <VariableName>" is created.
	  */
	  RepNotify,

	  MAX
};


/*
* BP Wrapper for EGraphType
* Distinguishes between different graph types. Graphs can have different properties; 
*	for example: functions have one entry point, ubergraphs can have multiples. 
*/
UENUM(BlueprintType)
enum class EBPElgKEWGraphType : uint8 {
	GT_Function,
	GT_Ubergraph,
	GT_Macro,
	GT_Animation,
	GT_StateMachine,
};


UENUM(BlueprintType)
enum class EBPElgKEWWResult : uint8
{
	outTrue		UMETA(DisplayName = "True"),
	outFalse	UMETA(DisplayName = "False"),
};

/*
* What "type" of function it is
* 
*/
UENUM(BlueprintType)
enum class EBPElgKEWWFunctionType : uint8
{
	// Regular function created in the blueprint
	Function,
	// Function that are overriding an function.
	Override,
	// 
	InheritedOverride,
	// Function added from an interface implemented in this blueprint
	Interface,
	// Function added from interface inherited from a parent class 
	InheritedInterface,
	// Its a macro function
	Macro,
};

/*
 Blueprint wrapper for EPinContainerType
* Enum used to define what container type a pin represents. *
*/
UENUM(BlueprintType)
enum class EBPElgPinContainerType : uint8
{
	None,
	Array,
	Set,
	Map
};

/*
*  Blueprint wrapper for ETypeTreeFilter
*  Filter flags for GetVariableTypeTree
*/
UENUM(BlueprintType)
enum class EBPElgEPinTypeFilter : uint8
{
	None = 0x00, // No Exec or Wildcards
	AllowExec = 0x01, // Include Executable pins
	AllowWildcard = 0x02, // Include Wildcard pins
	IndexTypesOnly = 0x04, // Exclude all pins that aren't index types
	RootTypesOnly = 0x08	// Exclude all pins that aren't root types
};


/*
*  Blueprint wrapper for ETypeTreeFilter
*  Filter flags for GetVariableTypeTree
*/
UENUM(BlueprintType)
enum class EBPElgBESVariableType : uint8
{
	Member,
	Local,
	Widget,
	Inherited,
	Unknown
};