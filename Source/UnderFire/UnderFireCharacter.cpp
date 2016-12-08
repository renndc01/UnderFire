// Fill out your copyright notice in the Description page of Project Settings.

#include "UnderFire.h"
#include "UnderFireCharacter.h"

#include "UnderFireProjectile.h"
#include "Animation/AnimInstance.h"


// Sets default values
AUnderFireCharacter::AUnderFireCharacter()
{

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//create mesh for 3rd person view
	//MeshTP = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PawnMeshTP"));
	GetMesh()->SetOnlyOwnerSee(false);
	GetMesh()->SetOwnerNoSee(false);
	GetMesh()->bCastDynamicShadow = true;
	GetMesh()->CastShadow = true;
	GetMesh()->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	GetMesh()->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	TP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TP_Gun"));
	TP_Gun->SetOnlyOwnerSee(false);			// only the owning player will see this mesh
	TP_Gun->SetOwnerNoSee(false);
	TP_Gun->bCastDynamicShadow = true;
	TP_Gun->CastShadow = true;
	// TP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	TP_Gun->SetupAttachment(RootComponent);


	TP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("TP_MuzzleLocation"));
	TP_MuzzleLocation->SetupAttachment(TP_Gun);
	TP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
//	GunOffset = FVector(100.0f, 30.0f, 10.0f);

	isAi = false;
	isPlayer = false;
	
	MaxHealth = 100;
	CurrentHealth = MaxHealth;
	isDead = false;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	isAIDS = false;
	bodyLife = 60.f;
	currentBodyLife = 0.f;
	MaxDistanceToInteract = 100.f;
	RegenerateHealthPercentage = 0.f;
	TimeFromHitToStartRegenerate = 0.f;
	LastHitTime = 0.f;

}

// Called when the game starts or when spawned
void AUnderFireCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	currentInventoryIndex = 0;
	if (PrimaryWeaponClass != NULL)
	{
		AUFWeapon* temp = GetWorld()->SpawnActor<AUFWeapon>(PrimaryWeaponClass, GetActorLocation(), GetActorRotation());
		temp->owningCharacter = this;
		currentWeapon = temp;
		temp->isSelected = true;
		addItemToInventory(temp);
	}
	if (SecondaryWeaponClass != NULL)
	{
		AUFWeapon* temp = GetWorld()->SpawnActor<AUFWeapon>(SecondaryWeaponClass, GetActorLocation(), GetActorRotation());
		temp->owningCharacter = this;
		temp->isSelected = false;
		temp->WeaponMesh->SetHiddenInGame(true);
		addItemToInventory(temp);
	}
	if (PrimaryGrenadeClass != NULL)
	{
		AUFWeapon* temp = GetWorld()->SpawnActor<AUFWeapon>(SecondaryWeaponClass, GetActorLocation(), GetActorRotation());
		temp->owningCharacter = this;
		temp->isSelected = false;
		temp->WeaponMesh->SetHiddenInGame(true);
		addItemToInventory(temp);
	}
	if (SecondaryGrenadeClass != NULL)
	{
		AUFWeapon* temp = GetWorld()->SpawnActor<AUFWeapon>(SecondaryWeaponClass, GetActorLocation(), GetActorRotation());
		temp->owningCharacter = this;
		temp->isSelected = false;
		temp->WeaponMesh->SetHiddenInGame(true);
		addItemToInventory(temp);
	}

	//float StartTime = GetWorld()->GetTimeSeconds();
	/*float CurrentTime = 0.f;
	while (CurrentTime < 1.0)
	{
		CurrentTime += GetWorld()->DeltaTimeSeconds;
	}
	*/
	for (int i = 0; i < inventory.Num(); i++)
	{
		inventory[i]->AttachToOwner();

	}

}

// Called every frame
void AUnderFireCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float currentTime = GetWorld()->GetTimeSeconds();
	if (LastHitTime > 0.f && (currentTime - TimeFromHitToStartRegenerate > LastHitTime))
	{
		RegenerateHealth();
	}
}

void AUnderFireCharacter::OnFirePressed()
{
	if (currentWeapon == NULL)
	{
		return;
	}
	//call fire on the currently selected weapon
	currentWeapon->StartFire();
}

void AUnderFireCharacter::OnFireReleased()
{
	if (currentWeapon == NULL)
	{
		return;
	}
	//stop fire for currently selected weapon
	currentWeapon->StopFire();
}

void AUnderFireCharacter::OnReloadPressed()
{
	if (currentWeapon)
	{
		currentWeapon->ReloadStart();
	}
}

