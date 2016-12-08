// Fill out your copyright notice in the Description page of Project Settings.

/*Base Weapon Info*/
#include "UnderFire.h"
#include "UFWeapon.h"
#include "Engine.h"

// Sets default values
AUFWeapon::AUFWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//initialize teh collision box
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Collision"));
	RootComponent = CollisionComponent;
	
	//create the model
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->AttachTo(RootComponent);
	WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Static Mesh"));
	WeaponStaticMesh->AttachTo(RootComponent);
	DrawWeaponTrace = false;
	CurrentState = EWeaponState::Idle;
	isFiringWeapon = false;
	isReloading = false;
	isBeingSelected = false;
	LastTimeFired = 0.f;
	wantsToFire = false;
	shouldFireAgain = false;
	RateOfFire = 0.f;
	BurstFireShotCount = 0.f;
	WeaponSpread = 0.f;
	BurstShotCount = 0.f;
	currentKick = 0.f;
	WeaponSpreadPercent = 0.f;
	MaxDamage = 0.f;
	NoEquipAnimTime = 0.f;
	isTakingOut = false;
	isPuttingAway = false;

	MuzzleOffSet = FVector(0.f, 0.f, 0.f);
	WeaponPositionOffSet = FVector(0.f, 0.f, 0.f);
	WeaponRotationOffSet = FRotator(0.f, 0.f, 0.f);

}

// Called when the game starts or when spawned
void AUFWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (VelocityInFPS)
	{
		Velocity *= .3048;
	}
	Velocity *= 100;

	TimeBetweenShots = (60 / RateOfFire);

	EffectiveRange *= 100;
}

// Called every frame
void AUFWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AUFWeapon::AttachToOwner()
{
	//AttachRootComponentTo(owningCharacter->GetMesh(), AttachTagName, EAttachLocation::SnapToTarget);
	AttachToComponent(owningCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachTagName);
	//WeaponMesh->AttachToComponent(owningCharacter->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachTagName);
	//SetActorLocation(GetActorLocation() + WeaponPositionOffSet);
	//WeaponMesh->SetRelativeLocationAndRotation(WeaponPositionOffSet, WeaponRotationOffSet.Quaternion());
	WeaponMesh->SetRelativeLocationAndRotation(WeaponPositionOffSet, WeaponRotationOffSet);//SetWorldLocationAndRotation(WeaponMesh->GetComponentLocation() + WeaponPositionOffSet, WeaponRotationOffSet);
	Attach_To_Owner_Event();
	//WeaponMesh->SetWorldRotation(WeaponRotationOffSet);
	//SetActorRotation(WeaponRotationOffSet);
}

/*
	gives the weapon stock the amount of ammo passed
*/
void AUFWeapon::GiveAmmo(int32 GivenStockAmmo)
{
	int32 StockAmmoUsed = StockMaxAmmo - StockCurrentAmmo;
	if (StockAmmoUsed < GivenStockAmmo)
	{
		StockCurrentAmmo = StockMaxAmmo;
	}
	else
	{
		StockCurrentAmmo += GivenStockAmmo;
	}
}

void AUFWeapon::UseAmmo(int32 count)
{
	if (ClipCurrentAmmo < count)
	{
		count = ClipCurrentAmmo;
	}
	ClipCurrentAmmo -= count;
}

/*
	give the weapon full stock ammo plus whatever is missing from the clip
*/
void AUFWeapon::GiveMaxAmmo()
{
	//ClipCurrentAmmo = ClipMaxAmmo;
	int32 ammoNeededForClip = ClipMaxAmmo - ClipCurrentAmmo;
	StockCurrentAmmo = StockMaxAmmo + ammoNeededForClip;
}

void AUFWeapon::ReloadStart()
{
	if (!isReloading)
	{
		isReloading = true;
		DetermineWeaponState();
	}
}

/*reloads ammo into the weapon magazine*/
void AUFWeapon::Reload()
{
	//there is no ammo left
	if (StockCurrentAmmo <= 0 || ClipCurrentAmmo >= ClipMaxAmmo || !isReloading)
	{
		return;
	}
	Reload_Event();
	DetermineWeaponState();
	//try and play the sound if specified
	if (ReloadSound != NULL)
	{
			UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
	}

	//try and play the reload anim
	if (ReloadAnim != NULL)
	{
		 //Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = owningCharacter->GetMesh()->GetAnimInstance();
			if (AnimInstance != NULL)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Playing Reload animtion"));
				AnimInstance->PlaySlotAnimationAsDynamicMontage(ReloadAnim, TEXT("UpperBody"));
			}
	}

	GetWorldTimerManager().SetTimer(TimerHandle_ReloadEnd, this, &AUFWeapon::ReloadEnd, NoAnimReloadTime, false);


	/*float startTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	while ((startTime - UGameplayStatics::GetRealTimeSeconds(GetWorld())) < NoAnimReloadTime)
	{

	}
	*/

}

