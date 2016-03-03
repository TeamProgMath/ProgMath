#pragma once 
#ifndef _CCONSTCONDVAR_H_
#define _CCONSTCONDVAR_H_

#include "cAbstCondVar.h"
#include "cRegArchValue.h"
#include "cRegArchGradient.h"

/*!
	\file cConstCondVar.h
	\brief Definition of the constant variance class
	
	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {
	/*! 
	 * \class cConstCondVar
	 * \brief Class to implement a constant conditional variance model
	 */
	class _DLLEXPORT_ cConstCondVar: public cAbstCondVar
	{
	private :
		double mvCste ; //< Value of constant variance
	public :

		void copy(const cConstCondVar & theConstCondVar);

		cConstCondVar(double theValue=1.0) ; ///< A simple constructor
	//	cConstCondVar(cAbstCondVar& theConstCondVar);
		virtual ~cConstCondVar() ; ///< A simple destructor
		virtual cAbstCondVar* PtrCopy() const ; /// < Return a copy of *this		
		void Print(ostream& theOut) const ; ///< Print the parameters
		//void SetDefaultInitPoint(double theMean, double theVar) ;
		void Delete(void) ; ///< Delete
		void Set(const double theValue, const uint theIndex=0, const uint theNumParam=0) ; ///< Set model parameters.
		void Set(const cDVector& theVectParam, const uint theNumParam=0) ; ///< Set model parameters.
		double Get(const uint theIndex=0, const uint theNumParam=0) ;
		void ReAlloc(const uint theSize, const uint theNumParam=0) ; ///< Allocation of the model parameters
		void ReAlloc(const cDVector& theVectParam, const uint theNumParam=0) ; ///< Allocation of the model parameters
		//cAbstCondVar& operator=(cAbstCondVar& theSrc); ///< Standard affectation
		void UpdateProxyVarParameters(void) {}; 
		double ComputeVar(uint theDate, const cRegArchValue& theData) const ; ///< Return conditional variance.
		uint GetNParam(void) const ; ///< Number of parameters in that model part
		uint GetNLags(void) const ; ///< Number of past gradients required to compute gradient at current time t.
		void ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResiduals) ;
		void RegArchParamToVector(cDVector& theDestVect, uint theIndex) ;
		void VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex = 0) ;
		void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals);
	
	} ;
}
#endif //_CCONSTCONDVAR_H_
