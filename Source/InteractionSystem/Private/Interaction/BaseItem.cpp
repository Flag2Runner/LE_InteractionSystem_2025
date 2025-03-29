// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/BaseItem.h"

#include "Components/BillboardComponent.h"
#include "Interaction/InteractionInterface.h"
#include "Components/StaticMeshComponent.h"
#include "Widgets/W_Note.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("ItemMesh"));
	ItemMeshComponent->SetupAttachment(GetRootComponent());

	FVector MeshLocation = ItemMeshComponent->GetComponentLocation();
	ItemMeshComponent->SetRelativeScale3D(ItemScale);

	InteractionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphereComponent"));
	InteractionSphereComponent->SetupAttachment(GetRootComponent());
	InteractionSphereComponent->InitSphereRadius(InteractionSphereRadius);
	InteractionSphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	InteractionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnInteractOverlapBegin);
	InteractionSphereComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnInteractOverlapEnd);

	NotifySphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("NotifySphereComponent"));
	NotifySphereComponent->SetupAttachment(GetRootComponent());
	NotifySphereComponent->InitSphereRadius(NotifySphereRadius);
	NotifySphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	NotifySphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnNotifyOverlapBegin);
	NotifySphereComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnNotifyOverlapEnd);

	NotifyBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(FName("NotifyBillboardComponent"));
	NotifyBillboardComponent->SetupAttachment(NotifySphereComponent);
	NotifyBillboardComponent->SetRelativeLocation(FVector(IconLocation));
	NotifyBillboardComponent->SetRelativeScale3D(FVector(0.2f));

	InteractionBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(FName("InteractionBillboardComponent"));
	InteractionBillboardComponent->SetupAttachment(InteractionSphereComponent);
	InteractionBillboardComponent->SetRelativeLocation(FVector(IconLocation));
	InteractionBillboardComponent->SetRelativeScale3D(FVector(0.3f));

	if(bIsDebug)
	{
		//Configure Debug.
	}
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	
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
		UE_LOG(LogTemp, Warning, TEXT("Created Inspect Item Actor"));

		//Still need to create the Inspect Actor Script.

		//Execute_Inspect();
		Execute_ReadNote(NoteWidgetReference, PlayerCharacterReference, NoteText);
	}

	if (bIsAutoRead)
	{
		AutoReadNote();
		
		UE_LOG(LogTemp, Warning, TEXT("Pitcked Up Item Via code"));
	}
	
}

void ABaseItem::AutoReadNote()
{
	if(bIsNoteActive && NoteWidgetClass) // Ensure the prefab class is assigned
	{
		if(UW_Note* NoteWidgetInstance = CreateWidget<UW_Note>(GetWorld(),
			NoteWidgetClass, FName("NoteWidget")))
		{
			NoteWidgetInstance->NoteText = NoteText;
			NoteWidgetInstance->AddToViewport();
		}
	}
	else
	{
		NoteWidgetReference->RemoveFromParent();
	}
}

void ABaseItem::OnInteractOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABaseItem::OnInteractOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ABaseItem::OnNotifyOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABaseItem::OnNotifyOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

