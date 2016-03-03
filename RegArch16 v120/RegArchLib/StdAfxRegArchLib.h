// stdafx.h�: fichier Include pour les fichiers Include syst�me standard,
// ou les fichiers Include sp�cifiques aux projets qui sont utilis�s fr�quemment,
// et sont rarement modifi�s
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


#define WIN32_LEAN_AND_MEAN             // Exclure les en-t�tes Windows rarement utilis�s



// TODO: faites r�f�rence ici aux en-t�tes suppl�mentaires n�cessaires au programme
