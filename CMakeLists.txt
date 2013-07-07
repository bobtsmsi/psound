CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)




PROJECT(pSound)



SET(LOCAL_PROJECT_NAME pSound)



SET(PSOUND_SOVERSION 6)
SET(PSOUND_MAJOR_VERSION 0)
SET(PSOUND_MINOR_VERSION 5)
SET(PSOUND_PATCH_VERSION 3)
SET(PSOUND_VERSION ${PSOUND_MAJOR_VERSION}.${PSOUND_MINOR_VERSION}.${PSOUND_PATCH_VERSION})



IF(NOT WIN32)
    IF(CMAKE_SIZEOF_VOID_P MATCHES "8")
        SET(LIB_POSTFIX "64" CACHE STRING "suffix for 32/64 dir placement")
        MARK_AS_ADVANCED(LIB_POSTFIX)
    ENDIF()

    SET(PKGCONFIG_FILES
        pSound
    )

    FOREACH(PKGCONFIG_FILE ${PKGCONFIG_FILES})
        CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/packaging/pkgconfig/${PKGCONFIG_FILE}.pc.in
        ${PROJECT_BINARY_DIR}/packaging/pkgconfig/${PKGCONFIG_FILE}.pc
        @ONLY
        )
    INSTALL(FILES ${PROJECT_BINARY_DIR}/packaging/pkgconfig/${PKGCONFIG_FILE}.pc DESTINATION lib${LIB_POSTFIX}/pkgconfig)
    ENDFOREACH(PKGCONFIG_FILE)
ENDIF(NOT WIN32)






CONFIGURE_FILE("${PROJECT_SOURCE_DIR}/src/pSound/Version.cpp.in" "${PROJECT_SOURCE_DIR}/src/pSound/Version.cpp")







OPTION(PSOUND_BUILD_PLATFORM_ANDROID "" OFF)
IF(PSOUND_BUILD_PLATFORM_ANDROID)

    SET(ANDROID TRUE)
    SET(ANDROID_PLATFORM 5)

    SET(PSOUND_ANDROID_TEMPLATES "${CMAKE_SOURCE_DIR}/PlatformSpecifics/Android")

ENDIF(PSOUND_BUILD_PLATFORM_ANDROID)









INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)



IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF(NOT CMAKE_BUILD_TYPE)









# OPTION(PSOUND_BUILD_DEBUG "" OFF)
SET(PSOUND_BUILD_DEBUG OFF)

IF(CMAKE_BUILD_TYPE MATCHES "Debug")
    SET(PSOUND_BUILD_DEBUG ON)
ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")




IF( WIN32 )
    ADD_DEFINITIONS(-W4 -DOSG_LIBRARY)
ELSE( WIN32 )
    ADD_DEFINITIONS(-Wall -DOSG_LIBRARY -fPIC)
ENDIF( WIN32 )





IF(PSOUND_BUILD_DEBUG)
    MESSAGE(STATUS "pSound build: DEBUG")

ELSE(PSOUND_BUILD_DEBUG)
    MESSAGE(STATUS "pSound build: RELEASE")

ENDIF(PSOUND_BUILD_DEBUG)





IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )

    ADD_DEFINITIONS(-DIS_X86_64=1)

ELSE( CMAKE_SIZEOF_VOID_P EQUAL 8 )

    ADD_DEFINITIONS(-DIS_X86_64=0)

ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )






IF( WIN32 )
    SET(OSG_INCLUDE "$ENV{PROGRAMFILES}\\OpenSceneGraph\\include" CACHE PATH "OSG headers")
    SET(OPENAL_INCLUDE "$ENV{PROGRAMFILES}\\OpenAL 1.1 SDK\\include" CACHE PATH "OpenAL headers")
    SET(OGGVORBIS_INCLUDE "$ENV{PROGRAMFILES}\\OggVorbis\\include" CACHE PATH "Ogg Vorbis headers")

ELSE( WIN32 )

    IF( NOT ANDROID )

        EXECUTE_PROCESS(COMMAND pkg-config openscenegraph-osg --cflags-only-I OUTPUT_VARIABLE tmp_var)
        STRING(REGEX REPLACE "\n" "" tmp_var "${tmp_var}")
        STRING(REGEX REPLACE "-I" "" tmp_var "${tmp_var}")

        SET(OSG_INCLUDE ${tmp_var} CACHE PATH "OSG Headers")



        EXECUTE_PROCESS(COMMAND pkg-config openal --cflags-only-I OUTPUT_VARIABLE tmp_var)
        STRING(REGEX REPLACE "\n" "" tmp_var "${tmp_var}")
        STRING(REGEX REPLACE "-I" "" tmp_var "${tmp_var}")

        SET(OPENAL_INCLUDE ${tmp_var} CACHE PATH "OpenAL Headers")

    ENDIF( NOT ANDROID )

ENDIF( WIN32 )


