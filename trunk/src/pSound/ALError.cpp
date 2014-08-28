/*!
 * @file ALError.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2012 - 2013 by Rocco Martino                            *
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
#include <pSound/ALError>

#include <osg/Notify>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
pSound::ALClearError(void)
{
    alGetError() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ALenum
pSound::ALGetError(void)
{
    return alGetError() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
const char*
pSound::ALGetErrorString(ALenum al_error)
{

#define CHECK_ERROR(ERR)    case ERR:   return #ERR ;


    switch( al_error )
    {
        CHECK_ERROR( AL_INVALID_NAME ) ;
        CHECK_ERROR( AL_INVALID_ENUM ) ;
        CHECK_ERROR( AL_INVALID_VALUE ) ;
        CHECK_ERROR( AL_INVALID_OPERATION ) ;
        CHECK_ERROR( AL_OUT_OF_MEMORY ) ;

        default:    return NULL ;
    }


    return NULL ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
pSound::ALCheckError(const char* message, const char* file, unsigned int line)
{
    ALenum  error = ALGetError() ;

    if( error != AL_NO_ERROR ) {
        if( file ) {
            osg::notify(osg::FATAL) << file << ":" << line << " : " << ALGetErrorString(error) << " after " << message << std::endl ;
        } else {
            osg::notify(osg::FATAL) << ALGetErrorString(error) << " after " << message << std::endl ;
        }
        return false ;
    }

    return true ;
}
/* ....................................................................... */
/* ======================================================================= */
