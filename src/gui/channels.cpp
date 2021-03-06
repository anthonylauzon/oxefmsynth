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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <strsafe.h>
#include "control.h"
#include "synthesizer.h"
#include "channels.h"

#define KEY_WIDTH   10
#define KEY_HEIGHT  10

CChannels::CChannels(HBITMAP bmp, CSynthesizer *synthesizer, char &channel, int x, int y)
{
    this->hwnd        = NULL;
    this->bmp         = bmp;
    this->channel     = &channel;
    this->synthesizer = synthesizer;
    rect.left         = x;
    rect.top          = y;
    rect.right        = x + KEY_WIDTH  * 8;
    rect.bottom       = y + KEY_HEIGHT * 2;
}

void CChannels::SetHandlers(HWND hwnd, HDC dc, HDC memdc)
{
    this->hwnd  = hwnd;
    this->dc    = dc;
    this->memdc = memdc;
    if (this->hwnd && this->dc && this->memdc)
    {
        Repaint();
    }
}

void CChannels::OnClick(POINT point)
{
    *channel = (char)((point.x - rect.left) / KEY_WIDTH);
    if (point.y > rect.top + KEY_HEIGHT)
    {
        *channel += 8;
    }
    if (hwnd)
    {
        Repaint();
        InvalidateRect(hwnd, &rect, FALSE);
    }
}

void CChannels::Repaint()
{
    char i;
    int  dcant = SaveDC(memdc);
    SelectObject(memdc,bmp);
    for (i=0;i< 8;i++)
        BitBlt(dc, rect.left +  i    * KEY_WIDTH, rect.top             , KEY_WIDTH, KEY_HEIGHT, memdc, (*channel==i)?KEY_WIDTH:0, 0, SRCCOPY);
    for (i=8;i<16;i++)
        BitBlt(dc, rect.left + (i-8) * KEY_WIDTH, rect.top + KEY_HEIGHT, KEY_WIDTH, KEY_HEIGHT, memdc, (*channel==i)?KEY_WIDTH:0, 0, SRCCOPY);
    RestoreDC(memdc,dcant);
}

int CChannels::GetType()
{
    return VL_CHANNELS;
}

bool CChannels::GetName(char* str)
{
    StringCchPrintfA(str, TEXT_SIZE, "Channel %02i", *channel + 1);
    return TRUE;
}
