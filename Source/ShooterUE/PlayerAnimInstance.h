// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERUE_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	
public:
	//Funciones
	//Indicamos que la funcion de abajo puede ser llamada como un nodo dentro de los Blueprint
	//UFUNCTION(BlueprintCallable)	 
	//void UpdateAnimationProperties(float Deltatime);	//FUNCION OBSOLETA, YA NO EXISTE, SE USA LA QUE ESTA ABAJO

	UFUNCTION(BlueprintCallable)
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	//Con virtual estamos diciendo que vamos a sobreescribir una funcion y a lo ultimo un override
	virtual void NativeInitializeAnimation() override;

private:

	//hacemos que sean visibles nuestras clases y variables que esten debajo antes del ;, además de poder verlo en los 
	//blueprints Las clases y variables la asignamos en la categoria Movement e indicamos el meta la cual decimos que 
	//seran privadas
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement,meta = (AllowPrivateAccess = "true"))
	
	//Clases
	class APlayerCharacter* PlayerCharacter;	//Variable puntero que sera utilizada para poder referenciar al jugador

	//variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement,meta = (AllowPrivateAccess = "true"))
	float Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement,meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement,meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement,meta = (AllowPrivateAccess = "true"))
	float MovementOffSetYaw;	//Yaw, referencia al eje Z
};
