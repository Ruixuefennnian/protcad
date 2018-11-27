//*******************************************************************************************************
//*******************************************************************************************************
//******************************	___  ____ ____ ___ ____ _ _  _  _  _   ******************************
//******************************	|__] |__/ |  |  |  |___ | |\ |  |__|   ******************************
//******************************	|    |  \ |__|  |  |___ | | \| o|  |   ******************************
//******************************   							               ******************************
//******************************		class protein is defined		   ******************************
//*******************************************************************************************************
//*******************************************************************************************************

//--Class Setup------------------------------------------------------------------------------------------
#include "assert.h"
#include <string.h>
#include <vector>
#include <algorithm>
#include "typedef.h"
#include "ran.h"
#include "molecule.h"
#include "chain.h"
#include "chainModBuffer.h"
#ifndef PDBWRITER_H
#include "pdbWriter.h"
#endif
#ifndef ATOMITERATOR_H
class atomIterator;
#endif
#ifndef RESIDUEITERATOR_H
class residueIterator;
#endif
#ifndef PROTEIN_H
#define PROTEIN_H
class protein : public molecule
{
public:
	friend class atomIterator;
    friend class residueIterator;
	friend class ruler;

//--Functions--------------------------------------------------------------------------------------------	
	
	//--Constructor and destructor declaration
	protein();
	protein(const string& _name);
	protein(const protein& _rhs);
	~protein();

	//--Coordinate and atom functions
	static UInt getHowMany() {return howMany; }
	void add(chain* _pChain);
	chain* getChain(UInt _chainIndex) {return itsChains[_chainIndex]; }
	dblVec getCoords(const UInt _chainIndex, const UInt _resIndex, const string _atomName)
		{ return itsChains[_chainIndex]->getCoords(_resIndex, _atomName);}
	dblVec getCoords(const UInt _chainIndex, const UInt _resIndex, const UInt _atomIndex) 
		{ return itsChains[_chainIndex]->getCoords(_resIndex, _atomIndex);}
	UInt getNumAtoms(const UInt _chainIndex, const UInt _resIndex) 
		{ return itsChains[_chainIndex]->getNumAtoms(_resIndex); }
	void setCoords(UInt _chainIndex, UInt _resIndex, UInt _atomIndex, dblVec _coords)
		{ return itsChains[_chainIndex]->setCoords(_resIndex, _atomIndex, _coords);}
	void makeAtomSilent(const UInt _chainIndex, const UInt _residueIndex, const UInt _atomIndex);
	void makeResidueSilent(const UInt _chainIndex, const UInt _residueIndex);
	vector <chainPosition*> getChainPositionVector(const UInt _chain);
	UIntVec getItsIndependentChainsMap() { return itsIndependentChainsMap; }
	vector <vector <int> > getItsChainLinkageMap() { return itsChainLinkageMap; }
	void finishProteinBuild();
	void listSecondaryStructure();
	void listDihedrals();
	double getRadius(UInt chainIndex, UInt resIndex, UInt atomIndex) {return itsChains[chainIndex]->getRadius(resIndex, atomIndex);}
	char getChainID(UInt chainIndex) {return itsChains[chainIndex]->getChainID();}
	void listConnectivity(UInt _chainIndex, UInt _resIndex) {return itsChains[_chainIndex]->listConnectivity(_resIndex);}

	
	//--Organization functions
	void updateTotalNumResidues();
	void initializeModificationMethods();

