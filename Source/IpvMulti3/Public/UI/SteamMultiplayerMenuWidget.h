// Fill out your copyright notice in the Description page of Project Settings.

// SteamMultiplayerMenuWidget.h - FIXED VERSION
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SteamMultiplayerMenuWidget.generated.h"

// Forward declarations
namespace EOnJoinSessionCompleteResult
{
    enum Type : int;
}

// Forward declare the search result class
class FOnlineSessionSearchResult;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSessionCreated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSessionJoined);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSessionsFound);

USTRUCT(BlueprintType)
struct FSessionInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ServerName;

    UPROPERTY(BlueprintReadOnly)
    FString HostName;

    UPROPERTY(BlueprintReadOnly)
    int32 Ping;

    UPROPERTY(BlueprintReadOnly)
    int32 MaxPlayers;

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentPlayers;

    UPROPERTY(BlueprintReadOnly)
    FString MapName;

    UPROPERTY(BlueprintReadOnly)
    int32 SessionIndex;
};

UCLASS()
class IPVMULTI3_API USteamMultiplayerMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    USteamMultiplayerMenuWidget(const FObjectInitializer& ObjectInitializer);
    
protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    
    // Steam session variables
    IOnlineSessionPtr SessionInterface;
    TSharedPtr<class FOnlineSessionSearch> SessionSearch;

    // Session creation settings
    FName SessionName;
    bool bIsLANMatch;
    bool bIsPresence;
    int32 MaxPlayers;
    
    // Delegates for session events
    FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
    FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
    FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

    FDelegateHandle OnCreateSessionCompleteDelegateHandle;
    FDelegateHandle OnFindSessionsCompleteDelegateHandle;
    FDelegateHandle OnJoinSessionCompleteDelegateHandle;

public:
    UFUNCTION(BlueprintCallable, Category = "Steam Multiplayer")
    void HostSession(FString InServerName, int32 MaxPlayerCount = 4, bool bUseLAN = false, bool bUsePresence = true);
    
    UFUNCTION(BlueprintCallable, Category = "Steam Multiplayer")
    void FindSessions(bool bUseLAN = false);
    
    UFUNCTION(BlueprintCallable, Category = "Steam Multiplayer")
    void JoinSession(int32 SessionIndex);
    
    UFUNCTION(BlueprintCallable, Category = "Steam Multiplayer")
    void DestroySession();

    // Blueprint events
    UPROPERTY(BlueprintAssignable, Category = "Steam Multiplayer")
    FOnSessionCreated OnSessionCreated;
    
    UPROPERTY(BlueprintAssignable, Category = "Steam Multiplayer")
    FOnSessionJoined OnSessionJoined;

    UPROPERTY(BlueprintAssignable, Category = "Steam Multiplayer")
    FOnSessionsFound OnSessionsFound;

    // Found sessions array for Blueprints
    UPROPERTY(BlueprintReadOnly, Category = "Steam Multiplayer")
    TArray<FSessionInfo> FoundSessionsList;

    // Helper function to get session info
    UFUNCTION(BlueprintCallable, Category = "Steam Multiplayer")
    void GetSessionInfo(int32 SessionIndex, FSessionInfo& SessionInfo);

private:
    // Internal session callbacks
    void OnCreateSessionComplete(FName InSessionName, bool bWasSuccessful);
    void OnFindSessionsComplete(bool bWasSuccessful);
    void OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result);
};