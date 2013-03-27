/*!
 * @file ALState.inl
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

#ifndef _PSOUND_ALSTATE_INL
#define _PSOUND_ALSTATE_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
pSound::ALState::setDistanceModel(DistanceModel model)
{
    alDistanceModel( (ALenum)model ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline pSound::ALState::DistanceModel
pSound::ALState::getDistanceModel(void) const
{
    return (pSound::ALState::DistanceModel)alGetInteger(AL_DISTANCE_MODEL) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
pSound::ALState::setSpeedOfSound(float speed)
{
    alSpeedOfSound(speed) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline float
pSound::ALState::getSpeedOfSound(void) const
{
    return alGetFloat(AL_SPEED_OF_SOUND) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
pSound::ALState::setDopplerFactor(float doppler_factor)
{
    alDopplerFactor(doppler_factor) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline float
pSound::ALState::getDopplerFactor(void) const
{
    return alGetFloat(AL_DOPPLER_FACTOR) ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _PSOUND_ALSTATE_INL */
