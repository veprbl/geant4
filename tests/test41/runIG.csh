#/bin/csh

#cd $VFEM/test41
mkdir -p $REFERENCE
cd $REFERENCE
rm -f *.*

set    work = "$G4MY/test41"
set    dir  = "$G4INSTALL/tests/test41/"

setenv PHYSLIST    standardIG
set    phys = "optG"
source ${dir}run_single.csh ${phys} ${work} ${dir} >& resG.out

ln -s $VFEM/test41/geant4-09-02-ref-00/*S.* ./
ln -s $VFEM/test41/geant4-09-02-ref-00/*0.* ./
ln -s $VFEM/test41/geant4-09-02-ref-00/*1.* ./
ln -s $VFEM/test41/geant4-09-01-ref-00/*3.* ./

source ${dir}plot.csh $1 >& p.out
