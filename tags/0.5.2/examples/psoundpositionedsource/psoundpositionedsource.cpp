#include <pSound/Source>
#include <pSound/ALState>

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Shape>

#include <osgDB/WriteFile>
#include <osgDB/ReadFile>




using namespace pSound ;




int
main(int argc, char** argv)
{
	//
	// [0] Create a context immediately. This is mandatory on Winzows
	//
	ALState::instance() ;



    //
    // [1] Read the buffer
    //

    Buffer*     buffer = dynamic_cast<Buffer*>( osgDB::readObjectFile("sound.wav") ) ;
//     Buffer*     buffer = <dynamic_cast<Buffer*>( osgDB::readObjectFile("sound.ogg") ) ;

    if( ! buffer ) {
        std::cerr << "Cannot read \"sound.wav\"" << std::endl ;
        exit( -1 ) ;
    }





    //
    // [2] Create and setup the source
    //

    Source*     source = new Source() ;

    source->setBuffer( buffer ) ;
    source->setAutoPlay( true ) ;

    source->setParam( Source::LOOPING, AL_TRUE ) ;
#if 0
    source->setParam( Source::REFERENCE_DISTANCE, atof( argv[1] )) ;
    source->setVelocityMixingFactor( atof(argv[2]) ) ;
#endif





    //
    // [3] create the graphics
    //

    osg::Geode* graphics = new osg::Geode() ;

    graphics->addDrawable(

        new osg::ShapeDrawable(
            new osg::Sphere()
        )

    ) ;




    //
    // [4] write out the graph
    //

    osg::ref_ptr<osg::Group>    root = new osg::Group() ;

    root->addChild( source ) ;
    root->addChild( graphics ) ;



    std::cout << "Writing graph to output.osgb.." << std::endl ;

    osgDB::writeNodeFile(*root, "output.osgb") ;

    std::cout << "Done!" << std::endl ;


    return 0 ;
}