MESSAGE(STATUS "OSG include: ${OSG_INCLUDE}")
MESSAGE(STATUS "OpenaL include: ${OPENAL_INCLUDE}")





IF( WIN32 )
    SET(OSG_LIBRARIES "$ENV{PROGRAMFILES}\\OpenSceneGraph\\lib" CACHE PATH "OSG libraries")
    SET(OPENAL_LIBRARIES "$ENV{PROGRAMFILES}\\OpenAL 1.1 SDK\\libs\\Win32" CACHE PATH "OpenAL libraries")
    SET(OGGVORBIS_LIBRARIES "$ENV{PROGRAMFILES}\\OggVorbis\\lib" CACHE PATH "Ogg Vorbis libraries")

ELSE( WIN32 )

    IF( NOT ANDROID )

        EXECUTE_PROCESS(COMMAND pkg-config openscenegraph-osg --libs-only-L OUTPUT_VARIABLE tmp_var)
        STRING(REGEX REPLACE "\n" "" tmp_var "${tmp_var}")
        STRING(REGEX REPLACE "-L" "" tmp_var "${tmp_var}")

        SET(OSG_LIBRARIES ${tmp_var} CACHE PATH "OSG libraries")



        EXECUTE_PROCESS(COMMAND pkg-config openal --libs-only-L OUTPUT_VARIABLE tmp_var)
        STRING(REGEX REPLACE "\n" "" tmp_var "${tmp_var}")
        STRING(REGEX REPLACE "-L" "" tmp_var "${tmp_var}")

        SET(OPENAL_LIBRARIES ${tmp_var} CACHE PATH "OpenAL libraries")

    ENDIF( NOT ANDROID )

ENDIF( WIN32 )


MESSAGE(STATUS "OSG libraries: ${OSG_LIBRARIES}")
MESSAGE(STATUS "OpenAL libraries: ${OPENAL_LIBRARIES}")





IF( WIN32 )
    SET(OSG_BINARIES "$ENV{PROGRAMFILES}\\OpenSceneGraph\\bin" CACHE PATH "OSG binaries")
    MESSAGE(STATUS "OSG binaries: ${OSG_BINARIES}")
ENDIF( WIN32 )




IF(NOT ANDROID)
    INCLUDE_DIRECTORIES(${OSG_INCLUDE})
    INCLUDE_DIRECTORIES(${OPENAL_INCLUDE})

    LINK_DIRECTORIES( ${OSG_LIBRARIES} )
    LINK_DIRECTORIES( ${OPENAL_LIBRARIES} )

ENDIF(NOT ANDROID)




IF( WIN32 )
    OPTION(PSOUND_BUILD_OGG_PLUGIN "" OFF)

    IF(PSOUND_BUILD_OGG_PLUGIN)
        INCLUDE_DIRECTORIES(${OGGVORBIS_INCLUDE})
        LINK_DIRECTORIES(${OGGVORBIS_LIBRARIES})
    ENDIF(PSOUND_BUILD_OGG_PLUGIN)
ENDIF( WIN32 )





CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")





SET(PSOUND_DOXYGEN_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/doc" CACHE PATH "Doxygen output directory")



SET(PSOUND_TEMP_DOXYGEN_INPUT "${PROJECT_BINARY_DIR}/doc/psound_doxygen_input_files.txt")
MARK_AS_ADVANCED(PSOUND_TEMP_DOXYGEN_INPUT)

FILE(WRITE ${PSOUND_TEMP_DOXYGEN_INPUT} "${PROJECT_SOURCE_DIR}/README\n")


FIND_PACKAGE(Doxygen)

IF(DOXYGEN_FOUND)
    ADD_CUSTOM_TARGET(  doc
                        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        COMMENT "Generating API documentation with Doxygen"
                        VERBATIM
    )
ENDIF(DOXYGEN_FOUND)




SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

INCLUDE(pSoundMacros)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(doc)




FILE(REMOVE ${PSOUND_TEMP_DOXYGEN_INPUT})




MESSAGE(STATUS "${PROJECT_NAME} install prefix: ${CMAKE_INSTALL_PREFIX}")





IF(ANDROID)
    CONFIGURE_FILE("${PSOUND_ANDROID_TEMPLATES}/Android.mk.master.in" "${CMAKE_BINARY_DIR}/Android.mk")
    CONFIGURE_FILE("${PSOUND_ANDROID_TEMPLATES}/Application.mk.master.in" "${CMAKE_BINARY_DIR}/Application.mk")
    CONFIGURE_FILE("${PSOUND_ANDROID_TEMPLATES}/AndroidManifest.xml.master.in" "${CMAKE_BINARY_DIR}/AndroidManifest.xml")

    MESSAGE(STATUS "Android build: use <PATH_TO_NDK>/ndk-build NDK_APPLICATION_MK=Application.mk")
ENDIF(ANDROID)
