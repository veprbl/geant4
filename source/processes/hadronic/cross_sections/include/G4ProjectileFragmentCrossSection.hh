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
#ifndef G4ProjectileFragmentCrossSection_h
#define G4ProjectileFragmentCrossSection_h 1

#include <cmath>
#include <iostream>

// Implements Physical Review C61, 034607 (2000)
// Rewrite starting from EPAX Version 2
 
class G4ProjectileFragmentCrossSection
{  
  public:
  G4ProjectileFragmentCrossSection()
  {
        p_S[1] = -2.38;                  // scale factor for xsect in barn    
        p_S[2] = 0.27;         

        p_P[1] = -2.5840E+00;            // slope of mass yield curve         
        p_P[2] = -7.5700E-03;

        p_Delta[1] = -1.0870E+00;        // centroid rel. to beta-stability
        p_Delta[2] = +3.0470E-02;
        p_Delta[3] = +2.1353E-04; 
        p_Delta[4] = +7.1350E+01; 

        p_R[1] = +0.885E+00;             // width parameter R
        p_R[2] = -9.8160E-03;

        p_Un[1] = 1.65;                  // slope par. n-rich ride of Z distr.

        p_Up[1] = 1.7880;                // slope par. p-rich ride of Z distr.  
        p_Up[2] = +4.7210E-03;   
        p_Up[3] = -1.3030E-05;   

        p_mn[1]  = 0.400;                // memory effect n-rich projectiles
        p_mn[2]  = 0.600;        

        p_mp[1] = -10.25;                // memory effect p-rich projectiles
        p_mp[2] = +10.1; 

        corr_d[1] = -25.0;       // correction close to proj.: centroid dzp
        corr_d[2] = 0.800;       
        corr_r[1] = +20.0;       // correction close to proj.: width R
        corr_r[2] = 0.820;
        corr_y[1] = 200.0;       // correction close to proj.: Yield_a
        corr_y[2] = 0.90;        
  }
  
  inline G4double doit(G4double Ap, G4double Zp, G4double At, G4double Zt, G4double A, G4double Z)
  {
//  calculate mass yield
        G4double Ap13 = pow(Ap, 1./3.);
        G4double At13 = pow(At, 1./3.);
        G4double S = p_S[2] * (At13 + Ap13 + p_S[1]);
//  cout << "debug0 "<<S<<" "<<At13<<" "<<Ap13<<" "<<p_S[1]<<" "<<p_S[2]<<endl;
        G4double p    = exp(p_P[2]*Ap + p_P[1]);
        G4double yield_a = p * S * exp(-p * (Ap - A));
  cout << "debug1 "<<yield_a<<endl;
//   modification close to projectile
        G4double f_mod_y=1.0;
        if (A/Ap > corr_y[2])
	{
          f_mod_y=corr_y[1]*pow(A/Ap-corr_y[2], 2) + 1.0;
        }
        yield_a= yield_a * f_mod_y;
  cout << "debug1 "<<yield_a<<endl;

//   calculate maximum of charge dispersion zprob
        G4double zbeta = A/(1.98+0.0155*pow(A, (2./3.)));
        G4double zbeta_p = Ap/(1.98+0.0155*pow(Ap, (2./3.)));
        G4double delta;
	if(A > p_Delta[4]) 
	{
          delta = p_Delta[1] + p_Delta[2]*A;
        }
	else
	{
          delta = p_Delta[3]*A*A;
        }

//   modification close to projectile
        G4double f_mod=1.0;
        if(A/Ap > corr_d[2]) 
	{
          f_mod = corr_d[1]*pow(A/Ap-corr_d[2], 2) + 1.0;
        }
        delta = delta*f_mod;
        G4double zprob = zbeta+delta;

//   correction for proton- and neutron-rich projectiles
        G4double  dq;
	if((Zp-zbeta_p)>0) 
	{
          dq = exp(p_mp[1] + G4double(A)/G4double(Ap)*p_mp[2]);
	  cout << "dq "<<A<<" "<<Ap<<" "<<p_mp[1]
	  <<" "<<p_mp[2]<<" "<<dq<<" "<<p_mp[1] + A/Ap*p_mp[2]<<endl;
	}
        else                       
        {
	  dq = p_mn[1]*pow(A/Ap, 2.0) + p_mn[2]*pow(A/Ap, 4.0);
        }
        zprob = zprob + dq * (Zp-zbeta_p);

//   small corr. since Xe-129 and Pb-208 are not on Z_beta line
        zprob = zprob + 0.0020*A;
	cout <<"zprob "<<A<<" "<<dq<<" "<<Zp<<" "<<zbeta_p
	     <<" "<<zbeta<<" "<<delta<<endl;

//  calculate width parameter R
        G4double r = exp(p_R[1] + p_R[2]*A);

//  modification close to projectile
        f_mod=1.0;
        if (A/Ap > corr_r[2]) 
	{
          f_mod = corr_r[1]*Ap*pow(A/Ap-corr_r[2], 4.0)+1.0;
        }
        r = r*f_mod;

//   change width according to dev. from beta-stability
        if ((Zp-zbeta_p) < 0.0) 
	{ 
          r=r*(1.0-0.0833*abs(Zp-zbeta_p));
        }

//   calculate slope parameters u_n, u_p
        G4double u_n = p_Un[1];
        G4double u_p = p_Up[1] + p_Up[2]*A + p_Up[3]*A*A;

//   calculate charge dispersion
        G4double expo, fract;
	if((zprob-Z) > 0) 
	{
//     neutron-rich
          expo = -r*pow(abs(zprob-Z), u_n);
          fract   =  exp(expo)*sqrt(r/3.14159);
        }
	else
	{
//     proton-rich
          expo = -r*pow(abs(zprob-Z), u_p);
          fract   =  exp(expo)*sqrt(r/3.14159);
 cout << "1 "<<expo<<" "<<r<<" "<<zprob<<" "<<Z<<" "<<u_p<<endl;
//     go to exponential slope
          G4double dfdz = 1.2 + 0.647*pow(A/2.,0.3);
          G4double z_exp = zprob + dfdz * log(10.) / (2.*r);
          if( Z>z_exp ) 
	  {
            expo = -r*pow(abs(zprob-z_exp), u_p);
            fract   =  exp(expo)*sqrt(r/3.14159)
                      / pow(pow(10, dfdz), Z-z_exp);
          }
        }

        cout << "debug "<<fract<<" "<<yield_a<<endl;
	G4double epaxv2=fract*yield_a;
	return epaxv2;
  }
  
  void testMe()
  {
    G4ProjectileFragmentCrossSection i;
    cout << i.doit(58, 28, 9, 4, 49, 28) << endl;
 // Sigma = 9.800163E-13 b
  }
 private:
  G4double p_S[3];
  G4double p_P[3];
  G4double p_Delta[5];
  G4double p_R[3];
  G4double p_Un[2];
  G4double p_Up[4];
  G4double p_mn[3];
  G4double p_mp[3];
  G4double corr_d[3];
  G4double corr_r[3];
  G4double corr_y[3];
};
#endif
