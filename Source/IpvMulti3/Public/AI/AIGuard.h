#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIGuard.generated.h"
class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EIAState : uint8
{
	Idle,
	Suspicious,
	Alerted
};

UCLASS()
class IPVMULTI3_API AAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIGuard();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnNoiseHear(APawn* HearInstigator, const FVector& Location, float Volume);

	FRotator OriginalRotator;
	UFUNCTION()
	void ResetOrientation();

	FTimerHandle TimerHandle_ResetOrientation;

	//Guard State
	UPROPERTY(ReplicatedUsing=OnRep_GuardState)
	EIAState GuardState;

	UFUNCTION()
	void OnRep_GuardState();
	
	void SetGuardState(EIAState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AIGuard")
	void OnStateChanged(EIAState NewState);
};