	void resetAllBuffers();
	static void silenceMessages() {messagesActive = false; }
	void accessChainZeroResZero();
	void symmetryLinkChainAtoB(UInt _aIndex, UInt _bIndex);
	void printAllLinkageInfo();
	int getIndexFromResNum(UInt _chainIndex, UInt _resnum);
	UInt getNumChains() const {return itsChains.size();}
	UInt getNumBpt(UInt restype) {return residue::getNumBpt(restype);}
	int getResNum(UInt _chainIndex, UInt _resIndex) {return itsChains[_chainIndex]->getResNum(_resIndex);}
	UInt getNumResidues(UInt _chainIndex) const;
	UInt getTypeFromResNum(UInt _chainIndex, UInt _resNum) { return itsChains[_chainIndex]->getTypeFromResNum(_resNum);}
	string getTypeStringFromAtomNum(UInt _chainIndex, UInt _resNum, UInt _atomNum) { return itsChains[_chainIndex]->getTypeStringFromAtomNum(_resNum, _atomNum);}
	string getTypeStringFromResNum(UInt _chainIndex, UInt _resNum) {return itsChains[_chainIndex]->getTypeStringFromResNum(_resNum);}
	void removeResidue(UInt _chainIndex, UInt _resNum) {return itsChains[_chainIndex]->removeResidue(_resNum);}

	//--Mutations functions
	void stripToGlycine();
	void activateForRepacking(const UInt _chainIndex, const UInt _residueIndex);
	void activateForRepacking(const UInt _chainIndex, const UInt _start, const UInt _end);
	void activateAllForRepacking(const UInt _chainIndex);
	void setResNotAllowed(const UInt _chainIndex, const UInt _residueIndex, const UInt _resType);
	void setListNotAllowed(const UInt _chainIndex, const UInt _residueIndex, const vector <UInt> _typeIndexVector);
	void setResAllowed(const UInt _chainIndex, const UInt _residueIndex, const UInt _resType);
	UIntVec getResAllowed (const UInt _chainIndex, const UInt _residueIndex);
	void setOnlyHydrophilic(const UInt _chainIndex, const UInt _residueIndex);
	void setOnlyROCHydrophobic(const UInt _chainIndex, const UInt _residueIndex);
	void setOnlyCharged(const UInt _chainIndex, const UInt _residueIndex);
	void setOnlyNativeIdentity(const UInt _chainIndex, const UInt _residueIndex);
	void setAllHydrogensOn(const bool _hydrogensOn);
	void setAllPolarHydrogensOn(const bool _polarHydrogensOn);
	void mutate(const UInt _chainIndex, const UInt _resIndex, const UInt _aaIndex);
	void mutateWBC(const UInt _chainIndex, const UInt _resIndex, const UInt _aaIndex);
	int mutate(vector <int> _position, UInt _resType);
	bool performRandomMutation(ran& _ran);
	bool performRandomMutation(ran& _ran, vector <int> _position);	
	residue* superimposeGLY(const UInt _chain, const UInt _residue);
	void commitLastMutation();
	void undoLastMutation();
	void saveCurrentState();
	void undoState();
	void commitState();	
	void saveState(string& _filename);
	void saveState(string _filename);
	void setupSystem(ran& _ran);
	vector <int> getLastModification();
	vector <int> chooseNextTargetPosition(ran& _ran);
	UInt chooseNextMutationIdentity(ran& _ran, vector <int> _position);
	int modify(ran& _ran);
	int modify(ran& _ran, vector <int> _position);
	void acceptModification();
	void rejectModification();
	double netCharge();
	
	//--Optimization functions
	void protOpt(bool _backbone); // --Sidechain and backbone optimization with a polarization based dielectric scaling of electrostatics-- dpike
	void protRelax(bool _backbone);
	void protOpt(bool _backbone, UIntVec _frozenResidues, UIntVec _activeChains);
	void optimizeSmallRotations(UInt _steps, double _stepSize);
	void optimizeSmallRotations(vector <UIntVec> _positions, UInt _steps, double _stepSize);
	void optimizeSmallRotations(UIntVec _position, UInt _steps, double _stepSize);
	vector <vector < double > > getRotationEnergySurface(vector < UIntVec > _active, UInt _steps, double _stepSize, UInt _activePos, vector <vector< double > > _bestChiArray, double &_lowestEnergy);
	vector < double >  getRotationEnergySurface(UIntVec _active, UInt _steps, double _stepSize, UInt _chiPos, vector <double>  _bestChiArray, double &_lowestEnergy);
	vector <UIntVec> rotamerDEE();
	vector <UIntVec> rotamerDEE(vector <UIntVec> _activePositions);
	void optimizeRotamers();
	void optimizeRotamers(vector <UIntVec> _positions);
	void optimizeRotamers(vector <UIntVec> _positions, vector <UIntVec> _rotamerArray);
	
