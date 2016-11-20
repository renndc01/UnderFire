// Fill out your copyright notice in the Description page of Project Settings.

#include "UnderFire.h"
#include "UnderFirePawn.h"
#include "UnderFireProjectile.h"
#include "Animation/AnimInstance.h"


// Sets default values
AUnderFirePawn::AUnderFirePawn()
{
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//create mesh for 3rd person view
	MeshTP = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PawnMeshTP"));
	MeshTP->SetOnlyOwnerSee(false);
	MeshTP->SetOwnerNoSee(true);
	MeshTP->bCastDynamicShadow = true;
	MeshTP->CastShadow = true;
	MeshTP->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	MeshTP->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

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
	GunOffset = FVector(100.0f, 30.0f, 10.0f);
}

// Called when the game starts or when spawned
void AUnderFirePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnderFirePawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AUnderFirePawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{


	Super::SetupPlayerInputComponent(InputComponent);
	// set up gameplay key bindings
	check(InputComponent);

	//InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AUnderFirePawn::OnFire);
	InputComponent->BindAxis("MoveForward", this, &AUnderFirePawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AUnderFirePawn::MoveRight);


	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("TurnRate", this, &AUnderFirePawn::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AUnderFirePawn::LookUpAtRate);
}

void AUnderFirePawn::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((TP_MuzzleLocation != nullptr) ? TP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			// spawn the projectile at the muzzle
			World->SpawnActor<AUnderFireProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = MeshTP->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}
void AUnderFirePawn::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AUnderFirePawn::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AUnderFirePawn::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUnderFirePawn::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}