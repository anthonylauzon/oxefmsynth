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

#define SAMPLE_RATE 44100.f

typedef struct
{
    unsigned int  timestamp;
    unsigned char status;
    unsigned char data1;
    unsigned char data2;
} EVENT;


class CTuneReader
{
private:
	unsigned int tune_cursor;
	void read_bytes(void *p, char size);
	static void quick_sort(EVENT* events, int left, int right);
public:
	CTuneReader();
    EVENT events[NEVENTS];
};