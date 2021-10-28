// Copyright 2019-2021 ElgSoft. All rights reserved. 


#include "ElgKEWViewerWidget.h"
#include <EditorUtilityWidget.h>
#include <Engine/Blueprint.h>
#include <Modules/ModuleManager.h>
#include <ContentBrowserModule.h>
#include <Widgets/Layout/SScrollBox.h>
#include <IContentBrowserSingleton.h>
#include <ElgKEWEditorSubSystem.h>
#include "ElgKEW_Log.h"


void SElgKEWViewerWidget::Construct(const FArguments& InArgs)
{
	BlueprintEditorPtr = InArgs._Kismet2;

	// Create the border that all of the content will get stuffed into
	ChildSlot
	[
		SNew(SVerticalBox)
		.AddMetaData<FTagMetaData>(FTagMetaData(TEXT("ElgKEWViewer")))
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Top)
		.AutoHeight()
		[
			SAssignNew(AssetPickerAnchor, SComboButton)
			.ButtonStyle(FEditorStyle::Get(), "PropertyEditor.AssetComboStyle")
			.OnGetMenuContent(this, &SElgKEWViewerWidget::MakeWidgetPicker)
			.ForegroundColor(this, &SElgKEWViewerWidget::OnGetComboForeground)
			.ContentPadding(FMargin(2, 2, 2, 1))
			.ButtonColorAndOpacity(this, &SElgKEWViewerWidget::OnGetWidgetBackground)
			.MenuPlacement(MenuPlacement_BelowAnchor)
			.ToolTipText(FText::FromString("Select Editor Widget to display"))
			.ButtonContent()
			[
				SNew(STextBlock)
				.ColorAndOpacity(this, &SElgKEWViewerWidget::OnGetComboForeground)
				.TextStyle(FEditorStyle::Get(), "PropertyEditor.AssetClass")
				.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
				.Text(this, &SElgKEWViewerWidget::OnGetComboTextValue)
			]			
		]

		+ SVerticalBox::Slot()
		.Padding(5.0f)
		.FillHeight(1.0f)
		[
			SAssignNew(ContextualBorderWidget, SBorder)
			.Padding(0)
			.BorderImage(FEditorStyle::GetBrush("NoBorder"))
			]
		];
}


SElgKEWViewerWidget::~SElgKEWViewerWidget()
{
	editorWidget = nullptr;
	CurrentAssetData = FAssetData();

	if (BlueprintEditorPtr.IsValid()) {
		if (UBlueprint* bp = BlueprintEditorPtr.Pin()->GetBlueprintObj()) {
			bp->OnCompiled().RemoveAll(this);
		}		
	}
}


void SElgKEWViewerWidget::HandleOnCompiled(class UBlueprint* InBlueprint)
{
	UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
	if (editorWidget) kewSS->WidgetBPEditorMap.Remove(editorWidget);
	InBlueprint->OnCompiled().RemoveAll(this);
	MakeKismetEditorWidget();
}


void SElgKEWViewerWidget::MakeKismetEditorWidget()
{	
	if (CurrentAssetData.IsValid()) {
		if (UBlueprint* blueprint = Cast<UBlueprint>(LoadObject<UObject>(NULL, *CurrentAssetData.GetExportTextName()))) {
			if (TSubclassOf<UEditorUtilityWidget> widgetClass = (TSubclassOf<UEditorUtilityWidget>)blueprint->GeneratedClass) {
				if (UWorld* world = GEditor->GetEditorWorldContext().World()) {
					editorWidget = CreateWidget<UEditorUtilityWidget>(world, widgetClass);
					if (editorWidget) {
						// let the subsystem know what blueprint editor the editor widget are added to, in case it want to access the blueprint.
						UElgKEWEditorSubSystem* kewSS = GEditor->GetEditorSubsystem<UElgKEWEditorSubSystem>();
						kewSS->WidgetBPEditorMap.Add(editorWidget, BlueprintEditorPtr);
						BlueprintEditorPtr.Pin()->GetBlueprintObj()->OnCompiled().AddRaw(this, &SElgKEWViewerWidget::HandleOnCompiled);
						TSharedRef<SScrollBox> content =
							SNew(SScrollBox)
							+ SScrollBox::Slot()
							[
								editorWidget->TakeWidget()
							];
						ContextualBorderWidget->SetContent(content);
					}
				}
			}
		}
	}
}


TSharedRef<class SWidget> SElgKEWViewerWidget::MakeWidgetPicker()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	// Can't access EditorUtilityWidgetBlueprint::StaticClass() so we just use the name 
	FName className = FName("EditorUtilityWidgetBlueprint");

	// Configure filter for asset picker
	FAssetPickerConfig AssetPickerConfig;
	AssetPickerConfig.Filter.ClassNames.Add(className);
	AssetPickerConfig.Filter.bRecursiveClasses = true;
	AssetPickerConfig.bAllowNullSelection = true;
	AssetPickerConfig.OnAssetSelected = FOnAssetSelected::CreateSP(this, &SElgKEWViewerWidget::OnAssetSelectedFromPicker);
	AssetPickerConfig.OnAssetEnterPressed = FOnAssetEnterPressed::CreateSP(this, &SElgKEWViewerWidget::OnAssetEnterPressedInPicker);
	AssetPickerConfig.InitialAssetViewType = EAssetViewType::List;
	AssetPickerConfig.bAllowDragging = false;

	return
		SNew(SBox)
		.HeightOverride(300)
		.WidthOverride(300)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Menu.Background"))
			[
				ContentBrowserModule.Get().CreateAssetPicker(AssetPickerConfig)
			]
		];
}


void SElgKEWViewerWidget::OnAssetSelectedFromPicker(const struct FAssetData& InAssetData)
{
	CurrentAssetData = InAssetData;
	AssetPickerAnchor->SetIsOpen(false);

	if (CurrentAssetData.IsValid()) {
		UE_LOG(LogElgKismetEditorWidget, Log, TEXT("OnAssetPicked:: Asset picked [%s]"), *CurrentAssetData.GetExportTextName());
		MakeKismetEditorWidget();
	}
	else {
		TSharedRef<SVerticalBox> content =
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight();
		ContextualBorderWidget->SetContent(content);
	}
}


void SElgKEWViewerWidget::OnAssetEnterPressedInPicker(const TArray<struct FAssetData>& InSelectedAssets)
{
	if (InSelectedAssets.Num()) {
		OnAssetSelectedFromPicker(InSelectedAssets[0]);
	}
}


FText SElgKEWViewerWidget::OnGetComboTextValue() const
{
	FText Value = FText::FromString("Select Asset");

	if (CurrentAssetData.IsValid()) {
		Value = FText::FromString(CurrentAssetData.AssetName.ToString());
	}
	return Value;
}


FSlateColor SElgKEWViewerWidget::OnGetComboForeground() const
{
	float Alpha = IsHovered() ? 1.0f : 0.6f;
	return FSlateColor(FLinearColor(1.f, 1.f, 1.f, Alpha));
}


FSlateColor SElgKEWViewerWidget::OnGetWidgetForeground() const
{
	float Alpha = IsHovered() ? 1.0f : 0.15f;
	return FSlateColor(FLinearColor(1.f, 1.f, 1.f, Alpha));
}


FSlateColor SElgKEWViewerWidget::OnGetWidgetBackground() const
{
	float Alpha = IsHovered() ? 0.8f : 0.4f;
	return FSlateColor(FLinearColor(1.f, 1.f, 1.f, Alpha));
}
