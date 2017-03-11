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

public class Client extends Panel
{
	Init applet;

	PrintStream out;

	private Comm comm;
	private DataInputStream in;

	private TextField inputfield = new TextField(40);
	private TextScroll outputarea = new TextScroll();

	private List list = new List();
	private Label sysmsg;

	private Vector people = new Vector();

	private Panel cardPeople = new Panel();

	private Label enter = new Label("Enter Message:");
	private Label here = new Label("People Online");

	private Button leave = new Button("Logout");

	public final static String NULL_STRING = "";
	public final static String ONE_SPACE = " ";
	public final static String THREE_SPACE = "   ";

	public final static String CENTER = "Center";
	public final static String SOUTH = "South";
	public final static String NORTH = "North";
	public final static String EAST = "East";
	public final static String WEST = "West";

	Client(Init applet, DataInputStream in, PrintStream out, Comm comm)
	{
		this.applet = applet;
		this.in = in;
		this.out = out;
		this.comm = comm;

		setBackground(Color.white);
		setLayout(new BorderLayout(3, 3));

		comm.output = outputarea;

		Panel enterPanel = new Panel();
		enterPanel.setLayout(new BorderLayout(0, 0));
		enter.setForeground(Color.black);
		enterPanel.add(WEST, enter);
		enterPanel.add(CENTER, inputfield);

		Panel centerPanel = new Panel();
		inputfield.setBackground(Color.white);
		centerPanel.setLayout(new BorderLayout(0, 0));
		centerPanel.add(CENTER, outputarea);
		centerPanel.add(SOUTH, enterPanel);

		Panel rightPanel = new Panel();

		list.setBackground(Color.white);
		list.setMultipleSelections(false);

		rightPanel.setLayout(new BorderLayout(0, 0));
		here.setForeground(Color.black);
		rightPanel.add(NORTH, here);
		rightPanel.add(CENTER, list);

		sysmsg = new Label(THREE_SPACE+applet.getAppletInfo());

		Panel northPanel = new Panel();
		northPanel.setLayout(new FlowLayout(FlowLayout.LEFT, 3, 3));
		leave.setBackground(Color.lightGray);
		northPanel.add(leave);

		add(CENTER, centerPanel);
		add(EAST, rightPanel);
		add(NORTH, northPanel);
		add(SOUTH, sysmsg);

		comm.client = this;
	}

	public boolean action(Event e, Object o)
	{
		if (e.target == inputfield)
		{
			if (inputfield.getText().equals(NULL_STRING))
			{
				addMsg(applet.getAppletInfo());
				return true;
			}

			out.println("say "+inputfield.getText());
			inputfield.setText(NULL_STRING);
			return true;
		}

		if (e.target == leave)
		{
			out.println("logout");
			return true;
		}
		if (e.target == list)
		{
			list.deselect(list.getSelectedIndex());
			return true;
		}
		return false;
	}

	public boolean mouseEnter(Event e, int x, int y)
	{
			inputfield.requestFocus();
			return true;
	}

	public final static
		void switchContainer(String pos, Component comp, Container cont)
	{
		if (comp.getParent() != null)
			comp.getParent().remove(comp);

		if (pos == null)
			cont.add(comp);
		else
			cont.add(pos, comp);
	}

	boolean interpret(String argument)
	{
		if (argument.startsWith(("-- ")))
		{
			addMsg(argument.substring(3));
			return true;
		}
		if (!argument.startsWith("AppletClient:"))
			return false;

		argument = argument.substring(13);

		if (argument.startsWith("AddPerson "))
		{
			applet.play(applet.getDocumentBase(),
							"Library.dir/drip.au");
			people.addElement(argument.substring(10));
			list.addItem(argument.substring(10));

			return true;
		}
		if (argument.startsWith("RemovePerson "))
		{
			removeFromVector(people, argument.substring(13));
			removeFromList(argument.substring(13));

			return true;
		}
		if (argument.equals("Say"))
		{
			addMsg(applet.getAppletInfo());
			return true;
		}
		return false;
	}

	private void removeFromVector(Vector vector, String string)
	{
		for(int i = 0; i < vector.size(); i++)
			if (string.equals((String)vector.elementAt(i)))
			{
				vector.removeElementAt(i);
				return;
			}
	}

	private void removeFromList(String string)
	{
		for (int i = 0; i < list.countItems(); i++)
			if (string.equals(list.getItem(i)))
			{
				list.delItem(i);
				return;
			}
	}

	private void addMsg(String message)
	{
		String msg = THREE_SPACE+message;
		if (sysmsg.getText().equals(msg)) return;
		applet.play(applet.getDocumentBase(), "Library.dir/drip.au");
		sysmsg.setText(msg);
	}
}
