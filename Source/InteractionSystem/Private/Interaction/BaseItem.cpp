// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/BaseItem.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

// void ABaseItem::OnInteract_Implementation()
// {
// 	IInteractionInterface::OnInteract_Implementation();
// 	//To Be implemented
// }
//
// void ABaseItem::Inspect_Implementation(ACharacter* Character, UStaticMesh* InspectedItemMesh, FText ItemName,
// 	FText ItemDescription)
// {
// 	IInteractionInterface::Inspect_Implementation(Character, InspectedItemMesh, ItemName, ItemDescription);
// 	//To Be implemented
// }
//
// void ABaseItem::ReadNote_Implementation(ACharacter* Character, FText NoteText)
// {
// 	IInteractionInterface::ReadNote_Implementation(Character, NoteText);
// 	//To be implemented
// }

