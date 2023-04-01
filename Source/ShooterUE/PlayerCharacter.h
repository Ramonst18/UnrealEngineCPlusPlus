// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class SHOOTERUE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	//variables privadas
	//UPROPERTY Damos propiedades a las variables, en este caso apareceran en blueprints
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;	//Nos servira tener enlazado un objeto a otro
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;		//Camara

	//Variables del movimiento de la camara
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess = "true"))
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess = "true"))
	float BaseLookUpRate;
	
//Si no indicamos si es publica, protected o privada la funcion, por defecto sera privada
	
public:
	// Sets default values for this character's properties
	APlayerCharacter();	//constructor de la clase

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	//funcion que se ejecuta al inicio de darle jugar

	//Funciones de movimiento del personaje
	void MoveForward(float Value);
	void MoveRight(float Value);

	//Funciones del movimiento de la camara
	void TurnAtRate(float Rate);
	void LookAtRate(float Rate);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Funcion que se ejecuta cada momento, buscar alguna alternativa no recomendado de usar

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//Funcion donde se detecta las teclas y que funciones se realizara

	
};
