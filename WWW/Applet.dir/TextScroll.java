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

public class TextScroll extends Canvas
{
	private int height, width;
	private int rows, cols;

	private Dimension minDimension = new Dimension(280, 180);

	private FontMetrics fm;

	private Image offscreen;

	private int fmHeight, fmWidth, fmDescent, y, ylen, yoffset;

	private final static int BUFSIZE = 30;
	private String buffer[];
	private Vector vector;

	private static final int BORDER = 5;
	private static final int RECT = 2;

	private Graphics tg;

	TextScroll()
	{
		setBackground(Color.white);
		setForeground(Color.black);

		vector = new Vector();
		buffer = new String[BUFSIZE];

		for (int i = 0; i < BUFSIZE; i++) buffer[i]=Client.NULL_STRING;
	}

	public void reshape(int x, int y, int w, int h)
	{
		offscreen = null;
		super.reshape(x, y, w, h);
	}

	public Dimension preferredSize() { return minDimension; }

	public void addNotify()
	{
		super.addNotify();

		fm = getFontMetrics(getFont());
		if (fm == null) return;

		fmWidth = fm.charWidth('W');
		fmHeight = fm.getHeight();
		fmDescent = fm.getDescent() + fm.getLeading();
	}

	public void update(Graphics g) { paint(g); }

	public void paint(Graphics g)
	{
		if (offscreen == null)
		{
			width = size().width - BORDER - BORDER;
			height = size().height - BORDER - BORDER;

			rows = height / fmHeight;
			yoffset = fmHeight + height - rows*fmHeight + BORDER;
			ylen = rows*fmHeight - fmHeight -fmHeight;
			y = BORDER+height - fmDescent - fmHeight;

			offscreen = createImage(size().width, size().height);
			tg = offscreen.getGraphics();
			tg.setColor(Color.white);
			tg.setFont(getFont());
			tg.fillRect(0, 0, size().width, size().height);
			tg.setColor(Color.lightGray);
			tg.drawRect(1, 1, size().width-2, size().height-2);
		}
		while (!vector.isEmpty())
		{
			parseString((String)vector.firstElement());
			vector.removeElementAt(0);
		}
		g.drawImage(offscreen, 0, 0, Color.white, this);
	}

	private void addLine(String string)
	{
		int x = BORDER;

		String buf;
		char c;

		tg.copyArea(BORDER, yoffset, width, ylen, 0, -fmHeight);
		tg.setColor(Color.white);
		tg.fillRect(BORDER, y + fmDescent - fmHeight, width, fmHeight);
		tg.setColor(Color.black);

		for (int i = BUFSIZE-1; i > 0; i--) buffer[i] = buffer[i - 1];
		buffer[0] = string;

		for (int j = 0; j < buffer[0].length(); j++)
		{
			c = buffer[0].charAt(j);
			tg.drawString(String.valueOf(c), x, y);
			x += fm.charWidth(c);
		}
	}

	private int stringWidth(String line)
	{
		StringBuffer stripped = new StringBuffer();
		char c;
		for (int i = 0; i < line.length(); i++)
		{
			c = line.charAt(i);
			stripped.append(c);
		}
		return fm.stringWidth(stripped.toString());
	}

	void addString(String string)
	{
		vector.addElement(string);
		repaint();
	}

	private void parseString(String string)
	{
		String line = string;
		String left = null;
		int length;

		while (stringWidth(line) > width)
		{
			length = line.length();
			length--;
			left = string.substring(length);
			line = line.substring(0, length);
		}

		addLine(line.toString());

		if (left != null) parseString(left.toString());
	}
}
