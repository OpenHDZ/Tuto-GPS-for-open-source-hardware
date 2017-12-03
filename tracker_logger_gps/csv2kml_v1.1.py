# -*- coding: cp1252 -*-

"""
CSV-2-KML Point Converter
Ecrit par Areour mohamed Cherif
Date : 29/11/2017
E-mail : openhardwaredz@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

"""

import Tkinter as tk
import ttk
import tkFileDialog as tfd
import csv

class Application(tk.Frame):
    
    def __init__(self, parent):
        tk.Frame.__init__(self, parent, borderwidth=10, bg="#FFFFFF")
        self.parent = parent
        self.grid()
        self.parent.title("CSV-2-KML Point Converter")
        self.parent.columnconfigure(0,weight=1)
        self.parent.rowconfigure(0, weight=1)
        self.createWidgets()
    
    def createWidgets(self):
        self.grid(sticky=tk.NSEW)
        
        self.lat_list=[]
        self.lon_list=[]
        self.dist_list=[]
        self.speed_list=[]
        self.alt_list=[]

#-------Begin Attributes--------        
        self.lat= tk.StringVar()
        self.lon= tk.StringVar()
        self.dist= tk.StringVar()
        self.speed= tk.StringVar()
        self.alt= tk.StringVar()
        self.complete = tk.StringVar()
        self.maxSpeed = tk.StringVar()
        self.maxAlt = tk.StringVar()

#-------Begin Methods-----------        
        self.quitButton()
        self.convertButton()
        self.openButton()
        
#-------Begin Widgets-----------
        self.latLabel = tk.Label(self, text="Latitude", background='#FFFFFF')
        self.latLabel.grid(column=0, row=0)
        
        self.lonLabel = tk.Label(self, text="Longitude", background='#FFFFFF')
        self.lonLabel.grid(column=1, row=0)
        
        self.distLabel = tk.Label(self, text="Distance", background='#FFFFFF')
        self.distLabel.grid(column=2, row=0)

        self.speedLabel = tk.Label(self, text="Vitesse", background='#FFFFFF')
        self.speedLabel.grid(column=3, row=0)

        self.altLabel = tk.Label(self, text="Altitude", background='#FFFFFF')
        self.altLabel.grid(column=4, row=0)                
      
        self.scroll = tk.Scrollbar(self, orient=tk.VERTICAL,width=25,command=self.onScroll)

#Latitude Listbox with scrollbar
        self.latList = tk.Listbox(self, exportselection=0, height=15, listvariable=self.lat, yscrollcommand=self.scroll.set)
        self.latList.grid(column=0, row=1, padx=(10,0))
                
#Longitude Listbox with scrollbar
        self.lonList = tk.Listbox(self, exportselection=0, height=15, listvariable=self.lon, yscrollcommand=self.scroll.set)
        self.lonList.grid(column=1, row=1, padx=(10,0))
        
#distance Listbox with scrollbar
        self.distList = tk.Listbox(self, exportselection=0, height=15, listvariable=self.dist, yscrollcommand=self.scroll.set)
        self.distList.grid(column=2, row=1, padx=(10,0))
        
#Speed Listbox with scrollbar
        self.speedList = tk.Listbox(self, exportselection=0, height=15, listvariable=self.speed, yscrollcommand=self.scroll.set)
        self.speedList.grid(column=3, row=1, padx=(10,0))
        
#Altitude Listbox with scrollbar
        self.altList = tk.Listbox(self, exportselection=0, height=15, listvariable=self.alt, yscrollcommand=self.scroll.set)
        self.altList.grid(column=4, row=1, padx=(10,0))
        
        self.scroll.grid(column=5,row=1)
        
#Conversion complete label
        self.convertLabel = tk.Label(self, textvariable=self.complete, background="#FFFFFF")
        self.convertLabel.grid(column=2, row=4, pady=(10,0))
		
        self.maxSpeedLabel = tk.Label(self, textvariable=self.maxSpeed, background="#FFFFFF",justify=tk.LEFT)
        self.maxSpeedLabel.grid(column=0, row=2, pady=(10,0))
        self.maxSpeed.set("Vitesse max :")

        self.maxAltLabel = tk.Label(self, textvariable=self.maxAlt, background="#FFFFFF",justify=tk.LEFT)
        self.maxAltLabel.grid(column=0, row=3, pady=(10,0))
        self.maxAlt.set("Altitude max :")
        
