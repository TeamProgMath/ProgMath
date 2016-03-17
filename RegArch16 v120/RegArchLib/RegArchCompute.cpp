#include "StdAfxRegArchLib.h"
/*!
	\file RegArchCompute.cpp
	\brief implementation of the Simulation / Estimation procedures for general RegArchModel
	
	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - last change feb-18-2011
*/


namespace RegArchLib {

	/*!
	 * \fn void RegArchSimul(const uint theNSample, const cRegArchModel& theModel, cRegArchValue& theData)
	 * \param const uint theNSample: size of the sample
	 * \param const cRegArchModel& theModel: the RegArch model
	 * \param cRegArchValue& theData: output parameter. The Y(t) values are stored in theData.mYt
	 */
	void RegArchSimul(const uint theNSample, const cRegArchModel& theModel, cRegArchValue& theData)
	{
		theData.ReAlloc(theNSample) ;
		theModel.mResids->Generate(theNSample, theData.mEpst) ;
		theModel.mVar->UpdateProxyVarParameters();
		if (theModel.mMean != NULL)
			theModel.mMean->UpdateProxyMeanParameters();
		for (register uint t = 0 ; t < theNSample ; t++)
		{	theData.mHt[t] = theModel.mVar->ComputeVar(t, theData) ;
			if (theModel.mMean != NULL)
			{	theData.mMt[t] = theModel.mMean->ComputeMean(t, theData) ;
			}
			theData.mUt[t] = sqrt(theData.mHt[t])*theData.mEpst[t] ;
			theData.mYt[t] = theData.mMt[t] + theData.mUt[t] ;
		}
	}

   /*!
	 * \fn void RegArchSimul(const uint theNSample, const cRegArchModel& theModel, cDVector& theYt)
	 * \param const uint theNSample: size of the sample
	 * \param const cRegArchModel& theModel: the RegArch model
	 * \param cDVector& theYt: output parameter. 
	 */
	void RegArchSimul(const uint theNSample, const cRegArchModel& theModel, cDVector& theYt)
	{
	cRegArchValue myValue(theNSample) ;
		RegArchSimul(theNSample, theModel, myValue) ;
		theYt = myValue.mYt ;

	}

   /*!
	* \fn static double ComputeLt(uint theDate, const cRegArchModel& theParam, cRegArchValue& theValue)
	* \param uint theDate: the date
	* \param const cRegArchModel& theParam: the model
	* \param cRegArchValue& theValue: output parameter.
	* \brief return the lt(theta ; Yt)
	*/
	static double ComputeLt(uint theDate, const cRegArchModel& theParam, cRegArchValue& theValue)
	{
		double myRes = 0;
		theValue.mHt[theDate] = theParam.mVar->ComputeVar(theDate, theValue);
		if (theParam.mMean != NULL)
			theValue.mMt[theDate] = theParam.mMean->ComputeMean(theDate, theValue);
		theValue.mUt[theDate] = theValue.mYt[theDate] - theValue.mMt[theDate];
		theValue.mEpst[theDate] = theValue.mUt[theDate] / sqrt(theValue.mHt[theDate]);
		myRes += -0.5*log(theValue.mHt[theDate]) + theParam.mResids->LogDensity(theValue.mEpst[theDate]);
		return myRes;
	}

	/*!
	 * \fn double RegArchLLH(const cRegArchModel& theParam, cDVector* theYt, cDMatrix* theXt)
	 * \param const cRegArchModel& theParam: the model
	 * \param cDVector* theYt: the observations
	 * \param cDMatrix* theXt: the regressors matrix. Default NULL
	 * \brief return the log-likelihood value
	 */
	double RegArchLLH(const cRegArchModel& theParam, cDVector* theYt, cDMatrix* theXt)
	{
	cRegArchValue myValue(theYt, theXt) ;
		return RegArchLLH(theParam, myValue) ;
	}

