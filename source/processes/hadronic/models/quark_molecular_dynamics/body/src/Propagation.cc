#include "G4ios.hh"
#include "g4std/fstream"

#include "G4KineticTrack.hh"
#include "G4KineticTrackVector.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

#include "G4ShortLivedConstructor.hh"
#include "G4ShortLivedTable.hh"

#include "G4VDecayChannel.hh"
#include "G4DecayTable.hh"

#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"

#include <algorithm>
#include "newvector.hh"
#include "Random.hh"
#include "newBinning.hh"
#include "Geometry.hh"
#include "Arguments.hh"
#include "HeatBath.hh"
#include "ThermDist.hh"
#include "HadronGas.hh"
#include "MathTools.hh"
#include "Error.hh"
#include "outputList.hh"
#include "Propagation.hh"
#include "ParticleBase.hh"
#include "reactionChannels.hh"
#include "Collision.hh"
#include "ParticleKinematics.hh"
#include "iso.hh"
#include "array.hh"
#include "Memory.hh"
#include "Tree.hh"
#include "EventHandling.hh"
#include "Kinematics.hh"
#include "output.hh"

#define Nnext 3

//extern outputList Output;

double ColorString::Ekin_min = 0.15;  // minimum kinetic energy for final break
double ColorString::Cutoff = 10.0;    // [GeV]

ParticleBase* makeParticle(const ParticleType& h)
{
  if ( h.isQuark() || h.isDiquark() )     
    return NEW Quark(h);
  else
    return NEW Hadron(h);
}

ParticleBase* makeParticle(const ParticleType& h,const QuantumProjections& q,double m)
{
  if ( h.isQuark() || h.isDiquark() )
    return NEW Quark(h,q);
  else {
    ParticleBase* p = NEW Hadron(h,q);
    if ( m >= 0.0 ) 
      p->SetMass(m);
    return p;
  }
}

ParticleBase* makeParticle(const ParticleType& h,const QuantumProjections& q,const Vektor3& p,const Vektor3& x,double m)
{
  if ( h.isQuark() || h.isDiquark() )
    return NEW Quark(h,q,p,x);
  else {
    ParticleBase* P = NEW Hadron(h,q,p,x);
    if ( m >= 0.0 ) 
      P->SetMass(m);
    return P;
  }
}

void PotentialBase::print(G4std::ostream& o,double min,double max,int n)
{
  double dx=(max-min)/n;
  for (int i=0; i<n; i++) {
    double x = min+i*dx;
    o << x << "  " << V(x,1) << "  " << V(x,-1) << "  ";
    try {
      o << V_inv(x,1) << G4endl;
    }
    catch ( ... ) { o << "*\n"; }
  }
}

class AnyQuark : public QuantumNumbers
{
public:
  AnyQuark() { setB(1.0/3.0); }
};

class AnyDiquark : public QuantumNumbers
{
public:
  AnyDiquark() { setB(2.0/3.0); }
};

void Quark::announceEvent() const 
{
  if ( traj_dt > FissionProbab )
    CollisionTab::addEntry(Time(),*this,FLUXTUBE);
}

ColorString::ColorString(const double& E, 
                         const Vektor3& ptot, 
                         const Vektor3& rtot, 
                         int n,
                         const QuantumState pparray[],
                         const G4std::vector<ParticleBase*>& types,
                         int nhadronever) :
                         N(n),
                         Mass0(E),
                         Ptot(ptot),
                         Rtot(rtot),
                         NhadronEver(nhadronever)
{
	PDGCode=0;
	FormationTime=0;
//
// building hadrons out of clusters of 2 quarks
//
  if ( N == 2 )
    for (int i=0; i<N; i++)
      array[i] = pparray[i];
//
// building hadrons out of clusters of 3 quarks
//
  else if ( N == 3 ) {
    int choose = rand_gen::Random(1,3);
    switch ( choose ) {
    case 1 : 
      array[1] = pparray[0]+pparray[1]; 
      array[0] = pparray[2]; 
      break;
    case 2 : 
      array[1] = pparray[1]+pparray[2]; 
      array[0] = pparray[0]; 
      break;
    case 3 : 
      array[1] = pparray[0]+pparray[2]; 
      array[0] = pparray[1]; 
      break;
    }
  }
//
// larger clusters are not considered
//
  else
    throw "Something went wrong!";

  pp = array[0]+array[1];
  beta = Ptot/(-sqrt(Mass0*Mass0+Ptot*Ptot));
  G4cerr << "CLUSTER: " << Mass0 << " --> ";
//
// make hadron out of cluster
//
  if ( 1 || Mass0 < Cutoff ) {
    ParticleType& knot = Knot<ParticleType>::FindKnot((ParticleType&)(QuantumNumbers&)pp);
    ParticleType& h = knot.selectType(pp.C(),types,Mass0);
    ParticleBase* H = makeParticle(h,pp);
    H->SetFlag(NhadronEver) ;
    H->SetMomentum(Ptot);
    H->SetCoordinates(Rtot);
    H->SetMass(Mass0);
 
    G4cerr << H->Name() << G4endl;
//
//  G4cout << "- NEW HADRON BY CLUSTERING at time=" << H->Time() << ": " << *H << G4endl;
//
    G4cout << "NEW HADRON BY CLUSTERING at time=" << H->Time() 
           << " and point="                       << H->Coordinates() 
           << " with momentum="                   << H->Momentum() 
           << " - PDGCode: "                      << H->PDGCode() 
           << " ("                                << H->Name()
           << ")" << G4endl;

		PDGCode = H->PDGCode();
    FormationTime = H->Time();

  }
  else {
    G4cerr << "Too high!!!!\n";
  }
}

