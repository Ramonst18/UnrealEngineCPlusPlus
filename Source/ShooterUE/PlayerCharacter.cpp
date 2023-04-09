// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	//ESTE ES EL CONSTRUCTOR
	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	//Indicamos que la clase puede usar la funcion Tick

	//NOTA: CON -> ESTAMOS ACCEDIENDO A ATRIBUTOS DE LA CLASE
	
	//variable, se crea un subObjeto Defautl <Tipo del objeto>, El metodo nos pide el nombre que tendra en unreal
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringCamera"));	
	CameraBoom->SetupAttachment(RootComponent);		//Le asignamos una funcion para atar el componente al RootComponent
	CameraBoom->TargetArmLength = 300;	//indicamos la distancia entre el springCamera y los objetos que tenga unidos
	CameraBoom->bUsePawnControlRotation = true;	//que rote el spring cuando rote el jugador


	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);	//atamos la camara con el spring camera
	//especificamente al Socket del springCamera
	FollowCamera->bUsePawnControlRotation = false;	//hacemos que la camara no rote
	//NOTA IMPORTANTE: todas variables con b al inicio significan que son booleanas

	//Damos valores a las variables del movimiento de la camara
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	//Hacemos que el personaje rote con el movimiento de la camara, solo la manera horizontal
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	
}//End constructor

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	
	//variables
	//int numero = 3;
	//float numeroDecimal = 5.6;
	//FString cadena = "ADIOS";

	//NOTA: el %d indica que pasaremos variables enteras y sera la posición en donde estaran
	//el %f pasaremos variables flotantes
	//el %s pasaremos variables cadenas, debemos de poner un * al inicio del nombre para que pueda acceder al valor
	//ya que el FString es conciderado como un objeto
	
	//imprimir en consola
	//UE_LOG(LogTemp, Warning, TEXT("HOLA MUNDO AAAAAAAAAAA %s %f %d"), *cadena, numeroDecimal, numero);

	//comprobamos que existe
	/*if (GEngine)
	{
		//llamamos a la clase GEngine y llamamos una funcion
		//imprimir en pantalla
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Yellow, TEXT("Esto es un mensaje de pruebas"));
	}*/
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PASAREMOS AL PlayerInputComponent LAS TECLAS QUE NOS FUNCIONARAN COMO EL ACCIONADOR AL MOVIMIENTO DEL PERSONAJE
	//pasamos el nombre del axis, el objeto al que hara el movimiento y pasamos la funcion( accedemos la direccion de memoria de nuestro personaje y obtenemos la funcion)
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this, &APlayerCharacter::MoveRight);

	//INPUTS DEL MOVIMIENTO DE LA CAMARA
	//movimiento mediante teclas
	PlayerInputComponent->BindAxis("TurnRate",this,&APlayerCharacter::TurnAtRate);	//Movimiento con funciones propias
	PlayerInputComponent->BindAxis("LookUpRate",this,&APlayerCharacter::LookAtRate);
	//Movimiento mediante el raton
	PlayerInputComponent->BindAxis("Turn",this,&APawn::AddControllerYawInput);	//Movimiento con funciones ya establecidas
	PlayerInputComponent->BindAxis("LookUp",this,&APawn::AddControllerPitchInput);

	//SALTO DEL PERSONAJE
	//BindAction espera: El nombre del actionMaping, en que momento se ejecutará(Cuando se presione, se suelte u otro), que clase lo hara y despues la funcion a ejecutar
	PlayerInputComponent->BindAction("Jump",IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump",IE_Released, this, &ACharacter::StopJumping);

	//DISPARO DEL PERSONAJE
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::Fire);	//Click Dispara
}

//FUNCIONES
void APlayerCharacter::MoveForward(float Value)
{
	//comprobamos que el controlador no sea nulo y que el valor sea distinto de cero
	if (Controller != nullptr && Value != 0.0f)
	{
		//obtenemos la rotacion
		const FRotator Rotation = Controller->GetControlRotation();

		//Obtenemos el Yaw rotation (la rotacion del eje z)
		const FRotator YawRotation(0, Rotation.Yaw,0);

		//Creamos una direccion, es a donde se movera el personaje
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);	//primero obtenemos la direccion en x
		AddMovementInput(Direction, Value);		//El movimiento se estará dando al character Movement
		
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	//comprobamos que el controlador no sea nulo y que el valor sea distinto de cero
	if (Controller != nullptr && Value != 0.0f)
	{
		//obtenemos la rotacion
		const FRotator Rotation = Controller->GetControlRotation();

		//Obtenemos el Yaw rotation (la rotacion del eje z)
		const FRotator YawRotation(0, Rotation.Yaw,0);

		//Creamos una direccion, es a donde se movera el personaje
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);	//primero obtenemos la direccion en y
		AddMovementInput(Direction, Value);		//El movimiento se estará dando al character Movement
		
	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	/*MOVIMIENTO DE LA CAMARA DE FORMA HORIZONTAL*/
	//Multiplicamos el valor obtenido por el BaseTurnRate y este por el GetWorld()->getDeltaSeconds
	//Este ultimo es para asegurar la velocidad del movimiento de la camara sea igual para casi todos
	//los fps actuales del juego
	AddControllerYawInput(Rate*BaseTurnRate*GetWorld()->GetDeltaSeconds());
	
}

void APlayerCharacter::LookAtRate(float Rate)
{
	/*MOVIMIENTO DE LA CAMARA DE FORMA VERTICAL*/
	
	AddControllerPitchInput(Rate*BaseLookUpRate*GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::Fire()
{
	//SONIDO
	//Comprobamos que existe el sonido de disparo
	if (FireSound)
	{
		//UGameplayStatics Nos permite obtener elementos de nuestro programa o ejecutar cosas
		//En este caso queremos ejecutar un sonido
		UGameplayStatics::PlaySound2D(this, FireSound);
	}
	
	//Animacion
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();	//Del mesh obtenemos el AnimINstance
	//Verificamos que tenemos el AnimINstance y el FireAnimMontage
	if (AnimInstance && FireAnimMontage)
	{
		//Ejecutamos un AnimMontage
		AnimInstance->Montage_Play(FireAnimMontage);
		//Del Montage nos iremos a la seccion StartFire y se ejecutara a partir de ahi
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}
	
}