	/*!
	 * \fn double RegArchLLH(const cRegArchModel& theParam,cRegArchValue& theData)
	 * \param const cRegArchModel& theParam: the model
	 * \param cRegArchValue& theData: theData.mYt contains the observations.
	 * \brief return the log-likelihood value
	 */
	double RegArchLLH(const cRegArchModel& theParam, cRegArchValue& theData)
	{
	int mySize = (int)theData.mYt.GetSize() ;
	double myRes = 0 ;
		theData.mEpst = theData.mHt = theData.mMt = theData.mUt = 0.0 ;
		theParam.mVar->UpdateProxyVarParameters();
		if (theParam.mMean != NULL)
			theParam.mMean->UpdateProxyMeanParameters();
		for(register int t=0 ; t < mySize ; t++)
		{	theData.mHt[t] = theParam.mVar->ComputeVar(t, theData) ;
			if (theParam.mMean != NULL)
				theData.mMt[t] = theParam.mMean->ComputeMean(t, theData) ;
			theData.mUt[t] = theData.mYt[t] - theData.mMt[t] ;
			theData.mEpst[t] = theData.mUt[t]/sqrt(theData.mHt[t]) ;
			myRes += -0.5*log(theData.mHt[t]) + theParam.mResids->LogDensity(theData.mEpst[t]) ;
		}
		return myRes ;
	}

	void RegArchGradLt(int theDate, cRegArchModel& theParam, cRegArchValue& theValue, cRegArchGradient& theGradData, cDVector& theGradlt)
	{	theGradlt = 0.0 ;
		theValue.mHt[theDate] = theParam.mVar->ComputeVar(theDate, theValue) ;
		if (theParam.mMean != NULL)
			theValue.mMt[theDate] = theParam.mMean->ComputeMean(theDate, theValue) ;
		theValue.mUt[theDate] = theValue.mYt[theDate] - theValue.mMt[theDate] ;
	double mySigmat = sqrt(theValue.mHt[theDate]) ;
		theValue.mEpst[theDate] = theValue.mUt[theDate]/mySigmat ;
		theParam.mVar->ComputeGrad(theDate, theValue, theGradData, theParam.mResids) ;
		theGradData.mCurrentGradSigma = theGradData.mCurrentGradVar / (2.0 * mySigmat);
		if (theParam.mMean != NULL)
			theParam.mMean->ComputeGrad(theDate, theValue, theGradData, theParam.mResids) ;
		theParam.mResids->ComputeGrad(theDate, theValue, theGradData) ;
		theGradData.mCurrentGradEps = -1.0*(theValue.mEpst[theDate] * theGradData.mCurrentGradSigma + theGradData.mCurrentGradMu)/ mySigmat ;
		theGradlt =  (-1.0/mySigmat) * theGradData.mCurrentGradSigma  + theGradData.mCurrentDiffLogDensity * theGradData.mCurrentGradEps  + theGradData.mCurrentGradDens;
	}
	
	void NumericRegArchGradLt(uint theDate, cRegArchModel& theParam, cRegArchValue* theValue, cDVector& theGradlt, double theh)
	{
	uint myNParam = theParam.GetNParam();
	cDVector myParam(myNParam);
	cDVector myParam1(myNParam);
		theParam.RegArchParamToVector(myParam);
	cRegArchModel myModel = cRegArchModel(theParam) ;
		theGradlt.ReAlloc(myNParam);
	double mylt0 = ComputeLt(theDate, theParam, theValue[myNParam]);
		myModel.VectorToRegArchParam(myParam);
		myParam1 = myParam;
		for (register uint i = 0; i < myNParam; i++)
		{
		double myh1 = abs(myParam[i] * theh);
			myParam1[i] += myh1;
			myModel.VectorToRegArchParam(myParam1);
		double mylt1 = ComputeLt(theDate, myModel, theValue[i]);
			theGradlt[i] = (mylt1 - mylt0) / myh1;
			myParam1[i] -= myh1;
		}
	}

