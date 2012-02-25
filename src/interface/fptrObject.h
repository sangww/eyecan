/*
    eyeCan: a program that enables people to use PC with their eyes
    Copyright (C) 2012  Sang-won Leigh

    eyeCan is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    eyeCan is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef _FPTR_OBJ
#define _FPTR_OBJ

#include <vector>

typedef unsigned long       DWORD;
typedef void (*fptr)(DWORD c);

class fptrObject {

	public:
		fptrObject();
		~fptrObject();
		
		std::vector<fptr> f;
		std::vector<int> a;
		std::vector<bool> n; //automatically call next?
		std::vector<bool> r; //automatically repeat this command?

		void call();
		void callRepeater();
		void reset();
		void attach(fptr fp, DWORD ch, bool next, bool repeat);
		void attach(fptr fp, DWORD ch, bool next);
		void attach(fptr fp, DWORD ch);
		void attach(fptr fp, bool next);
		void attach(fptr fp);
		void attachNull();

		int curIndex;
		int repIndex;
};

#endif
