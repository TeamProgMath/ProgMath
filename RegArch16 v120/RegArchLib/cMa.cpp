#include "StdAfxRegArchLib.h"
/*!
 \file cMa.cpp
 \brief sources for class cMa methods.

 \author Jean-Baptiste Durand, Ollivier TMAAMASCO
 \date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {
	/*!
	 * \fn cMa::cMa(uint theNMa):cAbstCondMean(eMa)
	 * \param int theNMa: number of lags.
	 */

	cMa::cMa(uint theNMa)
	:cAbstCondMean(eMa) // call constructor of cAbstCondMean with type eMa
	{
		mvMa.ReAlloc(theNMa) ;
		MESS_CREAT("cMa")
	}

	/*!
	 * \fn cMa::cMa(const cDVector& theMa):cAbstCondMean(eMa)
	 * \param const cDVector& theAr: vector of AR coefficients.
	 */
	cMa::cMa(const cDVector& theMa)
	:cAbstCondMean(eMa) // call constructor of cAbstCondMean with type eMa
	{
		mvMa = theMa ;
		MESS_CREAT("cMa")
	}

	/*!
	 * \fn cMa::~cMa()
	 */
	cMa::~cMa()
	{
		mvMa.Delete() ;
		MESS_DESTR("cMa")
	}

	/*!
	 * \fn cAbstCondMean* cMa::PtrCopy()
	 */
	cAbstCondMean* cMa::PtrCopy() const
	{
		 cMa *mycMa = new cMa();

		 mycMa->copy(*this);

		 return mycMa;
	}

	/*!
	 * \fn void cMa::Delete(void)
	 * \param void
	 */
	void cMa::Delete(void)
	{
		mvMa.Delete() ;
	}

	/*!
	 * \fn void cMa::Print(ostream& theOut) const
	 * \param ostream& theOut: the output stream, default cout.
	 */
	void cMa::Print(ostream& theOut) const
	{
	uint myNMa = mvMa.GetSize();
		theOut << "MA(" << myNMa << ") mean with:" << endl ;
		for (register uint i = 0 ; i <  myNMa ; i++)
			theOut << "\tMA[" << i+1 << "]=" << mvMa[i] << endl ;
	}

	/*!
	 * \fn void cMa::Set(double theValue, uint theIndex=0, uint theNumParam)
	 * \param double theValue: the theIndex th value
	 * \param uint theIndex: the index
	 * \param uint theNumParam: not used here
	 * \details mvMa[theIndex] = theValue
	 */
	void cMa::Set(double theValue, uint theIndex, uint theNumParam)
	{
		if (theIndex >= mvMa.GetSize())
			throw cError("Bad index") ;
		else
			mvMa[theIndex]=theValue ;
	}

	/*!
	 * \fn void cMa::Set(const cDVector& theVectParam, uint theNumParam)
	 * \param const cDVector& theVectParam: the vector of MA coefficients
	 * \param uint theNumParam: not used here
	 * \details mvMa = theVectParam
	 */
	void cMa::Set(const cDVector& theVectParam, uint theNumParam)
	{
		mvMa=theVectParam ;
	}

	double cMa::Get(uint theIndex, uint theNumParam)
	{
		return mvMa[theIndex] ;
	}

	/*!
	 * \fn void cMa::ReAlloc(uint theSize, uint theNumParam)
	 * \param uint theSize: new size of mvMA
	 * \param uint theNumParam; not used here.
	 * \details new allocation of mvMa 
	 */
	void cMa::ReAlloc(uint theSize, uint theNumParam)
	{
		mvMa.ReAlloc(theSize) ;
	}

	/*!
	 * \fn void cAr::ReAlloc(const cDVector& theVectParam, uint theNumParam)
	 * \param const cDVector& theVectParam: the vector of AR coefficients
	 * \param uint theNumParam: not used here.
	 * \details new allocation of mvAr
	 */
	void cMa::ReAlloc(const cDVector& theVectParam, uint theNumParam)
	{
		mvMa = theVectParam ;
	}

	/*!
	 * \fn cMa::ComputeMean(uint theDate, const cRegArchValue& theData) const
	 * \param int theDate: date of the computation
	 * \param cRegArchValue& theData: past datas.
	 * \details theData is not updated here.
	 */
	double cMa::ComputeMean(uint theDate, const cRegArchValue& theData) const
	{
		uint myq = mvMa.GetSize() ;

		double myRes = 0.0 ;
			for (register uint i = 1 ; i <= MIN(myq, theDate) ; i++)
				myRes += mvMa[i-1] * theData.mUt[theDate-i] ;
		return myRes ;
	}

	uint cMa::GetNParam(void) const
	{
		return mvMa.GetSize() ;
	}
	uint cMa::GetNLags(void) const
	{
		return mvMa.GetSize() ;
	}

	void cMa::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResids)
	{
	uint myq = mvMa.GetSize() ;
	register uint i ;
		for (i = 1 ; i <= MIN(myq, theDate) ; i++)
			theGradData.mCurrentGradMu[theBegIndex+i-1] += theValue.mUt[theDate - i] ;
		for (i = 1 ; i <= MIN(myq, theDate) ; i++)
			theGradData.mCurrentGradMu -=  mvMa[i-1] * theGradData.mGradMt[i-1] ;
	}

	void cMa::ComputeHess(uint theDate, const cRegArchValue & theData, cRegArchGradient & theGradData, cRegArchHessien & theHessData, uint theBegIndex, cAbstResiduals * theResiduals)
	{
		uint myq = mvMa.GetSize();
		register uint i;
		register uint j;
		// for every element
		for (i = 1; i <= MIN(myq, theDate); i++)
			theHessData.mCurrentHessMu -= mvMa[i - 1] * theHessData.mHessMt[i - 1];
		// for Ma coefficients ( 1st part: with grad)
		for (i = 1; i <= MIN(myq, theDate); i++)
			for (j = 1; j <= MIN(myq, theDate); j++)
				theHessData.mCurrentHessMu[theBegIndex + i - 1][theBegIndex + j - 1] -= theGradData.mGradMt[theDate - j][theDate - i];
		// for Ma coefficients (if we derivate first according to MA coefficients  )
		for (i = 1; i <= MIN(myq, theDate); i++)
			for (j = 1; j <= MIN(myq, theDate); j++)
				theHessData.mCurrentHessMu[theBegIndex + i - 1][theBegIndex + j - 1] -= theGradData.mGradMt[theDate - i][theBegIndex + j - 1];

		
	}

	void cMa::RegArchParamToVector(cDVector& theDestVect, uint theIndex)
	{
		uint mySize = mvMa.GetSize() ;
		if (theDestVect.GetSize() < mySize + theIndex)
			throw cError("Wrong size") ;
		mvMa.SetSubVectorWithThis(theDestVect, theIndex) ;
	}

	void cMa::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
		uint mySize = theSrcVect.GetSize() ;
		if (mvMa.GetSize() + theIndex > mySize)
			throw cError("Wrong size") ;
		mvMa.SetThisWithSubVector(theSrcVect, theIndex) ;
	}

	void cMa::copy(const cMa& theMa)
	{
		mvMa = theMa.mvMa;
	}

}//namespace