ColorString::~ColorString()
{
}

void ColorString::breakUp()
{
  if ( Mass0 < Cutoff ) {
    ParticleType& knot = Knot<ParticleType>::FindKnot((ParticleType&)(QuantumNumbers&)pp);
    ParticleType& h = knot.selectType(Mass0);
    //    ParticleType& h = TypeContainer::ParticleTypes->findType(pp,Mass0);
    ParticleBase* H = makeParticle(h,pp);
    H->SetMomentum(Ptot);
    H->SetCoordinates(Rtot);
    H->SetMass(Mass0);
  }
  else {
    G4cerr << "ZeroMass = " << Mass0 << ":\n";
  }
}

class energyFormula : private InverseFunctionWithNewton
{
  double M,m12,m22,m32;
  REAL ToBeInverted(REAL x) const { return sqrt(x*x+m12)+sqrt(x*x+m22)+sqrt(x*x+m32); }
public:
  energyFormula(double m,double m1,double m2,double m3) 
    : InverseFunctionWithNewton(m/3.0),M(m),m12(m1*m1),m22(m2*m2),m32(m3*m3) {}
  operator double() { return Inverse(M); }
};


Quark::~Quark() 
{
}

void Quark::reset()
{
  path = 0.0;
  traj_dt = 0.0;
  FissionProbab = -log(1.0-rand_gen::Random())/Colour::FissionRate;
}

void Quark::refresh()
{
  Colour::decay = false;
  if ( Color() ) {
    Vektor3 f = Force();
    double lf = length(f);
    Vektor3 v = Velocity();
    double x = f*v;
    if ( x < 0 ) {
      if ( lf ) 
	path += length(v)*Soup->TimeStep();
	//	path -= x/lf*Soup->TimeStep();
      traj_dt += path*Soup->TimeStep();
      Colour::decay = ( traj_dt > FissionProbab );
      if (  traj_dt > FissionProbab ) {
	Colour::decay = true;
	justFiss = 1.0;
      }
    }
    else if ( traj_dt ) {
      reset();
    }
  }
}


bool Colour::decay = false;
bool Colour::directHadrons = false;
bool Colour::allowDecay = true;
bool Colour::allowClustering = true;
bool Colour::allowDecomposition = false;
bool Colour::removeWhenUndefined = true;

double Colour::kappa = 0.9; // GeV/fm
double Colour::alpha = 1.0/137.0;; 
double Colour::alpha_s = 4.0/3.0*0.197*2.0; 
double Colour::B = 2; // scale of woods-saxxon
double Colour::a = 0.1; 
double Colour::R = 0.8; // fm^2
double Colour::A = 0.7;
double Colour::shift = 0.1;
double Colour::shift2 = shift*shift;
double Colour::FissionRate = 0.0; // p*A, Units:  fm^(-2)

double Colour::deconfined = 2e-2; // GeV/fm
//double Colour::deconfined = 1e-1;
double Colour::minDist = 2.5; 
double Colour::decompDist = 0.8; // fm

double* Colour::FlavorFission = 0;
PotentialBase* Colour::Pot = 0;

G4std::vector<ParticleType*> Colour::Quarks;

Colour::Colour(double h) 
  : Nbody(h),InverseFunctionWithNewton(0.3)
{
	Nquark = Npart;
	NquarkEver = Npart;


  G4MesonConstructor Mesons;
  Mesons.ConstructParticle();

  G4BaryonConstructor Baryons;
  Baryons.ConstructParticle();

  G4ShortLivedConstructor ShortLived;
  ShortLived.ConstructParticle();


	TheNewHadrons = new G4KineticTrackVector();

  //  G4std::vector<ParticleType*> Q=Knot<ParticleType>::Find("SQ");
  //  G4std::vector<ParticleType*> D=Knot<ParticleType>::Find("DQ");
  //  G4std::vector<ParticleType*> Q=TypeContainer::ParticleTypes->findType(AnyQuark());
  //  G4std::vector<ParticleType*> D=TypeContainer::ParticleTypes->findType(AnyDiquark());
  
  InverseFunction::reducePrecision = false;
  G4cout << "first try"<<G4endl;
  if ( Quarks.empty() )
    G4cerr << "Quarks not initialized...";
  G4cout << "second try"<<G4endl;
  if ( !Pot )
    G4cerr << "No Potential defined...";
  G4cout << "third try"<<G4endl;
  FlavorFission = NEW double[Quarks.size()]; G4cerr << "Check: " << Quarks.size() << G4endl; 
  FissionRate = Schwinger(kappa);
  double u = B/(2*kappa*a)*(1+sqrt(1-4*kappa*a/B))-1.0;
  r0 = R+a*log(u);
  E0 = -B/(1+u);
  a0 = E0-kappa*r0;
}

Colour::~Colour()
{
  delete [] FlavorFission;
}

void Colour::setQuarks(G4std::vector<ParticleType*>& L)
{
  Quarks.insert(Quarks.begin(),L.begin(),L.end());
}

