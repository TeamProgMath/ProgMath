#pragma once
#ifndef _CARFIMA_H_
#define _CARFIMA_H_

#include "cAbstCondMean.h"
#include "cRegArchValue.h"
#include "cRegArchGradient.h"
#include "cRegArchHessien.h"

#include <vector>

/*!
\file cARFIMA.h
\brief Definition of the ARFIMA(p, d, q) class

\author Jean-Baptiste DURAND, Ollivier TARAMASCO
\date jan-26-2016 - Last change jan-26-2016
*/
namespace RegArchLib {

	/*!
	* \class cArfima
	* \brief Class to implement a ARFIMA(p, d, q) model
	*/
	class _DLLEXPORT_ cArfima : public cAbstCondMean
	{
	private:
		cDVector mvAr; ///< Vector of AR coefficients. 
		cDVector mvMa; ///< Vector of MA coefficients.
		double mvFracD; ///< Value of the fractal d parameter
	private:
		uint mvNTruncLag; ///< Number of lags for AR representation
		cMa* mvTruncMa; ///< MA approximation of ARFIMA Model
		cPolynome mvPolMa;
		vector<cPolynome*> mvGradPolMa;

	public:
		cArfima(uint theNAr=0, uint theNMa=0, double theFracD=0, uint theNTruncLag = 100); ///< A simple constructor
		cArfima(const cDVector& theAr, const cDVector& theMa, double theFracD, uint theNTruncLag = 100); ///< Another constructor
		cArfima(const cArfima& theArfima); /// Recopy constructor
		virtual ~cArfima();  ///< aA simple destructor
		cAbstCondMean* PtrCopy(void) const; /// < Return a copy of *this
		void Delete(void); ///< Delete mvAr
	#ifndef _RDLL_
		void Print(ostream& theOut = cout) const; ///< print the parameters
	#else
		void Print(void);
	#endif //_RDLL_
		void SetDefaultInitPoint(double theMean, double theVar);
		void Set(const double theValue, const uint theIndex = 0, const uint theNumParam = 0); ///< Set model parameters.
		void Set(const cDVector& theVectParam, const uint theNumParam = 0); ///< Set model parameters.
		double Get(const uint theIndex = 0, const uint theNumParam = 0);
		void ReAlloc(const uint theSize, const uint theNumParam = 0); ///< Allocation of the model parameters
		void ReAlloc(const cDVector& theVectParam, const uint theNumParam = 0); ///< Allocation of the model parameters
		cArfima& operator=(const cArfima& theSrc); ///< Standard affectation
		void UpdateProxyMeanParameters(void) ;
		double ComputeMean(uint theDate, const cRegArchValue& theData) const; ///< Compute the conditional mean value
		uint  GetNAr(void) const;
		uint  GetNMa(void) const;
		uint GetNParam(void) const;
		uint GetNLags(void) const;
		void ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResiduals);
		void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, uint theBegIndex, cAbstResiduals* theResiduals);
		void RegArchParamToVector(cDVector& theDestVect, uint theIndex);
		void VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex = 0);
		void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals);
	};

}
#endif // __CARFFIMA_H_

