/*

Solucion al Problema A Amalgamed Artichokes by: TanmayKedari

Comentarios by: Calak de Astora

Repo: https://github.com/TanmayKedari/Amalgamated-Artichokes/tree/master

*/

#include<iostream>              //Call input outup standar library of C++
#include<math.h>                //Call Math library
#include<iomanip>               //Call format library

using namespace std;            //Assign the standar namespace

long int p,a,b,c,d;				//values till 1000
long long int n;				//value till 10^6

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

double formula()
{
	double x[n],max=0.0,nw;		//For getting decimal values
	long long int i,j;			//values till 10^6
	for(i=1;i<=n;i++)
	x[i]=p*(sin(a*i+b)+cos(c*i+d)+2);

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
void display(double h)
{
	cout<<"\nThe greatest decline is:"<<setprecision(12)<<h<<endl;
}

int main()
{
	int i,j=1;
	double h;
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