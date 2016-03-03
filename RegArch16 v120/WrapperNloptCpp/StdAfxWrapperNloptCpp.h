// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once
#ifdef _CLI_
	#ifndef _DLLEXPORT_
		#define _DLLEXPORT_  __declspec(dllexport)
	#endif // _DLLEXPORT_
#else
	#define _DLLEXPORT_
#endif // _CLI_


#define WIN32_LEAN_AND_MEAN             // Exclure les en-têtes Windows rarement utilisés
#include "WrapperNloptCpp.h"
#include <stdlib.h>   




// TODO: faites référence ici aux en-têtes supplémentaires nécessaires au programme
