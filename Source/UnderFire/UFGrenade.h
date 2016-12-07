// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UFWeapon.h"
#include "UnderFireProjectile.h"
#include "UFGrenade.generated.h"

/**
 * 
 */
UCLASS()
class UNDERFIRE_API AUFGrenade : public AUFWeapon
{
	GENERATED_BODY()
		AUFGrenade();
	
	virtual	void Fire() override;
	
};
