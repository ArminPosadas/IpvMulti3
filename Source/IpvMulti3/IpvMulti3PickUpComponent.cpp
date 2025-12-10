// Copyright Epic Games, Inc. All Rights Reserved.

#include "IpvMulti3PickUpComponent.h"

UIpvMulti3PickUpComponent::UIpvMulti3PickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UIpvMulti3PickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UIpvMulti3PickUpComponent::OnSphereBeginOverlap);
}

void UIpvMulti3PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AIpvMulti3Character* Character = Cast<AIpvMulti3Character>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
