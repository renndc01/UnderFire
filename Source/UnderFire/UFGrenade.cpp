// Fill out your copyright notice in the Description page of Project Settings.

#include "UnderFire.h"
#include "UFGrenade.h"




AUFGrenade::AUFGrenade()
{
	projectileType = EWeaponProjectile::EGrenade;
}

void AUFGrenade::Fire()
{
	Fire_Event();

	if (ProjectileClass)
	{
		//FRotator rotation = owningCharacter->GetMesh()->GetSocketRotation(AttachTagName);
		//FVector location = owningCharacter->GetMesh()->GetSocketLocation(AttachTagName);
		FRotator rotation = owningCharacter->GetActorRotation();
		FVector location = owningCharacter->GetActorLocation();

		AUnderFireProjectile* projectile = GetWorld()->SpawnActor<AUnderFireProjectile>(ProjectileClass, location, rotation);
		//projectile->InitializeVelocity(Velocity, 3.f);
		projectile->ProjectileMovement->MaxSpeed = Velocity;
		projectile->ProjectileMovement->SetVelocityInLocalSpace(FVector(Velocity, 0.0, 0.0));
		//projectile->ProjectileMovement->Velocity = FVector(Velocity, 0.f, 0.f);
		//projectile->ProjectileMovement->Velocity = Velocity;
		//projectile->DrawWeaponTrace = DrawWeaponTrace;
		//projectile->EffectiveRange = EffectiveRange;
		//projectile->Velocity = Velocity;
		projectile->SetLifeSpan(10000);
		projectile->MaxLifeTime = (EffectiveRange * 1.25) / Velocity;
		projectile->MaxDamage = MaxDamage;
		ReloadStart();
	}
}