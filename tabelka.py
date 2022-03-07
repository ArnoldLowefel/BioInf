seq_1 = "ATTGACTTAAG"
seq_2 = "AGCTAGG"

cols = len(seq_1) + 1
rows = len(seq_2) + 1

tabelka = [[0 for i in range(cols)] for j in range(rows)]

for i in range(cols):
	tabelka[0][i] = i * -2

for i in range(rows):
	tabelka[i][0] = i * -2
	
for i in range(1, rows):
	for j in range(1, cols):
		if seq_1[j-1] == seq_2[i-1]:
			dopasowanie = 2
		else:
			dopasowanie = -1
		tabelka[i][j] = max(tabelka[i-1][j] - 2, tabelka[i][j-1] - 2, tabelka[i-1][j-1] + dopasowanie)

for i in range(rows):
	for j in range(cols):
		spacje = 3 - len(str(tabelka[i][j]))
		for _ in range(spacje):
			print(" ", end="")
		print(str(tabelka[i][j]) + '|', end="")
	print("\n")