	//--Energy functions
	void setMoved (UInt chainIndex, UInt resIndex, bool _moved) {itsChains[chainIndex]->setMoved(resIndex, _moved);}
	void setMoved(bool _moved);
	bool getMoved(UInt chainIndex, UInt resIndex) {return itsChains[chainIndex]->getMoved(resIndex);}
	double protEnergy();
	void updateEnergy();
	double protEnergy(UInt chainIndex, UInt resIndex);
	double getMedianResidueEnergy();
	double getMedianResidueEnergy(UIntVec _activeChains);
	UInt getNumChis(const UInt _chainIndex, const UInt _resIndex, const UInt _bpt) {return itsChains[_chainIndex]->getNumChis(_resIndex,0); }
	UInt getNumHardClashes(UInt chainIndex, UInt resIndex);
	UInt getNumHardClashes();
	void updateClashes();
	UInt getMedianResidueNumHardClashes();
	//double getMedianResEnergy(UIntVec _activeChains, UIntVec _activeResidues);
	double getSolvationEnergy(UInt _chainIndex, UInt _residueIndex) {return itsChains[_chainIndex]->getSolvationEnergy(_residueIndex); }
	double getAtomCharge(UInt _chainNum, UInt _resNum, UInt _atomNum) { return itsChains[_chainNum]->getAtomCharge(_resNum, _atomNum); }
	double calculateHCA_O_hBondEnergy();
	UIntVec getEnergySurface(vector <UIntVec> _activePositions, vector <UIntVec> _rotamerArray, UIntVec _currentArray, UIntVec _bestArray, UInt _index, double& _lowestEnergy);
	double getHBondEnergy(const UInt _chain1, const UInt _res1, const UInt _chain2, const UInt _res2);
	double getResPairEnergy(const UInt _chain1, const UInt _res1, const UInt _chain2, const UInt _res2);
	double getIntraEnergy(const UInt _chainIndex1, const UInt _resIndex1, const UInt _atomIndex1, const UInt _chainIndex2, const UInt _resIndex2, const UInt _atomIndex2);
	double getPairwiseResidueEnergy(const UInt _chain1, const UInt _res1, const UInt _chain2, const UInt _res2);
	double getDielectric(UInt _chainIndex, UInt _residueIndex) {return itsChains[_chainIndex]->getDielectric(_residueIndex); }
	double getDielectric(UInt _chainIndex, UInt _resIndex, UInt _atomIndex) {return itsChains[_chainIndex]->itsResidues[_resIndex]->itsAtoms[_atomIndex]->getDielectric();}
	double intraEnergy();
	double intraSoluteEnergy(bool _updateDielectrics, UInt _activeChain);
	double interSoluteEnergy(bool _updateDielectrics, UInt _chain1, UInt _chain2);
	vector <double> chainBindingEnergy();
	void polarizability();
	void calculateDielectrics();
	vector <double> calculateSolvationEnergy(UInt _chainIndex, UInt _residueIndex, UInt _atomIndex) {return itsChains[_chainIndex]->itsResidues[_residueIndex]->calculateSolvationEnergy( _atomIndex);}
	//vector <double> calculateChainIndependentDielectric(chain* _chain, residue* _residue, atom* _atom);
	//vector <double> calculateResidueIndependentDielectric(residue* _residue, atom* _atom);
	void updateDielectrics();
	void updateMovedDependence();
	//void updatePositionDielectrics(UInt _chainIndex, UInt _residueIndex);
	//void updateChainIndependentDielectrics(UInt _chainIndex);
	//void updateResidueIndependentDielectrics(UInt _chainIndex, UInt _resIndex);
	double intraEnergy(const UInt _chain);
	double intraEnergy(UInt _chain1, UInt _chain2);
	double getPositionEnergy(vector <int> _position);
	double getPositionEnergy(vector <UInt> _position);
	double getRotamerEnergy(UInt _chain, UInt _residue) { return itsChains[_chain]->rotamerEnergy(_residue); }
	double getPositionEnergy(UInt _chainIndex, UInt _residueIndex);
	double getSelfEnergy(UInt _chainIndex, UInt _residueIndex);
	vector <double> protLigandBindingEnergy(UInt ligChainIndex, UInt ligResIndex);

