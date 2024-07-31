// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueControllerComponent.h"

int UDialogueControllerComponent::ParseField(char* buffer, int buffer_size) {
	//FString out = "";
	memset(buffer, 0, sizeof(char) * buffer_size);

	int i = 0;
	char c;
	bool end = false;
	bool quote = false;
	while (!end && i < buffer_size - 1) {
		if (Stream.get(c)) {
			switch (c) {
			case '"':
				quote = !quote;
				break;
			case '\n':
				end = true;
				break;
			case ',':
				if (!quote) {
					end = true;
					break;
				}
				// fallthrough
			default:
				buffer[i] = c;
				i++;
			}
		} else {
			break;
		}
	}

	return i;
}

bool UDialogueControllerComponent::ParseRecord(int64 Key, FDialogueRecord& DialogueRecord) {
	if (Key < 0) {
		// GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("key < 0")));
		return false;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("Key: %ld"), Key));
	Stream.clear();
	Stream.seekg(Key, Stream.beg);
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("tellg: %d"), (int)Stream.tellg()));
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("0x%04x"), Stream.rdstate()));

	char buffer[PARSE_BUFFER_SIZE];

	// ignore tag
	Stream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
	
	// order is extremely important
	ParseField(buffer, PARSE_BUFFER_SIZE);
	DialogueRecord.Speaker = FString(buffer);
	ParseField(buffer, PARSE_BUFFER_SIZE);
	DialogueRecord.Text = FString(buffer);
	ParseField(buffer, PARSE_BUFFER_SIZE);
	DialogueRecord.Priority = atoi(buffer);

	ParseField(buffer, PARSE_BUFFER_SIZE);
	const FString Next = FString(buffer);
	int64 NextKey;
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "next: " + Next);
	if (buffer[0] == 0 || buffer[0] == '-') { // fstring len() and comparisons dont work here in HTML5 builds for some reason. using char* is the workaround
		NextKey = Stream.tellg();
	} else if (buffer[0] == 'n' && buffer[1] == 'u' && buffer[2] == 'l' && buffer[3] == 'l') {
		NextKey = -1;
	} else if (TagKeyMap.Contains(Next)) {
		NextKey = TagKeyMap[Next];
	} else {
		NextKey = -2;
	}
	DialogueRecord.NextKey = NextKey;

	return true;
}

// Sets default values for this component's properties
UDialogueControllerComponent::UDialogueControllerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UDialogueControllerComponent::ParseKeys() {
	Stream.open(TCHAR_TO_ANSI(*(FPaths::ProjectContentDir() + DIALOGUE_FILEPATH))); // cast to char* is important for HTML5 build
	if (!Stream.is_open()) {
		UE_LOG(LogTemp, Error, TEXT("Could not parse %s: std::ifstream error 0x%04x"), *DIALOGUE_FILEPATH, Stream.rdstate());
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Could not parse %s: std::ifstream error 0x%04x"), Stream.rdstate()));
		return;
	}
	char c;
	FString Tag = ""; // tag at beginning of line
	int64 Key = 0; // start of current record
	int iterations = 0; // debug
	while (Stream.get(c) && iterations < MAX_LOOP_ITERATIONS) { // debug
		if (c == ',') {
			if (Tag != "") {
				TagKeyMap.Emplace(Tag, Key);
				//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("%s: %ld"), *Tag, TagKeyMap[Tag]));
				Tag = "";
			}
			Stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // go to next line
			Key = Stream.tellg();
		} else {
			Tag.AppendChar(c);
		}
		iterations++;
	}

	if (iterations >= MAX_LOOP_ITERATIONS) {
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("infinite loop in parsekeys")));
	}
}

bool UDialogueControllerComponent::ParseTagRecord(FString Tag, FDialogueRecord& DialogueRecord) {
	if (!Stream.is_open()) return false; // should we execute ParseKeys() instead?
	if (!TagKeyMap.Contains(Tag)) return false;
	return ParseRecord(TagKeyMap[Tag], DialogueRecord);
}

bool UDialogueControllerComponent::ParseNextRecord(FDialogueRecord& DialogueRecord) {
	if (!Stream.is_open()) {
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("not open")));
		return false;
	}
	return ParseRecord(DialogueRecord.NextKey, DialogueRecord);
}

void UDialogueControllerComponent::NewDialogue(FString Tag) {
	OnNewDialogue.Broadcast(Tag);
}

// Called when the game starts
void UDialogueControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	ParseKeys();
	
}


// Called every frame
void UDialogueControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

