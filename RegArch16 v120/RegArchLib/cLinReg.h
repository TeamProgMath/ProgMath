#pragma once 
#ifndef _CLINREG_H_
#define _CLINREG_H_

	/*!
		\file cLinReg.h
		\brief Definition of the cLinReg class
	
		\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
		\date dec-18-2006 - Last change feb-18-2011
	*/
#include "cAbstCondMean.h"
#include "cRegArchValue.h"
#include "cRegArchGradient.h"

namespace RegArchLib {
	class _DLLEXPORT_ cLinReg : public cAbstCondMean
	{
	private :
		cDVector mvBeta ;
	public :
		cLinReg(int theNLinReg = 0) ;
		cLinReg(cAbstCondMean& theAbstCondMean) ; /// Recopy constructor
		virtual ~cLinReg() ;
		cAbstCondMean* PtrCopy(void) const ; /// < Return a copy of *this
		void Delete(void) ;
		void Print(ostream& theOut) const ;
		void Print(void) const ; ///< print the parameters
		void SetDefaultInitPoint(double theMean, double theVar) ;
		void Set(const double theValue, const uint theIndex=0, const uint theNumParam=0)  ; ///< Set model parameters.
		void Set(const cDVector& theVectParam, const uint theNumParam=0) ; ///< Set model parameters.
		double Get(const uint theIndex=0, const uint theNumParam=0) ;
		void ReAlloc(const cDVector& theParam, const uint theNumParam=0) ;
		void ReAlloc(const uint theParam, const uint theNumParam=0) ;
		cAbstCondMean& operator=(cAbstCondMean& theSrc) ;
		double ComputeMean(uint theDate, const cRegArchValue& theData) const ;
		uint GetNParam(void) const ; ///< Return number of parameters
		uint GetNLags(void) const ;
		void RegArchParamToVector(cDVector& theDestVect, uint theIndex = 0) ;
		void VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex = 0) ;
		void ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResiduals) ;
		void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData,cRegArchHessien& theHessData, uint theBegIndex, cAbstResiduals* theResiduals) ;
	} ;

}
#endif // _CLinReg_H_
