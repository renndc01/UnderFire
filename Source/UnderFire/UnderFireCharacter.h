// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Engine.h"
#include "UFWeapon.h"
#include "UnderFireCharacter.generated.h"

UENUM(BlueprintType)
enum class ETeam : uint8
{
	allies, //americans
	axis, //germans
};

UENUM(BlueprintType)
enum class ESelectedWeapon  : uint8
{
	primary, //primary weapon
	secondary, //secondary weapon
	frag, //frag grenade
	smoke, //smoke grenade
};

USTRUCT(BlueprintType)
struct FInventoryItem {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFCharacterInventory)
		class AUFWeapon* weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFCharacterInventory)
		UTexture2D* weaponImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFCharacterInventory)
		UTexture2D* ammoImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFCharacterInventory)
		bool isSelected;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFCharacterInventory)
		int32 inventorySlot;
	void CycleIsSelected() { isSelected = !isSelected; }
	FInventoryItem(AUFWeapon* newWeapon, UTexture2D* newWeaponImage, UTexture2D* newAmmoImage,int32 newSlot)
	{
		weapon = newWeapon;
		weaponImage = newWeaponImage;
		isSelected = false;
		inventorySlot = newSlot;
	}
	FInventoryItem()
	{
		weapon = NULL;
		weaponImage = NULL;
		ammoImage = NULL;
		isSelected = false;
		inventorySlot = 0;
	}
	/*bool operator==(const FInventoryItem& firstInventoryItem,const FInventoryItem& secondInventoryItem)
	{
		return firestInventorySlot == secondInventoryItem.inventorySlot;
	}*/
};

UCLASS()
class UNDERFIRE_API AUnderFireCharacter : public ACharacter
{
	GENERATED_BODY()
		//third person mesh
		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* MeshTP;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* TP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* TP_MuzzleLocation;
private:

protected:
	int32 currentInventoryIndex;
public:
	// Sets default values for this pawn's properties
	AUnderFireCharacter();
	
	bool isAi;
	bool isPlayer;
	float currentBodyLife;

//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Inventory)
		//TSubclassOf<AUFWeapon> PrimaryWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = UFCharacterInventory)
		TSubclassOf<class AUFWeapon> PrimaryWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = UFCharacterInventory)
		TSubclassOf<class AUFWeapon> SecondaryWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = UFCharacterInventory)
		TSubclassOf<class AUFWeapon> PrimaryGrenadeClass;

	UPROPERTY(EditDefaultsOnly, Category = UFCharacterInventory)
		TSubclassOf<class AUFWeapon> SecondaryGrenadeClass;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AUnderFireProjectile> ProjectileClass;
	
	//team and name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFCharacter)
		ETeam Team;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFCharacter)
		FString CharacterName;

	//health stuff
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFHealth)
		float MaxHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFHealth)
		float CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UFHealth)
		float LastHitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFHealth)
		float TimeFromHitToStartRegenerate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFHealth)
		float RegenerateHealthPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFCharacter)
		bool isDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFCharacter)
		float bodyLife;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFCharacterInventory)
		TArray<AUFWeapon*> inventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFCharacterInventory)
		AUFWeapon* currentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFCharacterInventory)
		ESelectedWeapon selectedWeapon;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFCharacter)
		bool isAIDS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UFCharacter)
		float MaxDistanceToInteract;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	/** Fires a projectile. */
	void OnFirePressed();

	//aims down sights
	void OnAIDS();

	void OnFireReleased();
	
	void OnReloadPressed();
	
	void CycleNextWeapon();
	void CyclePreviousWeapon();

	void RegenerateHealth();
	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */

	void MoveRight(float Val);
	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	void ToggleCrouch();

	void BodyFadeOut(float DeltaTime);

	void Death();

	UFUNCTION(BlueprintCallable, Category = UFCharacter)
		bool getIsAI() { return isAi; }

	UFUNCTION(BlueprintCallable, Category = UFCharacter)
		bool getIsPlayer() { return isPlayer; }

	UFUNCTION(BlueprintCallable, Category = UFCharacterInventory)
		void addItemToInventory(AUFWeapon* item);

	UFUNCTION(BlueprintCallable, Category = UFCharacterInventory)
		TArray<AUFWeapon*> GetInventory();

	UFUNCTION(BlueprintCallable, Category = UFCharacterInventory)
		bool CharacterHasWeapon(AUFWeapon* weapon);

	UFUNCTION(BlueprintCallable, Category = UFWeapon)
		bool IsCharacterCloseEnoughToInteract(AActor* otherActor);

	UFUNCTION(BlueprintCallable, Category = UFHealth)
		void DoDamageCPlusPlus(float damage);
	
	UFUNCTION(BlueprintImplementableEvent, Category = UFWeapon)
		void DoDamage_Event(float damage);

	UFUNCTION(BlueprintImplementableEvent, Category = UFHealth)
		void Death_Event();
};
