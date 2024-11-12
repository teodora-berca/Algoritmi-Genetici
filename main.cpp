#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <iomanip>
#include <math.h>
#include <vector>
#include <queue>


using namespace std;

ifstream in("date.in");
ofstream out("date.out");

double numar_random(double a, double b, int precizie)
{
    double x;
    double z;
    x = floor(a + static_cast<double>(rand())/RAND_MAX*(b-a));
    int aux = precizie;
    int aux10 = 1;
    while(aux!=0)
    {
        aux10 = aux10 * 10;
        aux = aux - 1;
    }
    z = (static_cast<double>((rand()*RAND_MAX)%aux10))/aux10;
    x = x + z;
    return x;
}

string codificare(double individ, double d, int l, double a)
{
    int aux = floor((individ-a)/d);
    string codificat = "";
    while(aux!=0)
    {
        int rest = aux%2;
        if(rest==0)
        {
            codificat = '0'+codificat;
        }
        else
        {
            codificat = '1'+codificat;
        }
        aux = aux/2;
    }
    while(codificat.length()<l)
    {
        codificat = '0'+codificat;
    }
    return codificat;
}

double decodificare(string cromozom, double d, double a)
{
    int numar = 0;
    for(int i=0;i<cromozom.length();i++)
    {
        if(cromozom[i]=='1')
        {
            int putere = cromozom.length()-i-1;
            int aux = 1;
            while(putere>0)
            {
                aux = aux *2;
                putere = putere - 1;
            }
            numar = numar + aux;
        }
    }
    double decodificat = a + d * numar;
    return decodificat;
}


void crossing_over(string& s1, string& s2, int l, int afisareMesaj)
{
    int punct_de_rupere_1 = numar_random(0, l, 0);///punct de rupere 1
    int punct_de_rupere_2 = numar_random(0, l, 0);///punct de rupere 2

    if(punct_de_rupere_1==punct_de_rupere_2)
    {

    }
    else
    {
        if(punct_de_rupere_1>punct_de_rupere_2)
        {
            int aux = punct_de_rupere_1;
            punct_de_rupere_1 = punct_de_rupere_2;
            punct_de_rupere_2 = aux;
        }
        else
        {
            string aux1="";
            string aux2="";

            for(int i=punct_de_rupere_1;i<=punct_de_rupere_2;i++)
            {
                aux1 = s1[i] + aux1;
                aux2 = s2[i] + aux2;
            }

            int j = 0;

            for(int i=punct_de_rupere_1;i<=punct_de_rupere_2;i++)
            {
                s1[i] = aux2[j];
                s2[i] = aux1[j];
                j++;
            }
        }
    }
    if(afisareMesaj==1)
    {
        out<<s1<<" "<<s2<<" ";
        out<<"punct de rupere 1 :"<<punct_de_rupere_1<<"\n";
        out<<"punct de rupere 2 :"<<punct_de_rupere_2<<"\n";
    }
}

void mutatie(string& s, double probMutatie, int precizie)
{
    for(int i=0;i<s.length();i++)
    {
        double u = numar_random(0, 1, precizie);
        if(u<probMutatie)
        {
            if(s[i]=='0')
                s[i]='1';
            else
                s[i]='0';
        }
    }
}

double fit(double x, double a, double b, double c)
{
    double f = a * x * x + b * x + c;
    return f;
}

void intervale_probabilitate(double intervale[], double probabilitate[], int n)
{
    intervale[0]=0;
    double suma_partiala = 0;
    for(int i=1;i<=n;i++)
    {
        suma_partiala = suma_partiala + probabilitate[i];
        intervale[i] = suma_partiala;
    }
}

int cautare_binara(double intervale[], double u, int n)
{
    int m;
    int stg = 1;
    int dr = n;
    int gasit = 0;
    while(stg<=dr&&gasit==0)
    {
        m = (stg+dr)/2;
        if(intervale[m]==u)
            gasit = m + 1;
        else
        {
            if(intervale[m]<u)
            {
                if(u<intervale[m+1])
                    gasit = m+1;
                else
                    stg = m;
            }
            else
            {
                if(u>intervale[m-1])
                    gasit = m;
                else
                    dr = m;
            }
        }
    }
    return gasit;
}

