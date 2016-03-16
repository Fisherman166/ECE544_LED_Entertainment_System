import xlrd
import sys

def main():

	book = xlrd.open_workbook(sys.argv[1])

	for s in book.sheets():
		print 'menu_image ', s.name, '[] = {'	
		
		for row in range(s.nrows):
			for col in range(s.ncols):
				val = (s.cell(row,col).value)
				if val != '':
					print '{', row, ',', col, ',', val, '},' 
					
		print '};'
		
main()
