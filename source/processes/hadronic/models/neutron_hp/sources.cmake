#-----------------------------------------------------------------------
# sources.cmake
# Module : G4had_neu_hp
# Package: Geant4.src.G4processes.G4hadronic.G4hadronic_models.G4had_neu_hp
#
# Sources description for a library.
# Lists the sources and headers of the code explicitely.
# Lists include paths needed.
# Lists the internal granular and global dependencies of the library.
# Source specific properties should be added at the end.
#
# Generated on : 24/9/2010
#
# $Id$
#
#-----------------------------------------------------------------------

# List external includes needed.
include_directories(${CLHEP_INCLUDE_DIRS})
include_directories(${ZLIB_INCLUDE_DIRS})

# List internal includes needed.
include_directories(${CMAKE_SOURCE_DIR}/source/geometry/management/include)
include_directories(${CMAKE_SOURCE_DIR}/source/geometry/volumes/include)
include_directories(${CMAKE_SOURCE_DIR}/source/global/HEPGeometry/include)
include_directories(${CMAKE_SOURCE_DIR}/source/global/HEPRandom/include)
include_directories(${CMAKE_SOURCE_DIR}/source/global/management/include)
include_directories(${CMAKE_SOURCE_DIR}/source/materials/include)
include_directories(${CMAKE_SOURCE_DIR}/source/particles/bosons/include)
include_directories(${CMAKE_SOURCE_DIR}/source/particles/hadrons/barions/include)
include_directories(${CMAKE_SOURCE_DIR}/source/particles/hadrons/ions/include)
include_directories(${CMAKE_SOURCE_DIR}/source/particles/hadrons/mesons/include)
include_directories(${CMAKE_SOURCE_DIR}/source/particles/leptons/include)
include_directories(${CMAKE_SOURCE_DIR}/source/particles/management/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/cross_sections/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/management/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/de_excitation/management/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/de_excitation/photon_evaporation/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/de_excitation/util/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/low_energy/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/management/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/models/util/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/processes/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/hadronic/util/include)
include_directories(${CMAKE_SOURCE_DIR}/source/processes/management/include)
include_directories(${CMAKE_SOURCE_DIR}/source/track/include)

