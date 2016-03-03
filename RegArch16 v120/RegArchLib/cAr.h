#pragma once 
#ifndef _CAR_H_
#define _CAR_H_

#include "cAbstCondMean.h"
#include "cRegArchValue.h"
#include "cRegArchGradient.h"
#include "cRegArchHessien.h"

/*!
	\file cAr.h
	\brief header for class cAr.

	\author Jean-Baptiste DURAND, Ollivier TARAMASCO
	\date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {
	/*! 
	 * \class cAr
	 * \brief  Class to implement an AR model
	 */
	class _DLLEXPORT_ cAr: public cAbstCondMean
	{
	private :
		cDVector mvAr ; ///< Vector of AR coefficients.
	public :
		cAr(const uint theNAr = 0) ; ///< A simple constructor
		cAr(const cDVector& theAr) ; ///< Another constructor
		cAr(cAbstCondMean& theAbstCondMean) ; /// Recopy constructor
		virtual ~cAr() ;  ///< aA simple destructor
		cAbstCondMean* PtrCopy(void) const ; /// < Return a copy of *this
		void Delete(void) ; ///< Delete mvAr
		void Print(ostream& theOut) const ; ///< Print the parameters
		void Print(void) const ; ///< print the parameters
		void SetDefaultInitPoint(double theMean, double theVar) ;
		void Set(const double theValue, const uint theIndex=0, const uint theNumParam=0) ; ///< Set model parameters.
		void Set(const cDVector& theVectParam, const uint theNumParam=0) ; ///< Set model parameters.
		double Get(const uint theIndex=0, const uint theNumParam=0) ;
		void ReAlloc(const uint theSize, const uint theNumParam=0) ; ///< Allocation of the model parameters
		void ReAlloc(const cDVector& theVectParam, const uint theNumParam=0) ; ///< Allocation of the model parameters
		cAbstCondMean& operator=(cAbstCondMean& theSrc) ; ///< Standard affectation
		double ComputeMean(uint theDate, const cRegArchValue& theData) const ; ///< Compute the conditional mean value
		uint GetNParam(void) const ;
		uint GetNLags(void) const ;
		void ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResiduals) ;
		void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData,cRegArchHessien& theHessData, uint theBegIndex, cAbstResiduals* theResiduals) ;
		void RegArchParamToVector(cDVector& theDestVect, uint theIndex) ;
		void VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex = 0) ;

		
		void copy(const cAr & theAr);

	};
}
#endif // _CAR_H_