	//--Transformation functions
	double getBetaChi(UInt _chainIndex, UInt _residueIndex) {return itsChains[_chainIndex]->getBetaChi(_residueIndex); }
	void setBetaChi(UInt _chainIndex, UInt _residueIndex, double _chi) {return itsChains[_chainIndex]->setBetaChi(_residueIndex, _chi); }
	int setPhi(const UInt _chain, const UInt _res, double _angle);
	int setPsi(const UInt _chain, const UInt _res, double _angle);
	int setDihedral(const UInt _chainIndex, const UInt _resIndex, double _dihedral, UInt _angleType, UInt _direction);
	double getPhi(UInt _chain, UInt _res) {return itsChains[_chain]->getPhi(_res);}
	double getPsi(UInt _chain, UInt _res) {return itsChains[_chain]->getPsi(_res);}
	double getAngle(UInt _chain, UInt _res, UInt angleType) {return itsChains[_chain]->getAngle(_res, angleType);}
	void translate(const dblVec& _dblVec);
	void translate(const UInt _index, const dblVec& _dblVec);
	void translate(const UInt _index, const double _x,const double _y,const double _z);
	void translate(const double _x, const double _y, const double _z);
	void translateChain(UInt _chain, const double _x, const double _y, const double _z);
	void translateChainR(UInt _chain, const double _x, const double _y, const double _z);
	void transform(const UInt _index, const dblMat& _dblMat);
	void rotate(const UInt _index, const axis _axis, const double _theta);
	void rotate(const UInt _index, const point& _point, const dblVec& _R_axis, const double _theta);
	void rotate(const axis _axis, const double _theta);
	dblVec getBackBoneCentroid();
	dblVec getBackBoneCentroid(UInt _chain) { return itsChains[_chain]->getBackBoneCentroid(); }
	void coilcoil(const double _pitch);
	void coilcoil(UInt _chain, double _pitch);
	void eulerRotate(const double _phi, const double _theta, const double _psi);
	void undoEulerRotate(const double _phi, const double _theta, const double _psi);
	void eulerRotate(UInt _chain, const double _phi, const double _theta, const double _psi);
	void undoEulerRotate(UInt _chain, const double _phi, const double _theta, const double _psi);
	void rotateChain(UInt _chain, const axis _axis, const double _theta);