double Colour::Schwinger(double kap) 
{
  int nn = 2;
  double y = 0.0;
  double factor = A*sqr(kap)/4.0/pow(mathConstants::Pi,3)/sqr(mathConstants::hc);
  int f = 0;
  for (G4std::vector<ParticleType*>::iterator X=Quarks.begin(); X != Quarks.end(); X++) {
    for (int n=1; n<=nn; n++) 
      y += factor*exp(-mathConstants::Pi*sqr((*X)->getMass())*double(n)/(mathConstants::hc*kap))/double(n*n);
    FlavorFission[f++] = y;
  }
  while ( f ) 
    FlavorFission[--f] /= y;
  return y;
}

ParticleType& Colour::TunnelRate(double kap) 
{
  int nn = 2;
  double y = 0.0;
  double* arr = new double[Quarks.size()];
  double factor = A*sqr(kap)/4.0/pow(mathConstants::Pi,3)/sqr(mathConstants::hc);
  int f = 0;
  for (G4std::vector<ParticleType*>::iterator X=Quarks.begin(); X != Quarks.end(); X++) {
    for (int n=1; n<=nn; n++) 
      y += factor*exp(-mathConstants::Pi*sqr((*X)->getMass())*double(n)/(mathConstants::hc*kap))/double(n*n);
    arr[f++] = y;
  }
  double r = rand_gen::Random()*y;
  int j=-1;
  while ( ++j<Quarks.size() && (arr[j])<r );
  if ( j>=Quarks.size() )
    throw "Strange Error...";
  return *Quarks[j];
}

void Colour::setup()
{
  Nquark = 0;
	NquarkEver = 0;
	NhadronEver = 0 ;
  for (int i=0; i<Npart; i++) {
    if ( List[i]->isQuark() || List[i]->isDiquark() ) {
      ++Nquark;
			++NquarkEver;
		}
		else {
			++NhadronEver;
    }
  }
}

Vektor3 Colour::Ptot()
{
  Vektor3 P;
  for (int i=0; i<Npart; i++)
    P = P+List[i]->Momentum();
  return P;
}

ParticleType& Colour::selectQuark(criterion which) 
{ 
  //  if ( List[i]->isGluon() ) return TunnelRate(9.0/4.0*kappa);
  int f;
  //  do {
    f = 0; 
    double x = rand_gen::Random(); 
    while ( x > FlavorFission[f] ) ++f;
    //  }
    //  while ( (*Quarks)[f]->B() > 0.5 );
    G4cerr << "selected: " << f << G4endl;
  return *Quarks[f];
}

// NOTE:
//
//       +1   for  c c_bar
//       +0.5 for  c1 c1
//       -0.5 for  c1 c2_bar
//       -1   for  c c
//

double Colour::colors[7][7] = {{-1.0,+0.5,+0.5,0.0,-0.5,-0.5,+1.0},
		       {+0.5,-1.0,+0.5,0.0,-0.5,+1.0,-0.5},
		       {+0.5,+0.5,-1.0,0.0,+1.0,-0.5,-0.5},
		       {+0.0,+0.0,+0.0,0.0,+0.0,-0.0,-0.0},
		       {-0.5,-0.5,+1.0,0.0,-1.0,+0.5,+0.5},
		       {-0.5,+1.0,-0.5,0.0,+0.5,-1.0,+0.5},
		       {+1.0,-0.5,-0.5,0.0,+0.5,+0.5,-1.0}};

//
//       +1       for  c c_bar
//       +0.5     for  c1 c1
//       -0.25    for  c1 c2_bar
//       -0.125   for  c c
//

double Colour::colors_close[7][7] = {{-0.25,+0.5,+0.5,0.0,-0.125,-0.125,+1.0},
		       {+0.5,-0.25,+0.5,0.0,-0.125,+1.0,-0.125},
		       {+0.5,+0.5,-0.25,0.0,+1.0,-0.125,-0.125},
		       {+0.0,+0.0,+0.0,0.0,+0.0,-0.0,-0.0},
		       {-0.125,-0.125,+1.0,0.0,-0.25,+0.5,+0.5},
		       {-0.125,+1.0,-0.125,0.0,+0.5,-0.25,+0.5},
		       {+1.0,-0.125,-0.125,0.0,+0.5,+0.5,-0.25}};


double Colour::Factor(double r,int i,int j,int s) const { 
  int a = s*(int)List[i]->Color()+3;
  int b = (int)List[j]->Color()+3;
  double c0 = colors_close[a][b]; 
  double c1 = colors[a][b];
  return c0+colorProfile(r)*(c1-c0);
}

/*
REAL Colour::ToBeInverted(REAL x) const
{
  REAL s = 0;
  for (int i=0; i<Nquark; i++) {
    if ( i != kk ) {
      Vektor3 y = r_k-List[i]->Coordinates();
      Vektor3 z = y+(x/lf)*f;
      double lz = length(z);
      double ly = length(y);
      s += colors[List[kk]->Color()+3][List[i]->Color()+3]*(kappa*(lz-ly)
	-alpha_s/lz+alpha_s/ly)
	+ alpha*List[i]->Charge()*List[kk]->Charge()*(1.0/lz-1.0/ly);
    }
  }
  return s;
}


REAL Colour::ToBeInverted(REAL x) const
{
  REAL s = 0;
  for (int i=0; i<Nquark; i++) {
    if ( i != kk ) {
      Vektor3 y = r_k-List[i]->Coordinates();
      Vektor3 z = y+(x/lf)*f;
      double lz = length(z);
      double ly = length(y);
      s += colors[List[kk]->Color()+3][List[i]->Color()+3]*(lz-ly);
    }
  }
  return kappa*s;
}
*/
REAL Colour::ToBeInverted(REAL x) const
{
  REAL s = V(sigma,1.0);
  for (int i=0; i<Nquark; i++) {
    //    if ( i != kk ) {
      Vektor3 d = r_k-List[i]->Coordinates();
      double l1 = length(d+(x/lf)*f);
      double l2 = length(d+((x-sigma)/lf)*f);
      s += V(l1,Factor(l1,kk,i))+V(l2,Factor(l2,kk,i,-1));
  }

  return s;
}