#-------Begin Method Definitions
    def onScroll(self, *args):
        self.latList.yview(*args)
        self.lonList.yview(*args)
        self.distList.yview(*args)
        self.speedList.yview(*args)
        self.altList.yview(*args) 
    
    def quitButton(self):   
        self.quit = tk.Button(self, text='Quit', width=7, background="#363FD3", fg="#FFFFFF", relief=tk.FLAT, command=self.quit)
        self.quit.grid(column=0, row=4, pady=(10,0), padx=(10,0), sticky=tk.W)  
   
    def openButton(self):                
        self.csvFile = tk.Button(self, text='Load', width=7, background="#363FD3", fg="#FFFFFF", relief=tk.FLAT, command=self.openCsv)
        self.csvFile.grid(column=3, row=4, columnspan=3, pady=(10,0), padx=(0,22))
    
    def convertButton(self):    
        self.convert = tk.Button(self, text='Convert', background="#363FD3", fg="#FFFFFF", relief=tk.FLAT, command=self.kmlWriter, width=7)
        self.convert.grid(column=4, row=4, columnspan=2, pady=(10,0), padx=(10,0), sticky=tk.E)

    def openCsv(self):
        self.inputCSV = open(tfd.askopenfilename(filetypes=[('csv Files','*.csv')]),'rb')
        self.csvFile = csv.reader(self.inputCSV)
                
        for row in self.csvFile :
            self.lat_list.append(row[0])
            self.lon_list.append(row[1])
            self.dist_list.append(row[2])
            self.speed_list.append(row[3])
            self.alt_list.append(row[4])

        
        self.lat.set(tuple(self.lat_list))
        self.lon.set(tuple(self.lon_list))
        self.dist.set(tuple(self.dist_list))
        self.speed.set(tuple(self.speed_list))
        self.alt.set(tuple(self.alt_list))
        
        self.maxSpeed.set("Vitesse max : "+str(self.speedMax())+" km/h")
        self.maxAlt.set("Altitude max : "+str(self.altMax())+" m")
        
    def speedMax(self):
        maxSpeed = 0
        
        for i in self.speed_list:
            if float(i) >= maxSpeed :
                maxSpeed = float(i)       
        return maxSpeed

    def altMax(self):
        maxAlt = 0

        for i in self.alt_list:
            if float(i) >= maxAlt :
                maxAlt = float(i)     
        return maxAlt

    def distance(self):
        distF,distT=0,0
        dT = []
        for i in self.dist_list:
            if float(i) > 1.0:
            	dT.append(abs(distT-float(i)))
            	distT = float(i)
            
        for j in dT: 
            distF = distF + j 
            
        return distF
    

    def kmlWriter(self):
        
        taillData = len(self.lon_list)
        
        self.kmlFile = tfd.asksaveasfile(mode='w', defaultextension='*.kml', filetypes=[('kml File', '*.kml')])
        self.kmlFile.write('<?xml version="1.0" encoding="UTF-8"?>\n<kml xmlns="http://www.opengis.net/kml/2.2">\n\t<Document>\n')
        self.kmlFile.write('\t\t<name>Paths</name>\n')
        self.kmlFile.write('\t\t<description>Data taken by an Arduino board and a GPS sensor</description>\n')
        self.kmlFile.write('\t\t<Style id="yellowLineGreenPoly">\n')
        self.kmlFile.write('\t\t\t<LineStyle>\n\t\t\t\t<color>7f00ffff</color>\n\t\t\t\t<width>20</width>\n\t\t\t</LineStyle>\n\t\t\t<PolyStyle>\n\t\t\t\t<color>7f00ff00</color>\n\t\t\t</PolyStyle>\n\t\t</Style>\n')
        self.kmlFile.write('\t\t<Placemark>\n\t\t\t<name>Absolute Extruded</name>\n\t\t\t<description>Transparent green wall with yellow outlines</description>\n\t\t\t<styleUrl>#yellowLineGreenPoly</styleUrl>\n\t\t\t<LineString>\n\t\t\t\t<extrude>1</extrude>\n\t\t\t\t<tessellate>1</tessellate>\n\t\t\t\t<altitudeMode>absolute</altitudeMode>\n')
        self.kmlFile.write('\t\t\t\t<coordinates>\n')	

        for index in range(taillData): 
            alt=4000.0-float(self.alt_list[index])      
            self.kmlFile.write('\t\t\t\t\t'+self.lon_list[index] +','+ self.lat_list[index] + ','+str(alt)+'\n')
        
        self.kmlFile.write('\t\t\t\t</coordinates>\n')
        self.kmlFile.write('\t\t\t</LineString>\n')
        self.kmlFile.write('\t\t</Placemark>\n')
        self.kmlFile.write('\t</Document>\n</kml>')
        self.kmlFile.close()
        #Updates the label to indicate conversion is complete. 
        self.complete.set("Conversion Complete!")
def main():
        root = tk.Tk()
        app = Application(root)
        root.mainloop()
        
if __name__ == '__main__':
    main()
