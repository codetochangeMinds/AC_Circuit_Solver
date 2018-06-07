#include <iostream>
#include <math.h>
#include "complex.h"
using namespace std;

#define pi 3.1415926536
struct net
{
	string id;
	Complex voltage;
};

struct net_comp
{
    string net1;
    string net2;
    string comp;
    string mag;
};

struct net_vori
{
    string net1;
    bool inv = 0;
    string net2;
    string name;
    string dc;
    string amp;
    string freq;
    string damp;
    string delay;
};

/*struct elem_thru_net
{
    vector<net_comp> RLC;
    vector<net_vori> VI;
};*/

//first find no. of nets and voltage sources
//let n->no. of nets
//m->no. of voltage sources 
int n=2;
int m=1;




//solve Ax=z;
//get complex impedance for G matrix construction 
Complex get_Impedance(net_comp nets,string freq)
{
	char p=(nets.comp[0]);
	//cout<<p<<endl;
	double fr=stoi(freq.substr(0,freq.size()-3));
	char q=freq[freq.size()-3];
	//cout<<q<<endl;
	//frequency unit
	if(q=='K' || q=='k')
	{
		fr=fr*1000;
	}
	else if(q=='N' || q=='n')
    {
    	fr=fr/1000000000;
    }
    else if(q=='M')
    {
    	fr=fr*1000000;
    }
    else if(q=='m')
    {
    	fr=fr/1000;
    }

    //impedance of resistor/capacitor/inductor
	if(p=='R' || p=='r')
	{
		string valueR=(nets.mag).substr(0,(nets.mag).size()-1);
	    double valR=stoi(valueR);
	    //cout<<"hello";
	    //cout<<valueR<<endl;
		if(nets.mag[(nets.mag).size()-1]=='K' ||nets.mag[(nets.mag).size()-1]=='k' )
		{
			valR=1000*valR;
		}
		//mega
		else if(nets.mag[(nets.mag).size()-1]=='M')
		{
			valR=1000000*valR;
		}
		else if(nets.mag[(nets.mag).size()-1]=='m')
		{
			valR=valR/1000;
		}
		else if(nets.mag[(nets.mag).size()-1]=='N' ||nets.mag[(nets.mag).size()-1]=='n')
		{
			valR=valR/1000000000;
		}
		else if(nets.mag[(nets.mag).size()-1]=='P' || nets.mag[(nets.mag).size()-1]=='p')
		{
			valR=valR/1000000000000;
		}
		else 
		{
			//cout<<"hello";
			string valueR=(nets.mag).substr(0,(nets.mag).size()+1);
	    	double valR=stoi(valueR);
		}
		//cout<<valR<<endl;
	return div(makeComplex(1,0),makeComplex(valR,0));
	}
	else if(p=='C'|| p=='c')
	{
		string valueLC=(nets.mag).substr(0,(nets.mag).size()-2);
		//cout<<valueLC<<endl;
		double valLC=stoi(valueLC);
		if(nets.mag[(nets.mag).size()-2]=='K'||nets.mag[(nets.mag).size()-2]=='k')
		{
			valLC=1000*valLC;
		}
		else if(nets.mag[(nets.mag).size()-2]=='M')
		{
			valLC=1000000*valLC;
		}
		else if(nets.mag[(nets.mag).size()-2]=='m')
		{
			valLC=valLC/1000;
		}
		else if(nets.mag[(nets.mag).size()-2]=='N'||nets.mag[(nets.mag).size()-2]=='n')
		{
			valLC=valLC/1000000000;
		}
		else if(nets.mag[(nets.mag).size()-2]=='P'||nets.mag[(nets.mag).size()-2]=='p')
		{
			valLC=valLC/1000000000000;
		}
	//return div(makeComplex(1,0),makeComplex(0,valLC*2*pi*fr));
		return makeComplex(0,valLC*2*pi*fr);
	}
	else if(p=='L'||p=='l')
	{
		string valueLC=(nets.mag).substr(0,(nets.mag).size()-2);
		//cout<<valueLC<<endl;
		double valLC=stoi(valueLC);
		if(nets.mag[(nets.mag).size()-2]=='K'||nets.mag[(nets.mag).size()-2]=='k')
		{
			valLC=1000*valLC;
		}
		else if(nets.mag[(nets.mag).size()-2]=='M')
		{
			valLC=1000000*valLC;
		}
		else if(nets.mag[(nets.mag).size()-2]=='m')
		{
			valLC=valLC/1000;
		}
		else if(nets.mag[(nets.mag).size()-2]=='N'||nets.mag[(nets.mag).size()-2]=='n')
		{
			valLC=valLC/1000000000;
		}
		else if(nets.mag[(nets.mag).size()-2]=='P'||nets.mag[(nets.mag).size()-2]=='p')
		{
			valLC=valLC/1000000000000;
		}
		//cout<<valLC<<" "<<fr<<endl;
	return div(makeComplex(1,0),makeComplex(0,valLC*2*pi*fr));
	}
}

