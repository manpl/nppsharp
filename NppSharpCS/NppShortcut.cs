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

using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace NppSharp
{
	/// <summary>
	/// An instance of the shortcut key sequence, used to designate the shortcut for commands.
	/// </summary>
	public class NppShortcut
	{
		private bool _ctrl = false;
		private bool _alt = false;
		private bool _shift = false;
		private Keys _keyCode = Keys.None;

		/// <summary>
		/// Creates a new shortcut object.
		/// </summary>
		/// <param name="ctrl">Control key required?</param>
		/// <param name="alt">Alt key required?</param>
		/// <param name="shift">Shift key required?</param>
		/// <param name="keyCode">The virtual key code.</param>
		public NppShortcut(bool ctrl, bool alt, bool shift, Keys keyCode)
		{
			_ctrl = ctrl;
			_alt = alt;
			_shift = shift;
			_keyCode = keyCode;
		}

		/// <summary>
		/// Gets or sets a flag indicating if the control key is required.
		/// </summary>
		public bool Control
		{
			get { return _ctrl; }
			set { _ctrl = value; }
		}

		/// <summary>
		/// Gets or sets a flag indicating if the alt key is required.
		/// </summary>
		public bool Alt
		{
			get { return _alt; }
			set { _alt = value; }
		}

		/// <summary>
		/// Gets or sets a flag indicating if the shift key is required.
		/// </summary>
		public bool Shift
		{
			get { return _shift; }
			set { _shift = value; }
		}

		/// <summary>
		/// Gets or sets the virtual key code.
		/// </summary>
		public Keys KeyCode
		{
			get { return _keyCode; }
			set { _keyCode = value; }
		}
	}
}
