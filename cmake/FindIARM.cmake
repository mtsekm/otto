# - Try to find IARMBus and IRMgr
# Once done this will define:
#  IARMBUS_FOUND         - System has IARMBus
#  IARMBUS_INCLUDE_DIRS  - The IARMBus include directories
#  IARMBUS_LIBRARIES     - The libraries needed to use IARMBus
#  IRMGR_INCLUDE_DIRS    - The IRMgr include directories
#  IRMGR_HAL_LIBRARIES   - The libraries needed for IRMgr HAL

find_package(PkgConfig)

# Find the IARMBus library and include directories
find_library(IARMBUS_LIBRARIES NAMES IARMBus PATH_SUFFIXES lib)
find_path(IARMBUS_INCLUDE_DIRS NAMES libIARM.h PATH_SUFFIXES rdk/iarmbus)
find_path(IRMGR_INCLUDE_DIRS NAMES irMgr.h PATH_SUFFIXES rdk/iarmmgrs/ir)
find_path(IRMGR_INTERNAL_INCLUDE_DIRS NAMES irMgrInternal.h PATH_SUFFIXES rdk/iarmmgrs-hal)
find_library(IRMGR_HAL_LIBRARIES NAMES iarmmgrs-ir-hal PATH_SUFFIXES lib usr/lib)

# Set cache variables
set(IARMBUS_LIBRARIES ${IARMBUS_LIBRARIES} CACHE FILEPATH "Path to IARMBus library")
set(IARMBUS_INCLUDE_DIRS ${IARMBUS_INCLUDE_DIRS} CACHE PATH "Path to IARMBus include directories")
set(IRMGR_INCLUDE_DIRS ${IRMGR_INCLUDE_DIRS} CACHE PATH "Path to IRMgr include directories")
set(IRMGR_HAL_LIBRARIES ${IRMGR_HAL_LIBRARIES} CACHE FILEPATH "Path to IRMgr HAL library")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    IARMBUS
    DEFAULT_MSG
    IARMBUS_INCLUDE_DIRS IARMBUS_LIBRARIES
)

# Mark variables as advanced
mark_as_advanced(
    IARMBUS_FOUND
    IARMBUS_INCLUDE_DIRS
    IARMBUS_LIBRARIES
    IRMGR_INCLUDE_DIRS
    IRMGR_INTERNAL_INCLUDE_DIRS
    IRMGR_HAL_LIBRARIES
)
