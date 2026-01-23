// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SteamGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class IPVMULTI3_API USteamGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

private:
	void OnSessionUserInviteAccepted(const bool bWasSuccessful, const int32 ControllerId, 
									 TSharedPtr<const FUniqueNetId> UserId, const FOnlineSessionSearchResult& InviteResult);
};
