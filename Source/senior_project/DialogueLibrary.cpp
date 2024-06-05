// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueLibrary.h"

bool UDialogueLibrary::AddDialogue(float Time, FDialogue& Dialogue, TArray<FText> NewMessages, int NewPriority) {
	if ((NewPriority > Dialogue.Priority)) {
		Dialogue.Messages = NewMessages;
		Dialogue.Index = 0;
		Dialogue.Priority = NewPriority;
		Dialogue.StartTime = Time;
		return true;
	}
	return false;
}

FText UDialogueLibrary::GetText(float Time, FDialogue& Dialogue) {
	if (!Dialogue.Messages.IsValidIndex(Dialogue.Index)) {
		return FText();
	}
	FText out = Dialogue.Messages[Dialogue.Index];
	out = FText::FromString(out.ToString().Left((Time - Dialogue.StartTime) * Dialogue.TextSpeed));
	return out;
}
