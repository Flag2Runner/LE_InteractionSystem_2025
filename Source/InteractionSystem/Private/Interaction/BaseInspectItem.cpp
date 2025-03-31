// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/BaseInspectItem.h"

#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/WidgetTree.h"
#include "Components/PointLightComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseInspectItem::ABaseInspectItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("SceneRootComponent"));
	RootComponent = SceneComponent;

	UStaticMeshComponent* TempPhotoBothMeshReference = CreateDefaultSubobject<UStaticMeshComponent>(FName("PhotoBooth"));
	TempPhotoBothMeshReference->SetupAttachment(RootComponent);
	TempPhotoBothMeshReference->SetWorldScale3D(FVector(13.5f));

	UPointLightComponent* PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(FName("PointLight"));
	PointLightComponent->SetupAttachment(RootComponent);
	PointLightComponent->Intensity = 2500.f;
	PointLightComponent->LightingChannels.bChannel0 = false;
	PointLightComponent->LightingChannels.bChannel1 = true;
	PointLightComponent->LightingChannels.bChannel2 = false;

	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(FName("SceneCaptureComponent2D"));
	
	ItemPivotMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("ItemPivot"));
	ItemPivotMeshComponent->SetupAttachment(RootComponent);

	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("ItemMesh"));
	ItemMeshComponent->SetupAttachment(ItemPivotMeshComponent);

	ItemMeshComponent->LightingChannels.bChannel0 = false;
	ItemMeshComponent->LightingChannels.bChannel1 = true;
	ItemMeshComponent->LightingChannels.bChannel2 = false;

}

// Called when the game starts or when spawned
void ABaseInspectItem::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	SceneCaptureComponent->ShowOnlyActorComponents(this);
	SceneCaptureComponent->CaptureSource = SCS_FinalColorHDR;
	SceneCaptureComponent->bCaptureEveryFrame = true;
	SceneCaptureComponent->bCaptureOnMovement = true;

	
}

void ABaseInspectItem::OnMove(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>(); // Get movement input
	UE_LOG(LogTemp, Warning, TEXT("Move X: %f, Move Y: %f"), MoveVector.X, MoveVector.Y);

	if(!bIsInNote && (MoveVector.X != 0 || MoveVector.Y != 0))
	{
		FRotator NewRotation = ItemPivotMeshComponent->GetComponentRotation();

		NewRotation.Yaw += MoveVector.X * -2.f;
		NewRotation.Pitch += MoveVector.Y * -2.f;
		ItemPivotMeshComponent->SetWorldRotation(NewRotation);
	}
}

void ABaseInspectItem::Interact(const FInputActionValue& Value)
{
	if (InspectWidgetReference->IsValidLowLevel() || NoteWidgetReference->IsValidLowLevel())
	{
		if(InspectWidgetReference)
		{
			InspectWidgetReference->RemoveFromParent();
			InspectWidgetReference = nullptr;
		}
		if(NoteWidgetReference)
		{
			NoteWidgetReference->RemoveFromParent();
			NoteWidgetReference = nullptr;
		}
				
		RootComponent->SetVisibility(false);
		PlayerCharacterReference->EnableInput(PlayerController);
		DisableInput(PlayerController);
	}
}

// Called every frame
void ABaseInspectItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerController)
	{
		if (PlayerController->WasInputKeyJustPressed(EKeys::E))
		{
			
			
		}
		//Press to toggle between read note and inspect
		if (PlayerController->WasInputKeyJustPressed(EKeys::SpaceBar) && NoteWidgetReference->IsValidLowLevel())
		{
			UCanvasPanel* NoteCanvasPanel = Cast<UCanvasPanel>(NoteWidgetReference->WidgetTree->FindWidget(NoteWidgetReference->NoteCanvasPanelName));
			if(!bIsInNote)
			{
				NoteCanvasPanel->SetRenderOpacity(1);
				bIsInNote = true;
			}
			else
			{
				NoteCanvasPanel->SetRenderOpacity(0);
				bIsInNote = false;
			}
		}
		//Set mouse movement
		if (PlayerController->GetMousePosition(MouseX, MouseY) && !bIsInNote && bIsRotating)
		{
			FRotator CombinedRotators = ItemPivotMeshComponent->GetComponentRotation() + FRotator(0,MouseY * -3,MouseX * -3);
			ItemPivotMeshComponent->SetWorldRotation(CombinedRotators);
		}
		//Zoom In
		if (PlayerController->WasInputKeyJustPressed(EKeys::MouseScrollDown) && !bIsInNote)
		{
			float NewFOV = SceneCaptureComponent->FOVAngle - 1;
			UKismetMathLibrary::FClamp(NewFOV, 30,80);
			
		}
		//Zoom out
		if (PlayerController->WasInputKeyJustPressed(EKeys::MouseScrollUp) && !bIsInNote)
		{
			float NewFOV = SceneCaptureComponent->FOVAngle + 1;
			UKismetMathLibrary::FClamp(NewFOV, 30,80);
		}
		//ResetTransform of Inspected Item;
		if (PlayerController->WasInputKeyJustPressed(EKeys::RightMouseButton) && !bIsInNote)
		{
			ItemPivotMeshComponent->SetWorldRotation(InitialRotation);
			SceneCaptureComponent->FOVAngle = 50;
		}
		//Only Allows the rotation when clicking the left mouse button.
		if (PlayerController->WasInputKeyJustPressed(EKeys::LeftMouseButton))
		{
			bIsRotating = true;
		}
		if (PlayerController->WasInputKeyJustReleased(EKeys::MouseScrollUp))
		{
			bIsRotating = false;
		}
	}

}

