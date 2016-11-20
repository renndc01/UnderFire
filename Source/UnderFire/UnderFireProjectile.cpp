// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "UnderFire.h"
#include "UnderFireProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

AUnderFireProjectile::AUnderFireProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AUnderFireProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	//ProjectileMovement->ProjectileGravityScale = -980;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0;
	DamageAmount = 0.f;
	/*DrawWeaponTrace = false;
	EffectiveRange = 0.f;
	Velocity = 0.f;*/
}

void AUnderFireProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black, TEXT("Projectile Detected Hit"));
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100, GetActorLocation());
		
		//Destroy();
	}
}
/*
FHitResult AUnderFireProjectile::WeaponTrace(const FVector &TraceFrom, const FVector & TraceTo) const
{
	static FName weaponFireTag = FName(TEXT("WeaponTrace"));

	FCollisionQueryParams traceParams(weaponFireTag, true, Instigator);
	traceParams.bTraceAsyncScene = true;
	traceParams.bReturnPhysicalMaterial = true;
	traceParams.AddIgnoredActor(this);

	if (DrawWeaponTrace)
	{
		GetWorld()->DebugDrawTraceTag = "WeaponTrace";
	}

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, WEAPON_TRACE, traceParams);

	return Hit;
}

void AUnderFireProjectile::ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread)
{
	const FVector endTrace = Origin + ShootDir * EffectiveRange;
	const FVector endPoint = Impact.GetActor() ? Impact.ImpactPoint : endTrace;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Playing fire animtion"));
	DrawDebugLine(GetWorld(), Origin, Impact.TraceEnd, FColor::Red, true, 10000, 10.f);
}*/