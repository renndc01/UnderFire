// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Engine.h"
#include "UFWeapon.generated.h"


UENUM(BlueprintType)
namespace EWeaponState
{
	enum State
	{
		Idle,
		Firing,
		Reloading,
		Equipping,
	};
}



UENUM(BlueprintType)
namespace EWeaponProjectile
{
	enum ProjectileType
	{
		EBullet			UMETA(DisplayName = "Bullet"),
		EGrenade		UMETA(DisplayName = "Grenade"),
		ESpread			UMETA(DisplayName = "Spread"),
		EProjectile		UMETA(DisplayName = "Projectile")
	};
}

UENUM(BlueprintType)
namespace EWeaponFireType
{
	enum FireType
	{
		Single		UMETA(DisplayName = "Single Shot"),
		Burst		UMETA(DisplayName = "Burst Fire"),
		Full		UMETA(DisplayName = "Full Auto"),
	};
}

UCLASS()
class UNDERFIRE_API AUFWeapon : public AActor
{
	GENERATED_BODY()
	
protected:
	bool isFiringWeapon;

	bool isReloading;

	bool isBeingSelected;

	bool wantsToFire;

	bool shouldFireAgain;

	float LastTimeFired;

	FTimerHandle TimerHandle_HandleFiring;

	FTimerHandle TimerHandle_ReloadEnd;

	int32 BurstShotCount;

	float TimeBetweenShots;

	float WeaponSpreadPercent;
public:	
	// Sets default values for this actor's properties
	AUFWeapon();



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponAmmo)
		int32 StockMaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponAmmo)
		int32 StockCurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponAmmo)
		int32 ClipMaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponAmmo)
		int32 ClipCurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		UTexture2D* weaponImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		UTexture2D* ammoImage;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = UFWeaponInfo)
		TSubclassOf<class AUnderFireProjectile> ProjectileClass;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		FVector MuzzleOffSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		FVector WeaponPositionOffSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		FRotator WeaponRotationOffSet;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		class UAnimSequence* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		class UAnimSequence* ReloadAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		class USoundBase* ReloadSound;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
	class AUnderFireCharacter* OwningCharacter;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		FName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		TEnumAsByte<EWeaponFireType::FireType> FireType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		int32 BurstFireShotCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		float RateOfFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		float EffectiveRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		float WeaponSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		float NoAnimReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		float NoEquipAnimTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		float Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		bool VelocityInFPS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		TEnumAsByte<EWeaponProjectile::ProjectileType> projectileType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		float MaxDamage;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponCollision)
		UBoxComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponCollision)
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponCollision)
		UStaticMeshComponent* WeaponStaticMesh;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponDebug)
		bool DrawWeaponTrace;
	
	UPROPERTY(VisibleAnywhere, BLueprintReadWrite, Category = UFWeapon)
		TEnumAsByte<EWeaponState::State> CurrentState;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		bool isSelected;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UFWeapon)
		float currentKick;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UFWeapon)
		ACharacter* owningCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UFWeapon)
		bool isTakingOut;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UFWeapon)
		bool isPuttingAway;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFWeaponInfo)
		FName AttachTagName;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void Fire() PURE_VIRTUAL(AUFWeapon::FireWeapon, );

	void StopFire();

	void StartFire();

	void HandleFiring();

	void DetermineWeaponState();

	UFUNCTION(BlueprintCallable, Category = UFWeaponAmmo)
		void GiveAmmo(int32 GivenAmmo);

	UFUNCTION(BlueprintCallable, Category = UFWeaponAmmo)
		void GiveMaxAmmo();

	UFUNCTION(BlueprintCallable, Category = UFWeaponAmmo)
		void Reload();

	UFUNCTION(BlueprintCallable, Category = UFWeaponAmmo)
		bool HasStockAmmo();

	UFUNCTION(BlueprintCallable, Category = UFWeaponAmmo)
		bool HasClipAmmo();

	UFUNCTION(BlueprintCallable, Category = UFWeapon)
		void SetNewState(EWeaponState::State newState);

	UFUNCTION(BlueprintCallable, Category = UFWeapon)
		bool CanReload();

	UFUNCTION(BlueprintCallable, Category = UFWeapon)
		bool CanFire();

	UFUNCTION(BlueprintCallable, Category = UFWeapon)
		void BurstFireStart();

	UFUNCTION(BlueprintCallable, Category = UFWeapon)
		void BurstFireStop();

	UFUNCTION(BlueprintCallable, Category = UFWeapon)
		void UseAmmo(int32 count);

	UFUNCTION(BlueprintCallable, Category = UFWeapon)
		void ReloadEnd();
	
	UFUNCTION(BlueprintCallable, Category = UFWeapon)
		bool NeedsToReload();

	UFUNCTION(BlueprintCallable, Category = UFWeapon)
		void ReloadStart();

	UFUNCTION(BlueprintCallable, Category = UFWeapon)
		void AttachToOwner();
	
	UFUNCTION(BlueprintCallable, Category = UFWeapon)
		void CycleIsSelected() { isSelected = !isSelected; }

	UFUNCTION(BlueprintCallable, Category = UFWeapon)
		void Equip();
	
	UFUNCTION(BlueprintCallable, Category = UFWeapon)
		void UnEquip();

	UFUNCTION(BlueprintCallable, Category = UFWeapon)
		void swap();

	UFUNCTION(BlueprintCallable, Category = UFWeapon)
		bool IsOnGround();

	//UFUNCTION(BlueprintImplementableEvent, Category = UFWeapon)
		//void DoDamage_Event(ACharacter* otherCharacter, float damage);

	UFUNCTION(BlueprintImplementableEvent, Category = UFWeapon)
		void Fire_Event();

	UFUNCTION(BlueprintImplementableEvent, Category = UFWeapon)
		void Attach_To_Owner_Event();

	UFUNCTION(BlueprintImplementableEvent, Category = UFWeapon)
		void Reload_Event();
};