REAL Colour::Derivative(REAL x) const
{
  double s = 0; //V_prime(x-sigma)-V_prime(x);
  for (int i=0; i<Nquark; i++) {
    //    if ( i != kk ) {
      Vektor3 z = dr(r_k-(x/lf)*f,List[i]->Coordinates());
      Vektor3 z2 = dr(r_k+((x-sigma)/lf)*f,List[i]->Coordinates());
      double lz = length(z);
      double lz2 = length(z2);
      double a = 1,a2 = 1;
      if ( lz>0.0 ) 
	a = z*f/lf/lz;
      if ( lz2>0.0 ) 
	a2 = z2*f/lf/lz2;
      s += (V_prime(lz,Factor(lz,kk,i))*a+V_prime(lz2,Factor(lz,kk,i,-1))*a2);
      //    }
  }
  return s;
}


Matrize rotateFrame(const Vektor& r)
{
  Matrize U(3,3);
  Vektor r0 = (1.0/length(r))*r;
  REAL rho = sqrt(sqr(r0[1])+sqr(r0[2]));
  signed int s = 1;
  
  U.column(1) = r0;
  if ( rho ) {
    U(1,2) = r0[2]/rho*s;
    U(2,2) = -r0[1]/rho*s;
    U(1,3) = -r0[1]*r0[3]/rho*s;
    U(2,3) = -r0[2]*r0[3]/rho;
    U(3,3) = rho*s;
  }
  
  return U;
}

void Colour::decompose(ParticleBase* p)
{
  try {
    int n = Npart;
    CollisionTab::addEntry(Time(),*p,DECOMPOSITION);
    CollisionTab::perform(Time());
    Nquark += (Npart-n);
    //    sub(p);
  }
  catch ( char* s) {}

	for (int i=0; i<Npart; i++) {
		if ( List[i]->isQuark() && (List[i]->Flag() == 0) ) {
			++NquarkEver;
			List[i]->SetFlag(NquarkEver);
		}
	}
}

void Colour::decomposeAll()
{
  int i=0;
  while ( i<List.size() ) {
    if ( !List[i]->isQuark() ) {
      decompose(List[i]);
    }
    ++i;
  }
}

void Colour::decomposition(int i) 
{
  for (int j=0; j<Nquark; j++) {
    double d = distance(i,j);
    try {
      if ( d < decompDist ) {
	CollisionTab::addEntry(Time(),*((ParticleBase*)List[i]),DECOMPOSITION);
	G4cerr << "DECOMPOSITION: " << List[i]->Name() << "  " << d << G4endl;
	break;
      }
    }
    catch (char *s) {}
  }
}

