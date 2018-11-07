from tkinter import *
import tkinter.ttk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import matplotlib as matplotlib
import matplotlib.animation as animation
import matplotlib.pyplot as plt
from matplotlib import style
import math


f = Figure(figsize = (5, 5),dpi=100)
matplotlib.style.use('dark_background')

img = plt.imread("hanksville_edit.png")

f, ax = plt.subplots() #adding a subplot to figure f
f.add_subplot(111, frameon=False)
plt.tick_params(labelcolor='none', top='off', bottom='off', left='off', right='off')
plt.grid(False)
plt.xlabel("LONGITUDE (E/W)")
ylbl = plt.ylabel("LATITUDE (N/S)")
ylbl.set_position([-.2, .5])
plt.title("GPS Coordinates")
plt.subplots_adjust(left=.125, right=0.9, top=.9, bottom=.1)


color_count = 0

ax.imshow(img, aspect= "auto", extent=[0, 10000, 0, 10000])
ax.autoscale(True)
ax.set_ylim([0,10000])
ax.set_xlim([0,10000])

#color_string = "black"

count = 0

xtemp = 0  # old values of x and y for the rover is stored here while the rover clears the plot and waits for new point
ytemp = 0  # could do this for objective points too, probably should at some point with an array for obj pts.

objPtTempX= [0, 0, 0, 0, 0, 0]      # May use these arrays at some point to draw dotted lines from the rover
objPtTempY = [0, 0, 0, 0, 0, 0]     # location to each objective points (haven't done yet)



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
        color_string = "#0AF524"
    if(color_count == 6):
        color_string = "#42DAED"
        color_count = 0

    objPtTempX.append(long)
    objPtTempY.append(lat)

    objpt = ax.scatter(xList, yList, color=color_string)
    objpt

def connectpoints(x,y,p1,p2):
    x1, x2 = x[p1], x[p2]
    y1, y2 = y[p1], y[p2]
    plt.plot([x1,x2],[y1,y2],'k-')

def animate(self):  # function for animating the GPS coordinate movement
    global count
    #ax.imshow(img, extent=[0, 10000, 0, 10000])

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
    count =count + 1

    if (count > 20):
        ax.clear()

        ax.imshow(img, aspect = "auto", extent=[0, 10000, 0, 10000])
        count = 0
        ax.set_ylim([0, 10000])
        ax.set_xlim([0, 10000])
        ax.scatter(xtemp,ytemp, color="white")

        pt1Stats = objPtStats(pointStatsFrame, 1, float(obj_pt1.x_entry.get()), float(obj_pt1.y_entry.get()), xtemp,
                              ytemp,
                              0, 1, "#42DAED")
        pt2Stats = objPtStats(pointStatsFrame, 2, float(obj_pt2.x_entry.get()), float(obj_pt2.y_entry.get()), xtemp,
                              ytemp,
                              1*4, 1, "blue")
        pt3Stats = objPtStats(pointStatsFrame, 3, float(obj_pt3.x_entry.get()), float(obj_pt3.y_entry.get()), xtemp,
                              ytemp,
                              2 * 4, 1, "red")
        pt4Stats = objPtStats(pointStatsFrame, 4, float(obj_pt4.x_entry.get()), float(obj_pt4.y_entry.get()), xtemp,
                              ytemp,
                              3 * 4, 1, "purple")
        pt5Stats = objPtStats(pointStatsFrame, 5, float(obj_pt5.x_entry.get()), float(obj_pt5.y_entry.get()), xtemp,
                              ytemp,
                              4 * 4, 1, "orange")
        pt6Stats = objPtStats(pointStatsFrame, 6, float(obj_pt6.x_entry.get()), float(obj_pt6.y_entry.get()), xtemp,
                              ytemp,
                              5 * 4, 1, "#0AF524")

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
    def __init__(self, theFrame, title, rowN, columnN, color_picked):

        self.textLabel = Label(theFrame, text=title, bg = color_picked, fg = "white")

        self.x_entry = Entry(theFrame)
        self.x_entry_title = Label(theFrame, text="x: ", bg = color_picked, fg = "white")
        self.x_entry.insert(0, "0")

        self.y_entry = Entry(theFrame)
        self.y_entry_title = Label(theFrame, text="y: ", bg = color_picked, fg="white")
        self.y_entry.insert(0, "0")

        self.button = Button(theFrame, text="Submit", command= lambda:input_lat_and_long(self.y_entry.get(), self.x_entry.get()))

        self.textLabel.grid(row=rowN, column=columnN, columnspan=2,  padx=5, pady=5)

        self.x_entry.grid(row=rowN+1, column=columnN+1)
        self.x_entry_title.grid(row=rowN + 1, column=columnN, padx=5, pady=5)

        self.y_entry.grid(row=rowN+2, column=columnN+1, padx=5, pady=5)
        self.y_entry_title.grid(row=rowN + 2, column=columnN)


