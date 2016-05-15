import sys

def main():
	
	name = sys.argv[1]
	f = open(name, 'r')
	miss = 0.0
	hit = 0.0
	for line in f:
		#print line[1]
		if line[:3] == "Hit":
			hit += 1
		else:
			miss += 1
	print hit
	print miss
	print hit/(hit+miss)



if __name__ == "__main__":
   main()