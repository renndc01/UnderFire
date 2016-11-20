// Fill out your copyright notice in the Description page of Project Settings.

#include "UnderFire.h"
#include "UFWeaponProjectile.h"


// Sets default values
AUFWeaponProjectile::AUFWeaponProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUFWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUFWeaponProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

