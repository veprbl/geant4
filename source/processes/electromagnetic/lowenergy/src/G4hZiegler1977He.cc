//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4hZiegler1977He
//
// Author:        V.Ivanchenko (Vladimir.Ivanchenko@cern.ch)
// 
// Creation date: 20 July 2000
//
// Modifications: 
// 20/07/2000  V.Ivanchenko First implementation
// 10/05/2001  V.Ivanchenko Clean up againist Linux compilation with -Wall
// 26/08/2004  V.Ivanchenko Fix a problem of effective charge
//
// Class Description: 
//
// Electronic stopping power parametrised according to
// J.F.Ziegler, Helium Stopping Powers and
// Ranges in All Elemental Matter, Vol.4, Pergamon Press, 1977
//
// Class Description: End 
//
// -------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4hZiegler1977He.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"
#include "G4Material.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4hZiegler1977He::G4hZiegler1977He():G4VhElectronicStoppingPower(), 
  rateMass(4.0026/1.007276)
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4hZiegler1977He::~G4hZiegler1977He() 
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool G4hZiegler1977He::HasMaterial(const G4Material* material) 
{
  if(1 == (material->GetNumberOfElements())) return true;
  return false ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4hZiegler1977He::StoppingPower(const G4Material* material,
                                               G4double kineticEnergy) 
{
  G4double ionloss = 0.0 ;

  // pure material (normally not the case for this function)
  if(1 == (material->GetNumberOfElements())) {
    G4double z = material->GetZ() ;
    ionloss = ElectronicStoppingPower( z, kineticEnergy ) ;  

  }

  return ionloss ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4hZiegler1977He::ElectronicStoppingPower(G4double z,
                                          G4double kineticEnergy) const
{
  G4double ionloss ;
  G4int i = G4int(z)-1 ;  // index of atom
  if(i < 0)  i = 0 ;
  if(i > 91) i = 91 ;

  // The He4 data and the fit from: 
  // J.F.Ziegler, Helium Stopping Powers and
  // Ranges in All Elemental Matter, Vol.4, Pergamon Press, 1977
  
  G4double energy = kineticEnergy*rateMass/keV ;  // reduced He energy in keV
  
  static G4double a[92][9] = {
    //H-Solid 0.9661,0.4126,6.92,8.831,2.582,2.371,0.5462,-0.07932,-0.006853,
   {0.39,0.63,  4.17, 85.55,  19.55, 2.371, 0.5462,   -0.07932,-0.006853},
    //He-Solid 2.027,0.2931,26.34,6.66,0.3409,2.809,0.4847,-0.08756,-0.007281,
   {0.58, 0.59, 6.3, 130.0,   44.07, 2.809, 0.4847,   -0.08756,-0.007281},
   {1.42,  0.49,   12.25,32.,     9.161, 3.095, 0.4434,   -0.09259,-0.007459},
   {2.206, 0.51,   15.32, 0.25,   8.995, 3.28,  0.4188,   -0.09564,-0.007604},
   {3.691, 0.4128, 18.48,50.72,   9.,    3.426, 0.4,      -0.09796,-0.007715},
   {4.232, 0.3877, 22.99,35.,     7.993, 3.588, 0.3921,   -0.09935,-0.007804},
    //N-solid 2.51,0.4752,38.26,13.02,1.892,3.759,0.4094,-0.09646,-0.007661,
   {2.0,  0.548, 29.82,18.11,   4.37, 3.759, 0.4094,   -0.09646,-0.007661},
    //O-solid 1.766,0.5261,37.11,15.24,2.804,3.782,0.3734,-0.1011,-0.007874,
   {2.717, 0.4858, 32.88, 25.88, 4.336, 3.782, 0.3734,   -0.1011, -0.007874},
   {1.533, 0.531,  40.44,18.41,   2.718, 3.816, 0.3504,   -0.1046, -0.008074},
    //Ne-solid 1.183,0.55,39.83,17.49,4.001,3.863,0.3342,-0.1072,-0.008231,
   {2.303, 0.4861,   37.01, 37.96,   5.092, 3.863, 0.3342,-0.1072, -0.008231},
   {9.894, 0.3081, 23.65, 0.384, 92.93,  3.898, 0.3191,   -0.1086, -0.008271},
   {4.3,   0.47,   34.3,  3.3,   12.74,  3.961, 0.314,    -0.1091, -0.008297},
   {2.5,   0.625,  45.7,  0.1,    4.359, 4.024, 0.3113,   -0.1093, -0.008306},
   {2.1,   0.65,   49.34, 1.788,  4.133, 4.077, 0.3074,   -0.1089, -0.008219},
   {1.729, 0.6562, 53.41, 2.405,  3.845, 4.124, 0.3023,   -0.1094, -0.00824},
   {1.402, 0.6791, 58.98, 3.528,  3.211, 4.164, 0.2964,   -0.1101, -0.008267},
   {1.117, 0.7044, 69.69, 3.705,  2.156, 4.21,  0.2936,   -0.1103, -0.00827},
   {0.9172,0.724,  79.44, 3.648,  1.646, 4.261, 0.2994,   -0.1085, -0.008145},
   {8.554, 0.3817, 83.61,11.84,   1.875, 4.3,   0.2903,   -0.1103, -0.008259},
   {6.297, 0.4622, 65.39,10.14,   5.036, 4.334, 0.2897,   -0.1102, -0.008245},
   {5.307, 0.4918, 61.74,12.4,    6.665, 4.327, 0.2707,   -0.1127, -0.00837},
   {4.71,  0.5087, 65.28, 8.806,  5.948, 4.34,  0.2618,   -0.1138, -0.00842},
   {6.151, 0.4524, 83.,  18.31,   2.71,  4.361, 0.2559,   -0.1145, -0.008447},
   {6.57,  0.4322, 84.76,15.53,   2.779, 4.349, 0.24,     -0.1166, -0.00855},
   {5.738, 0.4492, 84.61,14.18,   3.101, 4.362, 0.2327,   -0.1174, -0.008588},
   {5.013, 0.4707, 85.58,16.55,   3.211, 4.375, 0.2253,   -0.1185, -0.008648},
   {4.32,  0.4947, 76.14,10.85,   5.441, 4.362, 0.2069,   -0.1214, -0.008815},
   {4.652, 0.4571, 80.73,22.,     4.952, 4.346, 0.1857,   -0.1249, -0.009021},
   {3.114, 0.5236, 76.67, 7.62,   6.385, 4.355, 0.18,     -0.1255, -0.009045},
   {3.114, 0.5236, 76.67, 7.62,   7.502, 4.389, 0.1806,   -0.1253, -0.009028},
   {3.114, 0.5236, 76.67, 7.62,   8.514, 4.407, 0.1759,   -0.1258, -0.009054},
   {5.746, 0.4662, 79.24, 1.185,  7.993, 4.419, 0.1694,   -0.1267, -0.009094},
   {2.792, 0.6346,106.1,  0.2986, 2.331, 4.412, 0.1545,   -0.1289, -0.009202},
   {4.667, 0.5095,124.3,  2.102,  1.667, 4.419, 0.1448,   -0.1303, -0.009269},
   {2.44,  0.6346,105.,   0.83,   2.851, 4.436, 0.1443,   -0.1299, -0.009229},
   {1.491, 0.7118,120.6,  1.101,  1.877, 4.478, 0.1608,   -0.1262, -0.008983},
   {11.72, 0.3826,102.8,  9.231,  4.371, 4.489, 0.1517,   -0.1278, -0.009078},
   {7.126, 0.4804,119.3,  5.784,  2.454, 4.514, 0.1551,   -0.1268, -0.009005},
   {11.61, 0.3955,146.7,  7.031,  1.423, 4.533, 0.1568,   -0.1261, -0.008945},
   {10.99, 0.41,  163.9,  7.1,    1.052, 4.548, 0.1572,   -0.1256, -0.008901},
   {9.241, 0.4275,163.1,  7.954,  1.102, 4.553, 0.1544,   -0.1255, -0.008883},
   {9.276, 0.418, 157.1,  8.038,  1.29,  4.548, 0.1485,   -0.1259, -0.008889},
   {3.999, 0.6152, 97.6,  1.297,  5.792, 4.489, 0.1128,   -0.1309, -0.009107},
   {4.306, 0.5658, 97.99, 5.514,  5.754, 4.402, 0.06656,  -0.1375, -0.009421},
   {3.615, 0.6197, 86.26, 0.333,  8.689, 4.292, 0.01012,  -0.1459, -0.009835},
   {5.8,   0.49,  147.2,  6.903,  1.289, 4.187,-0.04539,  -0.1542, -0.01025},
   {5.6,   0.49,  130.,  10.,     2.844, 4.577, 0.13,     -0.1285, -0.009067},
   {3.55,  0.6068,124.7,  1.112,  3.119, 4.583, 0.1253,   -0.1291, -0.009084},
   {3.6,   0.62,  105.8,  0.1692, 6.026, 4.58,  0.1174,   -0.1301, -0.009129},
   {5.4,   0.53,  103.1,  3.931,  7.767, 4.581, 0.111,    -0.1309, -0.009161},
   {3.97,  0.6459,131.8,  0.2233, 2.723, 4.582, 0.1046,   -0.1317, -0.009193},
   {3.65,  0.64,  126.8,  0.6834, 3.411, 4.6,   0.1052,   -0.1315, -0.009178},
   {3.118, 0.6519,164.9,  1.208,  1.51,  4.614, 0.1043,   -0.1315, -0.009175},
   {2.031, 0.7181,153.1,  1.362,  1.958, 4.619, 0.09769,  -0.1325, -0.009231},
   {14.4,  0.3923,152.5,  8.354,  2.597, 4.671, 0.1136,   -0.1298, -0.009078},
   {10.99, 0.4599,138.4,  4.811,  3.726, 4.706, 0.1206,   -0.1287, -0.009009},
   {16.6,  0.3773,224.1,  6.28,   0.9121,4.732, 0.1244,   -0.128,  -0.008968},
   {10.54, 0.4533,159.3,  4.832,  2.529, 4.722, 0.1156,   -0.1292, -0.00903},
   {10.33, 0.4502,162.,   5.132,  2.444, 4.71,  0.106,    -0.1305, -0.0091}, 
   {10.15, 0.4471,165.6,  5.378,  2.328, 4.698, 0.09647,  -0.1319, -0.009169},
   {9.976, 0.4439,168.,   5.721,  2.258, 4.681, 0.08536,  -0.1335, -0.009252},
   {9.804, 0.4408,176.2,  5.675,  1.997, 4.676, 0.07819,  -0.1345, -0.009302},
   {14.22, 0.363, 228.4,  7.024,  1.016, 4.663, 0.06867,  -0.1358, -0.009373},
   {9.952, 0.4318,233.5,  5.065,  0.9244,4.676, 0.06861,  -0.1357, -0.009363},
   {9.272, 0.4345,210.,   4.911,  1.258, 4.649, 0.05362,  -0.1379, -0.00948},
   {10.13, 0.4146,225.7,  5.525,  1.055, 4.634, 0.04335,  -0.1394, -0.009558},
   {8.949, 0.4304,213.3,  5.071,  1.221, 4.603, 0.02679,  -0.1418, -0.00969},
   {11.94, 0.3783,247.2,  6.655,  0.849, 4.584, 0.01494,  -0.1436, -0.009783},
   {8.472, 0.4405,195.5,  4.051,  1.604, 4.576, 0.007043, -0.1447, -0.009841},
   {8.301, 0.4399,203.7,  3.667,  1.459, 4.571, 0.0007046,-0.1456, -0.009886},
   {6.567, 0.4858,193.,   2.65,   1.66,  4.566,-0.005626, -0.1464, -0.00993},
   {5.951, 0.5016,196.1,  2.662,  1.589, 4.561,-0.01197,  -0.1473, -0.009975},
   {7.495, 0.4523,251.4,  3.433,  0.8619,4.572,-0.012,    -0.1472, -0.009965},
   {6.335, 0.4825,255.1,  2.834,  0.8228,4.569,-0.01755,  -0.148,  -0.01},   
   {4.314, 0.5558,214.8,  2.354,  1.263, 4.573,-0.01992,  -0.1482, -0.01001},
   {4.02,  0.5681,219.9,  2.402,  1.191, 4.57, -0.02547,  -0.149,  -0.01005},
   {3.836, 0.5765,210.2,  2.742,  1.305, 4.528,-0.04613,  -0.1521, -0.01022},
   {4.68,  0.5247,244.7,  2.749,  0.8962,4.494,-0.0637,   -0.1548, -0.01037},
   {3.223, 0.5883,232.7,  2.954,  1.05,  4.564,-0.027,    -0.1471, -0.009852},
   {2.892, 0.6204,208.6,  2.415,  1.416, 4.546,-0.04963,  -0.1523, -0.01022},
   {4.728, 0.5522,217.,   3.091,  1.386, 4.594,-0.03339,  -0.1496, -0.01006},
   {6.18,  0.52,  170.,   4.,     3.224, 4.608,-0.02886,  -0.1485, -0.00999},
   {9.,    0.47,  198.,   3.8,    2.032, 4.624,-0.02639,  -0.1481, -0.009971},
   {2.324, 0.6997,216.,   1.599,  1.399, 4.636,-0.02422,  -0.1477, -0.009939},
   {1.961, 0.7286,223.,   1.621,  1.296, 4.648,-0.02172,  -0.1471, -0.009903},
   {1.75,  0.7427,350.1,  0.9789, 0.5507,4.662,-0.1192,   -0.1752, -0.01196},
   {10.31, 0.4613,261.2,  4.738,  0.9899,4.69, -0.009867, -0.1449, -0.009771},
   {7.962, 0.519, 235.7,  4.347,  1.313, 4.715,-0.002113, -0.1435, -0.009689},
   {6.227, 0.5645,231.9,  3.961,  1.379, 4.729, 0.001392, -0.1428, -0.009644},
   {5.246, 0.5947,228.6,  4.027,  1.423, 4.729,-0.0005983,-0.143,  -0.009647},
   {5.408, 0.5811,235.7,  3.961,  1.358, 4.738, 0.001075, -0.1425, -0.009618},
   {5.218, 0.5828,245.,   3.838,  1.25,  4.751, 0.004244, -0.1419, -0.009576}
  };

  if ( energy < 1.0 ) {
    G4double slow  = a[i][0] ;
    G4double shigh = log( 1.0 + a[i][3]*1000.0 + a[i][4]*0.001 ) 
      * a[i][2]*1000.0 ;
    ionloss = slow*shigh / (slow + shigh) ; 
    ionloss *= sqrt(energy) ; 
    
  } else if ( energy < 10000.0 ) {
    G4double slow  = a[i][0] * pow(energy, a[i][1]) ;
    G4double e = energy/1000.0 ;
    G4double shigh = log( 1.0 + a[i][3]/e + a[i][4]*e ) * a[i][2]/e ;
    ionloss = slow*shigh / (slow + shigh) ; 
    
  } else {
    G4double le = log(1000.0/energy) ;
    ionloss = exp( a[i][5] + a[i][6]*le + a[i][7]*le*le + a[i][8]*le*le*le) ;
  }

  if ( ionloss < 0.0) ionloss = 0.0 ;

  ionloss /= HeEffChargeSquare(z, kineticEnergy*rateMass); 
  
  return ionloss;
}