void AUnderFireCharacter::MoveForward(float Value)
{
	if (!isDead)
	{
		if (Value != 0.0f)
		{
			// add movement in that direction
			AddMovementInput(GetActorForwardVector(), Value);
		}
	}
}

void AUnderFireCharacter::MoveRight(float Value)
{
	if (!isDead)
	{
		if (Value != 0.0f)
		{
			// add movement in that direction
			AddMovementInput(GetActorRightVector(), Value);
		}
	}
}

void AUnderFireCharacter::TurnAtRate(float Rate)
{
	if (!isDead)
	{
		// calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AUnderFireCharacter::LookUpAtRate(float Rate)
{
	if (!isDead)
	{
		// calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

/*
	callback function to make the character crouch
*/
void AUnderFireCharacter::ToggleCrouch()
{
	if (!isDead)
	{
		if (CanCrouch())
		{
			Crouch();
		}
		else
		{
			UnCrouch();
		}
	}
}

/*
	function for when the player aims down the sights, Honestly its names wrong, should be ADS not AIDS
*/
void AUnderFireCharacter::OnAIDS()
{
	if (!isDead)
	{
		isAIDS = !isAIDS;
	}
}

/*
	fades the body out after death, doesn't work right now because of material issues
*/
void AUnderFireCharacter::BodyFadeOut(float DeltaTime)
{
	if (isDead)
	{

	}
	//for some reason we arent dead anymore?
	else
	{
		//GetMesh->
	}
}

/*
	adds a weapon to the inventory, used for giving a weapon
*/
void AUnderFireCharacter::addItemToInventory(AUFWeapon* item)
{
	if (!isDead)
	{

		if (inventory.Contains(item))
		{
			int32 index = inventory.Find(item);
			inventory.RemoveAt(index);
			inventory.Insert(item, index);
		}
		else if (!inventory.Contains(item))
		{
			inventory.Add(item);
		}
		//item->inventorySlot = inventory.Num();
		inventory.Add(item);
	}
}

/*
	returns the inventory array 
*/
TArray<AUFWeapon*> AUnderFireCharacter::GetInventory()
{

	return inventory;
}

/*
	cycle the current weapon to the next weapon in the inventory
*/
void AUnderFireCharacter::CycleNextWeapon()
{
	if (isDead)
	{
		return;
	}
	currentWeapon->CycleIsSelected();
	currentWeapon->WeaponMesh->SetHiddenInGame(true);

	currentInventoryIndex++;
	if (currentInventoryIndex >= inventory.Num())
	{
		currentInventoryIndex = 0;
	}
	
	currentWeapon = inventory[currentInventoryIndex];
	currentWeapon->CycleIsSelected();
	currentWeapon->WeaponMesh->SetHiddenInGame(false);
}


/*
	cycle the current weapon to the previous weapon in the inventory
*/
void AUnderFireCharacter::CyclePreviousWeapon()
{
	if (isDead)
	{
		return;
	}
	currentWeapon->CycleIsSelected();
	currentWeapon->WeaponMesh->SetHiddenInGame(true);
	currentInventoryIndex--;
	if (currentInventoryIndex <= 0)
	{
		currentInventoryIndex = inventory.Num() - 1;
	}
	
	currentWeapon = inventory[currentInventoryIndex];
	currentWeapon->CycleIsSelected();
	
}


/*
	determines if the given weapon is in the characters inventory
*/
bool AUnderFireCharacter::CharacterHasWeapon(AUFWeapon* weapon)
{
	for (AUFWeapon* tempWeapon : inventory)
	{
		if (tempWeapon->WeaponName == weapon->WeaponName)
		{
			return true;
		}
	}
	return false;
}

/*
	determines if the character is close enough to interact with the other actor
*/
bool AUnderFireCharacter::IsCharacterCloseEnoughToInteract(AActor* otherActor)
{
	return (MaxDistanceToInteract >= FVector::Dist(GetActorLocation(), otherActor->GetActorLocation()));
}


/*
	apply damage to the character
*/
void AUnderFireCharacter::DoDamageCPlusPlus(float damage)
{
	CurrentHealth -= damage;
	
	if (CurrentHealth <= 0)
	{
		Death();
	}
	DoDamage_Event(damage);
	
}


/*
	give player health back
*/
void AUnderFireCharacter::RegenerateHealth()
{
	CurrentHealth += RegenerateHealthPercentage * MaxHealth;
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
}

/*
	callback for death
*/
void AUnderFireCharacter::Death()
{
	Death_Event();
	isDead = true;
}