	void RegArchLtAndGradLt(int theDate, cRegArchModel& theParam, cRegArchValue& theValue, cRegArchGradient& theGradData, double& theLt, cDVector& theGradlt)
	{
		theGradlt = 0.0 ;
		theValue.mHt[theDate] = theParam.mVar->ComputeVar(theDate, theValue) ;
		if (theParam.mMean != NULL)
			theValue.mMt[theDate] = theParam.mMean->ComputeMean(theDate, theValue) ;
		theValue.mUt[theDate] = theValue.mYt[theDate] - theValue.mMt[theDate] ;
	double mySigmat = sqrt(theValue.mHt[theDate]) ;
		theValue.mEpst[theDate] = theValue.mUt[theDate]/mySigmat ;
		theParam.mVar->ComputeGrad(theDate, theValue, theGradData, theParam.mResids) ;
		theGradData.mCurrentGradSigma = theGradData.mCurrentGradVar / (2.0 * mySigmat);
		if (theParam.mMean != NULL)
			theParam.mMean->ComputeGrad(theDate, theValue, theGradData, theParam.mResids) ;
		theParam.mResids->ComputeGrad(theDate, theValue, theGradData) ;
		theGradData.mCurrentGradEps = -1.0*(theValue.mEpst[theDate] * theGradData.mCurrentGradSigma + theGradData.mCurrentGradMu)/ mySigmat ;
		theGradlt = (-1.0 / mySigmat) * theGradData.mCurrentGradSigma + theGradData.mCurrentDiffLogDensity * theGradData.mCurrentGradEps + theGradData.mCurrentGradDens ;
		theLt = -0.5*log(theValue.mHt[theDate]) + theParam.mResids->LogDensity(theValue.mEpst[theDate]) ; 
	}

	void RegArchGradLLH(cRegArchModel& theParam, cRegArchValue& theData, cDVector& theGradLLH)
	{
	cRegArchGradient myGradData=cRegArchGradient(&theParam) ;
	cDVector myGradlt(myGradData.GetNParam()) ;
		theGradLLH = 0.0L ;
		theParam.mVar->UpdateProxyVarParameters();
		if (theParam.mMean != NULL)
			theParam.mMean->UpdateProxyMeanParameters();
		for (register int t = 0 ; t < (int)theData.mYt.GetSize() ; t++)
		{	RegArchGradLt(t, theParam, theData, myGradData, myGradlt) ;
			theGradLLH += myGradlt ;
			myGradData.Update();
		}
	}

	void RegArchLLHAndGradLLH(cRegArchModel& theParam, cRegArchValue& theValue, double& theLLH, cDVector& theGradLLH)
	{
	cRegArchGradient myGradData(&theParam) ;
	cDVector myGradlt(myGradData.GetNParam()) ;
	double myLt ;
		theGradLLH = 0.0L ;
		theLLH = 0.0 ;
		theParam.mVar->UpdateProxyVarParameters();
		if (theParam.mMean != NULL)
			theParam.mMean->UpdateProxyMeanParameters();
		for (register int t = 0 ; t < (int)theValue.mYt.GetSize() ; t++)
		{	RegArchLtAndGradLt(t, theParam, theValue, myGradData, myLt, myGradlt) ;
			theGradLLH += myGradlt ;
			theLLH += myLt ;
			myGradData.Update();
		}
	}

	void RegArchHessLt(int theDate, cRegArchModel& theParam, cRegArchValue& theValue, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cDMatrix& theHesslt)
	{	
		cDVector theGradlt;
		RegArchGradAndHessLt(theDate, theParam, theValue, theGradData, theHessData, theGradlt, theHesslt);
	}

	void NumericRegArchHessLt(int theDate, cRegArchModel& theParam, cRegArchValue* theValue, cRegArchGradient* theGradData, cDMatrix& theHesslt, double theh)
	{

	}

