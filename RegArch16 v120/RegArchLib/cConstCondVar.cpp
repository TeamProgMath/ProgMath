#include "StdAfxRegArchLib.h"

/*!
	\file cConstCondVar.cpp
	\brief sources for class cConstCondVar methods.

	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - Last change feb-18-2011
*/

namespace RegArchLib {
	/*!
	 * \fn cConstCondVar::cConstCondVar(double theData): cAbstCondVar(eCste)
	 * \param double theData: constant variance value. Default 1.
	 */
	cConstCondVar::cConstCondVar(double theValue)
	: cAbstCondVar(eCste)  // call constructor of cAbstCondVar with type eCste
	{	
		mvCste = theValue ;
		MESS_CREAT("cConstCondVar")
	}

	/*!
	 * \fn cConstCondVar::~cConstCondVar()
	 * \param none
	 * \details Nothing to do here.
	 */
	cConstCondVar::~cConstCondVar()
	{	
		MESS_DESTR("cConstCondVar")
	}

	/*!
	 * \fn cAbstCondVar* cConstCondVar::PtrCopy()
	 */

	cAbstCondVar* cConstCondVar::PtrCopy() const
	{
		 cConstCondVar *myConstCondVar = new cConstCondVar();

		 myConstCondVar->copy(*this);

		 return myConstCondVar;
	}

	/*!
	 * \fn void cConstCondVar::Delete(void)
	 * \param void
	 * \details Nothing to do here.
	 */
	void cConstCondVar::Delete(void)
	{
	}

	/*!
	 * \fn void void cConstCondVar::Print(ostream& theOut) const
	 * \param ostream& theOut: output stream (file or screen) default cout.
	 */
	void cConstCondVar::Print(ostream& theOut) const
	{
		theOut << "Constant variance model with:" << endl ;
		theOut << "\tConstant=" << mvCste << endl ;
	}

	/*!
	 * \fn void cConst::ReAlloc(uint theSize, uint theNumParam=0)
	 * \param uint theSize: not used. Not used for cConstClass
	 * \param uint theNumParam: not used for cConst class
	 * \details Nothing to do for cConstCondVar Class.
	 */
	void cConstCondVar::ReAlloc(uint theSize, uint theNumParam)
	{
	}

	/*!
	 * \fn void cConstCondVar::ReAlloc(const cDVector& theVectParam, uint theNumParam)
	 * \param const cDVector& theVectParam: the constant value is in theVectParam[0]
	 * \param uint theNumParam: not used for cConst class
	 * \details Here, mvCste = theVectParam[0]
	 */
	 void cConstCondVar::ReAlloc(const cDVector& theVectParam, uint theNumParam)
	{	if (theVectParam.GetSize() > 0)
			mvCste = theVectParam[0] ;
		else
			throw cError("Size of theVectParam must be > 0.") ;
	}

	 /*!
	  * \fn void cConstCondVar::Set(double theValue, uint theIndex, uint theNumParam)
	  * \brief fill the parameters vector
	  * \param double theValue: the constant value.
	  * \param uint theIndex: not used here. Default 0.
	  * \param uint theNumParam: not used for cConstCondVar model. Default 0.
	  * \details mvCste = theValue
	  */
	void cConstCondVar::Set(double theValue, uint theIndex, uint theNumParam)
	{
		mvCste = theValue ;

	}

	/*!
	 * \fn void cConstCondVar::Set(const cDVector& theVectParam, uint theNumParam)
	 * \brief fill the parameters vector
	 * \param const cDVector& theVectParam: the constant value is in theVectParam[0].
	 * \param uint theIndex: not used here. Default 0.
	 * \param uint theNumParam: not used for cConstCondVar model. Default 0.
	 * \details mvCste = theVectParam[0]
	 */
	void cConstCondVar::Set(const cDVector& theVectParam, uint theNumParam)
	{	if (theVectParam.GetSize() > 0)
			mvCste = theVectParam[0] ;
		else
			throw cError("the size of theVectParam must be > 0") ;
	}

	double  cConstCondVar::Get(uint theIndex, uint theNumParam)
	{
		return mvCste ;
	}

	/*!
	 * \fn double cConstCondVar::ComputeVar(uint theDate, const cRegArchValue& theData) const
	 * \param int theDate: date of the computation
	 * \param cRegArchValue& theData: past datas.
	 * \details theData must not be updated here.
	 */
	double cConstCondVar::ComputeVar(uint theDate, const cRegArchValue& theData) const
	{
		return mvCste ;
	}

	uint cConstCondVar::GetNParam(void) const
	{
		return 1 ;
	}
	uint cConstCondVar::GetNLags(void) const
	{
		return 0 ;
	}

	void cConstCondVar::ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResiduals)
	{
		theGradData.mCurrentGradVar[theGradData.GetNMeanParam()+theBegIndex] += 1.0 ;
	}

	void cConstCondVar::RegArchParamToVector(cDVector& theDestVect, uint theIndex)
	{
	uint mySize = theDestVect.GetSize() ;
		if (theIndex >= mySize)
			throw cError("Wrong size") ;
		theDestVect[theIndex] = mvCste;
	}

	void cConstCondVar::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
		if (1 + theIndex > theSrcVect.GetSize())
			throw cError("Wrong size") ;
		mvCste = theSrcVect[theIndex] ;
	}

	void cConstCondVar::copy(const cConstCondVar& theConstCondVar)
	{
		mvCste = theConstCondVar.mvCste;
	}

	void cConstCondVar::ComputeHess(uint theDate, const cRegArchValue & theData, cRegArchGradient & theGradData, cRegArchHessien & theHessData, cAbstResiduals * theResiduals)
	{
	}

}//namespace
