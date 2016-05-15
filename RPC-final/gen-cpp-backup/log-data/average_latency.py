import sys

def main():
	
	name = sys.argv[1]
	f = open(name, 'r')
	avg = 0.0
	sum_t = 0.0
	count = 0
	for line in f:
		#print line[1]
		n = int(line)
		sum_t += n
		count += 1
	print sum_t
	print count
	print sum_t/count



if __name__ == "__main__":
   main()