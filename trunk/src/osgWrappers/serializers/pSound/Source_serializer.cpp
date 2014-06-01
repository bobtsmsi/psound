/*!
 * @file Source_Serializer.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2012 - 2014 by Rocco Martino                            *
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

#include <osgDB/Registry>
#include <osgDB/ReadFile>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
#define ADD_PARAM_FUNC(PARAM, PARAM_TYPE) \
static bool check##PARAM(const pSound::Source& source) { (void) source ; return true ; } \
static bool read##PARAM(osgDB::InputStream& is, pSound::Source& source) \
{ \
    PARAM_TYPE  t ; \
    is >> t ; \
    source.setParam(pSound::Source::PARAM, t) ; \
    return true ; \
} \
static bool write##PARAM(osgDB::OutputStream& os, const pSound::Source& source) \
{ \
    os << source.getParam(pSound::Source::PARAM) << std::endl ; \
    return true ; \
}

ADD_PARAM_FUNC(PITCH, float) ;
ADD_PARAM_FUNC(GAIN, float) ;
ADD_PARAM_FUNC(MAX_DISTANCE, float) ;
ADD_PARAM_FUNC(ROLLOFF_FACTOR, float) ;
ADD_PARAM_FUNC(REFERENCE_DISTANCE, float) ;
ADD_PARAM_FUNC(MIN_GAIN, float) ;
ADD_PARAM_FUNC(MAX_GAIN, float) ;
ADD_PARAM_FUNC(CONE_OUTER_GAIN, float) ;
ADD_PARAM_FUNC(CONE_INNER_ANGLE, float) ;
ADD_PARAM_FUNC(CONE_OUTER_ANGLE, float) ;
ADD_PARAM_FUNC(SEC_OFFSET, float) ;

ADD_PARAM_FUNC(LOOPING, int) ;

ADD_PARAM_FUNC(POSITION, osg::Vec3) ;
ADD_PARAM_FUNC(VELOCITY, osg::Vec3) ;
ADD_PARAM_FUNC(DIRECTION, osg::Vec3) ;




static bool checkBuffer(const pSound::Source& source)
{
    return source.getBuffer() != NULL ;
}

static bool writeBuffer(osgDB::OutputStream& os, const pSound::Source& source)
{
    const pSound::Buffer* buffer = source.getBuffer() ;
    if(buffer) {
        os.writeObject(buffer) ;
    }
    return true ;
}

static bool readBuffer(osgDB::InputStream& is, pSound::Source& source)
{
    osg::ref_ptr<osg::Object>   tmp = is.readObject() ;
    pSound::Buffer*   buffer = dynamic_cast<pSound::Buffer*>(tmp.get()) ;

    if(buffer) {
        if( ! is.isBinary() ) {
            buffer = dynamic_cast<pSound::Buffer*>( osgDB::readObjectFile( buffer->getSoundFileName(), is.getOptions() ) ) ;
        }
    }

    source.setBuffer( buffer ) ;


    return true ;
}
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( Source,
                         new pSound::Source,
                         pSound::Source,
                         "osg::Object osg::Node pSound::Source" )
{
    ADD_BOOL_SERIALIZER(AutoPlay, false) ;

    BEGIN_ENUM_SERIALIZER( AutoComputePosition, COMPUTE_BY_MODELVIEW ) ;
        ADD_ENUM_VALUE( DO_NOT_COMPUTE ) ;
        ADD_ENUM_VALUE( COMPUTE_BY_VIEW ) ;
        ADD_ENUM_VALUE( COMPUTE_BY_MODELVIEW ) ;
    END_ENUM_SERIALIZER() ;

    ADD_BOOL_SERIALIZER(AutoComputeVelocity, true) ;
    ADD_DOUBLE_SERIALIZER(VelocityMixingFactor, 1.0) ;

    ADD_USER_SERIALIZER(PITCH) ;
    ADD_USER_SERIALIZER(GAIN) ;
    ADD_USER_SERIALIZER(MAX_DISTANCE) ;
    ADD_USER_SERIALIZER(ROLLOFF_FACTOR) ;
    ADD_USER_SERIALIZER(REFERENCE_DISTANCE) ;
    ADD_USER_SERIALIZER(MIN_GAIN) ;
    ADD_USER_SERIALIZER(MAX_GAIN) ;
    ADD_USER_SERIALIZER(CONE_OUTER_GAIN) ;
    ADD_USER_SERIALIZER(CONE_INNER_ANGLE) ;
    ADD_USER_SERIALIZER(CONE_OUTER_ANGLE) ;
    ADD_USER_SERIALIZER(SEC_OFFSET) ;
    ADD_USER_SERIALIZER(LOOPING) ;
    ADD_USER_SERIALIZER(POSITION) ;
    ADD_USER_SERIALIZER(VELOCITY) ;
    ADD_USER_SERIALIZER(DIRECTION) ;

    ADD_VEC3_SERIALIZER(Position, osg::Vec3()) ;
    ADD_VEC3_SERIALIZER(Direction, -osg::Z_AXIS) ;

    ADD_USER_SERIALIZER(Buffer) ;
}
/* ....................................................................... */
/* ======================================================================= */
