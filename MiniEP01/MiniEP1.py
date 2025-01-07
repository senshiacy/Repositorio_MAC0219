"""
	Nathália Yukimi Uchiyama Tsuno
	14600541
"""

import math

def main():
    # Setar as variáveis iniciais
    nx = 500
    x0 = t0 = 0
    xf = tf = 10

    # Definir vetores u, v, a e x de tamanho nx
    # O uso do None é para simular alocamento de lixo no C
    u = [None] * nx
    v = [None] * nx
    a = [None] * nx
    x = [None] * nx

    # Atribuir valores
    deltax = (xf - x0)/(nx - 1)
    deltat = deltax/2
    nt = ((tf - t0)/deltat) + 1

    j = x0
    for i in range(0, nx):
        x[i] = j
        j += deltax
    
    # Condições de contorno e iniciais
    for i in range(0, nx):
        u[i] = math.exp(-(x[i]-5)**2)
        v[i] = 0
        a[i] = 0

    # Processamento
    t = t0
    deltatleapfrog = 0

    while(t <= tf):
        if(t == t0):
            deltatleapfrog = deltat/2
        else:
            deltatleapfrog = deltat

        for i in range(1, nx - 1):
            a[i] = (u[i - 1] + u[i + 1] - 2*u[i])/(deltax**2) # obtido através de diferenças finitas

        for i in range(0, nx):
            v[i] = v[i] + a[i] * deltatleapfrog

        for i in range(0, nx):
            u[i] = u[i] + v[i] * deltat

        t += deltat
# Imprima a solução para determinar convergência (verificando que 𝑢(5, 10) = −1)
    i = nx//2 - 10
    while(i <= nx//2 + 10):
        print(x[i], (-1) * u[i])
        i +=1

main()