class objPtStats:
    def __init__(self, frame, pointNum, xCoordPt, yCoordPt, xCoordRover, yCoordRover, rowN, columnN, color_picked):
        idealAngle = 0
        eucDist = 0
        if(float(xCoordPt) >=0 and float(xCoordRover)>=0 and float(yCoordPt)>0 and float(yCoordRover)>=0):
            idealAngle = math.atan((yCoordPt-yCoordRover)/(xCoordPt-xCoordRover)) * (180/3.14159)
            idealAngle = round(idealAngle, 2)

            eucDist = math.sqrt(((xCoordPt-xCoordRover)*(xCoordPt-xCoordRover) + (yCoordPt-yCoordRover)*yCoordPt-yCoordRover))
            eucDist = round(eucDist, 2)

        self.textLabel = Label(frame, text=("P" + str(pointNum)), bg = color_picked)
        self.distToRover = Label(frame, text = eucDist, bg = color_picked)
        self.idealAngle =  Label(frame, text = ("ideal ∠: " + str(idealAngle)), bg = color_picked)


        self.textLabel.grid(row=rowN, column=columnN)
        self.distToRover.grid(row=rowN+1, column=columnN)
        self.idealAngle.grid(row=rowN+2, column=columnN, pady=5)



def pressButtons(self):
    self.button.invoke()

root = Tk()
root.geometry("1400x800")


root.configure(bg="#404040")

myCanvas = Canvas(root, width=640, height=480, borderwidth=3, highlightbackground="white", highlightthickness=3)
myFrame = Frame(root, borderwidth=10, width=1000, height=240, bg="grey")
myFrame.config(highlightbackground="black", highlightthickness=5)
myCanvas2 = Canvas(root, width=360, height=240, bg="#508AAF")
myCanvas3 = Canvas(root, width=360, height=240, bg="#A18D5E")
pointStatsFrame = Canvas(root, width=190, height=480, bg="grey")
myGraph = FigureCanvasTkAgg(f, master=myCanvas)
myGraph.get_tk_widget().pack(side="top", fill='both', expand=True)

obj_pt1 = obj_pt_set(myFrame, "Objective Point 1", 0, 0, "#42DAED")

obj_pt2 = obj_pt_set(myFrame, "Objective Point 2", 4, 0, "blue")
tkinter.ttk.Separator(myFrame, orient=VERTICAL).grid(column=2, row=0, rowspan=9, sticky='ns')

obj_pt3 = obj_pt_set(myFrame, "Objective Point 3", 0, 3, "red")
obj_pt4 = obj_pt_set(myFrame, "Objective Point 4", 4, 3, "purple")
tkinter.ttk.Separator(myFrame, orient=VERTICAL).grid(column=5, row=0, rowspan=9, sticky='ns')

obj_pt5 = obj_pt_set(myFrame, "Objective Point 5", 0, 6, "orange")
obj_pt6 = obj_pt_set(myFrame, "Objective Point 6", 4, 6, "#0AF524")
tkinter.ttk.Separator(myFrame, orient=HORIZONTAL).grid(column=0, row=3, columnspan=9, sticky='ew')

pt1Stats = objPtStats(pointStatsFrame, 1, float(obj_pt1.x_entry.get()), float(obj_pt1.y_entry.get()), xtemp,
                      ytemp,
                      0, 1, "#42DAED")
pt2Stats = objPtStats(pointStatsFrame, 2, float(obj_pt2.x_entry.get()), float(obj_pt2.y_entry.get()), xtemp,
                      ytemp,
                      1 * 4, 1, "blue")
pt3Stats = objPtStats(pointStatsFrame, 3, float(obj_pt3.x_entry.get()), float(obj_pt3.y_entry.get()), xtemp,
                      ytemp,
                      2 * 4, 1, "red")
pt4Stats = objPtStats(pointStatsFrame, 4, float(obj_pt4.x_entry.get()), float(obj_pt4.y_entry.get()), xtemp,
                      ytemp,
                      3 * 4, 1, "purple")
pt5Stats = objPtStats(pointStatsFrame, 5, float(obj_pt5.x_entry.get()), float(obj_pt5.y_entry.get()), xtemp,
                      ytemp,
                      4 * 4, 1, "orange")
pt6Stats = objPtStats(pointStatsFrame, 6, float(obj_pt6.x_entry.get()), float(obj_pt6.y_entry.get()), xtemp,
                      ytemp,
                      5 * 4, 1, "#0AF524")

myCanvas.grid(row=0, column=0, columnspan=2, rowspan=2)
pointStatsFrame.grid(row=0, column=2, rowspan=2, columnspan=1, padx=4, pady=4)
myFrame.grid(row=2, column=0, columnspan=1)
myCanvas2.grid(row=0, column=3)
myCanvas3.grid(row=1, column=3)

ani = animation.FuncAnimation(f, animate, interval=100)

root.mainloop()
