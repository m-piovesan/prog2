import subprocess
import random
import sys

if (sys.argv[1] == "make"):
	SOURCES = "make"
else:
	SOURCES = "gcc -Wall " + "".join([sys.argv[i] + " " for i in range(1, len(sys.argv))]) + " -o A3"

TESTS_COMP = ["./A3 -x 4 -y 4 -e 1 -r 10 -o output.txt", "./A3 -x 6 -y 6 -e 3 -r 10 -o output.txt", "./A3 -x 4 -y 4 -e 4 -r 10 -o output.txt", "./A3 -x 1 -y 1 -e 1 -r 10 -o output.txt", "./A3 -x 10 -y 5 -e 3 -r 10 -o output.txt", "./A3 -x 1 -y 5 -e 3 -r 10 -o output.txt", "./A3 -x 4 -y 4 -e 5 -r 10 -o output.txt", "./A3 -x 0 -y 0 -e 1 -r 10 -o output.txt"]
TESTS_DIFF = [(["output.txt"], ["1.1.txt"]), (["output.txt"], ["1.2.txt"]), (["output.txt"], ["2.1.txt"]), (["output.txt"],["2.2.txt"]), (["output.txt"],["3.1.txt"]), (["output.txt"], ["3.2.txt"]), ([],[]), ([],[])]

print("\n##================== TESTE: Compilação ==================##\n")
try:
	if (sys.argv[1] == "make"):
		res_compile = subprocess.call(["make"], stderr=subprocess.PIPE)
	else:
		res_compile = subprocess.check_output(SOURCES, shell=True)
except:	
	print("ERRO FATAL: O PROGRAMA NÃO COMPILOU!")
	exit(1)
print("\n##===========================================================##\n")

t_i = 0
for t_exec in TESTS_COMP:
	result = None
	try:
		print("\n##================== TESTE: " + t_exec + " ==================##\n")
		try:
			result = subprocess.check_output(t_exec, shell=True, timeout=5)
		except subprocess.CalledProcessError as grepexc:                                                                                                   
			print("Código de erro retornado pelo programa: ", grepexc.returncode, "\n")
		except Exception as e:
			print("Parada por timeout! O teste não foi concluído.\n")
		print(str(result, 'utf-8', 'ignore'))
	except Exception as e:
		if result != None:
			try:
				print(str(result, 'utf-8', 'ignore'))
			except:
				print(result)
	for t_j in range(len(TESTS_DIFF[t_i][0])):
		flag1 = False
		flag2 = False
		print("=> TESTE DE DIFERENCIAÇÃO (EXISTE NO ARQUIVO GERADO, MAS NÃO NA REFERÊNCIA): ", TESTS_DIFF[t_i][0][t_j], " x ", TESTS_DIFF[t_i][1][t_j], "\n")
		try:
			result = subprocess.check_output("grep -vxFf " + TESTS_DIFF[t_i][1][t_j] + " " + TESTS_DIFF[t_i][0][t_j], shell=True)
			try:
				print(str(result, 'utf-8', 'ignore'))
			except:
				print(result)
		except subprocess.CalledProcessError as grepexc:                                                                                                   
			if (grepexc.returncode == 1):
				flag1 = True
				print("OK!\n")
			else:
				print("Código de erro retornado pelo grep: ", grepexc.returncode, "\n")
		print("=> TESTE DE DIFERENCIAÇÃO (EXISTE NA REFERÊNCIA, MAS NÃO NO ARTIGO GERADO): ", TESTS_DIFF[t_i][1][t_j], " x ", TESTS_DIFF[t_i][0][t_j], "\n")
		try:
			result = subprocess.check_output("grep -vxFf " + TESTS_DIFF[t_i][0][t_j] + " " + TESTS_DIFF[t_i][1][t_j], shell=True)
			try:
				print(str(result, 'utf-8', 'ignore'))
			except:
				print(result)
		except subprocess.CalledProcessError as grepexc: 
			if (grepexc.returncode == 1):
				flag2 = True
				print("OK!\n")
			else:
				print("Código de erro retornado pelo grep: ", grepexc.returncode, "\n")
		if  flag1 and flag2:
			print("Os arquivos são idênticos em conteúdo!\n")
			
		print("== FIM GREP ==\n")
		try:
			if (grepexc.returncode != 2):
				result = subprocess.check_output("rm " + TESTS_DIFF[t_i][0][t_j], shell=True)
		except:
			continue
	print("\n##===========================================================##\n")
	t_i += 1

print("\n##================== TESTE: VALGRIND ==================##\n")
try:
	result = subprocess.check_output("valgrind " + TESTS_COMP[0], shell=True)
	print(str(res_compile, 'utf-8', 'ignore'))
except:
	print(result)
for file in ["output.txt"]:
	try:
		result = subprocess.call(["rm", file], stderr=subprocess.PIPE)
	except:
		continue
print("\n##===========================================================##\n")

try:
	if SOURCES == "make":
		result = subprocess.call(["make", "clean"], stderr=subprocess.PIPE)
		result = subprocess.call(["rm", "A3"], stderr=subprocess.PIPE)
	else:
		result = subprocess.call(["rm", "A3"], stderr=subprocess.PIPE)
except:
	pass
