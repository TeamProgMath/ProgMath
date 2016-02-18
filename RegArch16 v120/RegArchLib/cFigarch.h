#pragma once 
#ifndef _CFIGARCH_H_
#define _CFIGARCH_H_

#include "cAbstCondVar.h"
#include "cRegArchValue.h"
#include "cRegArchGradient.h"
/*!
\file cFigarch.h
\brief Definition of the FIGARCH(p, d, q) class

\author Jean-Baptiste DURAND, Ollivier TARAMASCO
\date jan-26-2016 - Last change jan-26-2016
*/
namespace RegArchLib {

	/*!
	* \class cFiarch
	* \brief Class to implement a 'pure' GARCH(p, q) model
	*/
	class _DLLEXPORT_ cFigarch : public cAbstCondVar
	{
	private:
		double mvConst; ///< Constant part of GARCH(p, q) variance model.
		cDVector mvArch; ///< Vector of ARCH coefficients. 
		cDVector mvGarch; ///< Vector of GARCH coefficients.
		double mvFracD; ///< Value of the fractal d parameter
	private :
		uint mvNTruncLag; ///< Number of lags for ARCH representation
		cArch*	mvTruncArch; ///< ARCH approximation of FIGARCH Model
		cPolynome mvPhi;
		cPolynome mvTeta;
		cPolynome mvARMAPol;
		cPolynome mvGradAlpha;
		cPolynome mvGradBeta;
		cPolynome mvGradD;
		cArch** mvTruncArchForGrad; ///< Used to compute gradient
		double mvh; ///< step for numerical gradient
	public:
		cFigarch(uint theNArch = 0, uint theNGarch = 0, double theFracD = 0, uint theNTruncLag=100, double theh = 1e-3); ///< A simple constructor
		cFigarch(double theConst, const cDVector& theArch, const cDVector& theGarch, double theFracD, uint theNTruncLag=100, double theh=1e-3); ///< Another constructor
		cFigarch(cAbstCondVar& theGarch);
		virtual ~cFigarch(); ///< A simple destructor
		virtual cAbstCondVar* PtrCopy() const; /// < Return a copy of *this				
		void Delete(void); /// Delete
		void Print(ostream& theOut = cout) const; ///< Print the parameters
		void SetDefaultInitPoint(double theMean, double theVar);
		void ReAlloc(const uint theSize, const uint theNumParam = 0); ///< Allocation of the model parameters
		void ReAlloc(const cDVector& theVectParam, const uint theNumParam = 0); ///< Allocation of the model parameters
		void Set(const double theValue, const uint theIndex = 0, const uint theNumParam = 0); ///< Set model parameters.
		void Set(const cDVector& theVectParam, const uint theNumParam = 0); ///< Set model parameters.
		double Get(const uint theIndex = 0, const uint theNumParam = 0);
		cAbstCondVar& operator=(cAbstCondVar& theSrc); ///< Standard affectation
		void UpdateProxyVarParameters(void);
		double ComputeVar(uint theDate, const cRegArchValue& theData) const;	///< Return conditional variance.
		uint GetNParam(void) const; ///< Number of parameters in that model part
		uint GetNArch(void) const; ///< Number of ARCH parameters
		uint GetNGarch(void) const; ///< Number of GARCH parameters
		uint GetNLags(void) const; ///< Number of past gradients required to compute gradient at current time t.
		cArch* GetTruncArchForGrad(uint theIndex);
		double Geth(void);
		void ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cAbstResiduals* theResiduals);
		void RegArchParamToVector(cDVector& theDestVect, uint theIndex = 0);
		void VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex = 0);
		void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals);
	};

}
#endif // _CFIGARCH_H_
