from tkinter import *
import tkinter.ttk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import matplotlib as matplotlib
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import time
import numpy as np

from matplotlib import style
import threading

f = Figure(figsize = (5, 5),dpi=100)
matplotlib.style.use('dark_background')


f, ax = plt.subplots() #adding a subplot to figure f
color_count = 0
ax.set_ylim([0,10000])
ax.set_xlim([0,10000])
#color_string = "black"

count = 0

xtemp = 0  # old values of x and y for the rover is stored here while the rover clears the plot and waits for new point
ytemp = 0  # could do this for objective points too, probably should at some point with an array for obj pts.

objPtTempX= [0, 0, 0, 0, 0, 0]
objPtTempY = [0, 0, 0, 0, 0, 0]



def input_lat_and_long(lat, long):
    xList = []
    yList = []
    if lat == "":
        return
    if long == "":
        return
    lat = float(lat)
    long = float(long)

    xList.append(long)
    yList.append(lat)
    #a.plot(xList, yList, 's')

    global color_count

    if (color_count == 0):
        color_string = "green"
    if (color_count == 1):
        color_string = "blue"
    if (color_count == 2):
        color_string = "red"
    if (color_count == 3):
        color_string = "purple"
    if (color_count == 4):
        color_string = "orange"
    if (color_count == 5):
        color_string = "yellow"
    if(color_count == 6):
        color_string = "grey"
        color_count = 0

    objPtTempX.append(long)
    objPtTempY.append(lat)

    objpt = ax.scatter(xList, yList, color=color_string)
    objpt


def animate(self):  # #function for animating the GPS coordinate movement
    global count

    pullData = open("/Users/Sean/Documents/newOutput1.dat", "r").read()
    dataList = pullData.split('\n')
    xList1 = []
    yList1 = []

    global xtemp
    global ytemp

    for eachLine in dataList:
        if len(eachLine) > 1:
            x, y, z, z2 = eachLine.split('  ')
            xList1.append(float(x))
            yList1.append(float(y))
            xtemp = float(x)
            ytemp = float(y)

    paths = ax.scatter(xList1, yList1, color="white")

    paths

    global color_count
    obj_pt1.button.invoke()
    color_count = 1
    obj_pt2.button.invoke()
    color_count = 2
    obj_pt3.button.invoke()
    color_count = 3
    obj_pt4.button.invoke()
    color_count = 4
    obj_pt5.button.invoke()
    color_count = 5
    obj_pt6.button.invoke()
    color_count = 6

    count =count + 1

    if (count > 20):
        ax.clear()
        count = 0
        ax.set_ylim([0, 10000])
        ax.set_xlim([0, 10000])
        ax.scatter(xtemp,ytemp, color="white")

        obj_pt1.button.invoke()
        color_count = 1
        obj_pt2.button.invoke()
        color_count = 2
        obj_pt3.button.invoke()
        color_count = 3
        obj_pt4.button.invoke()
        color_count = 4
        obj_pt5.button.invoke()
        color_count = 5
        obj_pt6.button.invoke()
        color_count = 6


class obj_pt_set:
    def __init__(self, theFrame, title, rowN, columnN):
        self.textLabel = Label(theFrame, text=title)

        self.x_entry = Entry(theFrame)
        self.x_entry_title = Label(theFrame, text="x: ")
        self.x_entry.insert(0, "0")

        self.y_entry = Entry(theFrame)
        self.y_entry_title = Label(theFrame, text="y: ")
        self.y_entry.insert(0, "0")


        self.button = Button(theFrame, text="Submit", command= lambda:input_lat_and_long(self.y_entry.get(), self.x_entry.get()))


        self.textLabel.grid(row=rowN, column=columnN, columnspan=2,  padx=5, pady=5)

        self.x_entry.grid(row=rowN+1, column=columnN+1)
        self.x_entry_title.grid(row=rowN + 1, column=columnN, padx=5, pady=5)

        self.y_entry.grid(row=rowN+2, column=columnN+1, padx=5, pady=5)
        self.y_entry_title.grid(row=rowN + 2, column=columnN)

        self.button.grid(row=rowN+3, column=columnN+1, padx=5, pady=5)

def pressButtons(self):
    self.button.invoke()

root = Tk()
root.geometry("1280x800")


root.configure(bg="#404040")

myCanvas = Canvas(root, width=640, height=480)
myFrame = Frame(root, borderwidth=10, width=1000, height=240, bg="grey")
myFrame.config(highlightbackground="black", highlightthickness=5)
myCanvas2 = Canvas(root, width=360, height=240, bg="#508AAF")
myCanvas3 = Canvas(root, width=360, height=240, bg="#A18D5E")
myGraph = FigureCanvasTkAgg(f, master=myCanvas)
myGraph.get_tk_widget().pack(side="top", fill='both', expand=True)

obj_pt1 = obj_pt_set(myFrame, "Objective Point 1", 0, 0)
obj_pt2 = obj_pt_set(myFrame, "Objective Point 2", 4, 0)
tkinter.ttk.Separator(myFrame, orient=VERTICAL).grid(column=2, row=0, rowspan=9, sticky='ns')

obj_pt3 = obj_pt_set(myFrame, "Objective Point 3", 0, 3)
obj_pt4 = obj_pt_set(myFrame, "Objective Point 4", 4, 3)
tkinter.ttk.Separator(myFrame, orient=VERTICAL).grid(column=5, row=0, rowspan=9, sticky='ns')

obj_pt5 = obj_pt_set(myFrame, "Objective Point 5", 0, 6)
obj_pt6 = obj_pt_set(myFrame, "Objective Point 6", 4, 6)


myCanvas.grid(row=0, column=0, columnspan=2, rowspan=2)
myFrame.grid(row=2, column=0, columnspan=2)
myCanvas2.grid(row=0, column=2)
myCanvas3.grid(row=1, column=2)

ani = animation.FuncAnimation(f, animate, interval=10)

root.mainloop()

