/*!
 * @file Buffer_Serializer.cpp
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
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
#define USE_INLINE_FILE 0
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
#if ! USE_INLINE_FILE
        unsigned int    size = is.readSize() ;
        char*   data = (char*)malloc(size) ;
        is.readCharArray(data, size) ;
        buffer.setData(data) ;
#else

        unsigned int size = is.readSize();
        if ( size>0 )
        {
            char* data = (char*) malloc( size ) ;
            if ( !data )
            {
                is.throwException( "Buffer serializer: readData(): Out of memory." );
                if ( is.getException() ) return false;
            }
            is.readCharArray( data, size );

            std::string ext = osgDB::getFileExtension( buffer.getSoundFileName() );
            osgDB::ReaderWriter* reader =
                osgDB::Registry::instance()->getReaderWriterForExtension( ext );
            if ( reader )
            {
                std::stringstream inputStream;
                inputStream.write( data, size );

                osgDB::ReaderWriter::ReadResult rr = reader->readObject( inputStream );
                if ( rr.validObject() ) {
                    osg::ref_ptr<osg::Object>       o = rr.takeObject();
                    osg::ref_ptr<pSound::Buffer>    tmp = dynamic_cast<pSound::Buffer*>(o.get());

                    if( tmp.valid() ) {

                        buffer.setBitsPerSample     (   tmp->getBitsPerSample() ) ;
                        buffer.setFrequency         (   tmp->getFrequency()     ) ;
                        buffer.setNumChannels       (   tmp->getNumChannels()   ) ;
                        buffer.setDataSize          (   tmp->getDataSize()      ) ;
                        buffer.setData              (   tmp->releaseData()      ) ;
                    }

                }
                else
                {
                    OSG_WARN << "Buffer serializer: readData(): "
                                            << rr.message() << std::endl;
                }
            }
            else
            {
                OSG_WARN << "Buffer serializer: readData(): Unable to find a plugin for "
                                        << ext << std::endl;
            }
            free( data ) ;
        }

#endif

    }

    return true ;
}

static bool writeData(osgDB::OutputStream& os, const pSound::Buffer& buffer)
{
    if( os.isBinary() ) {

#if ! USE_INLINE_FILE
        unsigned int    size = buffer.getDataSize() ;
        os.writeSize(size) ;
        const char*     data = (const char*)buffer.getData() ;
        if( data && size ) {
            os.writeCharArray(data, size) ;
        }
#else


        std::string fullPath = osgDB::findDataFile( buffer.getSoundFileName() );
        std::ifstream infile( fullPath.c_str(), std::ios::in|std::ios::binary );
        if ( infile )
        {
            infile.seekg( 0, std::ios::end );
            unsigned int size = infile.tellg();
            os.writeSize(size);

            if ( size>0 )
            {
                char* data = (char*) malloc( size ) ;
                if ( !data )
                {
                    os.throwException( "Buffer serializer: writeData(): Out of memory." );
                    if ( os.getException() ) return false;
                }

                infile.seekg( 0, std::ios::beg );
                infile.read( data, size );
                os.writeCharArray( data, size );
                free( data ) ;
            }
            infile.close();
        }
        else
        {
            OSG_WARN << "Buffer serializer: writeData(): Failed to open sound file "
                                << buffer.getSoundFileName() << std::endl;
            os << (unsigned int)0;
        }

#endif


    }

    else
    {
        os << std::endl ;
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
