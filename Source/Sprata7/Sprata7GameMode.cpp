// Copyright Epic Games, Inc. All Rights Reserved.

#include "Sprata7GameMode.h"
#include "Sprata7Character.h"
#include "UObject/ConstructorHelpers.h"

ASprata7GameMode::ASprata7GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
