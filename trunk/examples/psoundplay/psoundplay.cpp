#include <pSound/Source>

#include <stdio.h>
#include <signal.h>
#include <assert.h>

#include <osgDB/ReadFile>

#ifdef WIN32
#include <windows.h>
#endif




#ifdef WIN32
#define SLEEP(x)    Sleep( (x) * 1.0e3 )
#else
#define SLEEP(x)    usleep( (x) * 1.0e6 )
#endif




using namespace pSound ;




bool    done = false ;




void
quit_app(int s)
{
    (void)s ;

    done = true ;
}




int
main(int argc, char** argv)
{

    if( argc != 2 ) {

        std::cout << "usage: " << argv[0] << " sound_file.wav" << std::endl ;

        return -1 ;
    }



    // <Ctrl-c> sets done
    signal(SIGINT, quit_app) ;



    // create the source
    osg::ref_ptr<Source>        source = new Source() ;

    source->setParam( Source::LOOPING, AL_TRUE ) ;



    // read the buffer
    Buffer* buffer = dynamic_cast<Buffer*>( osgDB::readObjectFile(argv[1]) ) ;
    assert( buffer != NULL ) ;

    buffer->setReleaseMemoryAfterApply(true) ;



    // set the buffer
    source->setBuffer(buffer) ;



    // play
    source->play() ;





    while( ! done ) {

        if( ! source->isPlaying() ) {
            break ;
        }

        printf("\rt: %.1f", source->getParam(pSound::Source::SEC_OFFSET)) ;
        fflush(stdout) ;

        SLEEP(0.1) ;
    }

    printf("\n") ;




    // stop
    source->stop() ;




    // wait until stops
    while(source->isPlaying()) {
        SLEEP(1) ;
    }




    return 0 ;
}
