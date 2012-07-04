// NppSharp - C#/.NET Scripting Plugin for Notepad++
// Copyright (C) 2012  Chris Mrazek
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#define DEFLEX(x) npp::ILexer* lex##x() { return g.npp->GetLexer(x); }
DEFLEX(0)
DEFLEX(1)
DEFLEX(2)
DEFLEX(3)
DEFLEX(4)
DEFLEX(5)
DEFLEX(6)
DEFLEX(7)
DEFLEX(8)
DEFLEX(9)
DEFLEX(10)
DEFLEX(11)
DEFLEX(12)
DEFLEX(13)
DEFLEX(14)
DEFLEX(15)
DEFLEX(16)
DEFLEX(17)
DEFLEX(18)
DEFLEX(19)
DEFLEX(20)
DEFLEX(21)
DEFLEX(22)
DEFLEX(23)
DEFLEX(24)
DEFLEX(25)
DEFLEX(26)
DEFLEX(27)
DEFLEX(28)
DEFLEX(29)

LexerFactoryFunction GetLexerFactoryByIndex(int index)
{
	switch (index)
	{
#define GETLEX(x) case x: return lex##x;
		GETLEX(0)
		GETLEX(1)
		GETLEX(2)
		GETLEX(3)
		GETLEX(4)
		GETLEX(5)
		GETLEX(6)
		GETLEX(7)
		GETLEX(8)
		GETLEX(9)
		GETLEX(10)
		GETLEX(11)
		GETLEX(12)
		GETLEX(13)
		GETLEX(14)
		GETLEX(15)
		GETLEX(16)
		GETLEX(17)
		GETLEX(18)
		GETLEX(19)
		GETLEX(20)
		GETLEX(21)
		GETLEX(22)
		GETLEX(23)
		GETLEX(24)
		GETLEX(25)
		GETLEX(26)
		GETLEX(27)
		GETLEX(28)
		GETLEX(29)
	default:
		return NULL;
	}
}
