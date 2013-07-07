/*!
 * @file WAVE_IO.c
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
#include "WAVE_IO.h"

#include <stdlib.h>
#include <string.h>
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
WAVE*
WAVECreate(void)
{
    WAVE*   self = (WAVE*)malloc( sizeof(WAVE) ) ;

    memset(self, 0, sizeof(WAVE)) ;

    return self ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
WAVEDestroy(WAVE* self)
{
    free(self->Data) ;
    free(self->ExtraParams) ;
    free(self) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
WAVEReadFromFilePointer(WAVE* self, FILE* file)
{
    fseek(file, 0, SEEK_SET) ;

    /*
     * [0] azzero alcuni campi che potrebbero non essere riscritti
     */
    free(self->Data) ;
    free(self->ExtraParams) ;
    self->Data = NULL ;
    self->ExtraParamSize = 0 ;
    self->ExtraParams = NULL ;

    /*
     * [1] leggo fino a WAVE::Subchunk1Size incluso
     */
    fread(self, 20, 1, file) ;

    /*
     * [2] leggo il resto del subchunk fino a BitsPerSample
     */
    fread(&self->AudioFormat, 16, 1, file) ;

    /*
     * [3] leggo i parametri aggiuntivi, se ce ne sono
     */
    if(self->Subchunk1Size > 16) {
        fread(&self->ExtraParamSize, 2, 1, file) ;

        self->ExtraParams = malloc(self->ExtraParamSize) ;
        fread(self->ExtraParams, self->ExtraParamSize, 1, file) ;
    }

    /*
     * [4] leggo i 2 campi di Subchunk2
     */
    fread(&self->Subchunk2ID, 8, 1, file) ;

    /*
     * [5] leggo i dati
     */
    self->Data = malloc(self->Subchunk2Size) ;
    fread(self->Data, self->Subchunk2Size, 1, file) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