	void RegArchGradAndHessLt(int theDate, cRegArchModel& theParam, cRegArchValue& theValue, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cDVector& theGradlt, cDMatrix& theHesslt)
	{	
		// Grad
		theHesslt = 0.0;
		theValue.mHt[theDate] = theParam.mVar->ComputeVar(theDate, theValue); // mHt
		if (theParam.mMean != NULL)
			theValue.mMt[theDate] = theParam.mMean->ComputeMean(theDate, theValue); // mMt
		theValue.mUt[theDate] = theValue.mYt[theDate] - theValue.mMt[theDate]; // mUt
		double mySigmat = sqrt(theValue.mHt[theDate]); // mySigma
		theValue.mEpst[theDate] = theValue.mUt[theDate] / mySigmat; // mEpst
		theParam.mVar->ComputeGrad(theDate, theValue, theGradData, theParam.mResids); // mCurrentGradVar
		theGradData.mCurrentGradSigma = theGradData.mCurrentGradVar / (2.0 * mySigmat); // mCurrentGradSigma
		if (theParam.mMean != NULL)
			theParam.mMean->ComputeGrad(theDate, theValue, theGradData, theParam.mResids); // mCurrentGradMu
		theParam.mResids->ComputeGrad(theDate, theValue, theGradData); // mCurrentDiffLogDensity
		theGradData.mCurrentGradEps = -1.0*(theValue.mEpst[theDate] * theGradData.mCurrentGradSigma + theGradData.mCurrentGradMu) / mySigmat; // mCurrentGradEps
		theGradlt = (-1.0 / mySigmat) * theGradData.mCurrentGradSigma + theGradData.mCurrentDiffLogDensity * theGradData.mCurrentGradEps + theGradData.mCurrentGradDens;

		// Hess
		theParam.mVar->ComputeHess(theDate, theValue, theGradData, theHessData, theParam.mResids); // mCurrentHessVar VERIFY
		theHessData.mCurrentHessSigma = (1.0 / (2.0 * mySigmat)) *
			(theHessData.mCurrentHessVar - 2 * theGradData.mCurrentGradSigma*Transpose(theGradData.mCurrentGradSigma)); // mCurrentHessSigma
		if (theParam.mMean != NULL)
			theParam.mMean->ComputeHess(theDate, theValue, theGradData, theHessData, theParam.mResids); // mCurrentHessMu VERIFY
		theParam.mResids->ComputeHess(theDate, theValue, theGradData, theHessData, theParam.mResids); // mCurrentGradDiffLogDensity, mCurrentHessDens VERIFY TODO
		theHessData.mCurrentHessEps = (1.0 / (mySigmat*mySigmat)) * theGradData.mCurrentGradSigma * Transpose(theGradData.mCurrentGradMu)
			- (1.0 / mySigmat)*theHessData.mCurrentHessMu
			+ (2.0 * theValue.mUt[theDate] / (mySigmat * mySigmat * mySigmat)) * theGradData.mCurrentGradSigma * Transpose(theGradData.mCurrentGradSigma)
			+ (1.0 / (mySigmat * mySigmat)) * theGradData.mCurrentGradMu * Transpose(theGradData.mCurrentGradSigma)
			- (theValue.mUt[theDate] / (mySigmat*mySigmat)) * theHessData.mCurrentHessSigma; // mCurrentHessEps
		// formule finale
		theHesslt = (-1.0 / (mySigmat*mySigmat)) * theGradData.mCurrentGradSigma * Transpose(theGradData.mCurrentGradSigma)
			- (1.0 / mySigmat) * theHessData.mCurrentHessSigma
			+ theParam.mResids->Diff2LogDensity(theValue.mEpst[theDate]) * theGradData.mCurrentGradEps * Transpose(theGradData.mCurrentGradEps)
			+ theGradData.mCurrentDiffLogDensity * theHessData.mCurrentHessEps
			+ theHessData.mCurrentGradDiffLogDensity * Transpose(theGradData.mCurrentGradEps)
			+ theGradData.mCurrentGradEps * Transpose(theHessData.mCurrentGradDiffLogDensity)
			+ theHessData.mCurrentHessDens;
	}

	void RegArchLtGradAndHessLt(int theDate, cRegArchModel& theParam, cRegArchValue& theValue, double& thelt, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cDVector& theGradlt, cDMatrix& theHesslt)
	{

	}

	void RegArchHessLLH(cRegArchModel& theParam, cRegArchValue& theValue, cDMatrix& theHessLLH)
	{

	}

	void NumericComputeJ(cRegArchModel& theModel, cRegArchValue& theValue, cDMatrix& theHessLLH, double theh=1e-5)
	{

	}

	void NumericComputeCov(cRegArchModel &theModel, cRegArchValue &theData, cDMatrix &theCov)
	{

	}

