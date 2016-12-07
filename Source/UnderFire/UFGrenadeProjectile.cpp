// Fill out your copyright notice in the Description page of Project Settings.

#include "UnderFire.h"
#include "UFGrenadeProjectile.h"

AUFGrenadeProjectile::AUFGrenadeProjectile()
{
	GrenadeFuseTime = 0.f;
	MaxDamageRange = 0.f;
	GrenadeExplsionTimer = 0.f;

}

void AUFGrenadeProjectile::BeginPlay()
{
	GetWorldTimerManager().SetTimer(TimerHandle_GrenadeTimeOut, this, &AUFGrenadeProjectile::GrenadeTimeOut, GrenadeFuseTime, false);

}

void AUFGrenadeProjectile::GrenadeTimeOut()
{
	GrenadeTimeOut_Event();
	StartExplosion();
}

void AUFGrenadeProjectile::StartExplosion()
{
	StartExplosion_Event();
	GetWorldTimerManager().SetTimer(TimerHandle_GrenadeExplosionTimeOut, this, &AUFGrenadeProjectile::FinishExplosion, GrenadeExplsionTimer, false);
}

void AUFGrenadeProjectile::FinishExplosion()
{
	FinishExplosion_Event();
	Destroy();
}