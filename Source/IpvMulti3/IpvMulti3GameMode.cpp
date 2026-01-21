// Copyright Epic Games, Inc. All Rights Reserved.

#include "IpvMulti3GameMode.h"
#include "IpvMulti3Character.h"
#include "Game/IpvMulti3GameState.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AIpvMulti3GameMode::AIpvMulti3GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	GameStateClass = AIpvMulti3GameState::StaticClass();
}

void AIpvMulti3GameMode::CompleteMission(APawn* Pawn)
{
	if (Pawn == nullptr) return;
	Pawn->DisableInput(nullptr);
	if (SpectatorViewClass)
	{
		TArray<AActor*> ReturnActors;
		UGameplayStatics::GetAllActorsOfClass(this, SpectatorViewClass, ReturnActors);
		if (ReturnActors.Num() > 0)
		{
			AActor* NewViewTarget = ReturnActors[0];
			for (FConstPlayerControllerIterator It=GetWorld()->GetPlayerControllerIterator(); It; It++)
			{
				APlayerController*PC=It->Get();
				if (PC)
				{
					PC->SetViewTargetWithBlend(NewViewTarget, 1.0f, VTBlend_Cubic);
				}
			}
		}
	}
	AIpvMulti3GameState* GS = GetGameState<AIpvMulti3GameState>();
	if (GS)
	{
		GS->MultiCastOnMissionComplete(Pawn, true);
	}
	OnMissionCompleted(Pawn);
}
