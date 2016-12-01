// Fill out your copyright notice in the Description page of Project Settings.

#include "UnderFire.h"
#include "UnderFirePlayerCharacter.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "GameFramework/InputSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AUnderFirePlayerCharacter

AUnderFirePlayerCharacter::AUnderFirePlayerCharacter()
{

	isPlayer = true;
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(20.0f, 10.0f, 75.0f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("FP_MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));



	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->Hand = EControllerHand::Right;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	VrCameraOffset = FVector(0, 0, 0);

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;
	isActionButtonPressed = false;
	isFButtonPressed = false;
	timeFButtonPressed = 0.f;
}

void AUnderFirePlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (GEngine)
	{
		if (GEngine->HMDDevice.IsValid())
		{
			FirstPersonCameraComponent->SetRelativeLocation(VrCameraOffset);
		}
		else
		{
			
		}
	}
	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}
	CharacterName = FString(TEXT("Donovan"));
}

void AUnderFirePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isFButtonPressed)
	{
		timeFButtonPressed += DeltaTime;
		if (timeFButtonPressed >= timeRequiredToPressAction)
		{
			isActionButtonPressed = true;
		}

	}
	else
	{
		if (timeFButtonPressed > 0 && timeFButtonPressed < timeRequiredToPressAction)
		{
			if (currentWeapon)
			{
				currentWeapon->ReloadStart();
			}
		}
		timeFButtonPressed = 0.f;
	}
}

void AUnderFirePlayerCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AUnderFirePlayerCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AUnderFirePlayerCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFirePressed();
	}
	TouchItem.bIsPressed = false;
}

void AUnderFirePlayerCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
	{
		if (TouchItem.bIsPressed)
		{
			if (GetWorld() != nullptr)
			{
				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
				if (ViewportClient != nullptr)
				{
					FVector MoveDelta = Location - TouchItem.Location;
					FVector2D ScreenSize;
					ViewportClient->GetViewportSize(ScreenSize);
					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.X * BaseTurnRate;
						AddControllerYawInput(Value);
					}
					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.Y * BaseTurnRate;
						AddControllerPitchInput(Value);
					}
					TouchItem.Location = Location;
				}
				TouchItem.Location = Location;
			}
		}
	}
}



bool AUnderFirePlayerCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	bool bResult = false;
	if (FPlatformMisc::GetUseVirtualJoysticks() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		bResult = true;
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AUnderFirePlayerCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AUnderFirePlayerCharacter::EndTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AUnderFirePlayerCharacter::TouchUpdate);
	}
	return bResult;
}

// Called to bind functionality to input
void AUnderFirePlayerCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	// set up gameplay key bindings
	check(InputComponent);

	//character position input
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAction("Crouch", IE_Pressed, this, &AUnderFireCharacter::ToggleCrouch);

	//weapon inputs
	InputComponent->BindAction("Fire", IE_Pressed, this, &AUnderFireCharacter::OnFirePressed);
	InputComponent->BindAction("Fire", IE_Released, this, &AUnderFireCharacter::OnFireReleased);
	InputComponent->BindAction("AIDS", IE_Pressed, this, &AUnderFireCharacter::OnAIDS);
	InputComponent->BindAction("Reload", IE_Pressed, this, &AUnderFireCharacter::OnReloadPressed);
	InputComponent->BindAction("Action", IE_Pressed, this, &AUnderFirePlayerCharacter::OnActionPressed);
	InputComponent->BindAction("Action", IE_Released, this, &AUnderFirePlayerCharacter::OnActionReleased);
	InputComponent->BindAction("Next Weapon", IE_Pressed, this, &AUnderFireCharacter::CycleNextWeapon);
	InputComponent->BindAction("Previous Weapon", IE_Pressed, this, &AUnderFireCharacter::CyclePreviousWeapon);

	InputComponent->BindAxis("MoveForward", this, &AUnderFireCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AUnderFireCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("TurnRate", this, &AUnderFireCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AUnderFireCharacter::LookUpAtRate);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AUnderFirePlayerCharacter::OnResetVR);
}

void AUnderFirePlayerCharacter::OnActionPressed()
{
	isFButtonPressed = true;
	/*float timePressed = 0.f;
	float StartTime = GetWorld()->GetTimeSeconds();
	while (isActionButtonPressed && timePressed < 1.5)
	{
	timePressed = GetWorld()->GetTimeSeconds() - StartTime;
	}

	if (timePressed >= 1.5)
	{
	isActionButtonPressed = true;
	}
	else
	{
	if (currentWeapon)
	{
	currentWeapon->ReloadStart();
	}
	}*/
}

void AUnderFirePlayerCharacter::OnActionReleased()
{
	isFButtonPressed = false;
	isActionButtonPressed = false;
}

/*


*/