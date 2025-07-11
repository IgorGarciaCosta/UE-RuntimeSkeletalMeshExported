#pragma once
#include "CoreMinimal.h"

namespace RMEHelpers
{
	/* Retorna o caminho absoluto da pasta Downloads do usu�rio,
	   independente de SO ou de build (Editor / Shipping).             */
	inline FString GetDownloadsDirectory()
	{
#if PLATFORM_WINDOWS
		const FString Home = FPlatformMisc::GetEnvironmentVariable(TEXT("USERPROFILE"));
#elif PLATFORM_LINUX || PLATFORM_MAC
		const FString Home = FPlatformMisc::GetEnvironmentVariable(TEXT("HOME"));
#else
		const FString Home = FPlatformProcess::UserDir();   // fallback gen�rico
#endif
		FString Downloads = FPaths::Combine(Home, TEXT("Downloads"));

		// Caso n�o exista (ex.: em distros Linux minimalistas) criamos.
		IFileManager::Get().MakeDirectory(*Downloads, /*Tree*/ true);
		return Downloads;
	}
}
