# $Id: bench02.in.mod,v 1.1 2002-05-21 11:50:29 stesting Exp $
#
# Macro file for "TestEm2.cc" 
# (can be run in batch, without graphic)
#
# H2O L = 9.97 radl  R = 0.665 radl; electron 1 GeV
#
#  Experimental results: Electron-induced cascade showers
#                        J&H Crannel - Phys. Rev. 184-2 - August69
#
/control/verbose 2
#
/run/particle/setCut 5.0 mm
/run/initialize
#
/calor/setMat Water
/calor/setLbin 9 1.11
/calor/setRbin 6 0.111
/calor/update
#
/run/verbose 1
/event/printModulo 10000
#
/gun/particle e-
/gun/energy 1 GeV
/run/beamOn 1000