	void RegArchComputeI(cRegArchModel &theModel,cRegArchValue &theData, cDMatrix &theI)
	{

	}

	void RegArchComputeIAndJ(cRegArchModel &theModel,cRegArchValue &theData, cDMatrix &theI, cDMatrix &theJ)
	{

	}

	void RegArchComputeCov(cRegArchModel& theModel, cRegArchValue& theValue, cDMatrix& theCov)
	{

	}

	void NumericRegArchGradLLH(cRegArchModel& theModel, cRegArchValue& theValue, cDVector& theGradLLH, double theh)
	{
}

	void NumericRegArchHessLLHold(cRegArchModel& theModel, cRegArchValue& theValue, cDMatrix& theHessLLH, double theh)
	{

	}		

#ifndef _RDLL_
	void RegArchEstim(cRegArchModel& theModel, cRegArchValue& theValue, sGSLMultiMinResult& theResStruct, cRegArchModel& theResModel, cDVector* theInitPoint, eGSLMultiMinAlgoEnum theAlgo, double theStopValue, int theMaxIter, bool theVerbose) 
	{
	uint myNParam = theModel.GetNParam() ;

		if (theModel.mMean != NULL)
			theResModel.SetMean(*(theModel.mMean)) ;
		else
			theResModel.mMean = NULL ;

		theResModel.SetVar(*(theModel.mVar)) ;
		theResModel.SetResid(*(theModel.mResids)) ;

		if (theInitPoint == NULL)
		{
			theResModel.SetDefaultInitPoint(theValue);					
		}
		else
		{	if (theInitPoint->GetGSLVector() == NULL)
				theResModel.SetDefaultInitPoint(theValue) ;
			else
				theResModel.VectorToRegArchParam(*theInitPoint) ;
		}
	cDVector myX0(myNParam) ;
		theResModel.RegArchParamToVector(myX0) ;
	cGSLMultiMin myMultiMin(myX0, theAlgo) ;

	sParamOptimStruct myOtherParam ;
		myOtherParam.mParam = &theResModel ;
		myOtherParam.mValue = &theValue ;

	gsl_multimin_function_fdf myFunct ;
		myFunct.df = GslGradLLHFunction ;
		myFunct.f = GslLLHFunction ;
		myFunct.fdf = GslLLHAndGradLLHFunction ;
		myFunct.n = myNParam ;
		myFunct.params = &myOtherParam ;

		myMultiMin.SetFunction(&myFunct) ;

	cDVector myX1(myNParam) ;

	sGSLMultiMinResult myGSLRes ;
 		myMultiMin.GSLOptim(myX1, theResStruct, theStopValue, theMaxIter, theVerbose) ;

		theResModel.VectorToRegArchParam(myX1) ;
		
	}
#endif //_RDLL

#ifndef _RDLL_
	void RegArchEstim(cRegArchModel& theModel, cRegArchValue& theValue,  sGSLMultiMinResult& theResStruct, cRegArchModel& theResModel, cDVector* theInitPoint, sGSLMultiMinAlgoParam* theAlgoParam) 
	{
	uint myNParam = theModel.GetNParam() ;

		if (theModel.mMean != NULL)
			theResModel.SetMean(*(theModel.mMean)) ;
		else
			theResModel.mMean = NULL ;

		theResModel.SetVar(*(theModel.mVar)) ;
		theResModel.SetResid(*(theModel.mResids)) ;

		if (theInitPoint == NULL)
		{
			theResModel.SetDefaultInitPoint(theValue);					
		}
		else
		{	if (theInitPoint->GetGSLVector() == NULL)
				theResModel.SetDefaultInitPoint(theValue) ;
			else
				theResModel.VectorToRegArchParam(*theInitPoint) ;
		}
	cDVector myX0(myNParam) ;
		theResModel.RegArchParamToVector(myX0) ;
	
	sGSLMultiMinAlgoParam* myAlgoParam ;
	bool	myExist = (theAlgoParam != NULL) ;	
		if (!myExist)
		{
			myAlgoParam = (sGSLMultiMinAlgoParam *)malloc(sizeof(sGSLMultiMinAlgoParam)) ;
			myAlgoParam->mAlgoType = eBFGSTwo ;
			myAlgoParam->mNMaxIter = 200 ;
			myAlgoParam->mStepSize = 0.01 ;
			myAlgoParam->mTol = 0.01 ;
			myAlgoParam->mVerbose = false ;
		}
		else
		{
			myAlgoParam = theAlgoParam ;
		}
			
	cGSLMultiMin myMultiMin(myX0, myAlgoParam->mAlgoType) ;

	sParamOptimStruct myOtherParam ;
		myOtherParam.mParam = &theResModel ;
		myOtherParam.mValue = &theValue ;

	gsl_multimin_function_fdf myFunct ;
		myFunct.df = GslGradLLHFunction ;
		myFunct.f = GslLLHFunction ;
		myFunct.fdf = GslLLHAndGradLLHFunction ;
		myFunct.n = myNParam ;
		myFunct.params = &myOtherParam ;

		myMultiMin.SetFunction(&myFunct) ;

	cDVector myX1(myNParam) ;

		
 		myMultiMin.GSLOptim(myX1, theResStruct, *myAlgoParam) ;

		if (!myExist)
			free(myAlgoParam) ;

		theResModel.VectorToRegArchParam(myX1) ;
		
	}

