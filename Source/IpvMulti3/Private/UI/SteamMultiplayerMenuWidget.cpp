// Fill out your copyright notice in the Description page of Project Settings.

// SteamMultiplayerMenuWidget.cpp

// SteamMultiplayerMenuWidget.cpp
#include "UI/SteamMultiplayerMenuWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

USteamMultiplayerMenuWidget::USteamMultiplayerMenuWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    SessionName = TEXT("GameSession");
    bIsLANMatch = false;
    bIsPresence = true;
    MaxPlayers = 4;
}

void USteamMultiplayerMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // Get the online subsystem (Steam)
    IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld());
    if (OnlineSubsystem)
    {
        SessionInterface = OnlineSubsystem->GetSessionInterface();
        
        // Bind delegates
        OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(
            this, &USteamMultiplayerMenuWidget::OnCreateSessionComplete);
        OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(
            this, &USteamMultiplayerMenuWidget::OnFindSessionsComplete);
        OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(
            this, &USteamMultiplayerMenuWidget::OnJoinSessionComplete);
    }
}

void USteamMultiplayerMenuWidget::NativeDestruct()
{
    // Clear any existing session
    DestroySession();
    
    Super::NativeDestruct();
}

void USteamMultiplayerMenuWidget::HostSession(FString InServerName, int32 MaxPlayerCount, bool bUseLAN, bool bUsePresence)
{
    if (SessionInterface.IsValid())
    {
        // Check if session already exists
        FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SessionName);
        if (ExistingSession)
        {
            SessionInterface->DestroySession(SessionName);
        }

        // Store settings
        bIsLANMatch = bUseLAN;
        bIsPresence = bUsePresence;
        MaxPlayers = MaxPlayerCount;

        // Setup session settings
        FOnlineSessionSettings SessionSettings;
        SessionSettings.bIsLANMatch = bIsLANMatch;
        SessionSettings.bUsesPresence = bIsPresence;
        SessionSettings.bShouldAdvertise = true;
        SessionSettings.bAllowJoinInProgress = true;
        SessionSettings.bAllowInvites = true;
        SessionSettings.NumPublicConnections = MaxPlayers;
        SessionSettings.NumPrivateConnections = 0;
        SessionSettings.bUseLobbiesIfAvailable = true;
        SessionSettings.bUseLobbiesVoiceChatIfAvailable = true;
        
        // Set server name - Use InServerName parameter
        SessionSettings.Set(FName("SERVER_NAME_KEY"), InServerName, EOnlineDataAdvertisementType::ViaOnlineService);
        
        // Set game mode and map
        SessionSettings.Set(FName("GAME_MODE_KEY"), FString("YourGameMode"), EOnlineDataAdvertisementType::ViaOnlineService);
        SessionSettings.Set(FName("MAP_NAME_KEY"), FString("YourMapName"), EOnlineDataAdvertisementType::ViaOnlineService);
        
        // Add delegate
        OnCreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
        
        // Create session
        const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
        bool bCreated = SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), SessionName, SessionSettings);
        
        if (!bCreated)
        {
            SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
            UE_LOG(LogTemp, Error, TEXT("Failed to create session!"));
        }
    }
}

// Alternative if SEARCH_PRESENCE is still not found
void USteamMultiplayerMenuWidget::FindSessions(bool bUseLAN)
{
    if (SessionInterface.IsValid())
    {
        // Clear previous search
        SessionSearch = MakeShareable(new FOnlineSessionSearch());
        
        // Configure search parameters
        SessionSearch->bIsLanQuery = bUseLAN;
        SessionSearch->MaxSearchResults = 20;
        SessionSearch->PingBucketSize = 50;
        
        // Alternative search settings without SEARCH_PRESENCE
        SessionSearch->QuerySettings.Set(FName(TEXT("PRESENCESEARCH")), true, EOnlineComparisonOp::Equals);
        
        // Search for all sessions
        // Or use this if you want to search by specific attributes:
        // SessionSearch->QuerySettings.Set(FName(TEXT("GAME_MODE_KEY")), FString("YourGameMode"), EOnlineComparisonOp::Equals);
        
        // Add delegate
        OnFindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
        
        // Find sessions
        const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
        bool bFound = SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
        
        if (!bFound)
        {
            SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
            UE_LOG(LogTemp, Error, TEXT("Failed to find sessions!"));
        }
    }
}

void USteamMultiplayerMenuWidget::JoinSession(int32 SessionIndex)
{
    if (SessionInterface.IsValid() && SessionSearch.IsValid())
    {
        if (SessionIndex >= 0 && SessionIndex < SessionSearch->SearchResults.Num())
        {
            // Add delegate
            OnJoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
            
            // Join session
            const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
            bool bJoined = SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), SessionName, SessionSearch->SearchResults[SessionIndex]);
            
            if (!bJoined)
            {
                SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
                UE_LOG(LogTemp, Error, TEXT("Failed to join session!"));
            }
        }
    }
}