void Colour::one_step()
{

  Nbody::one_step();

  bool decayOccured = false;

  if ( allowDecomposition ) {
    int n = Npart;
    for (int i=Nquark; i<Npart; i++) 
      decomposition(i); 
    CollisionTab::perform(Time());
//		Nquark = 0;
//		for (int i=0; i<Npart; i++) {
//			if ( List[i]->isQuark() || List[i]->isDiquark() ) {
//				++Nquark;
//			}
//			if ( List[i]->isQuark() && (List[i]->Flag() == 0) ) {
//				++NquarkEver;
//				List[i]->SetFlag(NquarkEver);
//			}
//		}
    if ( n != Npart ) {
      setup();
    }
  }
  if ( allowDecay || allowClustering ) {

    for ( int i=0; i<Nquark; i++ ) {
      List[i]->refresh();
      if ( allowClustering ) 
        clusters(i);
      if ( allowDecay && decay && !eraseList.size() ) {
        decayOccured = true;
        ParticleType& h = selectQuark();
        f = List[i]->Force();
        lf = length(f);
        r_k = List[i]->Coordinates();
        Vektor3 p_k = List[i]->Momentum();
        double pkk = length(p_k);
        kk = i;
        Matrize U = rotateFrame(f);
        QuantumState pp_old = List[i]->getProperties();
        QuantumState pp_alpha(h,QuantumProjections(sign(List[i]->B()),List[i]->Color(),h.getIso3(),h.getSpin3()));
        QuantumState pp_beta = anti(pp_alpha);
        QuantumState pp_hadron = pp_beta + pp_old;
        int n_try = MAX_TRIES;
        double lambda;
        bool massTooHigh = false;
        do {
          try {
            double pt2 = -kappa/mathConstants::Pi*log(1-rand_gen::Random());
            double e_b = sqrt(pt2+sqr(h.getMass()));
            sigma = V_inv(2*e_b);
            double pt = sqrt(pt2);
            double phi = 2*mathConstants::Pi*rand_gen::Random();
            Vektor3 Pt = U*Vektor3(0,pt*cos(phi),pt*sin(phi));
            double ptx = length(Pt);
            Vektor3 new_P = List[i]->Momentum()-Pt;
            double pl = length(new_P);
            double eps;
            ParticleType* had;
            if ( directHadrons ) {
              double e_k = List[i]->E();
              had = &Knot<ParticleType>::FindKnot((ParticleType&)(QuantumNumbers&)pp_hadron).selectType(); 
              double e_m = sqrt(sqr(had->getMass())+new_P*new_P);
              sigma = 0;
              eps = e_b+e_m-e_k;
            }
            else {
              eps = 2*e_b;
              G4cerr << "sigma=" << sigma << G4endl;
            }
            if ( sigma>=0 ) {
              massTooHigh = false;
              lambda = Inverse(-eps);
              if ( ToBeInverted(lambda)+eps != 0.0 ) { 
								G4cerr << "WRONG! " << ToBeInverted(lambda)+eps << "\n"; lambda = -1; 
							}
              if ( lambda < 0 ) 
                throw "Wrong Mass...";
              Vektor3 x0 = r_k+(lambda/lf)*f;
              G4cerr << h.Name() << ": " << sigma << "  " 
                   << lambda << "  " << x0 << "  " << f << G4endl;
							//
							// color string fission from Schwinger mechanism
							//
							//
              if ( directHadrons ) {
                ParticleBase* p_hadron = makeParticle(*had,pp_hadron,new_P,r_k);
                ParticleBase* p_quark = makeParticle(h,pp_alpha,Pt,x0);
								NquarkEver += 1;
								NhadronEver += 1;
								p_quark->SetFlag(NquarkEver) ;
								p_hadron->SetFlag(NhadronEver) ;
								G4cerr << "- NEW HADRON at time=" << Time() << ": " << *p_hadron << G4endl;
								G4cerr << "- NEW QUARK at time=" << Time() << ": " << *p_quark << G4endl;
              }
              else {
                Vektor3 x1 = r_k+((lambda-sigma)/lf)*f;
                Vektor3 xs = 0.5*(x0+x1);
                ParticleBase* p_new1 = makeParticle(h,pp_alpha,Pt,x0);
                ParticleBase* p_new2 = makeParticle(h,pp_beta,-Pt,x1);
                Nquark += 2;
								NquarkEver += 2;
                G4cerr << "quarks created: " << length(xs) << G4endl;
								G4cerr << "quarks created: " << length(xs) << G4endl;
								G4cout << "- NEW QUARK at time=" << Time() << ": " << *p_new1 << G4endl;
								G4cout << "- NEW QUARK at time=" << Time() << ": " << *p_new2 << G4endl;
              }
              firstCall = true;
            }
            else {
              massTooHigh = true;
              --n_try;
            G4cerr << n_try << ": Mass to high. Trying again...!\n";
            }
          }  // of try
          catch ( char* s ) {
            massTooHigh = true;
            --n_try;
            G4cerr << "Error: " << s << G4endl;
            G4cerr << n_try << ": Mass to high. Trying again...!\n";
          }
          catch ( ... ) {
            massTooHigh = true;
            --n_try;
            G4cerr << n_try << ": Mass to high. Trying again...!\n";
          }
      	}    // of do
      	while ( n_try && massTooHigh );
      	  if ( !n_try ) {
      	    G4cerr << "Mass Too High!!! Giving up...\n";
      	  }
      	for ( int j=0; j<List.size(); j++ )
      	  List[j]->reset();
      }
      if ( eraseList.size() ) {
      	G4cerr << "ERASER:\n";
      	Vektor3 Ptot(0,0,0);
      	Vektor3 Rtot(0,0,0);
      	double Mtot = 0;
      	double eps = 0;
      	for (int l1=0; l1<eraseList.size(); l1++)
      	  for (int l2=l1+1; l2<eraseList.size(); l2++)
      	    eps += E_int(eraseList[l1],eraseList[l2]);
      	QuantumState p;
      	QuantumState* quarkList = NEW QuantumState[eraseList.size()];
      	G4std::vector<ParticleBase*> Types;
      	for (int l=0; l<eraseList.size(); l++) {
//
// insert produces error - type cast does not work? ******
//
      	  Types.insert(Types.end(),(ParticleBase*)List[eraseList[l]]);
      	  quarkList[l] = List[eraseList[l]]->getProperties();
      	  if ( l>0 ) 
      	    p = p + quarkList[l];
      	  else
      	    p = quarkList[l];
      	  Ptot += List[eraseList[l]]->Momentum();
      	  Rtot += List[eraseList[l]]->Mass()*List[eraseList[l]]->Coordinates();
      	  Mtot += List[eraseList[l]]->Mass();
      	  eps += List[eraseList[l]]->E();
      	  for (int j=0; j<Nquark; j++) {
      	    int l1;
      	    for (l1=0; l1<eraseList.size(); l1++) {
      	      if ( j == eraseList[l1] ) 
      		break;
      	    }
      	    if ( l1==eraseList.size() ) 
      	      eps += E_int(eraseList[l],j);
      	  }
      	}
      	Rtot *= 1.0/Mtot;
      	double ptot = sqrt(Ptot*Ptot);
      	G4cerr << "Energy: " << eps << "  " << ptot << G4endl;
      	G4cerr << "==> ";
      	while ( ptot >= eps ) {
      	  double borrowEnergy = ptot-eps+rand_gen::Random()*eps;
      	  eps += borrowEnergy;
      	  Ptot = Ptot*(borrowEnergy/ptot);
      	  G4cerr << "Momentum conservation not fulfilled!!!\n";
      	}
      	double p_mass = sqrt(eps*eps-ptot*ptot);
      	try {
      	  try {
//
// ------------------- Hadronization of cluster ----------------------------------
//
      	    ColorString NewHadron(p_mass,Ptot,Rtot,eraseList.size(),quarkList,Types,NhadronEver);

						G4cerr << "NEW Hadron: "      << NewHadron.GetPDGCode()
                   << " at time "         << NewHadron.GetFormationTime()
                   << " and point "       << NewHadron.GetPosition()
                   << " with 4momentum "  << NewHadron.Get4Momentum()
                   << G4endl;

            G4ParticleDefinition * NewHadronDefinition = 
                 G4ParticleTable::GetParticleTable()->FindParticle(NewHadron.GetPDGCode());

						G4cerr << "- PDG-Code from NewHadronDefinition: " << NewHadronDefinition->GetPDGEncoding()
						       << " (particle type "                      << NewHadronDefinition->GetParticleType() 
						       << ")" << G4endl;

            G4KineticTrack * NewHadronTrack = 
                 new G4KineticTrack(NewHadronDefinition, NewHadron.GetFormationTime(),NewHadron.GetPosition(),NewHadron.Get4Momentum());

						G4cerr << "- Formation time from NewHadronTrack: " << NewHadronTrack->GetFormationTime() 
						       << " (remaining lifetime "                  << NewHadronTrack->SampleResidualLifetime() 
						       << ")" << G4endl;

						G4cerr << " KineticTrackVector is empty? - " << TheNewHadrons->isEmpty() << G4endl;
						G4cerr << " KineticTrackVector has size " << TheNewHadrons->length() << G4endl;

            TheNewHadrons->insert(NewHadronTrack);

						G4cerr << " KineticTrackVector has size " << TheNewHadrons->length() << G4endl;

      	    NhadronEver++;
//
// -------------------------------------------------------------------------------
//

      	  }
      	  catch ( const ParticleType::undefinedParticle& ) {
      	    if ( !removeWhenUndefined ) 
      	      throw;
      	    G4cerr << "Removing!!\n";
      	  }
      	  catch ( ...  ) { throw; }
      	  G4std::sort(eraseList.begin(),eraseList.end());
      	  G4std::reverse(eraseList.begin(),eraseList.end());
      	  for (int X=0; X<eraseList.size(); X++){
      	    --Nquark;
      	    delete List[eraseList[X]];
      	  }
      	}
      	catch (...) { 
      	  for (int ii=0; ii<eraseList.size(); ii++)
      	    G4cerr << List[eraseList[ii]]->Name() << ",";
      	  G4cerr << "  : Combination not defined...\n";
      	}
      	eraseList.erase(eraseList.begin(),eraseList.end());
      }
    }    // of for ...
  }      // of  if ( allowDecay || allowClustering )
}

