// Fill out your copyright notice in the Description page of Project Settings.

#include "IpvMulti3\Public\AI\AIGuard.h"

#include "Net/UnrealNetwork.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AAIGuard::AAIGuard()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	GuardState = EIAState::Idle;
}

void AAIGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAIGuard, GuardState);
}

// Called when the game starts or when spawned
void AAIGuard::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotator = GetActorRotation();
}

void AAIGuard::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ThisClass::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &ThisClass::OnNoiseHear);
}


// Called every frame
void AAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAIGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr) return;
	
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 35.0f,
																	12.0f,
																	FColor::Magenta,
																	false,
																	6.0f);

	SetGuardState(EIAState::Alerted);
	
}

void AAIGuard::OnNoiseHear(APawn* HearInstigator, const FVector& Location, float Volume)
{
	//if (Instigator == nullptr) return;
	
	DrawDebugSphere(GetWorld(), Location, 30.0f,
		12.0f, FColor::Green, false, 6.0f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;

	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &ThisClass:: ResetOrientation, 3.0f);

	SetGuardState(EIAState::Suspicious);
}

void AAIGuard::ResetOrientation()
{
if (GuardState == EIAState::Alerted)
	
	SetActorRotation(OriginalRotator);

	SetGuardState(EIAState::Idle);
}

void AAIGuard::OnRep_GuardState()
{
	OnStateChanged(GuardState);
}

void AAIGuard::SetGuardState(EIAState NewState)
{
	if (GuardState == NewState) return;
	GuardState = NewState;
	OnStateChanged(GuardState);
}
