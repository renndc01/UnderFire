// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "UnderFirePlayerCharacter.h"
#include "UnderFireAiCharacter.h"
#include "UFWeapon.h"
#include "UnderFireGameMode.generated.h"

UCLASS(minimalapi)
class AUnderFireGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	//AUnderFireGameMode();
	AUnderFireGameMode(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameModeInfo)
		TArray<AUFWeapon*> AllWeaponsOnGround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameModeInfo)
		TArray<AUnderFirePlayerCharacter*> allPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameModeInfo)
		TArray<AUnderFireAiCharacter*> allAi;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameModeInfo)
		TArray<AUnderFireCharacter*> allCharacters;

	UFUNCTION(BlueprintCallable, Category = GameModeInfo)
		TArray<AUnderFireAiCharacter*> allAiOnTeam(ETeam team);

	UFUNCTION(BlueprintCallable, Category = GameModeInfo)
		TArray<AUnderFirePlayerCharacter*> allPlayersOnTeam(ETeam team);

};