void Colour::clusters(int i)
{
  Quark* q = (Quark*)List[i];
  int old_ptr[Nnext];
  double old_max[Nnext];
  int Nn = G4std::min(Nnext,Nquark);
  for (int l=0; l<Nn; l++) {
    q->next[l].pointer = -1;
  }
  for ( int j=0; j<Nquark; j++ ) 
    if ( i!=j ) {
      double dist=distance(i,j);
      int l=0;
      while ( l<Nn && q->next[l].pointer>0 && dist > q->next[l].dist ) ++l;
      if ( l<Nn ) {
	for (int k1=Nn-2; k1>=l; k1--) {
	  q->next[k1+1] = q->next[k1];
	}
	q->next[l].set(j,dist);
      }
    }
  Vektor3 F = ((Particle*)q)->Force();
  RGB col = ((ParticleBase*)q)->Color();
  int Nmax = -1;
  try {
    {for (int l=0; l<Nn && q->next[l].pointer>=0 && q->next[l].pointer<Nquark; l++) {
      F += List[q->next[l].pointer]->Force();
      col = col+List[q->next[l].pointer]->Color();
      double len = length(F);
      q->next[l].force = len;
      if ( col.isWhite() ) {
	Nmax = l;
	break;
      }
    }
    }
  }
  catch ( ... ) {}
  if (Nmax<0 || Nmax>1 )
    return;
  //    G4cerr << "NEXT (" << Nmax << "): ";
  //    G4cerr << q->next[Nmax+1].dist/q->next[Nmax].dist << "  ";
  //    G4cerr << q->next[Nmax].force << G4endl;
  if ( q->next[Nmax].force/(Nmax+2) < deconfined && q->next[Nmax+1].dist/q->next[Nmax].dist > minDist && q->next[Nmax+1].dist>decompDist ) {
    G4cerr << Nmax << " : " << col << "  " << q->next[Nmax].force << "  " << q->next[Nmax+1].dist/q->next[Nmax].dist << G4endl;
    G4cerr << *((ParticleBase*)q) << G4endl;
    G4cerr << *List[q->next[0].pointer] << G4endl;
      eraseList.insert(eraseList.end(),i);
      G4cerr << "DECONFINED: " << List[i]->Name() << "," ; 
      for (int i=0; i<=Nmax; i++) {
	eraseList.insert(eraseList.end(),q->next[i].pointer);
	G4cerr << List[q->next[i].pointer]->Name()  << ",";
      }
      G4cerr << G4endl;
      q->next[Nmax+1].pointer = -1;
  }
  for (int k=0; k<Nnext-1; k++) {
    q->next[k].reset();
  }
}

