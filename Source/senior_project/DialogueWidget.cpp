// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidget.h"

void UDialogueWidget::NativeConstruct() {

	Super::NativeConstruct();

	CanvasPanel = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
	
	Border = NewObject<UBorder>(this);
	CanvasPanel->AddChild(Border);

	Text = NewObject<UTextBlock>(this);
	Text->TextDelegate.BindDynamic(this, &UDialogueWidget::GetText);
	Border->AddChild(Text);
	
	WidgetTree->Modify();
	WidgetTree->RootWidget = CanvasPanel;
}

bool UDialogueWidget::AddDialogue(TArray<FText>* NewDialogue, int NewPriority) {
	if ((!Dialogue || NewPriority > DialoguePriority) && NewDialogue) {
		Dialogue = NewDialogue;
		DialoguePriority = NewPriority;
		DialogueStartTime = GetWorld()->GetTimeSeconds();
		return true;
	}
	return false;
}

FText UDialogueWidget::GetText() {
	if (!Dialogue || !Dialogue->IsValidIndex(DialogueIndex)) {
		//this->Visibility = ESlateVisibility::Collapsed;
		return FText();
	}
	FText out = (*Dialogue)[DialogueIndex];
	out = FText::FromString(out.ToString().Left((GetWorld()->GetTimeSeconds() - DialogueStartTime) * TextSpeed));
	return out;
}
