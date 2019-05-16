//*******************************************************************************************************
//*******************************************************************************************************
//*************************************                      ********************************************
//*************************************      protSequence     ********************************************
//*************************************                      ********************************************
//*******************************************************************************************************
//********************************* -get sequence from pdb file- ****************************************
//*******************************************************************************************************

//--Program setup----------------------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <time.h>
#include <sstream>
#include "ensemble.h"
#include "PDBInterface.h"
int main (int argc, char* argv[])
{
	//--Running parameters
    if (argc !=2)
	{
        cout << "protSequence <inFile.pdb>" << endl;
		exit(1);
	}
	enum aminoAcid {A,R,N,D,Dh,C,Cx,Cf,Q,E,Eh,Hd,He,Hp,I,L,K,M,F,P,O,S,T,W,Y,V,G,dA,dR,dN,dD,dDh,dC,dCx,dCf,dQ,dE,dEh,dHd,dHe,dHp,dI,dL,dK,dM,dF,dP,dO,dS,dT,dW,dY,dV,Csf,Sf4,Hca,Eoc,Oec,Hem};
	string aminoAcidString[] = {"A","R","N","D","D","C","C","C","Q","E","E","H","H","H","I","L","K","M","F","P","O","S","T","W","Y","V","G","dA","dR","dN","dD","dD","dC","dC","dC","dQ","dE","dE","dH","dH","dH","dI","dL","dK","dM","dF","dP","dO","dS","dT","dW","dY","dV","Csf","Sf4","Hca","Eoc","Oec","Hem"};
	string backboneSeq[] =   { "C", "L", "P", "T","E","Y","A","I",  "D",  "Q",  "R",  "F", "H", "W", "K", "S"};
	string backboneTypes[] = {"-π","-α","-ρ","-β","β","ρ","α","π","-πi","-αi","-ρi","-βi","βi","ρi","αi","πi"};
	string infile = argv[1];
	PDBInterface* thePDB = new PDBInterface(infile);
	ensemble* theEnsemble = thePDB->getEnsemblePointer();
	molecule* pMol = theEnsemble->getMoleculePointer(0);
	protein* _prot = static_cast<protein*>(pMol);

    UInt numChains = _prot->getNumChains();
    for (UInt i = 0; i < numChains; i++)
	{	cout << ">" << infile << endl;
        UInt numRes = _prot->getNumResidues(i);
        for (UInt j = 0; j < numRes; j++)
        {
            UInt restype = _prot->getTypeFromResNum(i,j);
			cout << aminoAcidString[restype];
        }
        cout << endl;
    }
	cout << endl << ">" << infile << ":" << endl;
	for (UInt i = 0; i < numChains; i++)
	{
        UInt numRes = _prot->getNumResidues(i);
		for (UInt j = 1; j < numRes-2; j++)
		{
			UInt backboneType = _prot->getBackboneSequenceType(i,j);
			cout << backboneSeq[backboneType];
		}
    }
	//pdbWriter(_prot,infile);
	return 0;
}



