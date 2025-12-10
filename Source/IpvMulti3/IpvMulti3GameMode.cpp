// Copyright Epic Games, Inc. All Rights Reserved.

#include "IpvMulti3GameMode.h"
#include "IpvMulti3Character.h"
#include "UObject/ConstructorHelpers.h"

AIpvMulti3GameMode::AIpvMulti3GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
