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

public class Init extends Applet
{

	private static final int PORT = 7070;
	private static final String ENTRANCE_PW = "LOGIN";

	private static final String CARD1 = "card1";
	private static final String CARD2 = "card2";
	private static final String CARD3 = "card3";

	private static final String RECONNECT = " RECONNECT ";
	private static final String ENTER = " ENTER ";


	private static String HOST;
	private Socket s;
	private DataInputStream in;
	private PrintStream out;
	private Comm comm;

	private Image closed = null;

	private CardLayout cardLayout = new CardLayout(0, 0);
	private Panel card1 = new Panel();
	private Client card2;
	private Panel card3 = new Panel();

	private ImagePanel open = new ImagePanel(400, 190, this);

	private Label label = new Label(Client.NULL_STRING);
	private Button button = new Button(Client.ONE_SPACE);
	private Panel buttonPanel = new Panel();

	private TextField field = new TextField(50);
	private Panel fieldPanel = new Panel();
	private String killMsg = null;

	public void init()
	{
		HOST = getDocumentBase().getHost();

		setForeground(Color.black);
		setBackground(Color.white);
		setLayout(cardLayout);

		card1.setLayout(new GridLayout(3, 1, 10, 10));
		card3.setLayout(new BorderLayout(3, 3));

		fieldPanel.setLayout(new FlowLayout(FlowLayout.CENTER, 10, 10));
		fieldPanel.add("Center", field);

		buttonPanel.setLayout(new FlowLayout(FlowLayout.CENTER, 10, 10));
		button.setBackground(Color.lightGray);
		buttonPanel.add("Center", button);

		resize(550, 250);

		try
		{
			open.newImage(getDocumentBase(), "Library.dir/open.gif");

			s = new Socket(HOST, PORT);
			in = new DataInputStream(s.getInputStream());
			out = new PrintStream(s.getOutputStream());

			comm = new Comm(in, this);
			
			out.println(ENTRANCE_PW+" "+getParameter("room"));

			add(CARD1, card1);
			add(CARD3, card3);

			showCard1();
		}
		catch (IOException e)
		{
			close();
			showStatus("CONNECT ERROR: "+e.toString());
		}
	}

	private void showCard1()
	{
		label.setText("WELCOME!  Enter the name you wish to be known by:");
		button.setLabel(ENTER);

		Client.switchContainer(null, label, card1);
		Client.switchContainer(null, fieldPanel, card1);
		Client.switchContainer(null, buttonPanel, card1);
		cardLayout.show(this, CARD1);
	}

	private void showCard2()
	{
		card2 = new Client(this, in, out, comm);
		add(CARD2, card2);
		cardLayout.show(this, CARD2);
	}

	private void showCard3()
	{
		button.setLabel(RECONNECT);

		Client.switchContainer("North", label, card3);
		Client.switchContainer("Center", open, card3);
		Client.switchContainer("South", buttonPanel, card3);
		cardLayout.show(this, CARD3);
	}

	public void reconnect()
	{
		showCard1();
		field.setEditable(true);
		try
		{
			s = new Socket(HOST, PORT);
			in = new DataInputStream(s.getInputStream());
			out = new PrintStream(s.getOutputStream());

			comm = new Comm(in, this);

			out.println(ENTRANCE_PW+" "+getParameter("room"));
		}
		catch (IOException e)
		{
			close();
			showStatus("CONNECT ERROR: "+e.toString());
		}
	}

	public void update(Graphics g) { paint(g); }

	public void paint(Graphics g)
	{
			if (closed == null)
				super.paint(g);
			else
				g.drawImage(closed, 0, 0, this);
	}

	void starter(String type)
	{

		if (type.startsWith(("AppletClient:Kill ")))
		{
			comm.error = type.substring(18);
			disconnect();
		}
		else if (type.equals("AppletClient:Enable"))
		{
			showCard2();
			label.setText(Client.NULL_STRING);
			field.setEditable(false);
		}
	}

	void disconnect()
	{
		label.setText(comm.error);
		showCard3();
		if (card2 != null) card2.removeAll();
		card2 = null;
		comm.stop();
		comm = null;
		try { s.close(); } catch(IOException e){}
		System.gc();
	}

	public String getAppletInfo()
	{
		return "SJChat v1.0 GNU Copyright (C) 1997 Frederick Manligas Nacino";
	}

	public boolean action(Event e, Object o)
	{
		if (e.target == button)
		{
			if (button.getLabel().equals(RECONNECT))
				reconnect();
			else if (button.getLabel().equals(ENTER))
				out.println( field.getText());
			return true;
		}
		return false;
	}

	public void close()
	{
		removeAll();
		closed=getImage(getDocumentBase(),"Library.dir/closed.gif");
		repaint();
	}
}
