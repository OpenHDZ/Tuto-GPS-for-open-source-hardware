"""
Programme qui est utilisé pour télécharger une carte et afficher la position des
GPS sur celle-ci en utilisant Google Map.

Ecrit par Areour mohamed Cherif
Date : 12/04/2017
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

import Tkinter
import urllib
import base64
import serial

largeur = 640
hauteur = 480
zoom = 4

ser = serial.Serial("/dev/ttyACM0",9600)

def getGPS():
	#fonction qui récupére les coord depuis l'arduino
	ser.flushInput()
	donnee = str(ser.readline())
	donnee = donnee.strip("\r\n")
	donnee = donnee.split(",")
	if (len(donnee) == 2):	
		return donnee

def getaddress(latitude, longitude, largeur, hauteur, zoom):
    #fonction qui crée la requette http pour Api google map  
	address = "http://maps.googleapis.com/maps/api/staticmap?center={0},{1}&zoom={2}&size={3}x{4}&format=gif&sensor=false&markers=color:blue%7Clabel:M%7C{0},{1}"\
	.format(latitude, longitude, zoom, largeur, hauteur)
	return address

def getmap(latitude, longitude, largeur, hauteur, zoom):
	#fonction qui va télécharger l'image et l'adaptée pour quelle soit affichée
	address = getaddress(latitude, longitude, largeur, hauteur, zoom)
	urlreader=urllib.urlopen(address)
	data = urlreader.read()
	base64data = base64.encodestring(data)
	image = Tkinter.PhotoImage(data=base64data)
	return image



window=Tkinter.Tk()
window.title("Open Hardware Algerie")

coord = getGPS()

latitude, longitude =coord[0],coord[1] 

mapimage = getmap(latitude, longitude, largeur, hauteur, zoom)

canvas = Tkinter.Canvas(window, width=largeur, height=hauteur)
canvas.create_image(0,0,image=mapimage,anchor=Tkinter.NW)
canvas.pack()

window.mainloop()    
