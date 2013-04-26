SET(MODULE_NAME pSound)




SET(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${MODULE_NAME})




SET(TARGET_H
    ${HEADER_PATH}/ALError
    ${HEADER_PATH}/ALState
    ${HEADER_PATH}/ALState.inl
    ${HEADER_PATH}/Buffer
    ${HEADER_PATH}/Buffer.inl
    ${HEADER_PATH}/Listener
    ${HEADER_PATH}/Listener.inl
    ${HEADER_PATH}/Source
    ${HEADER_PATH}/Source.inl
    ${HEADER_PATH}/Version
)




SET(TARGET_SRC
    ALError.cpp
    ALState.cpp
    Buffer.cpp
    Listener.cpp
    Source.cpp
    Version.cpp
)




IF( WIN32 )
    ADD_DEFINITIONS( "/wd4250" )
ENDIF( WIN32 )




SET( MODULE_LINK_LIBRARIES_GNU "osgDB openal alut vorbisfile" )
SET( MODULE_LINK_LIBRARIES_WIN "osgDB osg OpenThreads opengl32 OpenAL32 alut" )

PSOUND_SETUP_LIBRARY()
