#include "Avalon.h"
#include "LevelEditor.h"
#include "AvalonPythonBridge.h"
#include "AvalonStyle.h"


static const FName AvalonTabName("Avalon");

#define LOCTEXT_NAMESPACE "FAvalonModule"

// This function is triggered when the plugin is staring up
void FAvalonModule::StartupModule()
{

	// Create the Extender that will add content to the menu
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender());

	MenuExtender->AddMenuExtension(
		"LevelEditor",
		EExtensionHook::After,
		NULL,
		FMenuExtensionDelegate::CreateRaw(this, &FAvalonModule::AddMenuEntry)
	);
	ToolbarExtender->AddToolBarExtension(
		"Settings",
		EExtensionHook::After,
		NULL,
		FToolBarExtensionDelegate::CreateRaw(this, &FAvalonModule::AddToobarEntry));


	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);

}

void FAvalonModule::ShutdownModule()
{

}


void FAvalonModule::AddMenuEntry(FMenuBuilder& MenuBuilder)
{
	// Create Section
	MenuBuilder.BeginSection("Avalon", TAttribute<FText>(FText::FromString("Avalon")));
	{
		// Create a Submenu inside of the Section
		MenuBuilder.AddMenuEntry(
			FText::FromString("Tools..."),
			FText::FromString("Pipeline tools"),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateRaw(this, &FAvalonModule::MenuPopup))
		);

		MenuBuilder.AddMenuEntry(
			FText::FromString("Tools dialog..."),
			FText::FromString("Pipeline tools dialog"),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateRaw(this, &FAvalonModule::MenuDialog))
		);

	}
	MenuBuilder.EndSection();
}

void FAvalonModule::AddToobarEntry(FToolBarBuilder& ToolbarBuilder)
{
	ToolbarBuilder.BeginSection(TEXT("Avalon"));
	{
		ToolbarBuilder.AddComboButton(
			FUIAction(),
			FOnGetContent::CreateRaw(this, &FAvalonModule::MenuDialog),
			LOCTEXT("Avalon_label", "Avalon"),
			LOCTEXT("Avalon_tooltip", "Avalon Tools"),
			FSlateIcon(FAvalonStyle::GetStyleSetName(), "Avalon.Logo")
		);
	}
	ToolbarBuilder.EndSection();
}


void FAvalonModule::MenuPopup() {
	UAvalonPythonBridge* bridge = UAvalonPythonBridge::Get();
	bridge->RunInPython_Popup();
}

void FAvalonModule::MenuDialog() {
	UAvalonPythonBridge* bridge = UAvalonPythonBridge::Get();
	bridge->RunInPython_Dialog();
}

IMPLEMENT_MODULE(FAvalonModule, Avalon)
