file(GLOB SOURCE_FILES src/*.cpp)

if(WIN32)
    add_definitions("-DLEPTON_BUILDING_SHARED_LIBRARY")
endif(WIN32)

OpenSimAddLibrary(VENDORLIB LOWERINCLUDEDIRNAME
    KIT Lepton
    AUTHORS "Peter_Eastman"
    LINKLIBS ""
    INCLUDES "include/Lepton.h"
    SOURCES ${SOURCE_FILES}
    TESTDIRS test
    INCLUDEDIRS include include/lepton
    INCLUDEINSTALLREL include/lepton
    )

# TODO change header installation location to CMAKE_INSTALL_INCLUDEDIR/OpenSim/lepton
