// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UFWeapon.h"
#include "UnderFireProjectile.h"
#include "UFWeapon_Instant_Fire.generated.h"

/**
 * 
 */
UCLASS()
class UNDERFIRE_API AUFWeapon_Instant_Fire : public AUFWeapon
{
	GENERATED_BODY()
	
protected:
	
public:
	
	AUFWeapon_Instant_Fire();


	virtual	void Fire() override;
};
