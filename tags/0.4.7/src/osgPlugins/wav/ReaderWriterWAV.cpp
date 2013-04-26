/*!
 * @file ReaderWriterWAV
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

#include "WAVE_IO.h"

#include <pSound/Buffer>
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
class ReaderWriterWAV : public osgDB::ReaderWriter
{
public:
         ReaderWriterWAV()
         {
            supportsExtension("wav","WAV Sound format");
         }

        const char* className() const { return "WAV Sound Reader"; }


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

            osgDB::ifstream istream(fileName.c_str(), std::ios::in | std::ios::binary);
            if(!istream) return ReadResult::FILE_NOT_HANDLED;

            ReadResult rr = readSoundStream(istream, file);

            return rr;
        }

    private:
        static ReadResult readSoundStream(std::istream& fin, const std::string& file_name = "")
        {
            fin.seekg(0, std::ios::end) ;

            unsigned long   buf_size = fin.tellg() ;

            fin.seekg(0, std::ios::beg) ;


            char*   buf = (char*) malloc(buf_size) ;

            fin.read(buf, buf_size) ;


            pSound::Buffer*   data = readSoundMemory(buf) ;
            data->setSoundFileName( file_name ) ;

            free(buf) ;

            return data ;
        }





        static pSound::Buffer*    readSoundMemory(void* data)
        {
            WAVE*   wave = WAVECreate() ;
            WAVEReadFromMemory(wave, data) ;


            pSound::Buffer* buffer = new pSound::Buffer() ;

            buffer->setBitsPerSample(   wave->BitsPerSample ) ;
            buffer->setFrequency(       wave->SampleRate ) ;
            buffer->setNumChannels(     wave->NumChannels ) ;
            buffer->setDataSize(        wave->Subchunk2Size ) ;
            buffer->setData(            WAVEReleaseData(wave) ) ;


            WAVEDestroy(wave) ;

            return buffer ;
        }
} ;
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OSGPLUGIN(wav, ReaderWriterWAV)
/* ....................................................................... */
/* ======================================================================= */
 