	void RegArchEstim(cRegArchModel& theModel, cRegArchValue& theValue, cNLOPTResult& theResStruct, cRegArchModel& theResModel, cDVector* theInitPoint, cNLOPTAlgoParam* theAlgoParam)
	{
	uint myNParam = theModel.GetNParam();

		if (theModel.mMean != NULL)
			theResModel.SetMean(*(theModel.mMean));
		else
			theResModel.mMean = NULL;

		theResModel.SetVar(*(theModel.mVar));
		theResModel.SetResid(*(theModel.mResids));

		if (theInitPoint == NULL)
		{
			theResModel.SetDefaultInitPoint(theValue);
		}
		else
		{
			if (theInitPoint->GetGSLVector() == NULL)
				theResModel.SetDefaultInitPoint(theValue);
			else
				theResModel.VectorToRegArchParam(*theInitPoint);
		}
	cDVector myX0(myNParam);
		theResModel.RegArchParamToVector(myX0);

	cNLOPTAlgoParam* myAlgoParam = new cNLOPTAlgoParam();
	bool myExist = (theAlgoParam != NULL);
		if (!myExist)
		{
			myAlgoParam = new cNLOPTAlgoParam();
		}
		else
		{
			myAlgoParam->mAlgo = theAlgoParam->mAlgo;
			myAlgoParam->mfTol = theAlgoParam->mfTol;
			myAlgoParam->mMaxComputeTime = theAlgoParam->mMaxComputeTime;
			myAlgoParam->mMaxFuncEval = theAlgoParam->mMaxFuncEval;
			myAlgoParam->mMinimisation = theAlgoParam->mMinimisation;
			myAlgoParam->mStopVal = theAlgoParam->mStopVal;
			myAlgoParam->mxTol = theAlgoParam->mxTol;
			//myAlgoParam->mVerbose = theAlgoParam->mVerbose;
		}

	cNloptWrapperCpp myOptim = cNloptWrapperCpp();

	sParamOptimStruct myOtherParam;
		myOtherParam.mParam = &theResModel;
		myOtherParam.mValue = &theValue;
		//myOtherParam.mVerbose = myAlgoParam->mVerbose;
		myOtherParam.mNFuncEval = 0;

		myOptim.SetAlgorithm(myAlgoParam->mAlgo, myNParam);
		myOptim.SetObjectiveFunc((nlopt_func)NloptLLHAndGradLLHFunction, false, &myOtherParam);

	double* myX1 = GSLVectorToDoubleStar(myX0);


		myOptim.Optimize(myX1, myNParam, *myAlgoParam, theResStruct);

		if (!myExist)
			free(myAlgoParam);
	cDVector myXOptim(myNParam, theResStruct.mXOptim);
		theResModel.VectorToRegArchParam(myXOptim);
	}

#endif // _RDLL_
}