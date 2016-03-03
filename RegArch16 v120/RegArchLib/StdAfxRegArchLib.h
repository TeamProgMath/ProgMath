// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once

#ifdef _MSC_VER // Compilateur MS VS
	#include "TargetVerRegArchLib.h"
#endif // _MSC_VER

#include <cmath>
#include <iostream>

#include "StdAfxError.h"
#ifdef _RDLL_
	#include "StdAfxGslEquiv.h"
#endif // _RDLL_
#include "StdAfxVectorAndMatrix.h"
#ifndef _RDLL_
	#include "StdAfxWrapperGslCpp.h"
	#include "StdAfxWrapperNloptCpp.h"
#endif // _RDLL_

#include "RegArchDef.h"
#include "cAbstCondMean.h"
#include "cAbstCondVar.h"
#include "cAbstResiduals.h"

#include "cPolynome.h"

#include "cConst.h"
#include "cAr.h"
#include "cMa.h"
#include "cStdDevInMean.h"
#include "cVarInMean.h"
#include "cLinReg.h"

#include "cConstCondVar.h"
#include "cArch.h"
#include "cGarch.h"
#include "cTarch.h"
#include "cEgarch.h"
#include "cAparch.h"
#include "cFigarch.h"

#include "cNormResiduals.h"
#include "cStudentResiduals.h"

#include "GslAndNloptOptim.h"

#include "cCondMean.h"
#include "RegArchCompute.h"
#include "cRegArchModel.h"
#include "cRegArchValue.h"
#include "cRegArchGradient.h"
#include "cRegArchHessien.h"


#define WIN32_LEAN_AND_MEAN             // Exclure les en-têtes Windows rarement utilisés



// TODO: faites référence ici aux en-têtes supplémentaires nécessaires au programme
