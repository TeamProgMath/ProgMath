// stdafx.h�: fichier Include pour les fichiers Include syst�me standard,
// ou les fichiers Include sp�cifiques aux projets qui sont utilis�s fr�quemment,
// et sont rarement modifi�s
//

#pragma once
#ifdef _CLI_
	#ifndef _DLLEXPORT_
		#define _DLLEXPORT_  __declspec(dllexport)
	#endif // _DLLEXPORT_
#else
	#define _DLLEXPORT_
#endif // _CLI_


#define WIN32_LEAN_AND_MEAN             // Exclure les en-t�tes Windows rarement utilis�s
#include "WrapperNloptCpp.h"
#include <stdlib.h>   




// TODO: faites r�f�rence ici aux en-t�tes suppl�mentaires n�cessaires au programme
