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


	/* Chame esta função para efetuar o export */
	UFUNCTION(BlueprintCallable, Category = "Runtime OBJ Export")
	void ExportToOBJ(USkeletalMeshComponent* SkeletalMeshComp);
protected:
	virtual void BeginPlay() override;
};
