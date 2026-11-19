#Problema de la mochila

#Demostracion:

#1. Definir el conjunto potencia:

def power_set(A): #Definir el conjunto potencia

    if len(A) == 0: #Analiza el caso base del conjunto potencia

        return [[]] #Retorna el vacio

    else: #Si no esta vacio

        X = A[0] #Se queda con el indice 0
        B = A[1:] #Se queda con el resto de subconjuntos
        PS = power_set(B) #Vulve a calcular el conjunto potencia
        gsx = PS #Se queda con los subconjutntos que no son 0
        gcx = [s + [X] for s in PS] #Une los subconjuntos con 0

        return gsx + gcx #Concatena los conjuntos generados

# 2. Funciones lambda del profesor
lb  = lambda n: [i for i in range(n)]
pe  = lambda s, p: sum([p[i] for i in s])
val = lambda s, v: sum([v[i] for i in s])
res = lambda S, p, W: [s for s in S if pe(s, p) <= W]
mv  = lambda S, v: max([(val(s, v), s) for s in S])

#2.5 Solucion con del profe:

def bak_pak_prof(P, V, W, n): #Definicion de la mochila 

    if n == 0 or W == 0: #Caso base

        return 0 #Retorna 0
    
    elif P[n-1] > W: #En caso de que no este vacio y sea valido

        return bak_pak(P, V, W, n-1) #Retorna la mochila con n-1
    
    nto = bak_pak(P, V, W, n-1) #Almacena el valor actual de la mochila
    to = V[n-1] + bak_pak(P, V, W - P[n-1], n-1) #Concatena el valor de las mochilas

    return max(nto, to) #Retorna el maximo

#2.6 Programacion dinamica (Enfoque bottom-up)

def bak_pak(P, V, W, n, memo=None): #Inicializa la funcion con los parametros: Pesos(P), 
# Valores de utilidad (V), Peso maximo de la mochila (W), Numero de elementos (n) y la memoria vacia.

    if memo is None: #Si la memoria esta vacia

        memo = {}  # Inicializa la memoria en la primera llamada

    # 1. Verifica si el subproblema ya fue resuelto
    if (n, W) in memo: #Si esta el peso maximo y los elementos en la memoria

        return memo[(n, W)] #Regresa la memoria en forma de lista

    # Caso base
    if n == 0 or W == 0: #Si no hay objetos ni peso maximo

        return (0, []) #Retorna 0 o vacio

    # Si el peso excede la capacidad, se omite el objeto
    if P[n-1] > W: #Se calcula si el peso del objeto actual es mayor que el peso maximo restante

        resultado = bak_pak(P, V, W, n-1, memo) #Si no hay espacio suficiente pasamos al siguiente objeto

    else: #Si el objeto actual es menor al peso maximo restante

        nto = bak_pak(P, V, W, n-1, memo) #Simulamos si ignoramos el objeto

        #Para considerar el objeto:
        val_ant, sub_ant = bak_pak(P, V, W - P[n-1], n-1, memo) #Separamos los valores para conocer la lista de indices
        to = (V[n-1] + val_ant, [n-1] + sub_ant) #Simulamos que tomamos el objeto

        #La funcion lambada indica que comparamos el indice 0 del conjunto para saber cuales son los mayores
        resultado = max(nto, to, key=lambda x: x[0]) #Nos quedamos con la maxima ganancia posible entre la simulacion tomar objeto y no tomar objeto

    # 2. Guarda el resultado en memoria antes de retornar

    memo[(n, W)] = resultado #Guarda el resultado en la memoria

    return resultado #Retorna la memoria


# 3. Prueba del programa
P = [2, 3, 4, 5] # Pesos
V = [3, 4, 5, 6] # Valores
W = 5 # Capacidad máxima
n = 4 # Cantidad de objetos

A = lb(n) # [0, 1, 2, 3]
S = power_set(A) # Todos los subconjuntos
S_validos = res(S, P, W) # Subconjuntos que caben en la mochila
solucion = mv(S_validos, V)

solucion_pd = bak_pak(P, V, W, n)

print("Solución (Valor máximo, Subconjunto):", solucion)
print("Solucion por PD: ", solucion_pd)

#A = [1, 2, 3]
#print(len(power_set(A)))