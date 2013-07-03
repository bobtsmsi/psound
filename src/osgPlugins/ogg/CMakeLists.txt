SET(MODULE_NAME osgdb_ogg)




SET(TARGET_SRC
    ReaderWriterOGG.cpp
)




SET(MODULE_LINK_LIBRARIES_WIN "libogg libvorbis libvorbisfile osgDB osg OpenThreads opengl32 OpenAL32 pSound")

PSOUND_SETUP_PLUGIN()
