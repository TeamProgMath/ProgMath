#include "StdAfxRegArchLib.h"
/*!
 \file cRegArchModel.cpp
 \brief sources for class cRegArchModel methods.

 \author Jean-Baptiste Durand, Ollivier TMAAMASCO
 \date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {

	/*!
	 * \fn cRegArchModel::cRegArchModel()
	 * \param None
	 * \details A simple constructor
	 */
	cRegArchModel::cRegArchModel()
	{
		mMean = NULL ;
		mVar = NULL ;
		mResids = NULL ;
	}

	/*!
	 * \fn cRegArchModel::cRegArchModel(cCondMean* theMean, cAbstCondVar* theVar, cAbstResiduals* theResiduals)
	 * \param cCondMean* theMean: pointer to conditional mean model
	 * \param cAbstCondVar* theVar: pointer to conditional variance model
	 * \param cAbstResiduals* theResiduals: pointer to conditional residuals distribution model
	 * \details A simple constructor
	 */
	cRegArchModel::cRegArchModel(cCondMean& theMean, cAbstCondVar& theVar, cAbstResiduals& theResiduals)
	{	mMean = new cCondMean(theMean) ;			
		mVar = theVar.PtrCopy() ;
		mResids = theResiduals.PtrCopy() ;
		// CreateRealCondResiduals(theResiduals) ;
		
 		MESS_CREAT("cRegArchModel") ;
	}

	/*!
	 * \fn cRegArchModel::cRegArchModel(cRegArchModel& theModel)
	 * \param cRegArchModel& theModel: the source
	 * \details recopy constructor
	 */
		cRegArchModel::cRegArchModel(cRegArchModel& theModel)
		{	if (theModel.mMean != NULL)
				mMean = new cCondMean(*theModel.mMean) ;
			else
				mMean = new cCondMean() ;
			mVar = theModel.mVar->PtrCopy() ;
			mResids = theModel.mResids->PtrCopy() ;
			// CreateRealCondResiduals(*theModel.mResids) ;

 			MESS_CREAT("cRegArchModel") ;
		}

	/*!
	 * \fn cRegArchModel::~cRegArchModel()
	 */
	cRegArchModel::~cRegArchModel()
	{	if (mMean != NULL)
		{	mMean->Delete() ;
			delete mMean ;
		}
		if (mVar != NULL)
		{	mVar->Delete() ;
			delete mVar ;
		}
		if (mResids != NULL)
		{	mResids->Delete() ;
			delete mResids ;
		}
		MESS_DESTR("cRegArchModel") ;
	}

	/*!
	 * \fn cRegArchModel& cRegArchModel::operator=(cRegArchModel& theRegArchModel)
	 * \param cRegArchModel& theRegArchModel: the source
	 * \details = operator for cRegArchModel
	 */
	cRegArchModel& cRegArchModel::operator=(cRegArchModel& theRegArchModel)
	{
		*mMean =  *(theRegArchModel.mMean) ;
		mVar = theRegArchModel.mVar->PtrCopy() ;
		mResids = theRegArchModel.mResids->PtrCopy() ;
		return *this ;
	}

	void cRegArchModel::ReAllocMean(uint theNewSize)
	{
		if (mMean != NULL)
		{
			mMean->Delete() ;
			delete mMean ;
		}
		mMean = new cCondMean(theNewSize) ;
	}

	/*!
	 * \fn void cRegArchModel::SetMean(cCondMean& theCondMean)
	 * \param cCondMean& theCondMean: the conditional mean model
	 */
	void cRegArchModel::SetMean(cCondMean& theCondMean)
	{
		if (mMean != NULL)
		{
			mMean->Delete() ;
			delete mMean ;
		}

		mMean = new cCondMean(theCondMean) ;
	}

	/*!
	 * \fn void cRegArchModel::GetNMean(void)
	 * \param void
	 * \details return the number of mean components
	 */
	int cRegArchModel::GetNMean(void)
	{
		if (mMean == NULL)
			return 0 ;
		else
			return mMean->GetNMean() ;
	}

	/*!
	 * \fn void cRegArchModel::AddOneMean(cAbstCondMean& theOneMean)
	 * \param cAbstCondMean& theOneMean: the conditional mean component model
	 * \details Add a new mean component
	 */
	void cRegArchModel::AddOneMean(cAbstCondMean& theOneMean)
	{
		if (mMean == NULL)
			mMean = new cCondMean(0);
		mMean->AddOneMean(theOneMean) ;
	}

	/*!
	 * \fn void cRegArchModel::GetOneMean(int theNumMean)
	 * \param int theNumMean: the index of the mean component
	 * \details Return theNumMean th mean component
	 */
	cAbstCondMean* cRegArchModel::GetOneMean(int theNumMean)
	{	if (mMean != NULL)
			return mMean->GetOneMean(theNumMean) ;
		else
			return NULL ;
	}

	bool cRegArchModel::IsGoodMeanType(eCondMeanEnum theMeanEnum, int theIndex)
	{
		if (mMean != NULL)
		{	cAbstCondMean* myCondMean = mMean->GetOneMean(theIndex) ;
			return (myCondMean->GetCondMeanType() == theMeanEnum) ;
		}
		else
			return false ;
	}


	/*!
	 * \fn void cRegArchModel::PrintMean(ostream& theOut) const
	 * \param ostream& theOut: output stream (screen or file). Default: cout
	 */
	void cRegArchModel::PrintMean(ostream& theOut) const
    {	if (mMean != NULL)
    		mMean->Print(theOut) ;
    }

	/*!
	 * \fn void cRegArchModel::SetVar(cAbstCondVar& theCondVar)
	 * \param cCondVar& theCondVar: the conditional variance model
	 */
	void cRegArchModel::SetVar(cAbstCondVar& theCondVar)
	{
		if (mVar != NULL)
		{
			mVar->Delete() ;
			delete mVar ;
		}
		mVar = theCondVar.PtrCopy() ;
	}

	cAbstCondVar* cRegArchModel::GetVar(void)
	{
		return mVar  ;
	}

	/*!
	 * \fn void cRegArchModel::PrintVar(ostream& theOut) const
	 * \param ostream& theOut: output stream (screen or file). Default: cout
	 */
	void cRegArchModel::PrintVar(ostream& theOut) const
{	if (mVar != NULL)
		mVar->Print(theOut);
}

	/*!
	 * \fn void cRegArchModel::SetResid(cAbstResiduals& theCondResiduals)
	 * \param cAbstResiduals& theCondResiduals: the conditional residuals model
	 */
	void cRegArchModel::SetResid(cAbstResiduals& theCondResiduals)
	{
		if (mResids != NULL)
		{
			mResids->Delete() ;
			delete mResids ;
		}
/*	eDistrTypeEnum myDistrType = theCondResiduals.GetDistrType() ;
	bool mySimulFlag = theCondResiduals.GetSimulFlag() ;
		mResids = CreateRealCondResiduals(myDistrType, &(theCondResiduals.mDistrParameter), mySimulFlag) ;
*/
		mResids = theCondResiduals.PtrCopy() ;
		// CreateRealCondResiduals(theCondResiduals) ;
	}

	cAbstResiduals*  cRegArchModel::GetResid(void)
	{
			return mResids ;
	}

	/*!
	 * \fn void cRegArchModel::PrintResiduals(ostream& theOut) const
	 * \param ostream& theOut: output stream (screen or file). Default: cout
	 */
	void cRegArchModel::PrintResiduals(ostream& theOut) const
    {	if (mResids != NULL)
    		mResids->Print(theOut) ;
    }

	/*!
	 * \fn void cRegArchModel::Print(ostream& theOut) const
	 * \param ostream& theOut: output stream (screen or file). Default: cout
	 */
	void cRegArchModel::Print(ostream& theOut) const
    {
    	theOut << "Regression with ARCH type residuals parameters:" << endl ;
    	theOut << "-----------------------------------------------" << endl ;
    	PrintResiduals(theOut) ;
    	theOut << endl ;
    	PrintMean(theOut) ;
    	theOut << endl ;
    	theOut << "Conditional variance parameters:" << endl ;
    	theOut << "--------------------------------" << endl ;
    	PrintVar(theOut) ;
    	theOut << endl ;
    
    }

	void cRegArchModel::SetDefaultInitPoint(cRegArchValue& theValue)
	{
	double myMean = 0.0, myVar = 0.0 ;

		for (int i = 0 ; i < (int)theValue.mYt.GetSize() ; i++)
		{	myMean = ((double)i*myMean + theValue.mYt[i])/(double)(i+1) ;
			myVar = ((double)i*myVar + theValue.mYt[i]*theValue.mYt[i])/(double)(i+1) ;
		}
		myVar -= myMean*myMean ;
		if (mMean != NULL)
			mMean->SetDefaultInitPoint(myMean, myVar) ;
		mVar->SetDefaultInitPoint(myMean, myVar) ;
		mResids->SetDefaultInitPoint() ;
	}

	uint cRegArchModel::GetNParam(void) const
    {
    	if (mMean != NULL)
    		return mMean->GetNParam() + mVar->GetNParam() + mResids->GetNParam() ;
    	else
    		return mVar->GetNParam() + mResids->GetNParam() ;
    }

	uint cRegArchModel::GetNLags(void) const
    {
    	if (mMean == NULL)
    		return mVar->GetNLags() ;
    	else
    		return MAX(mMean->GetNLags(), mVar->GetNLags()) ;
    }

	void cRegArchModel::ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData)
    {
    }

	void cRegArchModel::RegArchParamToVector(cDVector& theDestVect) const
    {
    uint myIndex = 0 ;
    	if (mMean != NULL)
    	{	mMean->RegArchParamToVector(theDestVect, myIndex) ;
    		myIndex += mMean->GetNParam() ;
    	}
    	mVar->RegArchParamToVector(theDestVect, myIndex) ;
     	myIndex += mVar->GetNParam() ;
    	mResids->RegArchParamToVector(theDestVect, myIndex) ;
    }

	void cRegArchModel::VectorToRegArchParam(const cDVector& theSrcParam)
    {
    uint myIndex = 0 ;
    	if (mMean != NULL)
    	{	mMean->VectorToRegArchParam(theSrcParam, myIndex) ;
    		myIndex += mMean->GetNParam() ;
    	}
    	mVar->VectorToRegArchParam(theSrcParam, myIndex) ;
    	myIndex += mVar->GetNParam() ;
    	mResids->VectorToRegArchParam(theSrcParam, myIndex) ;
    
    	if ((mVar->GetCondVarType() == eEgarch) && (mResids->GetNParam() > 0))
    	{	
    	cEgarch* myVar = dynamic_cast<cEgarch *>(mVar) ;
    		if (myVar != NULL)
    		{	
    		double myAux = mResids->ComputeEspAbsEps() ;
    			myVar->SetEspAbsEps(myAux) ;
    		}
    	}
    
    }

}//namespace

