import math
def reflect(coordinates, axis=None, x=None, y=None):
	if hasattr(coordinates[0], "__iter__"):
		retfigure=[]
		for x in coordinates:
			retfigure.append(reflect(x, axis, x, y))
		return retfigure
	if axis=='y':return (-coordinates[0], coordinates[1])
	elif axis=='x':return (coordinates[0], -coordinates[1])
	elif not x==None:return ((coordinates[0]-(2*(coordinates[0]-x))), coordinates[1])
	elif not y==None:return (coordinates[0], (coordinates[1]-(2*(coordinates[1]-y))))
def rotate(coordinates, degree, clockwise=False):pass	
def length(coords1, coords2):return math.sqrt(abs((coords1[0]-coords2[0])**2)+abs((coords1[1]-coords2[1])**2))
