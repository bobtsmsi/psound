SET(MODULE_NAME osgdb_wav)




SET(TARGET_SRC
    ReaderWriterWAV.cpp
    WAVE_IO.c
)




SET(MODULE_LINK_LIBRARIES_WIN "osgDB osg OpenThreads opengl32 OpenAL32 pSound")

PSOUND_SETUP_PLUGIN()