#
# Define the Geant4 Module.
#
include(Geant4MacroDefineModule)
GEANT4_DEFINE_MODULE(NAME G4had_neu_hp
  HEADERS
    G4NeutronHPList.hh
    G4InterpolationIterator.hh
    G4NeutronHPIsoData.hh
    G4InterpolationManager.hh
    G4NeutronHPLevel.hh
    G4InterpolationScheme.hh
    G4NeutronHP2AInelasticFS.hh
    G4NeutronHPNames.hh
    G4NeutronHP2N2AInelasticFS.hh
    G4NeutronHPPartial.hh
    G4NeutronHP2NAInelasticFS.hh
    G4NeutronHPProduct.hh
    G4NeutronHP2NDInelasticFS.hh
    G4NeutronHPVector.hh
    G4NeutronHP2NInelasticFS.hh
    G4VNeutronHPEDis.hh
    G4NeutronHP2NPInelasticFS.hh
    G4NeutronHP2PInelasticFS.hh
    G4NeutronHP3AInelasticFS.hh
    G4NeutronHP3NAInelasticFS.hh
    G4NeutronHP3NInelasticFS.hh
    G4NeutronHP3NPInelasticFS.hh
    G4NeutronHP4NInelasticFS.hh
    G4NeutronHPAInelasticFS.hh
    G4NeutronHPAngular.hh
    G4NeutronHPAngularP.hh
    G4NeutronHPArbitaryTab.hh
    G4NeutronHPCapture.hh
    G4NeutronHPCaptureData.hh
    G4NeutronHPCaptureFS.hh
    G4NeutronHPChannel.hh
    G4NeutronHPChannelList.hh
    G4NeutronHPContAngularPar.hh
    G4NeutronHPContEnergyAngular.hh
    G4NeutronHPD2AInelasticFS.hh
    G4NeutronHPDAInelasticFS.hh
    G4NeutronHPDInelasticFS.hh
    G4NeutronHPData.hh
    G4NeutronHPDataPoint.hh
    G4NeutronHPDataUsed.hh
    G4NeutronHPDeExGammas.hh
    G4NeutronHPDiscreteTwoBody.hh
    G4NeutronHPElastic.hh
    G4NeutronHPElasticData.hh
    G4NeutronHPElasticFS.hh
    G4NeutronHPElementData.hh
    G4NeutronHPEnAngCorrelation.hh
    G4NeutronHPEnergyDistribution.hh
    G4NeutronHPEvapSpectrum.hh
    G4NeutronHPFCFissionFS.hh
    G4NeutronHPFFFissionFS.hh
    G4NeutronHPFSFissionFS.hh
    G4NeutronHPFastLegendre.hh
    G4NeutronHPField.hh
    G4NeutronHPFieldPoint.hh
    G4NeutronHPFinalState.hh
    G4NeutronHPFission.hh
    G4NeutronHPFissionBaseFS.hh
    G4NeutronHPFissionData.hh
    G4NeutronHPFissionERelease.hh
    G4NeutronHPFissionFS.hh
    G4NeutronHPFissionSpectrum.hh
    G4NeutronHPGamma.hh
    G4NeutronHPHash.hh
    G4NeutronHPHe3InelasticFS.hh
    G4NeutronHPInelastic.hh
    G4NeutronHPInelasticBaseFS.hh
    G4NeutronHPInelasticCompFS.hh
    G4NeutronHPInelasticData.hh
    G4NeutronHPInterpolator.hh
    G4NeutronHPIsotropic.hh
    G4NeutronHPJENDLHEData.hh
    G4NeutronHPJENDLHEElasticData.hh
    G4NeutronHPJENDLHEInelasticData.hh
    G4NeutronHPKallbachMannSyst.hh
    G4NeutronHPLCFissionFS.hh
    G4NeutronHPLabAngularEnergy.hh
    G4NeutronHPLegendreStore.hh
    G4NeutronHPLegendreTable.hh
    G4NeutronHPMadlandNixSpectrum.hh
    G4NeutronHPN2AInelasticFS.hh
    G4NeutronHPN2PInelasticFS.hh
    G4NeutronHPN3AInelasticFS.hh
    G4NeutronHPNAInelasticFS.hh
    G4NeutronHPNBodyPhaseSpace.hh
    G4NeutronHPND2AInelasticFS.hh
    G4NeutronHPNDInelasticFS.hh
    G4NeutronHPNHe3InelasticFS.hh
    G4NeutronHPNInelasticFS.hh
    G4NeutronHPNPAInelasticFS.hh
    G4NeutronHPNPInelasticFS.hh
    G4NeutronHPNT2AInelasticFS.hh
    G4NeutronHPNTInelasticFS.hh
    G4NeutronHPNXInelasticFS.hh
    G4NeutronHPNeutronYield.hh
    G4NeutronHPPAInelasticFS.hh
    G4NeutronHPPDInelasticFS.hh
    G4NeutronHPPInelasticFS.hh
    G4NeutronHPPTInelasticFS.hh
    G4NeutronHPPhotonDist.hh
    G4NeutronHPPhotonXSection.hh
    G4NeutronHPPolynomExpansion.hh
    G4NeutronHPSCFissionFS.hh
    G4NeutronHPSimpleEvapSpectrum.hh
    G4NeutronHPT2AInelasticFS.hh
    G4NeutronHPTCFissionFS.hh
    G4NeutronHPTInelasticFS.hh
    G4NeutronHPThermalBoost.hh
    G4NeutronHPThermalScattering.hh
    G4NeutronHPThermalScatteringData.hh
    G4NeutronHPThermalScatteringNames.hh
    G4NeutronHPWattSpectrum.hh
    G4VNeutronHPEnergyAngular.hh
    G4NeutronHPBGGNucleonInelasticXS.hh
    G4NeutronHPManager.hh
    G4NeutronHPReactionWhiteBoard.hh
  SOURCES
    G4NeutronHPIsoData.cc
    G4InterpolationManager.cc
    G4NeutronHPLevel.cc
    G4NeutronHP2AInelasticFS.cc
    G4NeutronHPList.cc
    G4NeutronHP2N2AInelasticFS.cc
    G4NeutronHPPartial.cc
    G4NeutronHP2NAInelasticFS.cc
    G4NeutronHPNames.cc
    G4NeutronHP2NDInelasticFS.cc
    G4NeutronHP2NInelasticFS.cc
    G4NeutronHPProduct.cc
    G4NeutronHP2NPInelasticFS.cc
    G4NeutronHPVector.cc
    G4NeutronHP2PInelasticFS.cc
    G4NeutronHP3AInelasticFS.cc
    G4NeutronHP3NAInelasticFS.cc
    G4NeutronHP3NInelasticFS.cc
    G4NeutronHP3NPInelasticFS.cc
    G4NeutronHP4NInelasticFS.cc
    G4NeutronHPAInelasticFS.cc
    G4NeutronHPAngular.cc
    G4NeutronHPArbitaryTab.cc
    G4NeutronHPCapture.cc
    G4NeutronHPCaptureData.cc
    G4NeutronHPCaptureFS.cc
    G4NeutronHPChannel.cc
    G4NeutronHPChannelList.cc
    G4NeutronHPContAngularPar.cc
    G4NeutronHPContEnergyAngular.cc
    G4NeutronHPD2AInelasticFS.cc
    G4NeutronHPDAInelasticFS.cc
    G4NeutronHPDInelasticFS.cc
    G4NeutronHPData.cc
    G4NeutronHPDeExGammas.cc
    G4NeutronHPDiscreteTwoBody.cc
    G4NeutronHPElastic.cc
    G4NeutronHPElasticData.cc
    G4NeutronHPElasticFS.cc
    G4NeutronHPElementData.cc
    G4NeutronHPEnAngCorrelation.cc
    G4NeutronHPFCFissionFS.cc
    G4NeutronHPFFFissionFS.cc
    G4NeutronHPFSFissionFS.cc
    G4NeutronHPFastLegendre.cc
    G4NeutronHPFastLegendre_14.cc
    G4NeutronHPFastLegendre_18.cc
    G4NeutronHPFastLegendre_21.cc
    G4NeutronHPFastLegendre_24.cc
    G4NeutronHPFastLegendre_26.cc
    G4NeutronHPFastLegendre_28.cc
    G4NeutronHPFastLegendre_30.cc
    G4NeutronHPField.cc
    G4NeutronHPFieldPoint.cc
    G4NeutronHPFinalState.cc
    G4NeutronHPFission.cc
    G4NeutronHPFissionBaseFS.cc
    G4NeutronHPFissionData.cc
    G4NeutronHPFissionFS.cc
    G4NeutronHPGamma.cc
    G4NeutronHPHe3InelasticFS.cc
    G4NeutronHPInelastic.cc
    G4NeutronHPInelasticBaseFS.cc
    G4NeutronHPInelasticCompFS.cc
    G4NeutronHPInelasticData.cc
    G4NeutronHPInterpolator.cc
    G4NeutronHPIsotropic.cc
    G4NeutronHPJENDLHEData.cc
    G4NeutronHPJENDLHEElasticData.cc
    G4NeutronHPJENDLHEInelasticData.cc
    G4NeutronHPKallbachMannSyst.cc
    G4NeutronHPLCFissionFS.cc
    G4NeutronHPLabAngularEnergy.cc
    G4NeutronHPLegendreStore.cc
    G4NeutronHPMadlandNixSpectrum.cc
    G4NeutronHPN2AInelasticFS.cc
    G4NeutronHPN2PInelasticFS.cc
    G4NeutronHPN3AInelasticFS.cc
    G4NeutronHPNAInelasticFS.cc
    G4NeutronHPNBodyPhaseSpace.cc
    G4NeutronHPND2AInelasticFS.cc
    G4NeutronHPNDInelasticFS.cc
    G4NeutronHPNHe3InelasticFS.cc
    G4NeutronHPNInelasticFS.cc
    G4NeutronHPNPAInelasticFS.cc
    G4NeutronHPNPInelasticFS.cc
    G4NeutronHPNT2AInelasticFS.cc
    G4NeutronHPNTInelasticFS.cc
    G4NeutronHPNXInelasticFS.cc
    G4NeutronHPPAInelasticFS.cc
    G4NeutronHPPDInelasticFS.cc
    G4NeutronHPPInelasticFS.cc
    G4NeutronHPPTInelasticFS.cc
    G4NeutronHPPhotonDist.cc
    G4NeutronHPSCFissionFS.cc
    G4NeutronHPT2AInelasticFS.cc
    G4NeutronHPTCFissionFS.cc
    G4NeutronHPTInelasticFS.cc
    G4NeutronHPThermalScattering.cc
    G4NeutronHPThermalScatteringData.cc
    G4NeutronHPThermalScatteringNames.cc
    G4NeutronHPWattSpectrum.cc
    G4NeutronHPBGGNucleonInelasticXS.cc
    G4NeutronHPManager.cc
    G4NeutronHPReasctionWhiteBoard.cc
  GRANULAR_DEPENDENCIES
    G4baryons
    G4bosons
    G4geometrymng
    G4globman
    G4had_mod_man
    G4had_mod_util
    G4hadronic_LE
    G4hadronic_deex_management
    G4hadronic_deex_photon_evaporation
    G4hadronic_deex_util
    G4hadronic_mgt
    G4hadronic_proc
    G4hadronic_util
    G4hadronic_xsect
    G4ions
    G4leptons
    G4materials
    G4mesons
    G4partman
    G4procman
    G4track
    G4volumes
  GLOBAL_DEPENDENCIES
    G4geometry
    G4global
    G4materials
    G4particles
    G4track
  LINK_LIBRARIES
    ${ZLIB_LIBRARIES}
  )

# List any source specific properties here

