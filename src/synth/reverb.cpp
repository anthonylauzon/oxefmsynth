/*
Oxe FM Synth: a software synthesizer
Copyright (C) 2004-2015  Daniel Moura <oxe@oxesoft.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "constants.h"
#include "reverb.h"
#include <math.h>
#include <string.h>

void CReverb::Init()
{
    sr     = 0;
    ti     = 0;
    da     = 0;
    icomb1 = 0;
    icomb2 = 0;
    icomb3 = 0;
    icomb4 = 0;
    iallp1 = 0;
    iallp2 = 0;
    memset(bcomb1,0,sizeof(bcomb1));
    memset(bcomb2,0,sizeof(bcomb2));
    memset(bcomb3,0,sizeof(bcomb3));
    memset(bcomb4,0,sizeof(bcomb4));
    memset(ballp1,0,sizeof(ballp1));
    memset(ballp2,0,sizeof(ballp2));
    state = INACTIVE;
    ou0  = 0;
    in1  = 0;
    in1l = 0;
    ou0l = 0;
    a0   = 0;
    a1   = 0;
    b1   = 0;
}

void CReverb::SetPar(char param, float value)
{
    switch (param)
    {
        case SAMPLERATE:
            sr = value;
            break;
        case TIME:
            ti = value;
            break;
        case DAMP:
            da = value;
            break;
    }
}

void CReverb::CalcCoefLowPass(float frequencia)
{
    float w     = 2.0f * sr; 
    float fCut  = 2.0f * PI * Key2Frequency(frequencia * MAXFREQFLT);
    float Norm  = 1.0f / (fCut + w); 
    b1          = lrintf((w - fCut) * Norm * 32768.f);
    a0 = a1     = lrintf(     fCut  * Norm * 32768.f);
}

void CReverb::Process(int *b, int size)
{
    int i        = 0;
    int ent      = 0;
    int aux      = 0;
    int retorno  = (int)(ti * 127.f);
    if (REVDAant != da) 
    {
        CalcCoefLowPass(da);
        REVDAant =         da;
    }
    for (i=0;i<size;i++)
    {
        ent  = b[i];
        b[i] = 0;
        // comb 1
        b[i] += bcomb1[icomb1];
        bcomb1[icomb1] = ent + ((bcomb1[icomb1] * retorno)/128);
        if (++icomb1>=TAMCOMB1) icomb1 = 0;
        // comb 2
        b[i] += bcomb2[icomb2];
        bcomb2[icomb2] = ent + ((bcomb2[icomb2] * retorno)/128);
        if (++icomb2>=TAMCOMB2) icomb2 = 0;
        // comb 3
        b[i] += bcomb3[icomb3];
        bcomb3[icomb3] = ent + ((bcomb3[icomb3] * retorno)/128);
        if (++icomb3>=TAMCOMB3) icomb3 = 0;
        // comb 4
        b[i] += bcomb4[icomb4];
        bcomb4[icomb4] = ent + ((bcomb4[icomb4] * retorno)/128);
        if (++icomb4>=TAMCOMB4) icomb4 = 0;
        // allpass 1
        aux = ballp1[iallp1];
        ballp1[iallp1] = ((aux * retorno)/128) + b[i];
        b[i]           = aux - ((ballp1[iallp1] * retorno)/128);
        if (++iallp1>=TAMALLP1) iallp1 = 0;
        // allpass 2
        aux = ballp2[iallp2];
        ballp2[iallp2] = ((aux * retorno)/128) + b[i];
        b[i]           = aux - ((ballp2[iallp2] * retorno)/128);
        if (++iallp2>=TAMALLP2) iallp2 = 0;
        // DC filter
        ou0  = b[i] - in1 + ((ou0*32674)/32768);
        in1  = b[i];
        b[i] = ou0>>2;
    }
    if (REVDAant < 1.f)
    {
        for (i=0;i<size;i++)
        {
            // low pass filter
            ou0l = ((b[i] * a0)/32768) + ((in1l * a1)/32768) + ((ou0l * b1)/32768);
            in1l = b[i];
            b[i] = ou0l;
        }
    }
    state = ACTIVE;
    if (!b[0] && !b[size>>1] && !b[size>>2] && !b[size-1])
        state = INACTIVE;
}

char CReverb::GetState()
{
    return state;
}

inline float CReverb::Key2Frequency(float valor)
{
    return C0 * powf(2.0f, valor / 12.0f);
}