void AUFWeapon::ReloadEnd()
{
	StockCurrentAmmo -= ClipMaxAmmo;
	ClipCurrentAmmo = ClipMaxAmmo;

	isReloading = false;
	DetermineWeaponState();
}
/*
	returns if the weapon has ammo in its reserve stock
*/
bool AUFWeapon::HasStockAmmo()
{
	return StockCurrentAmmo > 0;
}

/*returns if the weapon has ammo in its clip*/
bool AUFWeapon::HasClipAmmo()
{
	return ClipCurrentAmmo > 0;
}

void AUFWeapon::StartFire()
{
	
	if (!wantsToFire)
	{
		wantsToFire = true;
		DetermineWeaponState();
	}
	
}

bool AUFWeapon::NeedsToReload()
{
	return ClipCurrentAmmo <= 0;
}

void AUFWeapon::HandleFiring()
{
	if (NeedsToReload() && CanReload())
	{
		Reload();
	}
	else if (CanFire())
	{
		Fire();

		UseAmmo(1);

		BurstShotCount++;
		currentKick = BurstShotCount / WeaponSpread;
		if (currentKick > 1.25)
		{
			currentKick = 1.25;
		}
		if (FireSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}
	}

	if (FireType == EWeaponFireType::Burst)
	{

	}
	shouldFireAgain = (FireType == EWeaponFireType::Full && CurrentState == EWeaponState::Firing && TimeBetweenShots > 0.0f);
	if (shouldFireAgain)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &AUFWeapon::BurstFireStart, TimeBetweenShots, false);
	}
	LastTimeFired = GetWorld()->GetTimeSeconds();
	//DetermineWeaponState();
}

/*
	Determines what state the weapon should currently be in
*/
void AUFWeapon::DetermineWeaponState()
{
	EWeaponState::State newState = EWeaponState::Idle;
	
	if (isSelected)
	{
		if (isReloading)
		{
			if (CanReload())
			{
				newState = EWeaponState::Reloading;
			}
			else
			{
				newState = CurrentState;
			}
		}
		else if (!isReloading && wantsToFire && CanFire())
		{
			newState = EWeaponState::Firing;
		}
		else if (isBeingSelected)
		{
			newState = EWeaponState::Equipping;
		}
		else if (wantsToFire && !HasClipAmmo() && HasStockAmmo())
		{
			newState = EWeaponState::Reloading;
		}
	}
	SetNewState(newState);
}

void AUFWeapon::StopFire()
{
	if (wantsToFire)
	{
		wantsToFire = false;
		DetermineWeaponState();
		BurstShotCount = 0;
		currentKick = 0.f;
	}
}

/*
	ensures that the weapon can be set to the newly requested state
*/
void AUFWeapon::SetNewState(EWeaponState::State newState)
{
	EWeaponState::State PreviousState = CurrentState;

	
	//ending fire
	if (PreviousState == EWeaponState::Firing && newState != EWeaponState::Firing)
	{
		BurstFireStop();
	}
	CurrentState = newState;

	if (newState == EWeaponState::Reloading && PreviousState != EWeaponState::Reloading)
	{
		Reload();
	}

	//start firing
	if (PreviousState != EWeaponState::Firing && newState == EWeaponState::Firing)
	{
		BurstFireStart();
	}
	else if (PreviousState == EWeaponState::Firing && newState == EWeaponState::Firing)
	{
		
	}
}

void AUFWeapon::BurstFireStart()
{
	// start firing, can be delayed to satisfy TimeBetweenShots
	float GameTime = GetWorld()->GetTimeSeconds();
	if (LastTimeFired > 0 && TimeBetweenShots > 0.0f &&
		LastTimeFired + TimeBetweenShots > GameTime)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring,this, &AUFWeapon::HandleFiring, LastTimeFired + TimeBetweenShots - GameTime, false);
		//FPlatformProcess::Sleep(TimeBetweenShots);
	}
	HandleFiring();
}

void AUFWeapon::BurstFireStop()
{
	isFiringWeapon = false;
	GetWorldTimerManager().ClearTimer(TimerHandle_HandleFiring);
	shouldFireAgain = false;
}

bool AUFWeapon::CanReload()
{
	return HasStockAmmo() && ClipCurrentAmmo < ClipMaxAmmo;
}

bool AUFWeapon::CanFire()
{
	return ((ClipCurrentAmmo + StockCurrentAmmo > 0) && (CurrentState == EWeaponState::Idle || CurrentState == EWeaponState::Firing));
}

bool AUFWeapon::IsOnGround()
{
	return owningCharacter == NULL;
}

void AUFWeapon::Equip()
{
	CycleIsSelected();
	
	WeaponMesh->SetHiddenInGame(false);
}

void AUFWeapon::UnEquip()
{
	isPuttingAway = false;
	CycleIsSelected();
	WeaponMesh->SetHiddenInGame(true);
	isTakingOut = true;
	GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &AUFWeapon::Equip, NoEquipAnimTime, false);

}

void AUFWeapon::swap()
{
	isPuttingAway = true;
	GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &AUFWeapon::UnEquip, NoEquipAnimTime, false);

}