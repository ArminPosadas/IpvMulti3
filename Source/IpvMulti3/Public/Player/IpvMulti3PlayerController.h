// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IpvMulti3PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class IPVMULTI3_API AIpvMulti3PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnMissionCompleted(APawn* IntigatorPawn, bool bIsMissionSuccess);
};
