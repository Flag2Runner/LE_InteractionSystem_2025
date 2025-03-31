// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/BaseInspectItem.h"

#include "Blueprint/WidgetTree.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/SceneCapture2D.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseInspectItem::ABaseInspectItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("SceneRootComponent"));
	RootComponent = SceneComponent;
	
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
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(this, USceneCaptureComponent2D::StaticClass());
	
	SceneCaptureComponent = Cast<USceneCaptureComponent2D>(FoundActor);
	
	
}

// Called every frame
void ABaseInspectItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerController)
	{
		if (PlayerController->WasInputKeyJustPressed(FKey(EKeys::E)))
		{
			
		}
		if (PlayerController->WasInputKeyJustPressed(FKey(EKeys::W)))
		{
			
		}
		if (PlayerController->WasInputKeyJustPressed(FKey(EKeys::A)))
		{
			
		}
		if (PlayerController->WasInputKeyJustPressed(FKey(EKeys::S)))
		{
			
		}
		if (PlayerController->WasInputKeyJustPressed(FKey(EKeys::D)))
		{
			
		}
		//Press to toggle between read note and inspect
		if (PlayerController->WasInputKeyJustPressed(FKey(EKeys::SpaceBar)))
		{
			if()
			if(!bIsInNote)
			{
				NoteWidgetReference->
			}
		}
		//Set mouse movement
		if (PlayerController->GetMousePosition(MouseX, MouseY) && !bIsInNote && bIsRotating)
		{
			FRotator CombinedRotators = ItemPivotMeshComponent->GetComponentRotation() + FRotator(0,MouseY * -3,MouseX * -3);
			ItemPivotMeshComponent->SetWorldRotation(CombinedRotators);
		}
		//Zoom In
		if (PlayerController->WasInputKeyJustPressed(FKey(EKeys::MouseScrollDown)) && !bIsInNote)
		{
			float NewFOV = SceneCaptureComponent->FOVAngle - 1;
			UKismetMathLibrary::FClamp(NewFOV, 30,80);
			
		}
		//Zoom out
		if (PlayerController->WasInputKeyJustPressed(FKey(EKeys::MouseScrollUp)) && !bIsInNote)
		{
			float NewFOV = SceneCaptureComponent->FOVAngle + 1;
			UKismetMathLibrary::FClamp(NewFOV, 30,80);
		}
		//ResetTransform of Inspected Item;
		if (PlayerController->WasInputKeyJustPressed(FKey(EKeys::RightMouseButton)) && !bIsInNote)
		{
			ItemPivotMeshComponent->SetWorldRotation(InitialRotation);
			SceneCaptureComponent->FOVAngle = 50;
		}
		//Only Allows the rotation when clicking the left mouse button.
		if (PlayerController->WasInputKeyJustPressed(FKey(EKeys::LeftMouseButton)))
		{
			bIsRotating = true;
		}
		if (PlayerController->WasInputKeyJustReleased(FKey(EKeys::MouseScrollUp)))
		{
			bIsRotating = false;
		}
	}

}

void ABaseInspectItem::ReadNote_Implementation(ACharacter* Character, const FText& NoteText)
{
	IInteractionInterface::ReadNote_Implementation(Character, NoteText);

	PlayerCharacterReference = Character;

	UW_Note* NoteWidgetInstance = CreateWidget<UW_Note>(GetWorld(), NoteWidgetClass, FName("NoteWidget"));
		
	if(NoteWidgetInstance->IsValidLowLevel())
	{
		NoteWidgetReference =NoteWidgetInstance;
		NoteWidgetReference->NoteText = NoteText;
		NoteWidgetReference->AddToViewport();

		//Sets the opacity of the Canvas panel.
		NoteReadBorder = Cast<UBorder>(NoteWidgetReference->WidgetTree->FindWidget(NoteWidgetReference->NoteReadBorderName));
		if(NoteReadBorder != nullptr)
			NoteReadBorder->SetRenderOpacity(1);
		
		PlayerCharacterReference->DisableInput(PlayerController);
		PlayerCharacterReference->EnableInput(PlayerController);
	}
	
}

void ABaseInspectItem::Inspect_Implementation(ACharacter* Character, UStaticMesh* InspectedItemMesh,
	const FText& ItemName, const FText& ItemDescription)
{
	IInteractionInterface::Inspect_Implementation(Character, InspectedItemMesh, ItemName, ItemDescription);

	PlayerCharacterReference = Character;

	ItemMeshComponent->SetStaticMesh(InspectedItemMesh);

	UW_Inspect* InspectWidgetInstance = CreateWidget<UW_Inspect>(GetWorld(), InspectWidgetClass, FName("InspectWidget"));
	if(InspectWidgetInstance->IsValidLowLevel())
	{
		InspectWidgetReference = InspectWidgetInstance;
		InspectWidgetReference->ItemName = ItemName;
		InspectWidgetReference->ItemDescription = ItemDescription;
		InspectWidgetReference->AddToViewport();

		PlayerCharacterReference->DisableInput(PlayerController);
		PlayerCharacterReference->EnableInput(PlayerController);
		bIsInNote = false;
	}
}

