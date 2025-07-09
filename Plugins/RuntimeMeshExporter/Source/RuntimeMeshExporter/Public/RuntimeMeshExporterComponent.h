#pragma once
/*  RuntimeMeshExporter  –  Plugin
	Exporta em runtime a pose atual (skinned) de um SkeletalMeshComponent
	para um .OBJ.  NÃO sobrescreve arquivos: acrescenta _1, _2, _3…

	Uso em Blueprint:
		RuntimeMeshExporterComponent → ExportToOBJ(TargetMesh, "D:/Folder")
*/

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RuntimeMeshExporterComponent.generated.h"

class USkeletalMeshComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RUNTIMEMESHEXPORTER_API URuntimeMeshExporterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URuntimeMeshExporterComponent();

	/* Caminho completo do arquivo .obj a gerar */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Runtime OBJ Export")
	FString OutputPath = TEXT("D:/Temp/CurrentPose.obj");

	/* Chame esta função para efetuar o export */
	UFUNCTION(BlueprintCallable, Category = "Runtime OBJ Export")
	void ExportToOBJ(USkeletalMeshComponent* SkeletalMeshComp);
protected:
	virtual void BeginPlay() override;
};
