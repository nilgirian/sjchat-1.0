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

public class ImagePanel extends Canvas
{
	URL link;

	private Image image = null;
	private Image backupImage = null;
	private int height;
	private int width;
	private Applet applet;

	ImagePanel(int width, int height, Applet applet)
	{
		this.width = width;
		this.height = height;
		this.applet = applet;

		link = null;
		backupImage = null;

		setForeground(Color.white);
		setBackground(Color.white);
	}

	public Dimension minimumSize() { return new Dimension(width, height); }
	public Dimension preferredSize() { return minimumSize(); }
	public void update(Graphics g) { paint(g); }

	boolean hasImage()
	{
		try { return image.getHeight(this) > 0; }
				catch(NullPointerException e) { return false; }
	}

	public void paint(Graphics g)
	{
		if (!hasImage() && backupImage != null)
			try
			{
				g.drawImage(backupImage, 0, 0, this);
			}
			catch (NullPointerException e)
			{
				g.fillRect(0, 0, width, height);
			}
		try
		{
			g.drawImage(image, 0, 0, this);
		}
		catch (NullPointerException e)
		{
			g.fillRect(0, 0, width, height);
		}
	}

	void newLink(URL link) { this.link = link; }
	void backup(Image backupImage) { this.backupImage = backupImage; }

	void newImage(String string)
	{
		image = applet.getImage(applet.getDocumentBase(),
					"People/"+string.toLowerCase()+".jpg");
		prepareImage(image, this);
		repaint();
	}

	void newImage(URL url, String name)
	{
		try { getGraphics().fillRect(0, 0, width, height); }
						catch(NullPointerException e){}
		image = applet.getImage(url, name);
		prepareImage(image, this);
		repaint();
	}

	public void repaint()
	{
		try
		{
			getGraphics().fillRect(0,0, size().width,size().height);
		}
		catch (NullPointerException e) {}
		super.repaint();
	}
}
