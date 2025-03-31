// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInspectItem.h"
#include "Components/CanvasPanel.h"
#include "Components/SphereComponent.h"
#include "Interaction/InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "Widgets/W_Note.h"
#include "BaseItem.generated.h"

UCLASS()
class ABaseItem : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABaseItem();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteract_Implementation() override;

private:
	UFUNCTION()
	void AutoReadNote();

	void ConfigureDebug(bool bIsVisible);
	//*************************//
	//            UI           //
	//*************************//

	UPROPERTY(EditDefaultsOnly, Category = "Public | UI | Prefab")
	TSubclassOf<UW_Note> NoteWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Public | UI | Prefab")
	TSubclassOf<UW_Inspect> InspectWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Public | Mesh | Prefab")
	TSubclassOf<ABaseInspectItem> InspectItemClass;

	UPROPERTY(EditDefaultsOnly, Category = "Public | UI | Prefab")
	const UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Public | UI | Prefab")
	const UInputAction* MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Public | UI | Prefab")
	const UInputAction* InteractAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Public | UI | Teture")
	UTextureRenderTarget2D* RenderTarget2D;

	//*************************//
	//      Interaction        //
	//*************************//
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Public | Interaction | Settings")
	float InteractionSphereRadius = 150.f;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Public | Interaction | Settings")
	float NotifySphereRadius = 200.f;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Public | Interaction | Settings")
	FVector IconLocation;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Public | Interaction | Settings")
	bool bIsDebug;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Public | Interaction | Inspection")
	bool bIsInspect;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Public | Interaction | Inspection")
	FRotator DefaultInspectionRotation;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Public | Interaction | Inspection")
	FVector DefaultInspectionScale = FVector(1.f,1.f,1.f);
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Public | Interaction | Inspection")
	float DefaultInspectionOffset;
	
	//*************************//
	//      Reference          //
	//*************************//
public:
	UPROPERTY(BlueprintReadOnly, Category= "public | Reference")
	ACharacter* PlayerCharacterReference;
	
	UPROPERTY(BlueprintReadOnly, Category= "public | Reference")
	APlayerController* PlayerController;

	UPROPERTY(BlueprintReadOnly, Category= "public | Reference")
	class UW_Note* NoteWidgetReference;

	UPROPERTY(BlueprintReadOnly, Category = "public | Reference")
	ABaseInspectItem* InspectItemReference;

	UPROPERTY(VisibleAnywhere, Category = "public | Reference")
	UCanvasPanel* NoteCanvasPanel;
	
	UPROPERTY(EditDefaultsOnly, Category = "public | Reference | Prefab")
	UTexture2D* InteractIcon;

	UPROPERTY(EditDefaultsOnly, Category = "public | Reference | Prefab")
	UTexture2D* NotifyIcon;
public:
	UPROPERTY(EditDefaultsOnly, Category = "public | Reference | Collision")
	USphereComponent* NotifySphereComponent;

	UPROPERTY(EditDefaultsOnly, Category = "public | Reference | Collision")
	USphereComponent* InteractionSphereComponent;

	UPROPERTY(EditDefaultsOnly, Category = "public | Reference | Bilboard")
	UBillboardComponent* NotifyBillboardComponent;

	UPROPERTY(EditDefaultsOnly, Category = "public | Reference | Bilboard")
	UBillboardComponent* InteractionBillboardComponent;

	UPROPERTY(EditDefaultsOnly, Category = "public | Reference | Mesh")
	UStaticMeshComponent* ItemMeshComponent;

	

	
	//*************************//
	//         Mesh            //
	//*************************//
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Public | Mesh")
	UStaticMesh* ItemMesh;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Public | Mesh")
	FText ItemName;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Public | Mesh")
	FText ItemDescription;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Public | Mesh")
	FVector ItemScale = FVector(1,1,1);

	//*************************//
	//         Note            //
	//*************************//
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Public | Note")
	bool bIsAutoRead;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Public | Note", meta = (MultiLine = "true"))
	FText NoteText;

	UPROPERTY()
	bool bIsNoteActive = false;
	
	//*************************//
	//         Collision       //
	//*************************//
	
	UFUNCTION()
	void OnInteractOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnInteractOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnNotifyOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnNotifyOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
