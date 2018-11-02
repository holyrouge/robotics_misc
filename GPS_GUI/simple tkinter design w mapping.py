from tkinter import *
import tkinter.ttk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import time
import numpy as np

from matplotlib import style
import threading

f = Figure(figsize = (5, 5),dpi=100)
#a = f.add_subplot(111)
#a.autoscale()

#a2 = f.add_subplot(111)
#a2.autoscale()

f, ax = plt.subplots() #adding a subplot to figure f


def input_lat_and_long(lat, long):
    xList = []
    yList = []
    if lat == "":
        lat = "0.0"
    if long == "":
        long = "0.0"
    lat = float(lat)
    long = float(long)

    xList.append(long)
    yList.append(lat)
    #a.plot(xList, yList, 's')

    objpt = ax.scatter(xList, yList)
    #objpt


def animate(self):  # #function for animating the GPS coordinate movement
    #global count
    pullData = open("/Users/Sean/Documents/newOutput1.dat", "r").read()
    dataList = pullData.split('\n')

    xList1 = []
    yList1 = []

    #ax.clear()

    for eachLine in dataList:
        #eachLine.strip()
        x, y, z, z2 = eachLine.split("  ")
        xList1.append(float(x))
        yList1.append(float(y))
    #ax.clear()

    paths = ax.scatter(xList1, yList1)
    paths



    #return paths

    #anistop = movement.animate.after(1000, paths.remove())
    #anistop
        #a.clear()
        #a.plot(xList, yList, 's')   # #plotting the points in xList, yList (the ones in the .dat file;
                                    # #there should be 1 x value and 1 y value in this file at all times because
                                   # #we are only simulating movement of the rover's x-y coordinates (the s is for "scatter"
    #paths.subplot(111)

'''def reset(self):
    paths = animate
    paths.remove()
    paths'''

class obj_pt_set:
    def __init__(self, theFrame, title, rowN, columnN):
        self.textLabel = Label(theFrame, text=title)
        self.x_entry = Entry(theFrame)
        self.x_entry_title = Label(theFrame, text="x: ")

        if self.x_entry == "":
            self.x_entry = "0.0"

        self.y_entry = Entry(theFrame)
        self.y_entry_title = Label(theFrame, text="y: ")

        if self.y_entry == "":
            self.y_entry = "0.0"

        self.button = Button(theFrame, text="Submit", command=lambda: input_lat_and_long(self.y_entry.get(), self.x_entry.get()))

        self.textLabel.grid(row=rowN, column=columnN, columnspan=2,  padx=5, pady=5)

        self.x_entry.grid(row=rowN+1, column=columnN+1)
        self.x_entry_title.grid(row=rowN + 1, column=columnN, padx=5, pady=5)

        self.y_entry.grid(row=rowN+2, column=columnN+1, padx=5, pady=5)
        self.y_entry_title.grid(row=rowN + 2, column=columnN)

        self.button.grid(row=rowN+3, column=columnN+1, padx=5, pady=5)


root = Tk()
root.geometry("1280x800")

root.configure(bg="grey")

myCanvas = Canvas(root, width=640, height=480)
myFrame = Frame(root, width=1000, height=240, bg="black")
myCanvas2 = Canvas(root, width=360, height=240, bg="#508AAF")
myCanvas3 = Canvas(root, width=360, height=240, bg="#A18D5E")
myGraph = FigureCanvasTkAgg(f, master=myCanvas)
myGraph.get_tk_widget().pack(side="top", fill='both', expand=True)

obj_pt1 = obj_pt_set(myFrame, "Objective Point 1", 0, 0)
obj_pt2 = obj_pt_set(myFrame, "Objective Point 2", 3, 0)
obj_pt3 = obj_pt_set(myFrame, "Objective Point 3", 6, 0)
tkinter.ttk.Separator(myFrame, orient=VERTICAL).grid(column=2, row=0, rowspan=9, sticky='ns')
obj_pt4 = obj_pt_set(myFrame, "Objective Point 4", 0, 3)
obj_pt5 = obj_pt_set(myFrame, "Objective Point 5", 3, 3)
obj_pt5 = obj_pt_set(myFrame, "Objective Point 6", 6, 3)


myCanvas.grid(row=0, column=0, columnspan=2, rowspan=2)
myFrame.grid(row=2, column=0, columnspan=2)
myCanvas2.grid(row=0, column=2)
myCanvas3.grid(row=1, column=2)


ani = animation.FuncAnimation(f, animate, interval=1000)
#ani2 = animation.FuncAnimation(f, reset, interval=1000)


#aniReset = animation.FuncAnimation(f, reset, interval=1000)
root.mainloop()

