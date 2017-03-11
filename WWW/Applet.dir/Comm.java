// SJChat v1 Copyright (C) 1998 Frederick Manligas Nacino
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your option)
// any later version. 
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details. 
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to:
//
// Free Software Foundation, Inc.
// 59 Temple Place - Suite 330
// Boston, MA 02111-1307
// USA
//
//
// Or you may contact the author:
//
// silk@exodus.mit.edu
//
// Frederick Manligas Nacino
// 20334 Trails End
// Walnut, CA 91789
// USA

import java.applet.*;
import java.awt.*;
import java.io.*;
import java.net.*;
import java.util.*;

public class Comm extends Thread
{
	private DataInputStream in;
	private Init applet;

	TextScroll output;
	Client client;

	String error;

	Comm(DataInputStream in, Init applet)
	{
		this.applet = applet;
		this.in = in;
		start();
		setPriority(7);
	}

	public void run()
	{
		String line;
		try
		{

			for(;;)
			{
				line = in.readLine();
				if (line == null) break;
				if (client == null)
					applet.starter(line);
				else
					if (!client.interpret(line))
						output.addString(line);
			}
		}
		catch(IOException e)
		{
			applet.close();
		}
		finally
		{
			applet.disconnect();
		}
	}
}
