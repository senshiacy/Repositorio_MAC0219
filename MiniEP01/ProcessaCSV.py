import pandas as pd
import numpy as np

df = pd.read_csv('Resultados.csv')

arq = open("MiniEP1.txt", "w")

NUSP = input("Insira seu NUSP: ")

arq.write(str(NUSP) + "\n")
arq.write("python,100," + str(df['Python'].mean()/1000000) + "," + str(df['Python'].std()/1000000) + "\n")
arq.write("C,100," + str(df['C'].mean()/1000000) + "," + str(df['C'].std()/1000000) + "\n")

arq.close()
