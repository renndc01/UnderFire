// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnderFireProjectile.h"
#include "UFGrenadeProjectile.generated.h"

/**
 * 
 */
UCLASS()
class UNDERFIRE_API AUFGrenadeProjectile : public AUnderFireProjectile
{
	GENERATED_BODY()

protected:
	FTimerHandle TimerHandle_GrenadeTimeOut;
	FTimerHandle TimerHandle_GrenadeExplosionTimeOut;

public:

	AUFGrenadeProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFGrenadeInfo)
		float GrenadeFuseTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFGrenadeInfo)
		float GrenadeExplsionTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFGrenadeInfo)
		float MaxDamageRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFGrenadeInfo)
		class USoundBase* FireSound;

		void GrenadeTimeOut();

	UFUNCTION(BlueprintImplementableEvent, Category = UFWeaponGrenade)
		void GrenadeTimeOut_Event();

		void StartExplosion();

	UFUNCTION(BlueprintImplementableEvent, Category = UFWeaponGrenade)
		void StartExplosion_Event();

	void FinishExplosion();

	UFUNCTION(BlueprintImplementableEvent, Category = UFWeaponGrenade)
		void FinishExplosion_Event();

};