//#############logic from https://www.swarthmore.edu/NatSci/echeeve1/Ref/mna/MNA3.html#j%20matrix###########
//A=|G B|
//  |C D|
//intialize G matrix
//to construct B matrix(1 or 0)
//C=B`
//C[1][2]=transpose(B);
//D(m*m)=[0],as no independent source 
//D[1][1]=[0]
//x matrix(Assume voltage of every node and current through voltage source)
//z matrix(change when more than one source)
//solve linear equation and find x
//print solution knowing x(voltage of individual node)



void solve(net_comp net[],net_vori source)
{
	Complex A[3][3];
	Complex G[2][2];
	Complex B[2][1],C[1][2],D[1][1];
	Complex x[3][1];
	Complex z[3][1];
	//G matrix
	for (int i = 0; i < 6; i++)
	{
		// cout<<"hello"<<endl;
		 if(((net[i].net1)[0]=='N'||(net[i].net1)[0]=='n')&&((net[i].net2)[0]=='N'||(net[i].net2)[0]=='n'))
		 {
		 	string temp1=(net[i].net1).substr(3,(net[i].net1).size());
		 	string temp2 =(net[i].net2).substr(3,(net[i].net2).size());
		 	int temp1V=stoi(temp1);
		 	int temp2V=stoi(temp2);
		 	G[temp1V-1][temp1V-1]=Add(G[temp1V-1][temp1V-1],get_Impedance(net[i],source.freq));
		 	G[temp2V-1][temp2V-1]=Add(G[temp2V-1][temp2V-1],get_Impedance(net[i],source.freq));
		 	G[temp2V-1][temp1V-1]=Sub(G[temp2V-1][temp1V-1],get_Impedance(net[i],source.freq));
		 	G[temp1V-1][temp2V-1]=Sub(G[temp1V-1][temp2V-1],get_Impedance(net[i],source.freq));
		 }
		 else if(((net[i].net1)[0]=='N'||(net[i].net1)[0]=='n')&&((net[i].net2)=="0"))
		 {
		 	//cout<<"hi"<<endl;
		 	string temp1=(net[i].net1).substr(3,(net[i].net1).size());
		 	int temp1V=stoi(temp1);
		 	G[temp1V-1][temp1V-1]=Add(G[temp1V-1][temp1V-1],get_Impedance(net[i],source.freq));
		 }
	}
	//B matrix 1 if +ve terminal else -1 or 0 if not connected
	//B n*m need to traverse source structure
	string temp3=(source.net1).substr(3,(source.net1).size());
	int temp3V=stoi(temp3);
	//cout<<temp3<<endl;
	B[temp3V-1][0]=makeComplex(1,0);
	//cout<<B[temp3V-1][1].real<<","<<B[temp3V-1][1].img<<endl;
	if((source.net2)[0]=='N'||(source.net2)[0]=='n')
	{
		//cout<<"loop"<<endl;
		string temp4=(source.net2).substr(3,(source.net2).size());
		int temp4V=stoi(temp4);
		B[temp3V-1][0]=makeComplex(-1,0);
	}
	/*intially intialized with zero
	else if(source.net2=="0")
	{
		//make 
	}*/
	//C=transpose(B);
	for (int i = 0; i < 1; i++)
	{
		/* code */
		for (int j = 0; j <2; j++)
		{
			C[i][j]=B[j][i];
		}
	}
	//find no. of voltage source D m*m
	D[1][1]=makeComplex(0,0);

	//A matrix
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			A[i][j]=G[i][j];
		}
	}
	for (int i = 2; i < 3; i++)
	{
		for (int j = 0; j <2; j++)
		{
			A[i][j]=C[i-2][j];
		}
	}
	for (int i = 0; i <2 ; i++)
	{
		for (int j = 2; j < 3; j++)
		{
			A[i][j]=B[i][j-2];
		}
	}
	A[2][2]=D[1][1];
	/*for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cout<<i<<j<<","<<A[i][j].real<<","<<A[i][j].img<<endl;
		}
	}*/
    //voltage of each net  n*1
    //assuming voltage of ground to be zero
	Complex V[2][1];
	//current through voltage source m*1
	Complex I[1][1];
	//x is unknown matrix
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 1; j++)
		{
			x[i][j]=V[i][j];
		}
	}
	for (int i = 2; i < 3; ++i)
	{
		for (int j = 0; j < 1; j++)
		{
			x[i][j]=I[i-2][j];
		}
	}
	//z matrix (n+m)*1
	//current through current source to particular net
	Complex I_n[2][1];
	//Known complex voltage of source
	Complex V_s[1][1];
	double angle=(stoi(source.damp)*pi)/180;
	double magn=stoi(source.amp);
	//also have to consider delay and dc offset
	V_s[0][0]=makeComplex((magn)*cos(angle),(magn)*sin(angle));
	//cout<<V_s[0][0].real<<","<<V_s[0][0].img<<endl;
	for (int i = 0; i <2; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			z[i][j]=makeComplex(0,0);
		}
	}
	z[2][0]=V_s[0][0];

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 1; j++)
		{
			cout<<i<<j<<" "<<z[i][j].real<<" "<<z[i][j].img<<endl;
		}
	}

	Complex det=determinant(A,3);
	cout<<"det"<<det.real<<" "<<det.img<<endl;

	Complex adj[3][3];
	adjoint(A,adj);
	for (int i = 0; i < 3; i++)
    {
    	for (int j = 0; j < 3; j++)
    	{
    		cout<<i<<j<<" "<<adj[i][j].real<<" "<<adj[i][j].img<<endl;
    	}
    }

    Complex inv[3][3];
    inverse(A,inv);
    for (int i = 0; i < 3; i++)
    {
    	for (int j = 0; j < 3; j++)
    	{
    		cout<<i<<j<<" "<<inv[i][j].real<<" "<<inv[i][j].img<<endl;
    	}
    }
    multiply(inv,z,x);
    //printing voltage of each net and current through voltage source
    for (int i = 0; i < 3; i++)
    {
    	//cout<<"hello"<<endl;
    	for (int j = 0; j < 1; j++)
    	{
    		cout<<i<<j<<" "<<x[i][j].real<<" "<<x[i][j].img<<endl;
    	}
    }

    //print solution
    cout<<"Voltage"<<endl;
    cout<<"Source voltage"<<" ";
    /*string temp3=(source.net1).substr(3,(source.net1).size());
	int temp3V=stoi(temp3);*/
	//cout<<temp3<<endl;
	//B[temp3V-1][0]=makeComplex(1,0);
	//cout<<B[temp3V-1][1].real<<","<<B[temp3V-1][1].img<<endl;
	if((source.net2)[0]=='N'||(source.net2)[0]=='n')
	{
		//cout<<"loop"<<endl;
		string temp4=(source.net2).substr(3,(source.net2).size());
		int temp4V=stoi(temp4);
		Complex V=Sub(x[temp3V-1][0],x[temp4V-1][0]);
		cout<<abs(V)<<" "<<atan(V.img/V.real)<<endl;
		//B[temp3V-1][0]=makeComplex(-1,0);
	}
	else{
		Complex V=Sub(x[temp3V-1][0],makeComplex(0,0));
		cout<<abs(V)<<" "<<atan(V.img/V.real)<<endl;
	}
    
	for (int i = 0; i < 6; i++)
	{
		// cout<<"hello"<<endl;
		 if(((net[i].net1)[0]=='N'||(net[i].net1)[0]=='n')&&((net[i].net2)[0]=='N'||(net[i].net2)[0]=='n'))
		 {
		 	string temp1=(net[i].net1).substr(3,(net[i].net1).size());
		 	string temp2 =(net[i].net2).substr(3,(net[i].net2).size());
		 	int temp1V=stoi(temp1);
		 	int temp2V=stoi(temp2);
		 	//cout<<x[temp1V-1][1].real<<" "<<x[temp2V-1][1].real<<endl;
		 	Complex res=Sub(x[temp1V-1][0],x[temp2V-1][0]);
		 	cout<<net[i].comp<<" "<<abs(res)<<" "<<atan(res.img/res.real)<<endl;
		 }
		 else if(((net[i].net1)[0]=='N'||(net[i].net1)[0]=='n')&&((net[i].net2)=="0"))
		 {
		 	//cout<<"hi"<<endl;
		 	string temp1=(net[i].net1).substr(3,(net[i].net1).size());
		 	int temp1V=stoi(temp1);
		 	//cout<<temp1V<<endl;
		 	Complex res=Sub(x[temp1V-1][0],makeComplex(0,0));
		 	cout<<(net[i].comp)<<" "<<abs(res)<<" "<<atan(res.img/res.real)<<endl;
		 }
	}
	cout<<"Current"<<endl;
	cout<<"Source Voltage"<<abs(x[2][0])<<" "<<atan(x[2][0].img/x[2][0].real)<<endl;
	for (int i = 0; i < 6; i++)
	{
		// cout<<"hello"<<endl;
		 if(((net[i].net1)[0]=='N'||(net[i].net1)[0]=='n')&&((net[i].net2)[0]=='N'||(net[i].net2)[0]=='n'))
		 {
		 	string temp1=(net[i].net1).substr(3,(net[i].net1).size());
		 	string temp2 =(net[i].net2).substr(3,(net[i].net2).size());
		 	int temp1V=stoi(temp1);
		 	int temp2V=stoi(temp2);
		 	//cout<<x[temp1V-1][1].real<<" "<<x[temp2V-1][1].real<<endl;
		 	Complex res=div(Sub(x[temp1V-1][0],x[temp2V-1][0]),get_Impedance(net[i],source.freq));
		 	cout<<net[i].comp<<" "<<abs(res)<<" "<<atan(res.img/res.real)<<endl;
		 }
		 else if(((net[i].net1)[0]=='N'||(net[i].net1)[0]=='n')&&((net[i].net2)=="0"))
		 {
		 	//cout<<"hi"<<endl;
		 	string temp1=(net[i].net1).substr(3,(net[i].net1).size());
		 	int temp1V=stoi(temp1);
		 	//cout<<temp1V<<endl;
		 	Complex res=div(Sub(x[temp1V-1][0],makeComplex(0,0)),get_Impedance(net[i],source.freq));
		 	cout<<(net[i].comp)<<" "<<abs(res)<<" "<<atan(res.img/res.real)<<endl;
		 }
	}
}


int main()
{
	net_comp net[6];
	net_vori source[no_of_VS];
	string p,q,r,s;
	for (int i = 0; i < 6; i++)
	{
		cin>>r>>p>>q>>s;
		net[i].comp=r;
		net[i].net1=p;
		net[i].net2=q;
		net[i].mag=s;
	}

	string id1,id2,amp1,freq,damp1;
	cin>>id1>>id2>>amp1>>freq>>damp1;
	source.net1=id1;
	source.net2=id2;
	source.amp=amp1;
	source.freq=freq;
	source.damp=damp1;

	//cout<<source.net1<<" "<<source.net2<<" "<<source.amp<<" "<<source.freq<<endl;

	solve(net,source);

	Complex a;
	for(int i=0;i< 6;i++)
	{	
		a=get_Impedance(net[i],source.freq);
		cout<<a.real<<"+ j"<<a.img<<endl;
	}
}
