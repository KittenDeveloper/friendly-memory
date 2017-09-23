import math
def reflect(coordinates, axis=None, x=None, y=None):
	if hasattr(coordinates[0], "__iter__"):
		retfigure=[]
		for coords in coordinates:
			retfigure.append(reflect(coords, axis=axis, x=x, y=y))
		return retfigure
	if axis=='y':return (-coordinates[0], coordinates[1])
	elif axis=='x':return (coordinates[0], -coordinates[1])
	elif not x==None:return ((coordinates[0]-(2*(coordinates[0]-x))), coordinates[1])
	elif not y==None:return (coordinates[0], (coordinates[1]-(2*(coordinates[1]-y))))
def rotate(coordinates, degree, clockwise=False):
	if hasattr(coordinates[0], "__iter__"):
		retfigure=[]
		for coords in coordinates:
			retfigure.append(rotate(coords, degree, clockwise=clockwise))
		return retfigure
	x=coordinates[0]
	y=coordinates[1]
	if clockwise:
		if degree==90:return (y,-x)
		elif degree==180:return (-x,-y)
		elif degree==270:return (-y,x)
		elif degree==360:return coordinates
		else: return False
	else:
		if degree==90:return (-y, x)
		elif degree==180:return (-x, -y)
		elif degree==270:return (y, x)
		elif degree==360:return coordinates
		else: return False
def length(coords1, coords2):return math.sqrt(abs((coords1[0]-coords2[0])**2)+abs((coords1[1]-coords2[1])**2))
