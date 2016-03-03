#include "StdAfxRegArchLib.h"

/*!
	\file cArch.cpp
	\brief sources for class cArch methods.

	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - Last change feb-18-2011
*/

namespace RegArchLib {
	/*!
	 * \fn cArch::cArch(uint theNArch):cAbstCondVar(eArch)
	 * \param uint theNArch: number of lags
	 */
	cArch::cArch(uint theNArch)
	:cAbstCondVar(eArch)   // call constructor of cAbstCondVar with type eArch
	{
		mvArch.ReAlloc(theNArch) ;
		MESS_CREAT("cArch")
	}

	/*!
	 * \fn cArch::~cArch()
	 */
	cArch::~cArch()
	{	mvArch.Delete() ;
  		MESS_DESTR("cArch") ;
	}

	/*!
	 * \fn void cArch::Delete(void)
	 * \param void
	 * \details Delete mvVar
	 */
	void cArch::Delete(void)
	{	mvArch.Delete() ;
	}

	/*!
	 * \fn cAbstCondVar* cArch::PtrCopy()
	 */

	cAbstCondVar* cArch::PtrCopy() const
	{
		 cArch *mycArch = new cArch();

		 mycArch->copy(*this);

		 return mycArch;
	}

	/*!
	 * \fn void cArch::Print(ostream& theOut) const
	 * \param ostream& theOut: output stream (file or screen). Default cout.
	 */
	void cArch::Print(ostream& theOut) const
	{
	uint myNArch = mvArch.GetSize() ;
		theOut << "ARCH(" << myNArch << ") model with:" << endl ;
		for (register uint i = 0 ; i < mvArch.GetSize() ; i++)
			theOut << "\tARCH[" << i+1 << "]=" << mvArch[i] << endl ;
	}

	/*!
	 * \fn void cArch::ReAlloc(uint theSize, uint theNumParam)
	 * \param uint theSize: new size of mvAr
	 * \param uint theNumParam: not used here.
	 * \details new allocation of mvArch
	 */
	void cArch::ReAlloc(uint theSize, uint theNumParam)
	{
		mvArch.ReAlloc(theSize) ;
	}

	/*!
	 * \fn void cArch::ReAlloc(const cDVector& theVectParam, uint theNumParam)
	 * \param const cDVector& theVectParam: the vector of Const or ARCH coefficients
	 * \param uint theNumParam: =0, the constant part; =1 the ARCH coefficients
	 * \details new allocation of mvArch or mvConst
	 */
	void cArch::ReAlloc(const cDVector& theVectParam, uint theNumParam)
	{	switch (theNumParam)
		{	case 0: // mvConst
				mvArch = theVectParam ;
			break ;
			default :
	//			throw cError("cArch::ReAlloc - theNumParam must be in 0, 1") ;
			break ;
		}
	}

	/*!
	 * \fn void cArch::Set(double theValue, uint theIndex, uint theNumParam)
	 * \brief fill the parameters vector
	 * \param double theValue: the value of the "theIndex" th lag. Default 0.
	 * \param uint theIndex: the index.
	 * \param uint theNumParam: =0, mvConst, =1, ARCH parameters
	 * \details mvArch[theIndex] = theValue or mvConst = theValue
	 */
	void cArch::Set(double theValue, uint theIndex, uint theNumParam)
	{	switch (theNumParam)
		{	case 0 :
				if (theIndex < mvArch.GetSize())
					mvArch[theIndex] = theValue ;
				else
					throw cError("cArch::Set - wrong index") ;
			break ;
			default:
				throw cError("cArch::Set - theNumParam must be in 0, 1") ;
			break ;
		}
	}

	double cArch::Get(uint theIndex, uint theNumParam)
	{
		double res = 0.0;
		switch (theNumParam)
		{	case 0 :
				res = mvArch[theIndex] ;
			break ;
		}
		return res;
	}


	/*!
	 * \fn void cArch::Set(const cDVector& theVectParam, uint theNumParam)
	 * \brief fill the parameters vector
	 * \param const cDVector& theVectParam: the vector of values
	 * \param uint theNumParam: =0, mvConst; =1, mvArch
	 * \details mvArch = theVectParam or mvConst = theVectParam[0]
	 */
	void cArch::Set(const cDVector& theVectParam, uint theNumParam)
	{	switch (theNumParam)
		{	case 0 :
				mvArch = theVectParam ;
			break ;
			default:
				throw cError("cArch::Set - theNumParam must be in 0, 1") ;
			break ;
		}
	}



	/*!
	 * \fn double cArch::ComputeVar(uint theDate, const cRegArchValue& theData) const
	 * \param int theDate: date of computation
	 * \param const cRegArchValue& theData: past datas
	 * \details theData is not updated here.
	*/
	double cArch::ComputeVar(uint theDate, const cRegArchValue& theDatas) const
	{
	uint myp = mvArch.GetSize() ;
	double myRes = 0.0 ;
		for (register uint i = 1 ; i <= MIN(myp, theDate) ; i++)
			myRes += mvArch[i-1] * theDatas.mUt[theDate-i] * theDatas.mUt[theDate-i] ;
		return myRes ;
	}

	uint cArch::GetNParam(void) const
	{
		return mvArch.GetSize() ;
	}

	uint cArch::GetNLags(void) const
	{
		return mvArch.GetSize() ;
	}


	void cArch::SetDefaultInitPoint(double theMean, double theVar)
	{
	}

	cAbstCondVar & cArch::operator=(cAbstCondVar & theSrc)
	{
		// TODO: insérer une instruction return ici
		return theSrc;
	}

	void cArch::ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResiduals)
	{
		uint myp = mvArch.GetSize() ;
		const uint myBegIndex = theGradData.GetNMeanParam() + theBegIndex ;
		register uint i ;
		for (i = 1 ; i <= MIN(myp, theDate) ; i++)
			theGradData.mCurrentGradVar[myBegIndex+i-1] = theData.mUt[theDate - i]*theData.mUt[theDate - i] ;
		for (i = 1 ; i <= MIN(myp, theDate) ; i++)
			theGradData.mCurrentGradVar -= 2.0 * mvArch[i-1] * theData.mUt[theDate - i] * theGradData.mGradMt[i-1] ;
	}

	void cArch::ComputeHess(uint theDate, const cRegArchValue & theData, cRegArchGradient & theGradData, cRegArchHessien & theHessData, cAbstResiduals * theResiduals)
	{
	}


	void cArch::RegArchParamToVector(cDVector& theDestVect, uint theIndex)
	{
		uint mySize = GetNParam() ;
		if (theDestVect.GetSize() < mySize + theIndex)
			throw cError("Wrong size") ;
		mvArch.SetSubVectorWithThis(theDestVect, theIndex) ;
	}

	void cArch::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
		uint mySize = theSrcVect.GetSize() ;
		if (GetNParam() + theIndex > mySize)
			throw cError("Wrong size") ;
		mvArch.SetThisWithSubVector(theSrcVect, theIndex) ;
	}

	void cArch::copy(const cArch& theArch)
	{
		mvArch = theArch.mvArch;
	}
	

}//namespace
