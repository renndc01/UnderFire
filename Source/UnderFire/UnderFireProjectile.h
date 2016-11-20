// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Engine.h"
#include "UFWeapon.h"
#include "UnderFireProjectile.generated.h"

#define WEAPON_TRACE ECC_GameTraceChannel1

UCLASS(config=Game)
class AUnderFireProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	
protected:
	//FHitResult WeaponTrace(const FVector &TraceFrom, const FVector & TraceTo) const;
	//void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread);
public:
	AUnderFireProjectile();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;
	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	AUFWeapon* owningWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DamageInfor)
		float DamageAmount;


		/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