/*
id Colour::clusters(int i)
{
  Quark* q = (Quark*)List[i];
  int old_ptr[Nnext];
  double old_max[Nnext];
  for (int l=0; l<Nnext; l++) {
    old_ptr[l] = q->next[l].pointer;
    old_max[l] = q->next[l].max;
  }
  for ( int j=0; j<Nquark; j++ ) 
    if ( i!=j ) {
      double dist=length(dr(((ParticleBase*)q)->Coordinates(),List[j]->Coordinates()));
      int l=Nnext;
      for (int k=Nnext; k; k--) {
	if ( dist < q->next[k-1].dist )
	  l--;
      }
      if ( l<Nnext ) {
	for (int k1=Nnext-2; k1>=l; k1--) {
	  q->next[k1+1] = q->next[k1];
	}
	q->next[l].set(j,dist);
      }
    }
  Vektor3 F = ((Particle*)q)->Force();
  {for (int l=0; l<Nnext; l++) {
    F += List[q->next[l].pointer]->Force();
    double len = length(F)/kappa;
    q->next[l].force = len;
  }
  }
  {for (int l=Nnext-2; l>=0 && l<Nnext; l--)
    if ( q->next[l].force < deconfined && q->next[l+1].dist/q->next[l].dist > minDist ) {
      RGB color = ((ParticleBase*)q)->Color();
      try {
	for (int k=0; k<=l; k++) {
	  //	eraseList.insert(eraseList.end(),q->next[k].pointer);
	  color = color+List[q->next[k].pointer]->Color();
	  cerr << q->next[k].pointer << ",";
	}
	cerr << " : " << q->next[l].force << "  " << q->next[l+1].dist/q->next[l].dist << G4endl;
	eraseList.insert(eraseList.end(),i);
	cerr << "DECONFINED: " << i << "," ;
	q->next[l+1].pointer = -1;
	break;
      }
      catch ( ... ) { 
	cerr << "Color-error caught!\n"; }
    }
  }
  for (int k=0; k<Nnext-1; k++) {
    q->next[k].reset();
  }
}
*/
void Colour::print(G4std::ostream& o) 
{ 
  o << "#  time=" << Time() << ", Npart=" << Npart <<
    ",  Nquark=" << Nquark << G4endl;
  for (int i=0; i<Npart; i++) { o << *List[i] << G4endl; }
}

double Colour::field(const Vektor3& r) 
{
  Vektor3* d = NEW Vektor3[Nquark];
  for (int k=0; k<Nquark; k++) {
    d[k] = dr(r,List[k]->Coordinates());
    d[k] /= length(d[k]);
  }
  double s = 0.0;
  for (int i=0; i<Nquark; i++) {
    for (int j=i;j<Nquark; j++)
      if ( i == j )
	s-=d[i]*d[i];
      else
	s+=(d[i]*d[j])*2*Factor(0,i,j);
  }
  delete [] d;
  return kappa*sqrt(-s);
  
}

void Colour::writeField(G4std::ostream& o,double x0,double x1,double dx,double y0,double y1,double dy)
{
  for (int i=0; i<Nquark; i++) {
    o << List[i]->Color() << "  " << List[i]->Coordinates() << "  " 
      << List[i]->Force() << G4endl;
  }
}

void Colour::Correlation()
{
  /*
  Binning g(10,0,3,1);
  Binning F(10,0,2,1);
  for (int i=0; i<Nquark; i++) {
    Vektor3 f = List[i]->Force();
    F.AddEntry(length(f),1.0,1);
    for (int j=i+1; j<Npart; j++) {
      Vektor3 y = dr(List[i]->Coordinates(),List[j]->Coordinates());
      double dr = length(y);
      g.AddEntry(dr,1.0,1);
    }
  }
  */
  //  Output[1] << "# time = " << Time() << G4endl;
  //  Output[1] << g << G4endl;
  //  Output[2] << "# time = " << Time() << G4endl;
  //  Output[2] << F << G4endl;
}


void inTheBox::checkRange()
{
  for (int i=0; i<Npart; i++) {
    Vektor3 x = List[i]->Coordinates();
    if ( !G.isInside(x) ) {
      G.reflect(x);
      List[i]->SetCoordinates(x);
    }
  }
}

