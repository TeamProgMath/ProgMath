#pragma once 
#ifndef _CARCH_H_
#define _CARCH_H_

#include "cAbstCondVar.h"
#include "cRegArchValue.h"
#include "cRegArchGradient.h"
#include "cRegArchHessien.h"

	/*!
		\file cArch.h
		\brief Definition of the ARCH(p) class
	
		\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
		\date dec-18-2006 - last change nov-10-2009
	*/
namespace RegArchLib {
	/*! 
	 * \class cArch
	 * \brief Class to implement a 'pure' ARCH model
	 */
	class _DLLEXPORT_ cArch: public cAbstCondVar
	{
	private :
		double mvConst ; ///< Constant part of ARCH(p) variance model.
		cDVector mvArch ; ///< Vector of ARCH coefficients. 
	public :
		cArch(uint theNArch = 0) ; ///< A simple constructor
		cArch(cAbstCondVar& theArch) ; 
		virtual ~cArch() ; ///< A simple destructor
		virtual cAbstCondVar* PtrCopy() const ; /// < Return a copy of *this		
		void Delete(void) ; ///< Free memory
		void Print(ostream& theOut=cout) const ; ///< Print the ARCH(p) parameters
		void SetDefaultInitPoint(double theMean, double theVar) ;
		void Set(const double theValue, const uint theIndex=0, const uint theNumParam=0) ; ///< Set model parameters.
		void Set(const cDVector& theVectParam, const uint theNumParam=0) ; ///< Set model parameters.
		double Get(const uint theIndex=0, const uint theNumParam=0) ;
		void ReAlloc(const uint theSize, const uint theNumParam=0) ; ///< Allocation of the model parameters
		void ReAlloc(const cDVector& theVectParam, const uint theNumParam=0) ; ///< Allocation of the model parameters
		cAbstCondVar& operator=(cAbstCondVar& theSrc); ///< Standard affectation
		void UpdateProxyVarParameters(void) {}; 
		double ComputeVar(uint theDate, const cRegArchValue& theData) const; ///< Return conditional variance.
		uint GetNParam(void) const ; ///< Number of parameters in that model part
		uint GetNLags(void) const ; ///< Number of past gradients required to compute gradient at current time
		void ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResiduals) ;
		void RegArchParamToVector(cDVector& theDestVect, uint theIndex) ;
		void VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex = 0) ;
		void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals);
		void copy(const cArch & theArch);
	} ;
}

#endif // _CARCH_H_
