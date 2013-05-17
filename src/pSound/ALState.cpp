/*!
 * @file ALState.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2010 - 2013 by Rocco Martino                            *
 *   martinorocco@gmail.com                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
#include <pSound/ALState>
#include <pSound/ALError>

#include <osg/Notify>



#ifdef WIN32
    #include <alc.h>
#else
    #include <AL/alc.h>
#endif
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
#ifndef PSOUND_ERROR_CHECKS_ENABLED
    #define PSOUND_ERROR_CHECKS_ENABLED 1
#endif



#if PSOUND_ERROR_CHECKS_ENABLED

#define PSOUND_CHECK_ERROR( FN ) \
{ \
    ALClearError() ; \
    (void) FN ; \
/*    PS_ASSERT(ALCheckError(#FN)) ; */ \
    ALCheckError(#FN) ; \
}

#else

#define PSOUND_CHECK_ERROR( FN ) \
{ \
    (void) FN ; \
}
#endif
/* ....................................................................... */
/* ======================================================================= */




using namespace pSound ;




/* ======================================================================= */
/* ....................................................................... */
ALState::ALState(void)
{
    osg::notify(osg::DEBUG_INFO) << "pSound::ALState: new instance => " << this << std::endl ;

	_initAL() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ALState::ALState(const ALState& other, const osg::CopyOp& copyop):
    osg::Object(other, copyop)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ALState::~ALState(void)
{
    osg::notify(osg::DEBUG_INFO) << "pSound::ALState: destroying instance => " << this << std::endl ;

	_deinitAL() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
ALState::_initAL(void)
{

    ALCdevice*  device = alcOpenDevice(NULL) ;

    if( device == NULL ) {
        osg::notify(osg::FATAL) << "pSound::ALState: cannot open default device" << std::endl ;
    }
    else {
        ALCcontext* context = alcCreateContext( device, NULL ) ;

        if( ! context ) {
            osg::notify(osg::FATAL) << "pSound::ALState: cannot create context" << std::endl ;
        }
        else {
            alcMakeContextCurrent( context ) ;
        }
    }


    // clear error code
    alGetError() ;


    _loadDefaults(this) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
ALState::_deinitAL(void)
{
    ALCcontext* context = alcGetCurrentContext() ;

    if( context ) {


        ALCdevice*  device = alcGetContextsDevice( context ) ;

        alcMakeContextCurrent( NULL ) ;
        alcDestroyContext( context ) ;

        if( device ) {
            alcCloseDevice( device ) ;
        }
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ALState*
ALState::instance(void)
{
    static osg::ref_ptr<ALState>    state = new ALState() ;

    return state.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
ALState::_loadDefaults(ALState* state)
{
    osg::notify(osg::DEBUG_INFO) << "Sound::ALState::_loadDefaults(state = " << state << ")" << std::endl ;

    PSOUND_CHECK_ERROR( state->setDistanceModel(EXPONENT_DISTANCE_CLAMPED) ) ;
    PSOUND_CHECK_ERROR( state->setSpeedOfSound(343.3f) ) ;
    PSOUND_CHECK_ERROR( state->setDopplerFactor(1.0f) ) ;
}
/* ....................................................................... */
/* ======================================================================= */
