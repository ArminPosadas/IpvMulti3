// Fill out your copyright notice in the Description page of Project Settings.

#include "SteamGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"

void USteamGameInstance::Init()
{
	Super::Init();
    
	// Initialize Steam subsystem
	IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld());
	if (OnlineSubsystem)
	{
		UE_LOG(LogTemp, Log, TEXT("Online Subsystem: %s"), *OnlineSubsystem->GetSubsystemName().ToString());
        
		// Bind to invite acceptance
		IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FOnSessionUserInviteAcceptedDelegate OnSessionUserInviteAcceptedDelegate;
			OnSessionUserInviteAcceptedDelegate.BindUObject(this, &USteamGameInstance::OnSessionUserInviteAccepted);
			SessionInterface->AddOnSessionUserInviteAcceptedDelegate_Handle(OnSessionUserInviteAcceptedDelegate);
		}
	}
}

void USteamGameInstance::Shutdown()
{
	Super::Shutdown();
}

void USteamGameInstance::OnSessionUserInviteAccepted(const bool bWasSuccessful, const int32 ControllerId,
	TSharedPtr<const FUniqueNetId> UserId, const FOnlineSessionSearchResult& InviteResult)
{
	if (bWasSuccessful)
	{
		// Handle invite acceptance
		IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld());
		if (OnlineSubsystem)
		{
			IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
			if (SessionInterface.IsValid())
			{
				SessionInterface->JoinSession(*UserId, NAME_GameSession, InviteResult);
			}
		}
	}
}