void ABaseInspectItem::ReadNote_Implementation(ACharacter* Character, const FText& NoteText)
{
	IInteractionInterface::ReadNote_Implementation(Character, NoteText);

	PlayerCharacterReference = Character;

	if(UW_Note* NoteWidgetInstance = CreateWidget<UW_Note>(GetWorld(), NoteWidgetClass, FName("NoteWidget")))
	{
		NoteWidgetReference =NoteWidgetInstance;
		NoteWidgetReference->NoteText = NoteText;
		NoteWidgetReference->AddToViewport();

		//Sets the opacity of the Canvas panel.
		NoteReadBorder = Cast<UBorder>(NoteWidgetReference->WidgetTree->FindWidget(NoteWidgetReference->NoteReadBorderName));
		if(NoteReadBorder != nullptr)
			NoteReadBorder->SetRenderOpacity(1);
		
		PlayerCharacterReference->DisableInput(PlayerController);
		EnableInput(PlayerController);
	}
	
}

void ABaseInspectItem::Inspect_Implementation(ACharacter* Character, UStaticMesh* InspectedItemMesh,
	const FText& ItemName, const FText& ItemDescription)
{
	IInteractionInterface::Inspect_Implementation(Character, InspectedItemMesh, ItemName, ItemDescription);

	PlayerCharacterReference = Character;

	ItemMeshComponent->SetStaticMesh(InspectedItemMesh);
	UW_Inspect* InspectWidgetInstance = CreateWidget<UW_Inspect>(GetWorld(), InspectWidgetClass, FName("InspectWidget"));
	if(InspectWidgetInstance)
	{
		InspectWidgetReference = InspectWidgetInstance;
		InspectWidgetReference->ItemName = ItemName;
		InspectWidgetReference->ItemDescription = ItemDescription;
		InspectWidgetReference->AddToViewport();

		PlayerCharacterReference->DisableInput(PlayerController);
		EnableInput(PlayerController);
		bIsInNote = false;
	}
}

void ABaseInspectItem::SetInspectRotationScaleOffset(const FRotator& ItemRotation, float NewInspectionOffset,
                                                     const FVector& InspectionScale)
{
	InitialRotation = ItemRotation;
	FVector NewMeshLocation = ItemMeshComponent->GetComponentLocation();
	NewMeshLocation.Z = NewInspectionOffset;
	InspectionOffset = NewMeshLocation;
	ItemMeshComponent->SetWorldLocation(InspectionOffset);
	ItemMeshComponent->SetWorldScale3D(InspectionScale);
	ItemPivotMeshComponent->SetWorldRotation(InitialRotation);
}

void ABaseInspectItem::InitVariables(TSubclassOf<UW_Note> NewNoteWidgetClass,
	TSubclassOf<UW_Inspect> NewInspectWidgetClass, const UInputMappingContext* NewInputMappingContext,
	const UInputAction* NewMoveAction, const UInputAction* NewInteractAction, UTextureRenderTarget2D* NewRenderTarget2D)
{
	NoteWidgetClass = NewNoteWidgetClass;
	InspectWidgetClass = NewInspectWidgetClass;
	InputMappingContext = NewInputMappingContext;
	MoveAction = NewMoveAction;
	InteractAction = NewInteractAction;
	RenderTarget2D =NewRenderTarget2D;

	SceneCaptureComponent->TextureTarget = RenderTarget2D;
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (InputSubsystem && InputMappingContext)
		{
			InputSubsystem->AddMappingContext(InputMappingContext, 0);
		}

		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
		if (EnhancedInputComponent && MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseInspectItem::OnMove);

			//Interaction
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ABaseInspectItem::Interact);
		}
	}
	
	
}