void Radiation::one_step()
{
  Nbody::one_step();
  bool decayOccured = false;
  if ( allowDecay ) {
    for ( int i=0; i<Nquark; i++ ) {
      List[i]->refresh();
      if ( allowDecay && decay ) {
	decayOccured = true;
	int n_try = MAX_TRIES;
	bool massTooHigh = false;
	  ParticleType& h = selectQuark();
	  if ( h.isDiquark() && List[i]->isDiquark() ) {
	    massTooHigh = true;
	    continue;
	  }
	  G4cerr << "PRODUCED --> " << h.Name() << G4endl;
	  f = List[i]->Force();
	  lf = length(f);
	  r_k = List[i]->Coordinates();
	  Vektor3 p_k = List[i]->Momentum();
	  double pkk = length(p_k);
	  kk = i;
	  QuantumState pp_old = List[i]->getProperties();
	  signed int c = (List[i]->isDiquark() || h.isDiquark()) ? -1 : +1;
	  signed int c1 = (h.isDiquark() ) ? -1 : +1;
	  QuantumState pp_alpha(h,QuantumProjections(c*sign(List[i]->B()),h.getColor(int(c1*List[i]->Color())),c*h.getIso3(),c*h.getSpin3()));
	  QuantumState pp_beta = anti(pp_alpha);
	  QuantumState pp_hadron = pp_beta + pp_old;
	  ParticleType& had1 = Knot<ParticleType>::FindKnot((ParticleType&)(QuantumNumbers&)pp_hadron); 
	  double lambda;
	  double eps = List[i]->Coordinates(3)*kappa + List[i]->E()-h.getMass();
	  do {
	  try {
	    ParticleType& had = had1.selectType(eps);
	    G4cerr << had1.Name() << "  " << had.Name() << "  " << eps << G4endl;
	    double m_had = had.getMass(eps);
	    Vektor4 x0(List[i]->Coordinates(),List[i]->Time());
	    Vektor4 p0(List[i]->Momentum(),List[i]->E());
	    Vektor4 x1,x2,p1,p2;
	    Kinematics(x0,x1,x2,p0,p1,p2,List[i]->Mass(),h.getMass(),m_had).calculate(kappa);
	    ParticleBase* p_hadron = makeParticle(had,pp_hadron,m_had);
	    ParticleBase* p_quark = makeParticle(h,pp_alpha);
	    p_quark->SetCoordinates4(x1);
	    p_quark->SetMomentum4(p1);
	    p_hadron->SetCoordinates4(x2);
	    p_hadron->SetMomentum4(p2);
	    delete List[i];
	    firstCall = true;
	    massTooHigh = false;
	  }
	  catch ( ... ) {
	    massTooHigh = true;
	    --n_try;
	    G4cerr << n_try << ": Mass to high. Trying again...!\n";
	  }
	}
	while ( n_try && massTooHigh );
	if ( !n_try ) {
	  G4cerr << "Mass Too High!!! Giving up...\n";
	}
	for ( int j=0; j<List.size(); j++ )
	  List[j]->reset();
      }
    }
  }
}

void Radiation::checkRange()
{
  /*
  for (G4std::vector<Particle*>::iterator X=List.begin(); X!=List.end(); X++) {
    if ( (*X)->Color() && (*X)->Coordinates(3)<0 ) {
      G4std::vector<Particle*>::iterator Y = X--;
      delete *Y;
      --Nquark;
    }
  }
  */
}

void Radiation::init(double dt)
{
  G4std::vector<Particle*>::iterator X=List.begin(); 
  do {
    Vektor4 x((*X)->Coordinates(),0);
    Vektor4 p((*X)->Momentum(),(*X)->E());
    if ( !G.moveToSurface(x,p) || x[0]>=dt ) {
      G4std::vector<Particle*>::iterator Y = X--;
      delete *Y;
      --Nquark;
    }
    else
      (*X)->SetCoordinates4(x);
    ++X;
  }
  while ( X!=List.end() );
}

void Radiation::FindCorrelations() 
{
  G4std::vector<int> erase;
  const int N = 2;
  for (int i=0; i<Nquark; i++) 
    if ( G4std::find(erase.begin(),erase.end(),i) == erase.end() ) {
      int* ind = new int[N];
      double* dist = new double[N];
      for (int k=0; k<N; k++) {
	dist[k] = 1e30;
	ind[k] = 0;
      }
      for (int j=0; j<Nquark; j++) {
        if ( i!=j && G4std::find(erase.begin(),erase.end(),j) == erase.end() ) {
          //       G4cerr << List[j]->Momentum() << "  " << List[j]->E() << G4endl;
          Vektor3 d = dr(List[i]->Coordinates(),List[j]->Coordinates()-List[j]->Momentum()/List[j]->E()*List[j]->Time());
          double ld = length(d);
          int k=N;
          while ( k>0 && ld < dist[k-1] ) --k;
          for (int l=N-1; l>k; l--) {
            dist[l] = dist[l-1];
            ind[l] = ind[l-1];
          }
          dist[k] = ld;
          ind[k] = j;
        }
      }
      RGB tot_color = List[i]->Color();
      try {
	int k=0;
	while ( !tot_color.isWhite() && k<N ) {
	  tot_color = tot_color + List[ind[k]]->Color();
	  ++k;
	}
	if ( k<N ) {
	  QuantumState pp = List[i]->getProperties();
	  Vektor3 P = List[i]->Momentum();
	  double E = List[i]->E();
	  for (int l=0; l<k; l++) {
	    pp = pp + List[ind[l]]->getProperties();
	    P = P + List[ind[l]]->Momentum();
	    E = E + List[ind[l]]->E();
	  }
	  double P_2 = P*P;
	  double s = E*E-P_2;
	  ParticleType& had = Knot<ParticleType>::FindKnot((ParticleType&)(QuantumNumbers&)pp).selectType(sqrt(s));
	  double M = had.getMass();
	  double Pnew_2 = s-M*M;
	  if ( Pnew_2 < 0 ) 
	    throw "not possible...";
	  G4cerr << had << G4endl;
	  G4cerr << "s,M = " << sqrt(s) << ", " << M << G4endl;
	  ParticleBase* created = makeParticle(had,pp,M);
	  created->SetMomentum(P*sqrt(Pnew_2/P_2));
	  created->SetCoordinates(List[i]->Coordinates());
	  erase.insert(erase.end(),i);
	  for (int l1=0; l1<k; l1++)
	    erase.insert(erase.end(),ind[l1]);
	  Nquark -= k+1;
	}
      }
      catch ( char* s ) {}
      delete [] ind;
      delete [] dist;
    }
  if ( erase.size() ) {
    G4std::sort(erase.begin(),erase.end(),G4std::greater<int>());
    G4cerr << "#  direct " << erase.size() << G4endl;
    for (int j=0; j<erase.size(); j++) {
      delete List[erase[j]];
    }
  }
}
