// Fill out your copyright notice in the Description page of Project Settings.

#include "UnderFire.h"
#include "UFWeapon_Instant_Fire.h"
#include "UnderFireAiCharacter.h"


AUFWeapon_Instant_Fire::AUFWeapon_Instant_Fire()
{
	projectileType = EWeaponProjectile::EBullet;
}

void AUFWeapon_Instant_Fire::Fire()
{
	
	Fire_Event();
	if (ProjectileClass)
	{
		FRotator rotation = owningCharacter->GetControlRotation();
		const FVector location = WeaponMesh->GetSocketLocation("Muzzle"); //+ owningCharacter->GetActorLocation();
		float tempSpread = WeaponSpread * .1;
		if (currentKick > 0)
		{
			tempSpread = WeaponSpread * currentKick;
		}
		AUnderFireCharacter* UFCharacter = Cast<AUnderFireCharacter>(owningCharacter);
		if (UFCharacter)
		{
			if (UFCharacter->isAIDS)
			{
				tempSpread *= .5;
			}
		}
		rotation += FRotator(FMath::FRandRange(tempSpread * -1, tempSpread), FMath::FRandRange(tempSpread * -1, tempSpread), FMath::FRandRange(tempSpread * -1, tempSpread));
		AUnderFireProjectile* projectile = GetWorld()->SpawnActor<AUnderFireProjectile>(ProjectileClass, location, rotation);
		//projectile->InitializeVelocity(Velocity, 3.f);
		projectile->ProjectileMovement->MaxSpeed = Velocity;
		projectile->ProjectileMovement->SetVelocityInLocalSpace(FVector(Velocity, 0.0, 0.0));
		//projectile->ProjectileMovement->Velocity = FVector(Velocity, 0.f, 0.f);
		//projectile->ProjectileMovement->Velocity = Velocity;
		//projectile->DrawWeaponTrace = DrawWeaponTrace;
		//projectile->EffectiveRange = EffectiveRange;
		//projectile->Velocity = Velocity;
		projectile->SetLifeSpan((EffectiveRange * 1.25) / Velocity);
		projectile->DamageAmount = MaxDamage;
	}
	/*const int32 randomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(randomSeed);
	const float currentSpread = WeaponSpread;
	const float spreadCone = FMath::DegreesToRadians(currentSpread * .5);
	const FVector aimDir = WeaponMesh->GetSocketRotation("MF").Vector();
	const FVector start = WeaponMesh->GetSocketLocation("MF");
	const FVector shootDir = WeaponRandomStream.VRandCone(aimDir, spreadCone, spreadCone);
	const FVector end = start + shootDir * EffectiveRange;
	const FHitResult impact = WeaponTrace(start, end);

	ProcessInstantHit(impact, start, shootDir, randomSeed, currentSpread);*/
}

