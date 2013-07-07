/*!
 * @file WAVE_IO.h
 * @author Rocco Martin0
 *
 * Contiene funzioni di I/O da/verso file di tipo WAVE
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

#ifndef _WAVE_IO_H
#define _WAVE_IO_H

/* ======================================================================= */
#include <stdio.h>
/* ======================================================================= */




#ifdef __cplusplus
extern "C"
{
#endif




/* ======================================================================= */
/* ....................................................................... */
//! Una rappresentazione di un file WAVE
/*!
 * Questa struttura contiene l'header di un file WAVE così come specificato
 * all'indirizzo:
 * @link http://ccrma.stanford.edu/courses/422/projects/WaveFormat/ \n
 * Ho riportato i commenti originali \n
 * \n
 * Il campo data viene distrutto in WAVEDestroy a meno che non sia stata
 * chiamata WAVEReleaseData
 */
typedef struct {
    /*!
     * Contains the letters "RIFF" in ASCII form
     */
    char                ChunkID[4] ;

    /*!
     * 36 + SubChunk2Size, or more precisely:
     * 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
     * This is the size of the rest of the chunk
     * following this number.  This is the size of the
     * entire file in bytes minus 8 bytes for the
     * two fields not included in this count:
     * ChunkID and ChunkSize.
     */
    unsigned int        ChunkSize ;

    /*!
     * Contains the letters "WAVE"
     * (0x57415645 big-endian form).
     */
    char                Format[4] ;




    /* The "WAVE" format consists of two subchunks: "fmt " and "data":
     * The "fmt " subchunk describes the sound data's format: */

    /*!
     * Contains the letters "fmt "
     *(0x666d7420 big-endian form).
     */
    char                Subchunk1ID[4] ;

    /*!
     * 16 for PCM.  This is the size of the
     * rest of the Subchunk which follows this number.
     */
    unsigned int        Subchunk1Size ;

    /*!
     * PCM = 1 (i.e. Linear quantization)
     * Values other than 1 indicate some
     * form of compression.
     */
    unsigned short int  AudioFormat ;

    /*!
     * Mono = 1, Stereo = 2, etc.
     */
    unsigned short int  NumChannels ;

    /*!
     * 8000, 44100, etc.
     */
    unsigned int        SampleRate ;

    /*!
     * SampleRate * NumChannels * BitsPerSample / 8
     */
    unsigned int        ByteRate ;

    /*!
     * NumChannels * BitsPerSample / 8
     */
    unsigned short int  BlockAlign ;

    /*!
     * 8 bits = 8, 16 bits = 16, etc
     */
    unsigned short int  BitsPerSample ;

    /*!
     * if PCM, then doesn't exist
     */
    unsigned short int  ExtraParamSize ;

    /*!
     * space for extra parameters
     */
    void*               ExtraParams ;




    /* The "data" subchunk contains the size of the data and the actual sound: */

    /*!
     * Contains the letters "data"
     * (0x64617461 big-endian form).
     */
    char                Subchunk2ID[4] ;

    /*!
     * NumSamples * NumChannels * BitsPerSample / 8
     * This is the number of bytes in the data.
     * You can also think of this as the size
     * of the read of the subchunk following this
     * number. */
    unsigned int        Subchunk2Size ;

    /*!
     * The actual sound data.
     */
    void*               Data ;
} WAVE ;
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
//! Costruttore
/*!
 * L'area di memoria creata viene inizializzata a zero
 */
WAVE*       WAVECreate(void) ;




//! Distruttore
/*!
 * Libera anche la memoria allocata a Data, a meno che non sia stata
 * chiamate WAVEReleaseData; per tutto il resto non c'è salvezza
 * @sa WAVEReleaseData
 */
void        WAVEDestroy(WAVE* self) ;




//! Legge un file da struttura FILE*
/*!
 * In questo momento non effettua alcun controllo sulla coerenza
 * dei dati, quindi ci si aspetta sempre un file non danneggiato
 */
void        WAVEReadFromFilePointer(WAVE* self, FILE* file) ;




//! chiama WAVEReadFromFilePointer
/*!
 */
void        WAVEReadFromFile(WAVE* self, const char* file_name) ;




//! Legge da file contenuto in memoria
/*!
 * In questo momento non effettua alcun controllo sulla coerenza
 * dei dati, quindi ci si aspetta sempre un file non danneggiato
 */
void        WAVEReadFromMemory(WAVE* self, void* data) ;




//! Scrive un file su struttura FILE
/*!
 * In questo momento non effettua alcun controllo sulla coerenza
 * dei dati, quindi ci si aspetta sempre una struttura valida
 */
void        WAVEWriteToFilePointer(WAVE* self, FILE* file) ;




//! chiama WAVEWriteToFilePointer
/*!
 */
void        WAVEWriteToFile(WAVE* self, const char* file_name) ;




//! Stampa l'header sullo stream indicato
/*!
 */
void        WAVEPrintHeader(WAVE* self, FILE* fp) ;




//! Cede il ptr "Data" e ne dimentica l'esistenza
/*!
 */
void*       WAVEReleaseData(WAVE* self) ;
/* ....................................................................... */
/* ======================================================================= */




#ifdef __cplusplus
} // extern "C"
#endif




#endif /* _WAVE_IO_H */
