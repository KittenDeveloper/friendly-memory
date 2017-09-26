class String(object):
	def __init__(self, val=None):
		if in_str!=None:self.set(str(val))
		self.chars=[]
	def __eq__(self, other):return str(other)==str(self)
	def __ne__(self, other):return not str(other)==str(self)
	def set(self, nstr):
		self.chars=[]
		for x in nstr:
			self.chars.append(x)
	def __str__(self):return ''.join(self.chars)
	def concat(self, nstr):
		for x in nstr:
			self.chars.append(x)
	def append(self, other):self.concat(str(other))
	def prepend(self, in_str):
		self.chars=(str(in_str)+str(self)).split('')
	def __iter__(self):
		for x in self.chars:
			yield x
	def insert(self, x, y):for z in str(y):chars.insert(x,z)
	def join(self, in_seq):str(self).join(in_seq)
	def split(self, other):return str(self).split(str(other))
	def tostr(self):return str(self)
	def __getitem__(self, key):return str(self)[key]
	def __setitem__(self, key, value):self.chars[key]=value
	def __delitem__(self, key):del self.chars[key]
	def __add__(self, other):return (str(self)+str(other))
	def __radd__(self, other):return (str(in_str)+str(self))
	def __iadd__(self, other):self.concat(str(other))
	def __mul__(self, other):
		other=int(other)
		return str(self)*other
	def __imul__(self, other):
		other=int(other)
		self.set(str(self)*other)
	def __sub__(self, other):
		if type(other)!=int:other=int(other)
		return str(self)[:-other]
	def __isub__(self, other):
		if type(other)!=int:
			other=int(other)
		a=[]
		for x in range(other):a.append(self.chars.pop())
		return a
	def __div__(self, other):
		other=str(other)
		return str(self).split(other)
	def __idiv__(self, other):raise NotImplementedError
	def __rdiv__(self, other):
		try:selfnum=eval(str(self))
		except NameError:raise TypeError("unsupported operand type(s) for /: 'int' and 'str'")
		return other/selfnum
