// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/BaseItem.h"

#include "Blueprint/WidgetTree.h"
#include "Components/BillboardComponent.h"
#include "Interaction/InteractionInterface.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include  "Interaction/BaseInspectItem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/W_Note.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("SceneRootComponent"));
	RootComponent = SceneComponent;
	
	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("ItemMesh"));
	ItemMeshComponent->SetupAttachment(RootComponent);

	InspectItemReference = CreateDefaultSubobject<ABaseInspectItem>(FName("InspectItem"));
	InspectItemReference->SetActorHiddenInGame(false);

	InteractionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphereComponent"));
	InteractionSphereComponent->SetupAttachment(RootComponent);
		
	InteractionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnInteractOverlapBegin);
	InteractionSphereComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnInteractOverlapEnd);

	NotifySphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("NotifySphereComponent"));
	NotifySphereComponent->SetupAttachment(RootComponent);

	NotifySphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnNotifyOverlapBegin);
	NotifySphereComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnNotifyOverlapEnd);

	NotifyBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(FName("NotifyBillboardComponent"));
	NotifyBillboardComponent->SetupAttachment(NotifySphereComponent);
	NotifyBillboardComponent->SetRelativeScale3D(FVector(0.2f));

	InteractionBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(FName("InteractionBillboardComponent"));
	InteractionBillboardComponent->SetupAttachment(InteractionSphereComponent);
	InteractionBillboardComponent->SetRelativeScale3D(FVector(0.3f));
	
}

//Used to update the values in the editor when they are changed
void ABaseItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if(ItemMesh != nullptr)
		ItemMeshComponent->SetStaticMesh(ItemMesh);
	
	ItemMeshComponent->SetWorldScale3D(ItemScale);
	
	InteractionSphereComponent->SetSphereRadius(InteractionSphereRadius);
	
	NotifySphereComponent->SetSphereRadius(NotifySphereRadius);
	
	NotifyBillboardComponent->SetRelativeLocation(FVector(IconLocation));
	if(NotifyIcon != nullptr)
		NotifyBillboardComponent->SetSprite(NotifyIcon);
	
	InteractionBillboardComponent->SetRelativeLocation(FVector(IconLocation));
	if(InteractIcon != nullptr)
		InteractionBillboardComponent->SetSprite(InteractIcon);
	
	//Configure Debug.
	ConfigureDebug(bIsDebug);
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerCharacterReference = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	if(ItemMesh != nullptr)
		ItemMeshComponent->SetStaticMesh(ItemMesh);

	ItemMeshComponent->SetRelativeScale3D(ItemScale);

	InteractionSphereComponent->SetSphereRadius(InteractionSphereRadius);
	NotifySphereComponent->SetSphereRadius(NotifySphereRadius);

	if(InteractIcon != nullptr)
		InteractionBillboardComponent->SetSprite(InteractIcon);
	
	if(NotifyIcon != nullptr)
		NotifyBillboardComponent->SetSprite(NotifyIcon);

	InteractionBillboardComponent->SetRelativeLocation(IconLocation);
	NotifyBillboardComponent->SetRelativeLocation(IconLocation);
	
	ConfigureDebug(false);
	
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseItem::OnInteract_Implementation()
{
	IInteractionInterface::OnInteract_Implementation();
	if(!bIsAutoRead && bIsInspect)
	{
		FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
		SpawnParameters.Name = FName("InspectItem");
		
		if (!InspectItemReference->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Error, TEXT("SpawnActor failed! Check if InspectItemClass is valid and the world exists."));
			return;
		}
		
		InspectItemReference->InitVariables(NoteWidgetClass, InspectWidgetClass, InputMappingContext, MoveAction, InteractAction, RenderTarget2D);
		
		InspectItemReference->SetActorLocation(FVector(1000,0,0));
		
		InspectItemReference->SetInspectRotationScaleOffset(DefaultInspectionRotation, DefaultInspectionOffset, DefaultInspectionScale);
		InspectItemReference->SetActorHiddenInGame(true);

		

		UE_LOG(LogTemp, Warning, TEXT("Created Inspect Item Actor"));
		Execute_Inspect(InspectItemReference, PlayerCharacterReference, ItemMeshComponent->GetStaticMesh(), ItemName, ItemDescription);
		Execute_ReadNote(InspectItemReference, PlayerCharacterReference, NoteText);
		
	}

	if (bIsAutoRead)
	{
		AutoReadNote();
		
		UE_LOG(LogTemp, Warning, TEXT("Pitcked Up Item Via code"));
	}
	
}

void ABaseItem::AutoReadNote()
{
	if(!bIsNoteActive && NoteWidgetClass) // Ensure the prefab class is assigned
	{
		UW_Note* NoteWidgetInstance = CreateWidget<UW_Note>(GetWorld(), NoteWidgetClass, FName("NoteWidget"));
		
		if(NoteWidgetInstance->IsValidLowLevel())
		{
			NoteWidgetReference =NoteWidgetInstance;
			NoteWidgetReference->NoteText = NoteText;
			NoteWidgetReference->AddToViewport();

			//Disable the input for the character movement and set the look input to be ignored.
			PlayerCharacterReference->GetCharacterMovement()->DisableMovement();
			PlayerController->SetIgnoreLookInput(true);
			//Sets the opacity of the Canvas panel.
			NoteCanvasPanel = Cast<UCanvasPanel>(NoteWidgetReference->WidgetTree->FindWidget(NoteWidgetReference->NoteCanvasPanelName));
			if(NoteCanvasPanel != nullptr)
				NoteCanvasPanel->SetRenderOpacity(1);
			bIsNoteActive = true;
			
		}
	}
	else
	{
		//Enables the input for the character movement and set the look input to not be ignored.
		if(NoteCanvasPanel !=nullptr)
			NoteCanvasPanel->SetRenderOpacity(0);
		if(NoteWidgetReference != nullptr)
			NoteWidgetReference->RemoveFromParent();
		NoteCanvasPanel = nullptr;
		NoteWidgetReference = nullptr;
		
		PlayerCharacterReference->GetCharacterMovement()->SetMovementMode(PlayerCharacterReference->GetCharacterMovement()->DefaultLandMovementMode);
		PlayerController->SetIgnoreLookInput(false);
		bIsNoteActive = false;
		
		
	}
}

void ABaseItem::ConfigureDebug(bool bIsVisible)
{
	NotifySphereComponent->SetVisibility(bIsVisible);
	InteractionSphereComponent->SetVisibility(bIsVisible);
	NotifyBillboardComponent->SetVisibility(bIsVisible);
	InteractionBillboardComponent->SetVisibility(bIsVisible);
}

void ABaseItem::OnInteractOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == PlayerCharacterReference)
	{
		InteractionBillboardComponent->SetVisibility(true);
		NotifyBillboardComponent->SetVisibility(false);
		
	}
}

void ABaseItem::OnInteractOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor == PlayerCharacterReference)
	{
		InteractionBillboardComponent->SetVisibility(false);
		NotifyBillboardComponent->SetVisibility(true);
		
	}
}

void ABaseItem::OnNotifyOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == PlayerCharacterReference)
	{
		NotifyBillboardComponent->SetVisibility(true);
		
	}
}

void ABaseItem::OnNotifyOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor == PlayerCharacterReference)
	{
		NotifyBillboardComponent->SetVisibility(false);
		
	}
}