WAVEReadFromMemory(WAVE* self, void* void_data)
{
    const char* data = (const char*) void_data ;

    /*
     * [0] azzero alcuni campi che potrebbero non essere riscritti
     */
    free(self->Data) ;
    free(self->ExtraParams) ;
    self->Data = NULL ;
    self->ExtraParamSize = 0 ;
    self->ExtraParams = NULL ;

    /*
     * [1] leggo fino a WAVE::Subchunk1Size incluso
     */
    memcpy(self, data, 20) ;
    data += 20 ;

    /*
     * [2] leggo il resto del subchunk fino a BitsPerSample
     */
    memcpy(&self->AudioFormat, data, 16) ;
    data += 16 ;

    /*
     * [3] leggo i parametri aggiuntivi, se ce ne sono
     */
    if(self->Subchunk1Size > 16) {
        memcpy(&self->ExtraParams, data, 2) ;
        data += 2 ;

        self->ExtraParams = malloc(self->ExtraParamSize) ;
        memcpy(self->ExtraParams, data, self->ExtraParamSize) ;
        data += self->ExtraParamSize ;
    }

    /*
     * [4] leggo i 2 campi di Subchunk2
     */
    memcpy(&self->Subchunk2ID, data, 8) ;
    data += 8 ;

    /*
     * [5] leggo i dati
     */
    self->Data = malloc(self->Subchunk2Size) ;
    memcpy(self->Data, data, self->Subchunk2Size) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
WAVEReadFromFile(WAVE* self, const char* file_name)
{
#ifdef WIN32
    FILE*	f ;
    fopen_s(&f, file_name, "r") ;
#else
    FILE*   f = fopen(file_name, "r") ;
#endif

    WAVEReadFromFilePointer(self, f) ;
    fclose(f) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
WAVEWriteToFilePointer(WAVE* self, FILE* file)
{
    fseek(file, 0, SEEK_SET) ;

    /*
     * [1] scrivo fino a WAVE::Subchunk1Size incluso
     */
    fwrite(self, 20, 1, file) ;

    /*
     * [2] scrivo il resto del subchunk fino a BitsPerSample
     */
    fwrite(&self->AudioFormat, 16, 1, file) ;

    /*
     * [3] scrivo i parametri aggiuntivi, se ce ne sono
     */
    if(self->Subchunk1Size > 16) {
        fwrite(&self->ExtraParamSize, 2, 1, file) ;
        fwrite(self->ExtraParams, self->ExtraParamSize, 1, file) ;
    }

    /*
     * [4] scrivo i 2 campi di Subchunk2
     */
    fwrite(&self->Subchunk2ID, 8, 1, file) ;

    /*
     * [5] scrivo i dati
     */
    fwrite(self->Data, self->Subchunk2Size, 1, file) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
WAVEWriteToFile(WAVE* self, const char* file_name)
{
#ifdef WIN32
    FILE*	f ;
    fopen_s(&f, file_name, "w") ;
#else
    FILE*   f = fopen(file_name, "w") ;
#endif

    WAVEWriteToFilePointer(self, f) ;
    fclose(f) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
WAVEPrintHeader(WAVE* self, FILE* fp)
{
    if( strncmp("RIFF", self->ChunkID, 4) ) {
        fprintf(fp, "%s\n", "Corrupted WAVE file or file not loaded") ;
        return ;
    }


    fprintf(fp, "ChunkID: \"%.4s\"\n", self->ChunkID) ;
    fprintf(fp, "ChunkSize: %u\n", self->ChunkSize) ;
    fprintf(fp, "Format: \"%.4s\"\n", self->Format) ;
    fprintf(fp, "Subchunk1ID: \"%.4s\"\n", self->Subchunk1ID) ;
    fprintf(fp, "Subchunk1Size: %u\n", self->Subchunk1Size) ;

    switch(self->AudioFormat)
    {
        case 0x0001:
            fprintf(fp, "AudioFormat: WAVE_FORMAT_PCM\n") ;
        break ;

        case 0x0003:
            fprintf(fp, "AudioFormat: WAVE_FORMAT_IEEE_FLOAT\n") ;
        break ;

        case 0x0006:
            fprintf(fp, "AudioFormat: WAVE_FORMAT_ALAW\n") ;
        break ;

        case 0x0007:
            fprintf(fp, "AudioFormat: WAVE_FORMAT_MULAW\n") ;
        break ;

        case 0xFFFE:
            fprintf(fp, "AudioFormat: WAVE_FORMAT_EXTENSIBLE\n") ;
        break ;

        default:
            fprintf(fp, "AudioFormat: UNKNOWN (0x%.4x)\n", self->AudioFormat & 0x0000FFFF) ;
        break ;
    }

    fprintf(fp, "NumChannels: %u\n", self->NumChannels) ;
    fprintf(fp, "SampleRate: %u\n", self->SampleRate) ;
    fprintf(fp, "ByteRate: %u\n", self->ByteRate) ;
    fprintf(fp, "BlockAlign: %u\n", self->BlockAlign) ;
    fprintf(fp, "BitsPerSample: %u\n", self->BitsPerSample) ;
    fprintf(fp, "ExtraParamSize: %u\n", self->ExtraParamSize) ;

    fprintf(fp, "ExtraParams: ") ;
    if(self->ExtraParamSize) {
        const char* extra_params = (const char*)(self->ExtraParams) ;
        const char* extra_params_end = (const char*)(self->ExtraParams) +  self->ExtraParamSize ;

        while(extra_params != extra_params_end) {
            fprintf(fp, "%.2x", *extra_params++ & 0x000000FF) ;
        }
    }
    fprintf(fp, "\n") ;

    fprintf(fp, "Subchunk2ID: \"%.4s\"\n", self->Subchunk2ID) ;
    fprintf(fp, "Subchunk2Size: %u\n", self->Subchunk2Size) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void*
WAVEReleaseData(WAVE* self)
{
    void*   d = self->Data ;
    self->Data = NULL ;
    return d ;
}
/* ....................................................................... */
/* ======================================================================= */
