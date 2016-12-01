// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "UnderFire.h"
#include "UnderFireGameMode.h"
#include "UnderFireHUD.h"
#include "UnderFirePlayerCharacter.h"

AUnderFireGameMode::AUnderFireGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Underfire/Blueprints/UnderfirePlayerCharacterBP"));
	
	if (PlayerPawnClassFinder.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnClassFinder.Class;
	}

	
	
	//DefaultPawnClass = AUnderFirePlayerCharacter::StaticClass();
	// use our custom HUD class
	//HUDClass = AUnderFireHUD::StaticClass();
}


void AUnderFireGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (TActorIterator<AUFWeapon> WeaponItr(GetWorld()); WeaponItr; ++WeaponItr)
	{
		AUFWeapon* weapon = *WeaponItr;
		if (weapon->IsOnGround())
		{
			AllWeaponsOnGround.Add(weapon);
		}
	}

	for (TActorIterator<AUnderFirePlayerCharacter> PlayerItr(GetWorld()); PlayerItr; ++PlayerItr)
	{
		AUnderFirePlayerCharacter* player = *PlayerItr;
		players.Add(player);
	}

	for (TActorIterator<AUnderFireAiCharacter> AiItr(GetWorld()); AiItr; ++AiItr)
	{
		AUnderFireAiCharacter* ai = *AiItr;
		allAi.Add(ai);
	}
}

TArray<AUnderFireAiCharacter*> AUnderFireGameMode::allAiOnTeam(ETeam team)
{
	TArray<AUnderFireAiCharacter*> onTeam;

	for (AUnderFireAiCharacter* temp : allAi)
	{
		if (temp->Team == team)
		{
			onTeam.Add(temp);
		}
	}

	return onTeam;
}

TArray<AUnderFirePlayerCharacter*> AUnderFireGameMode::allPlayersOnTeam(ETeam team)
{

	TArray<AUnderFirePlayerCharacter*> onTeam;

	for (AUnderFirePlayerCharacter* temp : players)
	{
		if (temp->Team == team)
		{
			onTeam.Add(temp);
		}
	}

	return onTeam;

}