#pragma once 
#ifndef _CCONDVAR_H_
#define _CCONDVAR_H_

#include "RegArchDef.h"
#include "cAbstCondVar.h"
#include "cRegArchValue.h"
#include "cRegArchGradient.h"
#include <vector>

/*!
\file cCondVar.h
\brief Definition of conditional var class model

\author test
\date dec-18-2006 - Last change feb-18-2011
*/


namespace RegArchLib {
	/*!
	* \class cCondVar
	* \brief Class definition of a conditional var model
	* \details A conditional var model is always a sum of 'elementary' conditional var components.
	*/
	class _DLLEXPORT_ cCondVar
	{
	private:
		vector<cAbstCondVar*> mvCondVar; ///< Vector of the mvNCondVar conditional var components
		uint mvNCondVar;
	public:
		cCondVar(uint theNCondVar = 0); ///< A simple constructor
		cCondVar(const cCondVar& theCondVar);
		virtual ~cCondVar(); ///< A simple destructor
		void Delete(void); ///< Free memory.
		uint GetNVar(void) const; ///< Return the number of var components.
		vector<cAbstCondVar*> GetCondVar(void) const; ///< Return mvCondVar
	//	void SetOneVar(uint theWhatVar, eCondVarEnum theCode); ///< Set the type of a given var component. 
		void SetOneVar(uint theWhatVar, cAbstCondVar& theAbstCondVar); ///< Set theWhatVar th component of the conditional var model
	//	void AddOneVar(cAbstCondVar& theAbstCondVar); ///< Set theWhatVar th component of the conditional var model
		cAbstCondVar* GetOneVar(uint theIndex) const; ///< Return a pointer to the given var component  (no object is allocated)
		void Print(ostream& theOut = cout) const; ///< Print the conditional var model
		friend ostream& operator <<(ostream& theOut, const cCondVar& theCondVar); ///< Print the conditional var model
		//void SetDefaultInitPoint(double theVar, double theVar);
		void GetCondVarType(eCondVarEnum* theCodeType) const; ///< Return the type of each var component	
		cCondVar& operator =(cCondVar& theSrc); ///< affectation operator
		double ComputeVar(uint theDate, const cRegArchValue& theData) const; ///< Return conditional var.
																			  /** Number of parameters in CondVar */
		uint GetNParam(void) const;
		double Get(uint theNumVar = 0, uint theIndex = 0, uint theNumParam = 0);
		uint GetNLags(void) const; ///< (Maximal) number of past gradients required to compute gradient at current time t.
		void ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cAbstResiduals* theResiduals);
		void RegArchParamToVector(cDVector& theDestVect, uint theIndex) const;
		void VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex = 0);
		void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals);
	};
}

#endif // _CCONDVAR_H_
