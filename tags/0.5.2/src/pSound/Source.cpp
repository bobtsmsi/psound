/*!
 * @file Source.cpp
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
#include <pSound/Source>
#include <pSound/ALState>
#include <pSound/ALError>

#include <osg/Notify>

#include <osgUtil/CullVisitor>

#ifdef WIN32
    #include <alc.h>
#else
    #include <AL/alc.h>
#endif


#include <osg/Notify>
#include <osg/io_utils>

#include <iostream>

#include <osgDB/ReadFile>

#include <stdlib.h>
#include <float.h>
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
Source::Source(void):
    m_source                ( 0 ),
    m_auto_play             ( false ),
    m_auto_compute_velocity ( true ),
    m_last_traversal_number ( 0 ),
    m_last_simulation_time  ( 0.0 ),
    m_direction             ( -osg::Z_AXIS ),
    m_velocity_mixing_factor( 1 )
{
    setCullingActive(false) ;

    ALState::instance() ;

    PSOUND_CHECK_ERROR( alGenSources(1, &m_source) ) ;

    Source::_loadDefaults(this) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Source::Source(const Source& other, const osg::CopyOp& copyop):
    osg::Node               ( other, copyop ),
    m_source                ( 0 ),
    m_auto_play             ( other.m_auto_play ),
    m_auto_compute_velocity ( other.m_auto_compute_velocity ),
    m_last_traversal_number ( other.m_last_traversal_number ),
    m_last_simulation_time  ( other.m_last_simulation_time ),
    m_position              ( other.m_position ),
    m_direction             ( other.m_direction ),
    m_dsdt                  ( other.m_dsdt ),
    m_velocity_mixing_factor( other.m_velocity_mixing_factor )
{
    PSOUND_CHECK_ERROR( alGenSources(1, &m_source) ) ;



    m_buffer = other.m_buffer ;



    PSOUND_CHECK_ERROR( alSourcei(m_source, AL_BUFFER, m_buffer->getBuffer()) ) ;




    Source::_copyParameters(this, &other) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Source::~Source(void)
{
    PSOUND_CHECK_ERROR( alDeleteSources(1, &m_source) ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Source::traverse(osg::NodeVisitor& nv)
{
    if( nv.getVisitorType() == osg::NodeVisitor::CULL_VISITOR ) {

        osgUtil::CullVisitor*   cv = dynamic_cast<osgUtil::CullVisitor*>(&nv) ;

        if( cv ) {

            const unsigned int  tn = cv->getTraversalNumber() ;

            if(tn > m_last_traversal_number) {
                m_last_traversal_number = tn ;
                _update(cv) ;
            }
        }

    }



    this->osg::Node::traverse(nv) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Source::_update(osgUtil::CullVisitor* cv)
{
    // reinitialize the buffer if needed
    if( m_buffer.valid() ) {
        m_buffer->initBuffer() ;

        if( ! isPlaying() ) {

            if( m_auto_play ) {

                // Set auto_play to false once started, but this solution
                // generates issues that I have not well evaluated yet

                m_auto_play = false ;

                play() ;

            }
        }
    }








    const osg::Vec3 position = m_position * *cv->getModelViewMatrix() ;
    const osg::Vec3 direction = osg::Matrix::transform3x3( m_direction, *cv->getModelViewMatrix() ) ;



    if( m_auto_compute_velocity ) {

        const double    simulation_time = cv->getFrameStamp()->getSimulationTime() ;
        const double    dt = simulation_time - m_last_simulation_time ;

        m_last_simulation_time = simulation_time ;



        const osg::Vec3 last_position = this->getParam( POSITION ) ;


        const osg::Vec3 dsdt = (position - last_position) / dt ;



        m_dsdt =    m_dsdt * (1.0 - m_velocity_mixing_factor) +

                    dsdt * m_velocity_mixing_factor ;



        this->setParam( VELOCITY, m_dsdt ) ;
    }



    this->setParam( POSITION, position ) ;
    this->setParam( DIRECTION, direction ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
Source::setBuffer(Buffer* data)
{
    m_buffer = data ;


    if( m_buffer->initBuffer() ) {
        PSOUND_CHECK_ERROR( alSourcei(m_source, AL_BUFFER, m_buffer->getBuffer()) ) ;
        return true ;
    }

    return false ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Source::play(void)
{
    PSOUND_CHECK_ERROR( alSourcePlay(m_source) ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Source::stop(void)
{
    PSOUND_CHECK_ERROR( alSourceStop(m_source) ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
Source::isPlaying(void) const
{
    ALint   source_state ;
    PSOUND_CHECK_ERROR( alGetSourcei(m_source, AL_SOURCE_STATE, &source_state) ) ;

    return (source_state == AL_PLAYING) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Source::setParam(FloatParam param, float val)
{
    PSOUND_CHECK_ERROR( alSourcef(m_source, (ALenum)param, val) ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
float
Source::getParam(FloatParam param) const
{
    float   val ;

    PSOUND_CHECK_ERROR( alGetSourcef(m_source, (ALenum)param, &val) ) ;

    return val ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Source::setParam(Vec3fParam param, const osg::Vec3& val)
{
    PSOUND_CHECK_ERROR( alSource3f(m_source, (ALenum)param, val.x(), val.y(), val.z()) ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osg::Vec3
Source::getParam(Vec3fParam param) const
{
    float   val[3] ;

    PSOUND_CHECK_ERROR( alGetSource3f(m_source, (ALenum)param, &val[0], &val[1], &val[2]) ) ;

    return osg::Vec3(val[0], val[1], val[2]) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Source::setParam(IntParam param, int val)
{
    PSOUND_CHECK_ERROR( alSourcei(m_source, (ALenum)param, val) ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
int
Source::getParam(IntParam param) const
{
    int     val = 0 ;

    PSOUND_CHECK_ERROR( alGetSourcei(m_source, (ALenum)param, &val) ) ;

    return val ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Source::_loadDefaults(Source* source)
{
    osg::notify(osg::DEBUG_INFO) << "pSound::Source::_loadDefaults(source = " << source << ")" << std::endl ;

    source->setParam(   PITCH,              1.0f    ) ;
    source->setParam(   GAIN,               1.0f    ) ;
    source->setParam(   MAX_DISTANCE,       FLT_MAX ) ;
    source->setParam(   ROLLOFF_FACTOR,     1.0f    ) ;
    source->setParam(   REFERENCE_DISTANCE, 1.0f    ) ;
    source->setParam(   MIN_GAIN,           0.0f    ) ;
    source->setParam(   MAX_GAIN,           1.0f    ) ;
    source->setParam(   CONE_OUTER_GAIN,    0.0f    ) ;
    source->setParam(   CONE_INNER_ANGLE,   360.0f  ) ;
    source->setParam(   CONE_OUTER_ANGLE,   360.0f  ) ;
    source->setParam(   SEC_OFFSET,         0.0f  ) ;

    source->setParam(   POSITION,           osg::Vec3(0.0f, 0.0f, 0.0f) ) ;
    source->setParam(   VELOCITY,           osg::Vec3(0.0f, 0.0f, 0.0f) ) ;
    source->setParam(   DIRECTION,          osg::Vec3(0.0f, 0.0f, 0.0f) ) ;

    source->setParam(   LOOPING,            AL_FALSE) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Source::_copyParameters(Source* dst, const Source* src)
{
    osg::notify(osg::DEBUG_INFO) << "pSound::Source::_copyParameters(dst = " << dst << ", src = " << src << std::endl ;

    dst->setParam( PITCH,               src->getParam(PITCH) ) ;
    dst->setParam( GAIN,                src->getParam(GAIN) ) ;
    dst->setParam( MAX_DISTANCE,        src->getParam(MAX_DISTANCE) ) ;
    dst->setParam( ROLLOFF_FACTOR,      src->getParam(ROLLOFF_FACTOR) ) ;
    dst->setParam( REFERENCE_DISTANCE,  src->getParam(REFERENCE_DISTANCE) ) ;
    dst->setParam( MIN_GAIN,            src->getParam(MIN_GAIN) ) ;
    dst->setParam( MAX_GAIN,            src->getParam(MAX_GAIN) ) ;
    dst->setParam( CONE_OUTER_GAIN,     src->getParam(CONE_OUTER_GAIN) ) ;
    dst->setParam( CONE_INNER_ANGLE,    src->getParam(CONE_INNER_ANGLE) ) ;
    dst->setParam( CONE_OUTER_ANGLE,    src->getParam(CONE_OUTER_ANGLE) ) ;
    dst->setParam( SEC_OFFSET,          src->getParam(SEC_OFFSET) ) ;


    dst->setParam( POSITION,            src->getParam(POSITION) ) ;
    dst->setParam( VELOCITY,            src->getParam(VELOCITY) ) ;
    dst->setParam( DIRECTION,           src->getParam(DIRECTION) ) ;


    dst->setParam( LOOPING,             src->getParam(LOOPING) ) ;
}
/* ....................................................................... */
/* ======================================================================= */
