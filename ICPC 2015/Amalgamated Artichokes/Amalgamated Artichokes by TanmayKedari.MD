
## Problema A
## Amalgamated Artichokes by TanmayKedari (Fuerza Bruta)
### Time limit: 5 Segundos

# 1. Explicación del Código Actual

El programa se divide en las tres funciones principales del flujo: entrada, procesamiento y salida.

## A. La función input() (Entrada de datos)

• Recibe los 6 parámetros del problema: p, a, b, c, d, n.
• Usa un bloque de validación (cin.clear(), cin.ignore(), goto) por si el usuario introduce un dato no numérico, obligándolo a repetir la entrada.
• Nota: En las competencias ICPC las entradas siempre son limpias y correctas según la especificación, por lo que las validaciones con goto no son necesarias.

```C++
void input()
{
	one:cin>>p>>a>>b>>c>>d>>n;		//input
	if(!p||!a||!b||!c||!d||!n)
	{
		cout<<"\nEnter integer value only:"<<endl;
		cin.clear();
		cin.ignore();
		goto one;
	}
}
```

## B. La función formula() (El procesamiento)

Aquí reside el núcleo matemático del problema:
1. Generación de precios: Calcula el precio del alcaucil (artichoke) para cada paso i desde 1 hasta n usando la fórmula trigonométrica dada: \(p \cdot (\sin(a \cdot i + b) + \cos(c \cdot i + d) + 2)\). Guarda estos valores en un arreglo x.

```C++
double formula()
{
	double x[n],max=0.0,nw;		//For getting decimal values
	long long int i,j;			//values till 10^6
	for(i=1;i<=n;i++)
	x[i]=p*(sin(a*i+b)+cos(c*i+d)+2);
```

2. Búsqueda del declive máximo (Fuerza Bruta): Utiliza dos bucles (for anidados). El primero fija un precio en el día i, y el segundo busca los días futuros j. Si el precio futuro es menor, calcula la resta (x[i] - x[j]) y la compara con la variable max para quedarse con la mayor caída.

```C++
for(i=1;i<=n;i++)
	{
		for(j=i+1;j<=n;j++)
		{
			if(x[j]>x[i])
			break;
			nw=x[i]-x[j];
			if(nw>max)
			max=nw;

		}
	}
	return max;
}
```

## C. La función display() (Salida de datos)

• Aquí es donde se usa la librería <iomanip> que consultaste antes. Con setprecision(12) se asegura de imprimir el resultado con 12 decimales de precisión, ya que los jueces flotantes del ICPC requieren una precisión muy alta para dar el problema como válido.

```C++
void display(double h)
{
	cout<<"\nThe greatest decline is:"<<setprecision(12)<<h<<endl;
}
```

## D. Funcion main()

El main del programa original se encarga de controlar el flujo principal y la interacción con el usuario, permitiendo que el programa procese múltiples casos de prueba de forma consecutiva a través de un ciclo interactivo.

1. Declaración de variables:

```C++
int main()
{
	int i,j=1;
	double h;
```

• i: Funciona como una variable de control para decidir si el usuario quiere continuar ingresando datos o salir del programa.
• j=1: Es un contador que lleva el número del caso actual. Empieza en 1.
• h: Es la variable donde se guardará el resultado final (la máxima caída) que devuelva la función matemática.


2. El ciclo interactivo do-while

El código utiliza una estructura do { ... } while(i); la cual asegura que las instrucciones de su interior se ejecuten al menos una vez, y se sigan repitiendo mientras el valor de i sea diferente de cero.

```C++
    do
    {
    cout<<"Case "<<j++<<":"<<endl;
	input();
	h=formula();
	display(h);
    cout<<"\nDo you want to continue (Y=1/N=0):";
    cin>>i;
    }while(i);
}
```

3. Dentro del ciclo (El paso a paso):

Imprime en pantalla el número de caso actual. El operador j++ es un post-incremento: primero imprime el valor actual de j (por ejemplo, 1) y justo después le suma uno para que esté listo para el siguiente caso (2).

`cout<<"Case "<<j++<<":"<<endl;`

Llama a la función input(). El programa se detiene aquí temporalmente esperando a que el usuario escriba los 6 números (p, a, b, c, d, n) correspondientes a las variables globales.

`input();`

Una vez que se tienen los datos, ejecuta la función formula(). Esta realiza todo el cálculo de los precios astronómicos y la búsqueda por fuerza bruta de la mayor caída, y guarda ese número decimal en la variable h.

`h = formula();`

Envía el resultado h a la función de salida, la cual se encarga de imprimirlo en la consola formateado con los 12 decimales de precisión gracias a <iomanip>.

`display(h);`

Le pregunta directamente al usuario si desea realizar otro cálculo. El usuario debe ingresar un número y este se guarda en i.

`cout<<"\nDo you want to continue (Y=1/N=0):";`

4. Evaluación de la condición

cpp } while(i); 
Al llegar al final, evalúa el valor que el usuario introdujo en i:
• Si el usuario presionó 1 (o cualquier número diferente de cero), C++ lo interpreta como Verdadero (true), por lo que el ciclo vuelve a empezar desde el do, aumentando el contador de casos.
• Si el usuario presionó 0, se interpreta como Falso (false), el ciclo se rompe, llega al return 0; y el programa finaliza limpiamente.

```C++
cin>>i;
    }while(i);
}
```

# 2. ¿Por qué este código va a fallar en el ICPC? (Puntos Críticos)

1. Desbordamiento de Memoria (VLA):
En la línea `double x[n],max=0.0,nw;`, estás creando un Variable Length Array en el stack (pila). Cuando n = 1,000,000, ese arreglo requiere unos 8 Megabytes de memoria continua. En la mayoría de las configuraciones de C++, la pila de memoria es más pequeña, lo que provocará un Stack Overflow (Runtime Error).

```C++
double formula()
{
	double x[n],max=0.0,nw;		//For getting decimal values
	long long int i,j;			//values till 10^6
	for(i=1;i<=n;i++)
	x[i]=p*(sin(a*i+b)+cos(c*i+d)+2);
```

2. Lentitud Absoluta (O(n²)):
Los dos bucles anidados para buscar la caída máxima comparan cada día contra todos los días futuros. Si n = 10⁶, el programa hará aproximadamente $\frac{10^{12}}{2}$ operaciones. Una computadora normal procesa unas 10⁸ operaciones por segundo. Tu código tardaría varias horas en terminar, cuando el límite del ICPC suele ser de 1 a 2 segundos.

```C++
for(i=1;i<=n;i++)
	{
		for(j=i+1;j<=n;j++)
		{
			if(x[j]>x[i])
			break;
			nw=x[i]-x[j];
			if(nw>max)
			max=nw;

		}
	}
	return max;
}
```