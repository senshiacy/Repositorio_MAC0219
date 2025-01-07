import pandas as pd
import numpy as np

df = pd.read_csv('Resultados.csv')

arq = open("Resultados.txt", "w")

NUSP = input("Insira seu NUSP: ")

arq.write(str(NUSP) + "\n")
arq.write("C(MiniEP1),100," + str(df['C1'].mean()/1000000) + "," + str(df['C1'].std()/1000000) + "\n")
arq.write("C(MiniEP2),100," + str(df['C2'].mean()/1000000) + "," + str(df['C2'].std()/1000000) + "\n")

arq.close()
