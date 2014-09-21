/*!
 * @file ReaderWriterOGG
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2009 by Rocco Martino                                   *
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
#include <osgDB/Registry>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <pSound/Buffer>

#include <cstdio>

#include <vorbis/vorbisfile.h>
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
class ReaderWriterOGG : public osgDB::ReaderWriter
{
public:
         ReaderWriterOGG()
         {
            supportsExtension("ogg","OGG Sound format");
         }

        const char* className() const { return "OGG Sound Reader"; }


        virtual ReadResult  readObject(std::istream& fin, const Options* options = 0) const
        {
            return readSound(fin, options);
        }

        virtual ReadResult  readObject(const std::string& file, const Options* options = 0) const
        {
            return readSound(file, options);
        }


        ReadResult readSound(std::istream& fin, const Options* = 0) const
        {
            return readSoundStream(fin);
        }

        ReadResult readSound(const std::string& file, const Options* options = 0) const
        {
            std::string ext = osgDB::getLowerCaseFileExtension(file);
            if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

            std::string fileName = osgDB::findDataFile(file, options);
            if (fileName.empty()) return ReadResult::FILE_NOT_FOUND;

            FILE*   f = fopen(fileName.c_str(), "rb") ;

            if( ! f ) {
                return ReadResult::FILE_NOT_HANDLED ;
            }

            ReadResult rr = readSoundStream(f, file) ;

            fclose(f) ;

            return rr;
        }

    private:
        static ReadResult readSoundStream(std::istream& fin)
        {

#ifdef WIN32
            FILE*       tmp = fopen("psound_temp_file", "wb") ;
#else
            FILE*       tmp = tmpfile() ;
#endif

            {
                const int   buf_size = 1024 * 10 ;
                char        buf[buf_size] ;

                fin.seekg(0, std::ios::end) ;
                unsigned int  size = fin.tellg() ;
                fin.seekg(0, std::ios::beg) ;


                {
                    unsigned int    aaa = size % buf_size ;
                    fin.read(buf, aaa) ;
                    fwrite(buf, 1, aaa, tmp) ;
                    size -= aaa ;
                }


                while( size != 0 ) {
                    fin.read(buf, buf_size) ;
                    fwrite(buf, 1, buf_size, tmp) ;
                    size -= buf_size ;
                }
            }

#ifdef WIN32
            fclose(tmp) ;
            tmp = fopen("psound_temp_file", "rb") ;
#endif


            pSound::Buffer*   data = readSoundStream(tmp) ;


#ifdef WIN32
            fclose(tmp) ;
            remove("psound_temp_file") ;
#endif


            return data ;
        }





        static pSound::Buffer*    readSoundStream(FILE* infile, const std::string& file_name = "")
        {
            fseek(infile, 0, SEEK_SET) ;


            pSound::Buffer*   data = new pSound::Buffer() ;


            {
                #define OGG_LITTLE_ENDIAN   0
                #define OGG_BIG_ENDIAN      1
                #define BUFFER_SIZE         (1024 * 10)



                vorbis_info*        vinfo ;
                OggVorbis_File      ofile ;
                int                 bit_stream = 0 ;

                ov_open(infile, &ofile, NULL, 0) ;

                vinfo = ov_info(&ofile, -1) ;

                char*           buf = NULL ;
                unsigned int    buf_size = 0 ;
                unsigned int    total_bytes = 0 ;
                unsigned int    bytes = 0 ;



                data->setBitsPerSample(16) ;
                data->setNumChannels( vinfo->channels ) ;
                data->setFrequency( vinfo->rate ) ;

				buf_size = BUFFER_SIZE ;
                buf = (char*) realloc(buf, buf_size) ;

                do {

                    if( buf_size - total_bytes < BUFFER_SIZE ) {
                        buf_size *= 2 ;
                        buf = (char*) realloc(buf, buf_size) ;
                    }


                    bytes = ov_read(&ofile, &buf[total_bytes], BUFFER_SIZE, OGG_LITTLE_ENDIAN, 2, 1, &bit_stream) ;
                    total_bytes += bytes ;
                } while( bytes > 0 ) ;


                data->setDataSize(total_bytes) ;
                data->setData(buf) ;
            }

            data->setSoundFileName( file_name ) ;

            return data ;
        }
} ;
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OSGPLUGIN(ogg, ReaderWriterOGG)
/* ....................................................................... */
/* ======================================================================= */
 
