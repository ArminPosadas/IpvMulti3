// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IpvMulti3GameMode.generated.h"

UCLASS(minimalapi)
class AIpvMulti3GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AIpvMulti3GameMode();

	void CompleteMission(APawn* Pawn);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnMissionCompleted(APawn* Pawn);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> SpectatorViewClass;
};



