// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/IpvMulti3GameState.h"

#include "EngineUtils.h"
#include "IpvMulti3/Public/Player/IpvMulti3PlayerController.h"

void AIpvMulti3GameState::MultiCastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccess)
{
	/*
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		if (APawn* Pawn = PC->GetPawn())
			Pawn->DisableInput(nullptr);
			*/
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AIpvMulti3PlayerController* PC = Cast<AIpvMulti3PlayerController>(It->Get());
		if (PC)
		{
			PC->OnMissionCompleted(InstigatorPawn, bMissionSuccess);
			APawn* Pawn = PC->GetPawn();
			if (Pawn)
			{
				Pawn -> DisableInput(nullptr);
			}
		}
	}
}
