/*!
 * @file Buffer.cpp
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
#include <pSound/Buffer>
#include <pSound/ALState>
#include <pSound/ALError>

#include <assert.h>
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
Buffer::Buffer(void):
    m_buffer(0),

    m_bits_per_sample(0),
    m_num_channels(0),
    m_sample_rate(0),

    m_data_size(0),
    m_data(NULL),

    m_duration(0.0),

    m_dirty(true),

    m_release_data(false)
{

    ALState::instance() ;


    PSOUND_CHECK_ERROR( alGenBuffers(1, &m_buffer) ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Buffer::Buffer(const Buffer& other, const osg::CopyOp& copyop):
    osg::Object(other, copyop),

    m_buffer(0),

    m_bits_per_sample(0),
    m_num_channels(0),
    m_sample_rate(0),

    m_data_size(0),
    m_data(NULL),

    m_duration(0.0),

    m_dirty(true),

    m_release_data(other.m_release_data)
{
    assert( m_release_data == false ) ;


   PSOUND_CHECK_ERROR(  alGenBuffers(1, &m_buffer) ) ;


    ALenum  format = 0 ;

    switch(other.getNumChannels())
    {
        case 1:
            switch(other.getBitsPerSample())
            {
                case 8:     format = AL_FORMAT_MONO8 ;      break ;
                case 16:    format = AL_FORMAT_MONO16 ;     break ;
                default:                                    break ;
            }
        break ;


        case 2:
            switch(other.getBitsPerSample())
            {
                case 8:     format = AL_FORMAT_STEREO8 ;    break ;
                case 16:    format = AL_FORMAT_STEREO16 ;   break ;
                default:                                    break ;
            }
        break ;


        default:
        break ;
    }


    if(format) {

        m_bits_per_sample = other.m_bits_per_sample ;
        m_sample_rate = other.m_sample_rate ;
        m_num_channels = other.m_num_channels ;


        PSOUND_CHECK_ERROR( alBufferData(m_buffer, format, other.getData(), m_data_size, m_sample_rate) ) ;

        if( m_release_data ) {
            osg::notify(osg::DEBUG_INFO) << "pSound::Buffer::initBuffer(" << this << "): releasing m_data" << std::endl ;
            freeData() ;
        }

        m_dirty = false ;

    } else {
        osg::notify(osg::FATAL) << "pSound::BufferCopyConstructor(" << this << " <- " << &other << "): Invalid format: " << (int)format << std::endl ;
    }


    setSoundFileName( other.getSoundFileName() ) ;


    computeDuration() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Buffer::Buffer( unsigned short int  bits_per_sample,
                unsigned int        sample_rate,
                unsigned short int  num_channels,
                unsigned int        data_size,
                void*               data):
    m_buffer(0),

    m_bits_per_sample(0),
    m_num_channels(0),
    m_sample_rate(0),

    m_data_size(0),
    m_data(NULL),

    m_dirty(true)
{
    PSOUND_CHECK_ERROR( alGenBuffers(1, &m_buffer) ) ;
    set(bits_per_sample, sample_rate, num_channels, data_size, data) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Buffer::~Buffer(void)
{
    PSOUND_CHECK_ERROR( alDeleteBuffers(1, &m_buffer) ) ;

    free(m_data) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Buffer::set(    unsigned short int  bits_per_sample,
                unsigned int        sample_rate,
                unsigned short int  num_channels,
                unsigned int        data_size,
                void*               data)
{
    m_bits_per_sample = bits_per_sample ;
    m_sample_rate = sample_rate ;
    m_num_channels = num_channels ;
    m_data_size = data_size ;

    free(m_data) ;
    m_data = malloc(data_size) ;
    memcpy(m_data, data, data_size) ;

    m_dirty = true ;

    computeDuration() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
Buffer::initBuffer(void)
{
    if( ! m_dirty ) {
        return true ;
    }


    osg::notify(osg::DEBUG_INFO) << "pSound::Buffer::initBuffer(" << this << ")" << std::endl ;


    ALenum  format = 0 ;

    switch(m_num_channels)
    {
        case 1:
            switch(m_bits_per_sample)
            {
                case 8:     format = AL_FORMAT_MONO8 ;      break ;
                case 16:    format = AL_FORMAT_MONO16 ;     break ;

                default:
                    osg::notify(osg::FATAL) << "pSound::Buffer::initBuffer(" << this << "): unsupported format: " << m_bits_per_sample << " bps" << std::endl ;
                    return false ;
                break ;
            }
        break ;


        case 2:
            switch(m_bits_per_sample)
            {
                case 8:     format = AL_FORMAT_STEREO8 ;    break ;
                case 16:    format = AL_FORMAT_STEREO16 ;   break ;

                default:
                    osg::notify(osg::FATAL) << "pSound::Buffer::initBuffer(" << this << "): unsupported format: " << m_bits_per_sample << " bps" << std::endl ;
                    return false ;
                break ;
            }
        break ;


        default:
            osg::notify(osg::FATAL) << "pSound::Buffer::initBuffer(" << this << "): unsupported format: " << m_num_channels << " cnannels" << std::endl ;
            return false ;
        break ;
    }


    PSOUND_CHECK_ERROR( alBufferData(m_buffer, format, m_data, m_data_size, m_sample_rate) ) ;

    if( m_release_data ) {
        osg::notify(osg::DEBUG_INFO) << "pSound::Buffer::initBuffer(" << this << "): releasing m_data" << std::endl ;
        freeData() ;
    }

    m_dirty = false ;


    computeDuration() ;


    return true ;
}
/* ....................................................................... */
/* ======================================================================= */