	//--Rotamer functions
	void setRotamerNotAllowed(const UInt _chainIndex, const UInt _resIndex, const UInt _resType, const UInt _bpt, const UInt _rotamer);
	void listAllowedRotamers(UInt _chain, UInt _resIndex);
	void setRotamer(const UInt _chainIndex, const UInt _resIndex, const UInt _bpt, const UInt _rotamer);
	void setRotamerWBC(const UInt _chainIndex, const UInt _resIndex, const UInt _bpt, const UInt _rotamer);
	void setPolarHRotamer(const UInt _chainIndex, const UInt _resIndex, const UInt _rotamer);
	UIntVec getCurrentRotamer(UInt _chainPos, UInt _resIndex) { return itsChains[_chainPos]->getCurrentRotamer(_resIndex); }
	void setCanonicalHelixRotamersOnly(const UInt _chainIndex, const UInt _resIndex, const UInt _resType);
	bool performRandomRotamerChange(ran& _ran);
	bool performRandomRotamerRotation(ran& _ran);
	bool performRandomRotamerChange(ran& _ran, vector <int> _position);
	bool performRandomRotamerRotation(ran& _ran, vector <int> _position);
	void undoLastRotamerChange();
	void undoLastRotamerRotation();
	void commitLastRotamerChange();
	void commitLastRotamerRotation();
	void setCanonicalHelixRotamersOnly(const UInt _chainIndex, const UInt _resIndex);
	void setCanonicalHelixRotamersOnly(const UInt _chainIndex);
	UIntVec getAllowedRotamers(UInt _chainIndex, UInt _resIndex, UInt _resType, UInt _bpt) { return itsChains[_chainIndex]->getAllowedRotamers(_resIndex, _resType, _bpt); }
	vector <UIntVec> getAllowedRotamers(UInt _chainIndex, UInt _resIndex, UInt _resType) { return itsChains[_chainIndex]->getAllowedRotamers(_resIndex, _resType); }
	void setRelativeChi(const UInt _chainIndex, const UInt _resIndex, const UInt _bpt, const UInt _chi, const double _angle);
	void setChi (const UInt _chainIndex, const UInt _resIndex, const UInt _bpt, const UInt _chi, const double _angle);
	double getChi (const UInt _chainIndex, const UInt _resIndex, const UInt _bpt, const UInt _chi) { return itsChains[_chainIndex]->getChi(_resIndex, _bpt, _chi); }
	vector < vector <double> >  getSidechainDihedrals(UInt _chainIndex, UInt _indexInChain) {return itsChains[_chainIndex]->getSidechainDihedralAngles(_indexInChain);}
	void setSidechainDihedralAngles(UInt _chainIndex, UInt _indexInChain, vector< vector<double> > Angles);
		
	//--Surface area functions
	double getVolume(UInt _method);
	void initializeSpherePoints();
	void initializeSpherePoints(UInt _chain);
	void initializeSpherePoints(UInt _chain, UInt _residue);
	void removeSpherePoints();
	void removeSpherePoints(UInt _chain);
	void removeSpherePoints(UInt _chain, UInt _residue);
	double tabulateSurfaceArea();
	double tabulateSurfaceArea(UInt _chain);
	double tabulateSurfaceArea(UInt _chain, UInt _residue);
	double tabulateSurfaceArea(UInt _chainIndex, UInt _residueIndex, UInt _atomIndex);
	double tabulateSolvationEnergy();
	double tabulateSolvationEnergy(UInt _chain);
	double tabulateSolvationEnergy(UInt _chain, UInt _residue);
	double getItsSolvationParam();
	void setItsSolvationParam(UInt _param);

//--Defined functions------------------------------------------------------------------------------------
private:
	bool isValidHelixRotamer ( UInt _resType, UInt _bpt, UInt _allowedRotamer ); // contains our definitions for canonical helix rotamers
	int chooseTargetChain(ran& _ran);
	int chooseModificationMethod(ran& _ran);
	
	//--Variable declarations
	static bool messagesActive;
	static bool calcSelfEnergy;
	static UInt howMany;
	UInt itsNumResidues;
	vector < vector < vector  <double> > > energies;
	static UInt itsSolvationParam;
	vector <chain*> itsChains;
	vector <UInt> itsIndependentChainsMap;
	vector<vector<int> > itsChainLinkageMap;
	bool (protein::*itsModificationMethods[5])(ran& _ran);
	
	//--Buffering
	int itsLastModifiedChain;
	int itsLastModificationMethod;
	ran itsRan;
};
#endif
