/*!
 * @file Serializer_Buffer.cpp
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
#include <pSound/Buffer>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
static bool checkData(const pSound::Buffer& buffer)
{
    return buffer.getData() != NULL ;
}

static bool readData(osgDB::InputStream& is, pSound::Buffer& buffer)
{
    if( is.isBinary() ) {
        unsigned int    size = is.readSize() ;
        char*   data = (char*)malloc(size) ;
        is.readCharArray(data, size) ;
        buffer.setData(data) ;
    }
    return true ;
}

static bool writeData(osgDB::OutputStream& os, const pSound::Buffer& buffer)
{
    if( os.isBinary() ) {
        unsigned int    size = buffer.getDataSize() ;
        os.writeSize(size) ;
        const char*     data = (const char*)buffer.getData() ;
        if( data && size ) {
            os.writeCharArray(data, size) ;
        }
    }
    return true ;
}
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( Buffer,
                         new pSound::Buffer,
                         pSound::Buffer,
                         "osg::Object pSound::Buffer" )
{
    ADD_BOOL_SERIALIZER(ReleaseMemoryAfterApply, false) ;
    ADD_STRING_SERIALIZER(SoundFileName, "") ;
    ADD_USHORT_SERIALIZER(BitsPerSample, 0) ;
    ADD_UINT_SERIALIZER(Frequency, 0) ;
    ADD_USHORT_SERIALIZER(NumChannels, 0) ;
    ADD_UINT_SERIALIZER(DataSize, 0) ;
    ADD_USER_SERIALIZER(Data) ;
}
/* ....................................................................... */
/* ======================================================================= */