void afisare_populatie(double populatie[], string cromozom[], double fitness[], int n)
{
    for(int i=1;i<=n;i++)
    {
        out<<i<<": "<<cromozom[i]<<" "<<"x= "<<populatie[i]<<" f="<<fitness[i]<<"\n";
    }
}

int maxim_generatie(double fitness[], int n)
{
    int maxim = 1;
    for(int i=1;i<=n;i++)
    {
        if(fitness[maxim]<fitness[i])
        {
            maxim=i;
        }
    }
    return maxim;
}

int main()
{
    srand(time(0));
    int n; ///dimensiune populatie
    double c1, c2; ///domeniul de definitie
    double a, b, c; ///parametrii functiei
    int precizie, probRecombinare, probMutatie, nrEtape;
    int aux, aux2;
    in>>n;
    in>>c1>>c2;
    in>>a>>b>>c;
    in>>precizie>>probRecombinare>>probMutatie>>nrEtape;
    int putere = 1;
    aux = precizie;
    while(aux>0)
    {
        putere = putere * 10;
        aux --;
    }
    int l = ceil(log2((c2-c1)*putere));
    aux = l;
    aux2 = 1;
    while(aux>0)
    {
        aux2 = aux2 *2;
        aux--;
    }
    double d = (c2-c1)/aux2;
    double fitnessTotal = 0;
    double populatie[n+1]={0};
    double fitness[n+1]={0};
    double probabilitate[n+1];
    string cromozom[n+1];
    for(int i=1;i<=n;i++)
    {
        double x = numar_random(c1, c2, precizie);
        populatie[i] = x;
        cromozom[i] = codificare(x, d, l, c1);
        fitness[i] = fit(populatie[i], a, b, c);
        fitnessTotal = fitnessTotal + fitness[i];
    }
    for(int i=1; i<=n;i++)
    {
        probabilitate[i]=fitness[i]/fitnessTotal;
    }
    out<<"Populatia initiala:"<<"\n";
    for(int i=1;i<=n;i++)
    {
        out<<i<<": ";
        out<<cromozom[i]<<" ";
        out<<"x=";
        out<<populatie[i]<<" ";
        out<<"f=";
        out<<fitness[i]<<"\n";
    }
    out<<"Probabilitati selectie:"<<"\n";
    for(int i=1;i<=n;i++)
    {
        out<<"cromozom "<<i<<" probabilitate "<<probabilitate[i]<<"\n";
    }
    double intervale[n+1];
    intervale_probabilitate(intervale, probabilitate, n);
    out<<"Intervale probabilitati de selectie:"<<"\n";
    for(int i=0;i<=n;i++)
    {
        out<<intervale[i]<<" ";
    }
    double populatie_noua[n+1];
    for(int i=1;i<=n;i++)
    {
        double u = numar_random(0,1,precizie);
        int gasit = cautare_binara(intervale, u, n);
        out<<"u="<<u<<" selectam cromozomul "<<gasit<<"\n";
        populatie_noua[i]=populatie[gasit];
    }
    out<<"Dupa selectie:"<<"\n";
    string cromozom_nou[n+1];
    double fitness_nou[n+1];
    for(int i=1;i<=n;i++)
    {
        cromozom_nou[i]=codificare(populatie_noua[i], d, l, c1);
        fitness_nou[i]=fit(populatie_noua[i], a, b, c);
    }
    afisare_populatie(populatie_noua, cromozom_nou, fitness_nou, n);
    out<<"Probabilitatea de incrucisare este "<<static_cast<double>(probRecombinare)/100<<"\n";
    int nr=0;
    queue<int> recombinare;
    for(int i=1;i<=n;i++)
    {
        double u = numar_random(0,1,precizie);
        if(u<static_cast<double>(probRecombinare)/100)
            {
                recombinare.push(i);
                nr++;
            }
    }
    int j=1;
    while(j<nr)
    {
        if(nr-j==3)
        {
            int cr1 = recombinare.front();
            recombinare.pop();
            int cr2 = recombinare.front();
            recombinare.pop();
            int cr3 = recombinare.front();
            recombinare.pop();
            out<<"Recombinare intre cromozomul "<<cr1<<" si cromozomul "<<cr2<<":"<<"\n";
            crossing_over(cromozom_nou[cr1], cromozom_nou[cr2], l, 1);
            out<<"Rezultat    "<<cromozom_nou[cr1]<<" "<<cromozom_nou[cr2]<<"\n";
            populatie_noua[cr1]=decodificare(cromozom_nou[cr1],d,c1);
            fitness_nou[cr1]=fit(populatie_noua[cr1],a,b,c);
            populatie_noua[cr2]=decodificare(cromozom_nou[cr2],d,c1);
            fitness_nou[cr2]=fit(populatie_noua[cr2],a,b,c);
            out<<"Recombinare intre cromozomul "<<cr2<<" si cromozomul "<<cr3<<":"<<"\n";
            crossing_over(cromozom_nou[cr2], cromozom_nou[cr3], l, 1);
            out<<"Rezultat    "<<cromozom_nou[cr2]<<" "<<cromozom_nou[cr3]<<"\n";
            populatie_noua[cr2]=decodificare(cromozom_nou[cr2],d,c1);
            fitness_nou[cr2]=fit(populatie_noua[cr2],a,b,c);
            populatie_noua[cr3]=decodificare(cromozom_nou[cr3],d,c1);
            fitness_nou[cr3]=fit(populatie_noua[cr3],a,b,c);
            j = j + 3;
        }
        else
        {
            int cr1 = recombinare.front();
            recombinare.pop();
            int cr2 = recombinare.front();
            recombinare.pop();
            out<<"Recombinare intre cromozomul "<<cr1<<" si cromozomul "<<cr2<<":"<<"\n";
            crossing_over(cromozom_nou[cr1], cromozom_nou[cr2], l, 1);
            out<<"Rezultat    "<<cromozom_nou[cr1]<<" "<<cromozom_nou[cr2]<<"\n";
            populatie_noua[cr1]=decodificare(cromozom_nou[cr1],d,c1);
            fitness_nou[cr1]=fit(populatie_noua[cr1],a,b,c);
            populatie_noua[cr2]=decodificare(cromozom_nou[cr2],d,c1);
            fitness_nou[cr2]=fit(populatie_noua[cr2],a,b,c);
            j = j + 2;
        }
    }
    out<<"Dupa recombinare"<<"\n";
    afisare_populatie(populatie_noua, cromozom_nou, fitness_nou, n);
    out<<"Probabilitatea de mutatie "<<static_cast<double>(probMutatie)/100<<"\n";
    out<<"S-au modificat cromozomii:"<<"\n";
    for(int i=1;i<=n;i++)
    {
        string aux = cromozom_nou[i];
        mutatie(aux,static_cast<double>(probMutatie)/100,precizie);
        if(aux!=cromozom_nou[i])
        {
            cromozom_nou[i]=aux;
            out<<i<<"\n";
            populatie_noua[i]=decodificare(cromozom_nou[i],d,c1);
            fitness_nou[i]=fit(populatie_noua[i],a,b,c);
        }
    }
    out<<"Dupa mutatie"<<"\n";
    afisare_populatie(populatie_noua, cromozom_nou, fitness_nou, n);
    out<<"Evolutia maximului"<<"\n";
    j=maxim_generatie(fitness_nou,n);
    double maxim = fitness_nou[j];
    out<<maxim<<"\n";
    int copie_n = nrEtape;


    while(copie_n != 0)
    {
        for(int i=1;i<=n;i++)
        {
            populatie[i]=populatie_noua[i];
            cromozom[i]=cromozom_nou[i];
            fitness[i]=fitness_nou[i];
        }
        j = maxim_generatie(fitness, n);
        populatie_noua[1]=populatie[j];
        cromozom_nou[1]=cromozom[j];
        fitness_nou[1]=fitness[j];
        fitnessTotal = 0;
        for(int i=1;i<=n;i++)
        {
            fitness[i]=fit(populatie[i],a,b,c);
            fitnessTotal = fitnessTotal + fitness[i];
        }
        for(int i=1;i<=n;i++)
        {
            probabilitate[i]=fitness[i]/fitnessTotal;
        }
        intervale_probabilitate(intervale,probabilitate,n);
        for(int i=2;i<=n;i++)
        {
            double u = numar_random(0,1,precizie);
            int gasit = cautare_binara(intervale, u, n);
            populatie_noua[i]=populatie[gasit];
        }
        for(int i=2;i<=n;i++)
        {
            cromozom_nou[i]=codificare(populatie_noua[i],d,l,c1);
            fitness_nou[i]=fit(populatie_noua[i],a,b,c);
        }
        nr=0;
        queue<int> recombinare;
        for(int i=1;i<=n;i++)
        {
            double u = numar_random(0,1,precizie);
            if(u<static_cast<double>(probRecombinare)/100)
                {
                    recombinare.push(i);
                    nr++;
                }
        }
        j=1;
        while(j<nr)
        {
            if(nr-j==3)
            {
                int cr1 = recombinare.front();
                recombinare.pop();
                int cr2 = recombinare.front();
                recombinare.pop();
                int cr3 = recombinare.front();
                recombinare.pop();

                crossing_over(cromozom_nou[cr1], cromozom_nou[cr2], l, 0);
                populatie_noua[cr1]=decodificare(cromozom_nou[cr1],d,c1);
                fitness_nou[cr1]=fit(populatie_noua[cr1],a,b,c);

                populatie_noua[cr2]=decodificare(cromozom_nou[cr2],d,c1);
                fitness_nou[cr2]=fit(populatie_noua[cr2],a,b,c);

                crossing_over(cromozom_nou[cr2], cromozom_nou[cr3], l, 0);
                populatie_noua[cr2]=decodificare(cromozom_nou[cr2],d,c1);
                fitness_nou[cr2]=fit(populatie_noua[cr2],a,b,c);

                populatie_noua[cr3]=decodificare(cromozom_nou[cr3],d,c1);
                fitness_nou[cr3]=fit(populatie_noua[cr3],a,b,c);

                j = j + 3;
            }
            else
            {
                int cr1 = recombinare.front();
                recombinare.pop();
                int cr2 = recombinare.front();
                recombinare.pop();

                crossing_over(cromozom_nou[cr1], cromozom_nou[cr2], l, 0);

                populatie_noua[cr1]=decodificare(cromozom_nou[cr1],d,c1);
                fitness_nou[cr1]=fit(populatie_noua[cr1],a,b,c);

                populatie_noua[cr2]=decodificare(cromozom_nou[cr2],d,c1);
                fitness_nou[cr2]=fit(populatie_noua[cr2],a,b,c);

                j = j + 2;
            }
        }
        for(int i=1;i<=n;i++)
        {
            string aux = cromozom_nou[i];
            mutatie(aux,static_cast<double>(probMutatie)/100,precizie);
            if(aux!=cromozom_nou[i])
            {
                cromozom_nou[i]=aux;
                populatie_noua[i]=decodificare(cromozom_nou[i],d,c1);
                fitness_nou[i]=fit(populatie_noua[i],a,b,c);
            }
        }
        j=maxim_generatie(fitness_nou,n);
        maxim = fitness_nou[j];
        out<<maxim<<"\n";
        copie_n--;
    }
    cout<<d<<"\n";
    return 0;
}