void USteamMultiplayerMenuWidget::DestroySession()
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->DestroySession(SessionName);
    }
}

void USteamMultiplayerMenuWidget::OnCreateSessionComplete(FName InSessionName, bool bWasSuccessful)
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
    }
    
    if (bWasSuccessful)
    {
        UE_LOG(LogTemp, Log, TEXT("Session created successfully!"));
        
        // Broadcast to blueprint
        OnSessionCreated.Broadcast();
        
        // Travel to game map
        GetWorld()->ServerTravel(TEXT("/Game/Maps/YourGameMap?listen"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create session!"));
    }
}

void USteamMultiplayerMenuWidget::OnFindSessionsComplete(bool bWasSuccessful)
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
    }
    
    FoundSessionsList.Empty();
    
    if (bWasSuccessful && SessionSearch.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("Found %d sessions"), SessionSearch->SearchResults.Num());
        
        // Convert to blueprint-friendly format
        for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
        {
            FSessionInfo SessionInfo;
            GetSessionInfo(i, SessionInfo);  // Use the new function
            FoundSessionsList.Add(SessionInfo);
            
            // Log session info for debugging
            UE_LOG(LogTemp, Log, TEXT("Session %d: %s - %d/%d players - %dms ping"), 
                i, 
                *SessionInfo.ServerName,
                SessionInfo.CurrentPlayers,
                SessionInfo.MaxPlayers,
                SessionInfo.Ping);
        }
        
        // Broadcast that sessions were found
        OnSessionsFound.Broadcast();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find sessions!"));
    }
}

void USteamMultiplayerMenuWidget::OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
    }
    
    if (Result == EOnJoinSessionCompleteResult::Success)
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully joined session!"));
        
        // Get travel URL - Use the parameter InSessionName
        FString TravelURL;
        if (SessionInterface->GetResolvedConnectString(InSessionName, TravelURL))
        {
            APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
            if (PlayerController)
            {
                // Broadcast to blueprint
                OnSessionJoined.Broadcast();
                
                // Client travel to the server
                PlayerController->ClientTravel(TravelURL, TRAVEL_Absolute);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to join session! Result: %d"), static_cast<int32>(Result));
    }
}

void USteamMultiplayerMenuWidget::GetSessionInfo(int32 SessionIndex, FSessionInfo& SessionInfo)
{
    // Clear the output first
    SessionInfo = FSessionInfo();
    
    // Check if we have valid session search results
    if (!SessionSearch.IsValid())
    {
        SessionInfo.ServerName = TEXT("No Session Search Available");
        SessionInfo.SessionIndex = -1;
        return;
    }
    
    // Check bounds
    if (SessionIndex < 0 || SessionIndex >= SessionSearch->SearchResults.Num())
    {
        SessionInfo.ServerName = TEXT("Invalid Session Index");
        SessionInfo.SessionIndex = -1;
        return;
    }
    
    // Get the session result
    const FOnlineSessionSearchResult& SearchResult = SessionSearch->SearchResults[SessionIndex];
    
    // Set the session index
    SessionInfo.SessionIndex = SessionIndex;
    SessionInfo.Ping = SearchResult.PingInMs;
    
    // Get server name
    FString ServerName;
    if (SearchResult.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), ServerName))
    {
        SessionInfo.ServerName = ServerName;
    }
    else
    {
        SessionInfo.ServerName = FString::Printf(TEXT("Server %d"), SessionIndex + 1);
    }
    
    // Get map name
    FString MapName;
    if (SearchResult.Session.SessionSettings.Get(FName("MAP_NAME_KEY"), MapName))
    {
        SessionInfo.MapName = MapName;
    }
    else
    {
        SessionInfo.MapName = TEXT("Unknown Map");
    }
    
    // Get player counts
    SessionInfo.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
    
    // Calculate current players (approximate)
    int32 OpenConnections = SearchResult.Session.NumOpenPublicConnections;
    SessionInfo.CurrentPlayers = FMath::Max(0, SessionInfo.MaxPlayers - OpenConnections);
    
    // Get host name - Using OwningUserName (which is an FString)
    if (!SearchResult.Session.OwningUserName.IsEmpty())
    {
        SessionInfo.HostName = SearchResult.Session.OwningUserName;
    }
    // Fallback to using the unique ID if display name is empty
    else if (SearchResult.Session.OwningUserId.IsValid())
    {
        SessionInfo.HostName = SearchResult.Session.OwningUserId->ToString();
    }
    else
    {
        SessionInfo.HostName = TEXT("Unknown Host");
    }
}