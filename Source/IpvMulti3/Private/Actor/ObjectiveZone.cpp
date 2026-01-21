// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ObjectiveZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "IpvMulti3/IpvMulti3Character.h"
#include "IpvMulti3/IpvMulti3GameMode.h"

// Sets default values
AObjectiveZone::AObjectiveZone()
{
	bReplicates = true;
	OverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComponent"));
	OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComponent->SetCollisionResponseToChannels(ECR_Ignore);
	OverlapComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = OverlapComponent;
	OverlapComponent->SetHiddenInGame(false);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComponent->DecalSize = FVector(1,1,1);
	DecalComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AObjectiveZone::BeginPlay()
{
	Super::BeginPlay();
	
}

void AObjectiveZone::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AObjectiveZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			10.f,
			FColor::Green,
			FString::Printf(TEXT("Overlapped"))
		);
	}
	AIpvMulti3Character* MyPawn = Cast<AIpvMulti3Character>(OtherActor);
	if (MyPawn == nullptr) return;
	{
		if (MyPawn->bIsCarryingObjective)
		{
			AIpvMulti3GameMode* GM = Cast<AIpvMulti3GameMode>(GetWorld()->GetAuthGameMode());
			if (GM)
			{
				GM->CompleteMission(MyPawn);
			}
		}
	}
}

