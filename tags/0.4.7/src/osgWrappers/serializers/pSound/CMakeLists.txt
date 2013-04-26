SET(MODULE_NAME osgdb_serializers_psound)




SET(TARGET_SRC
    Buffer_serializer.cpp
    Source_serializer.cpp
)




IF( WIN32 )
    ADD_DEFINITIONS( "/wd4250" )
ENDIF( WIN32 )




SET(MODULE_LINK_LIBRARIES_WIN "osgDB osg OpenThreads opengl32 pSound")

PSOUND_SETUP_PLUGIN()
