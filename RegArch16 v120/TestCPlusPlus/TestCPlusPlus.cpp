// Test.cpp : définit le point d'entrée pour l'application console.
//

#include "StdAfxTestCplusPlus.h"
 
using namespace std;
using namespace ErrorNameSpace;
using namespace VectorAndMatrixNameSpace;
using namespace WrapperGslCppNameSpace;
using namespace WrapperNloptCppNamespace;
using namespace RegArchLib ;


int _tmain(int argc, _TCHAR* argv[])
{
	cout.precision(12) ; 

cFigarch myFigarch(1, 1, 0.5, 100);
	myFigarch.Set(.1, 0, 0);
	myFigarch.Set(.1, 0, 1);
	myFigarch.Set(.8, 0, 2);

cAr	myAr(2) ;

	myAr.Set(.8, 0) ;
	myAr.Set(-.2, 1) ;
//	myAr.Print() ;


cMa myMa(1) ;
	myMa.Set(0.8, 0) ;
//	myMa.Set(0.6, 1) ;

cCondMean myCondMean ;
	//myCondMean.SetOneMean(0, myAr) ;

	//myCondMean.SetOneMean(1, myMa) ;

/*	cCondMean myCondMean1 = cCondMean(myCondMean) ;
	cout << "CondMean :" << myCondMean << endl ;
	cout << "CondMean1 : " <<  myCondMean1 << endl ;
	myCondMean1.Delete() ;
//	cout << "CondMean :" << myCondMean << endl ;
//	cout << "CondMean1 : " <<  myCondMean1 << endl ;
	myCondMean1 = myCondMean ;
//	cout << "CondMean1 : " <<  myCondMean1 << endl ;
*/
cGarch myGarch(1,1) ;
	myGarch.Set(0.1, 0, 0) ;
	myGarch.Set(0.1, 0, 1) ;
	myGarch.Set(0.8, 0, 2) ;


//cConstCondVar myConstVar(1.0) ;

cStudentResiduals myStudent(5, true) ;
cNormResiduals myNormResid ;

cConstCondVar myConstVar(1.0) ;
cConst myConstMean(10.0);
cArch myArch(1);
	myArch.Set(0.4, 0, 0);
	myArch.Set(0.5, 0, 1);
	
cStdDevInMean myStdDevInMean(.6);
cVarInMean myVarInMean(.6);
cRegArchModel myModel ;
//myModel.SetMean(myCondMean) ;
//	myModel.AddOneMean(myConstMean);
//	myModel.AddOneMean(myAr);

//	myModel.PrintMean();

//	myModel.SetMean(myCondMean);
	
//	myModel.AddOneMean(myMa);
//	myModel.SetResid(myStudent) ;
	myModel.SetResid(myNormResid);
//	myModel.SetVar(myGarch);
//	myModel.SetVar(myConstVar);	
	
	myModel.SetVar(myFigarch) ;
	cout << "Modele : " ;
	myModel.Print() ;
//	return 0 ;
	
/*
cRegArchModel* myModele1 = new cRegArchModel(myModele) ;
	cout << "Modele1 : " ;
	myModele1->Print() ;
cRegArchModel myModele2 = *myModele1 ;
	cout << "Modele2 : " ;
	myModele2.Print() ;
	delete myModele1 ;
	
cArch myArch(1) ;
	myArch.Set(0.1, 0, 0) ;
	myArch.Set(0.9, 0, 1) ;

cAparch myAparch(1, 1) ;
	myAparch.Set(0.1, 0, 0) ; // cste ;
	myAparch.Set(1.3, 0, 1) ; // Delta ;
	myAparch.Set(0.1, 0, 2) ; // Arch ;
	myAparch.Set(0.3, 0, 3) ; // Gamma ;
	myAparch.Set(0.8, 0, 4) ; // Garch ;


cTarch myTarch(1) ;
	myTarch.Set(0.1, 0, 0) ;
	myTarch.Set(0.8, 0, 1) ;
	myTarch.Set(0.7, 0, 2) ;
//	myTarch.Print(cout) ;


/*
cEgarch myEgarch(myModel.mResids, 1, 1) ;
	myEgarch.Set(-0.1217, 0, 1) ; // cste
	myEgarch.Set(0.06046, 0, 2) ; // ARCH
	myEgarch.Set(0.985106, 0, 3) ; // GARCH
	myEgarch.Set(3.198596, 0, 4) ; // TETA
	myEgarch.Set(1.0, 0, 5) ; //GAMMA
	myEgarch.Print() ;


cConst myConst(100.0) ;
*/
//cVarInMean myVarInMean(0.4) ;



uint myNSimul = 10000;
cRegArchValue myValue(myNSimul) ;
	RegArchSimul(myNSimul, myModel, myValue) ;



/*
ofstream myFile;
	myFile.open("TestC++.txt");
	myFile << myValue.mYt << endl;
	myFile.close();

	return 0;
*/
	
	cRegArchModel myResModel(myModel);
uint myNParam = myModel.GetNParam();
cNLOPTResult myResStruct(myNParam);
cNLOPTAlgoParam* myAlgoParam = new cNLOPTAlgoParam();
	myAlgoParam->mAlgo = NLOPT_LD_TNEWTON;
	myAlgoParam->mMaxComputeTime = 600;
	myAlgoParam->mMinimisation = false;
	myAlgoParam->mStopVal = 1e299;
	myAlgoParam->mfTol = 1e-6;
	myAlgoParam->mxTol = 1e-6;

	
cDVector myInitPoint = cDVector(myNParam, 0.0) ;
// Ox Garch init value
/*

Parameter      Starting Value
Cst(M)               0.010000
Cst(V)               0.040000
d-Figarch            0.450000
ARCH(Phi1)           0.100000
GARCH(Beta1)         0.400000

*/
	//myInitPoint[0] = 10;
	myInitPoint[0] = 0.04;
	myInitPoint[1] = 0.1;
	myInitPoint[2] = 0.4;
	myInitPoint[3] = 0.45;

	RegArchEstim(myModel, myValue, myResStruct, myResModel, &myInitPoint, myAlgoParam);
	cout << endl << myResStruct.mConvergenceString << endl;
	myResModel.Print();
	cout << endl << "LLH = " << myResStruct.mFunctValue << endl << "Temps : " << myResStruct.mComputeTime << " s" << endl;

/*
cRegArchGradient myGradient = cRegArchGradient(&myModel);
cRegArchHessien myHessien = cRegArchHessien(&myModel);
uint myNParam = myModel.GetNParam();
cDVector myGradLt = cDVector(myNParam);
cDMatrix myHessLt = cDMatrix(myNParam, myNParam);
double mylt = 0;
cDVector myGradNum = cDVector(myNParam);
cRegArchValue* myValueGrad = new cRegArchValue[myNParam+1];
cRegArchGradient* myGradientHess = new cRegArchGradient[myNParam + 1];

for (uint j = 0; j <= myNParam; j++)
	{
		myValueGrad[j].ReAlloc(myValue.mYt, myValue.mXt);
		myGradientHess[j].ReAlloc(&myModel);
	}


cDMatrix myHessNum = cDMatrix(myNParam, myNParam);
cDMatrix myDiff = cDMatrix(myNParam, myNParam);
cDVector myDiffGrad = cDVector(myNParam);
	cout.precision(3);
for (register uint t = 0; t < myNSimul; t++)
	{
			
		NumericRegArchGradLt(t, myModel, myValueGrad, myGradNum);
		RegArchGradLt(t, myModel, myValue, myGradient, myGradLt);
		myDiffGrad = myGradLt - myGradNum;
		for (register uint i = 0; i < myNParam; i++)
		myDiffGrad[i] = 100 * abs(myDiffGrad[i] / myGradNum[i]);
			

		NumericRegArchHessLt(t, myModel, myValueGrad, myGradientHess, myHessNum);
		RegArchHessLt(t, myModel, myValue, myGradient, myHessien, myHessLt);
		myDiff = 100*(myHessNum - myHessLt) ;
		for (register uint i = 0; i < myNParam; i++)
			for (register uint j = 0; j < myNParam; j++)
				myDiff[i][j] = abs(myDiff[i][j] / myHessNum[i][j]);

		
		cout << "t=" << t << endl;
//		cout << myDiffGrad << endl;
		cout << myDiff << endl;
//		cout << myGradient.mCurrentGradMu << endl;
//		cout << myHessien.mCurrentHessVar << endl;
//		cout << myHessLt << endl;
//		cout << myHessien.mCurrentHessDens << endl;
		
		myGradient.Update();
		myHessien.Update();
		cout << endl;
		cout << "Maxi = " << Maxi(Abs(myDiff)) << "\n" << endl;
	}

*/
	exit(0);

/*
//redéfinition de myModel avec valeur empiriques
cRegArchModel myModelEmpi;
cRegArchValue myValueEmpi(myNSimul);
myValueEmpi.ReAlloc(myValue.mYt);
double meanEmpi, varEmpi ;
myValueEmpi.ComputeMeanAndVar(meanEmpi,varEmpi);
cConstCondVar myConstVarEmpi(varEmpi);
cConst myConstMeanEmpi(meanEmpi);
cCondMean myCondMeanEmpi ;
myCondMeanEmpi.AddOneMean(myConstMeanEmpi);
myModelEmpi.SetMean(myCondMeanEmpi);
myModelEmpi.SetVar(myConstVarEmpi);
myModelEmpi.SetResid(myNormResid);

//cRegArchModel myResModel(myModel);
cRegArchModel myResModel(myModelEmpi);


double myMeanSerie, myVarSerie ;
myValue.ComputeMeanAndVar(myMeanSerie, myVarSerie) ;
myResModel.mMean->SetDefaultInitPoint(myMeanSerie, myVarSerie) ;
myResModel.mVar->SetDefaultInitPoint(myMeanSerie, myVarSerie);
myResModel.Print() ;

		ofstream fichier("../test.txt", ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
 
        if(fichier)
        {
                fichier <<	myValue.mYt << endl; 
                fichier.close();
        }
        else
                cerr << "Impossible d'ouvrir le fichier !" << endl; 


*/


}
