#include <iostream>
#include <fstream>
#include <string.h>
#include <ctype.h>
#include <vector>
#include "complex.h"
using namespace std;

#define pi 3.1415926536

int count0=0;
int input_error = 0;
int nodes;

int acS;
struct net_comp
{
    string net1;
    string net2;
    string name;
    string mag;
};


struct net_vori
{
    string net1;
    int inv = 0;
    string net2;
    string name;
    string dc;
    string freq;
    string damp;
    string delay;
};

vector<net_comp> comp;

struct drw
{
    int netname = -2;
    string right = "null";
    string left = "null";
    int type = -2; 
    int degree = 0;
};

struct avail
{
    vector<int> first;
    vector<int> second;
};

struct elem_thru_net
{
    vector<net_comp> RLC;
    vector<net_vori> VI;
};

ifstream fin;
ofstream fout;


///svg function to draw circuit elements
void resistance(int x, int y, int r, string s)  //box of 150x20
{
    x=x-50;
    y=y-10;


    fout<<"<path d=\"M-25 10 L 20 10 30 5 40 15 50 5 60 15 70 5 80 10 125 10\" stroke=\"red\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" transform = \" rotate( "<<90*r<<","<<x+50<<","<<y+10<<") translate( "<<x<<","<<y<<") \" />\n";
    fout<<"<text x=\"35\" y=\"-10\" font-size=\"20\" fill=\"red\" font-style = \"italic\" transform = \" rotate( "<<90*r<<","<<x+50<<","<<y+10<<") translate( "<<x<<","<<y<<") \" >\n";
    fout<<s<<"</text>\n";
}

void inductor(int x, int y, int r, string s) //box of 150x20
{
    x=x-40;
    y= y-20;
    fout<<"<path d=\"M-35 20 L 0 20 \" stroke=\"red\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" transform = \"rotate( "<<90*r<<","<<x+40<<","<<y+20<<")  translate( "<<x<<","<<y<<") \"/>\n";
    fout<<"<path d=\"M80 20 L 115 20 \" stroke=\"red\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" transform = \"rotate( "<<90*r<<","<<x+40<<","<<y+20<<")  translate( "<<x<<","<<y<<") \"/>\n";
    fout<<"<path d=\"M0 20 L 10 20 Q 20 5 30 20 40 5 50 20 60 5 70 20 L 80 20 \" stroke=\"red\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" transform = \"rotate( "<<90*r<<","<<x+40<<","<<y+20<<")  translate( "<<x<<","<<y<<") \"/>\n";
    fout<<"<text x=\"30\" y=\"0\" font-size=\"20\" fill=\"red\" font-style = \"italic\" transform = \"rotate( "<<90*r<<","<<x+40<<","<<y+20<<")  translate( "<<x<<","<<y<<") \">\n";
    fout<<s<<"</text>\n";
}

void capacitor(int x, int y, int r, string s)  //box of 150x30
{
    x=x-25;
    y=y-20;
    fout<<"<path d=\"M-50 20 L 0 20 \" stroke=\"red\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" transform = \"rotate( "<<90*r<<","<<x+25<<","<<y+20<<")  translate( "<<x<<","<<y<<") \"/>\n";
    fout<<"<path d=\"M50 20 L 100 20 \" stroke=\"red\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" transform = \"rotate( "<<90*r<<","<<x+25<<","<<y+20<<")  translate( "<<x<<","<<y<<") \"/>\n";
    fout<<"<path d=\"M0 20 L 20 20 20 5 20 35 \" stroke=\"red\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" transform = \" rotate( "<<90*r<<","<<x+25<<","<<y+20<<") translate( "<<x<<","<<y<<")  \" /> \n";
    fout<<"<path d=\"M 30 35 L 30 5 30 20 50 20 \" stroke=\"red\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" transform = \" rotate( "<<90*r<<","<<x+25<<","<<y+20<<") translate( "<<x<<","<<y<<")  \"/> \n";
    fout<<"<text x=\"15\" y=\"0\" font-size=\"20\" fill=\"red\" font-style = \"italic\" transform = \" rotate( "<<90*r<<","<<x+25<<","<<y+20<<") translate( "<<x<<","<<y<<")  \" >\n";
    fout<<s<<"</text>\n";
}

void current(int x, int y, int r, string s)  //box of 150x30
{
    x=x-50;
    y=y-200;
    fout<<"<path d= \"M-25 200  L27 200 \" stroke=\"red\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" transform = \"  translate( "<<x<<","<<y<<")  \"/>\n";
    fout<<"<circle cx = \"37\" cy = \"200\" r =\"10\" stroke=\"red\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" transform = \"  translate( "<<x<<","<<y<<") \"/>\n";
    fout<<"<circle cx = \"53\" cy = \"200\" r =\"10\" stroke=\"red\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" transform = \"  translate( "<<x<<","<<y<<") \"/>\n";
    fout<<"<path d= \"M63 200  L125 200 \" stroke=\"red\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" transform = \"  translate( "<<x<<","<<y<<") \"/>\n";
    fout<<"<rect x = \"20\" y=\"185\" width=\"50\" height=\"30\" stroke=\"red\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" transform = \"   translate( "<<x<<","<<y<<")\"/>\n";
    fout<<"<text x=\"5\" y=\"185\" font-size=\"10\" fill=\"red\" font-style = \"italic\" transform = \" rotate( "<<180*r<<","<<x+45<<","<<y+200<<")  translate( "<<x+r*5<<","<<(y+(r*25))<<")\">\n";
    fout<<"+</text>\n";
    fout<<"<text x=\"70\" y=\"185\" font-size=\"10\" fill=\"red\" font-style = \"italic\" transform = \" rotate( "<<180*r<<","<<x+45<<","<<y+200<<")  translate( "<<x+r*5<<","<<(y+(r*25))<<")\">\n";
    fout<<"-</text>\n";
    fout<<"<text x=\"35\" y=\"180\" font-size=\"20\" fill=\"red\" font-style = \"italic\" transform = \"  translate( "<<x<<","<<y<<")  \">\n";
    fout<<s<<"</text>\n";
}

void voltage(int x, int y, int r, string s) //box of 150x30
{
    x= x-50;
    y= y-250;
    fout<<"<path d= \"M-25 250  L35 250 Q40 260 50 250 60 240 65 250 L125 250 \" stroke=\"red\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" transform = \" rotate( "<<180*r<<","<<x+50<<","<<y+250<<") translate( "<<x<<","<<y<<")  \"/>\n";
    fout<<"<circle cx = \"50\" cy = \"250\" r =\"15\" stroke=\"red\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" transform = \" rotate( "<<180*r<<","<<x+50<<","<<y+250<<") translate( "<<x<<","<<y<<")  \"/>\n";
    fout<<"<text x=\"25\" y=\"245\" font-size=\"10\" fill=\"red\"  transform = \" rotate( "<<180*r<<","<<x+50<<","<<y+250<<") translate( "<<x<<","<<y+r*12<<")  \">\n";
    fout<<"+</text>\" \n";
    fout<<"<text x=\"40\" y=\"230\" font-size=\"20\" fill=\"red\" font-style = \"italic\" transform = \" translate( "<<x<<","<<y<<")  \">\n";
    fout<<s<<"</text>\n";
}

void ground(int x, int y)
{
    fout<<"<path d= \"M"<<x<<" "<<y<<" L"<<x+13<<" "<<y<<" "<<x+13<<" "<<y+45<<" "<<x+1<<" "<<y+45<<" "<<x+25<<" "<<y+45<<" \" stroke=\"blue\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" />\n";
    fout<<"<path d= \"M"<<x+5<<" "<<y+50<<" L"<<x+21<<" "<<y+50<<" "<<" \" stroke=\"blue\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" />\n";
    fout<<"<path d= \"M"<<x+9<<" "<<y+55<<" L"<<x+17<<" "<<y+55<<" "<<" \" stroke=\"blue\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" />\n";
    
}

void connect(int x1, int y1, int x2, int y2, int diff, int g)
{
    fout<<"<circle cx = \""<<x1<<"\" cy = \""<<y1<<"\" r =\"2\" stroke=\"blue\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"blue\"/>\n";
    fout<<"<circle cx = \""<<x2<<"\" cy = \""<<y2<<"\" r =\"2\" stroke=\"blue\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"blue\"/>\n";
    if(y1 == y2)
    fout<<"<path d= \"M"<<x1<<" "<<y1<<" L"<<x2<<" " <<y2<<" \" stroke=\"blue\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" />\n";
    else
    {   fout<<"<path d= \"M"<<x1<<" "<<y1<<" L"<<x1+diff<<" "<<y1<<" "<<x1+diff<<" "<<y2<<" "<<x2<<" "<<y2<<" \" stroke=\"blue\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" />\n";
        if(g!=0)
        fout<<"<circle cx = \""<<x1+diff<<"\" cy = \""<<y2<<"\" r =\"2\" stroke=\"blue\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"blue\"/>\n";
    }
}

void connect_last(int x1, int y1, int x2, int y2, int diff, int g)
{
    fout<<"<circle cx = \""<<x1<<"\" cy = \""<<y1<<"\" r =\"2\" stroke=\"blue\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"blue\"/>\n";
    fout<<"<circle cx = \""<<x2<<"\" cy = \""<<y2<<"\" r =\"2\" stroke=\"blue\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"blue\"/>\n";
    fout<<"<path d= \"M"<<x1<<" "<<y1<<" L"<<x1+diff<<" "<<y1<<" "<<x1+diff<<" "<<y2+17<<" "<<x2+diff<<" "<<y2+17<<" "<<x2+diff<<" "<<y2+17<<" "<<x2+diff<<" "<<y2<<" "<<x2<<" "<<y2<<" \" stroke=\"blue\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" />\n";
    if(g!=0)
    fout<<"<circle cx = \""<<x1+diff<<"\" cy = \""<<y2+17<<"\" r =\"2\" stroke=\"blue\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"blue\"/>\n";
}

void start(int width, int height)
{
    fout<<"<svg xmlns=\"http://www.w3.org/2000/svg\" width=\""<<width<<"px\" height=\""<<height<<"px\">\n";
}

void end()
{
    fout<<"</svg>\n";
}


//helper function in parsing
bool notreal(string s)
{
        int decimal=0;
        int flag=0;
        for (int j = 0; j < s.size()-1; ++j)
        {
            if(s[j]=='.')
            {
                decimal++;
            }
            if(int(s[j]-48>=0)||int(s[j]-48)<=9||48-int(s[j])==2)
            {
                flag=0;
            }
            else flag=1;
        }
        if(decimal<=1 && flag==0)
            return false;
        else 
            return true;
}


//parse function 
void check(vector<string> a)
{
    int i=0;
    //cout<<a.size()<<endl;
    while(i<a.size())
        {
           // cout<<i<<endl;
            //defining rules of parsing
              if(a[i].substr(0, 3) == "Net" || a[i].substr(0, 3) == "NET")
                {
                    //cout<<i<<;
                    //{
                        if(a[i+1].substr(0, 3) == "Net" || a[i+1].substr(0, 3) == "NET" || a[i+1] =="0")

                        {
                               //assuming resistor start from 'R' or 'r'
                               //cout<<i<<endl;
                               if(a[i-1][0]=='R'||a[i-1][0]=='r')
                                {
                                    //cout<<"hello";
                                        //cout<<i;
                                         // for (int j = 0; j < a[i+2].size()-2; j++)
                                         // {
                                            
                                            string s=a[i+2].substr(0,(a[i+2].size()));
                                            //cout<<"hello"<<s[j];
                                            //cout<<"r"<<s;
                                            if(notreal(s))
                                            {
                                                //cout<<"hello"<<endl;
                                                cout<<"Nan, Value should be integerR"<<endl;
                                            }
                                        //}
                                        //i++;
                                }
                                //assuming Inductance start from 'L' or 'l'
                                else if(a[i-1][0]=='L'||a[i-1][0]=='l')
                                {
                                    
                                    // if((a[i+2][(a[i+2].size())-2]=='N'||a[i+2][(a[i+2].size())-2]=='n')) 
                                    // {
                                        //cout<<"helloC";
                                        if((a[i+2][(a[i+2].size())-1]=='H'||a[i+2][(a[i+2].size())-1]=='h'))
                                        {    
                                            // for (int j = 0; j < a[i+2].size()-2; ++j)
                                            // {
                                                
                                                string s=a[i+2].substr(0,(a[i+2].size())-1);
                                                if(notreal(s))
                                                {
                                                    cout<<"Nan, Value should be integerL"<<endl;
                                                }
                                            //}
                                        }
                                        else
                                            cout<<"Error in unit of Inductor"<<endl;

                                       // i++;
                                //     }
                                // else
                                //         cout<<a[i+2][(a[i+2].size())-2]<<"Wrong prefix given for Inductor"<<endl;
                                }
                                //assuming capacitance start from 'C' or 'c'
                                else if(a[i-1][0]=='C'||a[i-1][0]=='c')
                                {
                                    
                                    // if((a[i+2][(a[i+2].size())-2]=='N'||a[i+2][(a[i+2].size())-2]=='n')) 
                                    // {
                                        //cout<<"helloC";
                                        if((a[i+2][(a[i+2].size())-1]=='F'||a[i+2][(a[i+2].size())-1]=='f'))
                                        {    
                                            // for (int j = 0; j < a[i+2].size()-2; ++j)
                                            // {
                                                
                                                string s=a[i+2].substr(0,(a[i+2].size())-1);
                                                if(notreal(s))
                                                {
                                                    cout<<"Nan, Value should be integerC"<<endl;
                                                }
                                            //}

                                        // }
                                        // else
                                        //     cout<<"Error in unit of capacitor"<<endl;
                                         }   
                                        else
                                            cout<<"Wrong prefix given for capacitor"<<endl;

                                        //i++;
                                }
                                // else if(!(a[i-1][0]=='C'||a[i-1][0]=='c' || a[i-1][0]=='L'||a[i-1][0]=='l' || a[i-1][0]=='R'||a[i-1][0]=='r'))
                                //     {
                                //         cout<<"Component should be named properly"<<endl;
                                //         i++;
                                //     }

                                if(a[i+2] == "SINE")
                                    {   
                                        if((a[i-1][0]=='v'|| a[i-1][0]=='V') || (a[i-1][0]=='I' || a[i-1][0]=='i'))
                                        {
                                            //assuming damping to be zero
                                            if(a[i+3]=="(" && ((a[i+8]==")") || a[i+9]==")"))
                                            {
                                                if(notreal(a[i+4])) cout<<"offset Error"<<endl;
                                                else if(notreal(a[i+5])) cout<<"Amplitude Error"<<endl;
                                                else if(notreal(a[i+6].substr(0,a[i+6].size()-3))) 
                                                {
                                                    cout<<"frequency Error"<<endl;
                                                }
                                                else if(notreal(a[i+7].substr(0,a[i+7].size()-1))) 
                                                {
                                                     cout<<"delay Error"<<endl;
                                                }
                                                // else if(a[i+8]!=")")
                                                // {
                                                //     if(notreal(a[i+8])) cout<<"damping Error"<<endl;
                                                // }
                                                else
                                                {

                                                    char b=a[i+6][(a[i+6].size())-3];
                                                    char c=a[i+6][(a[i+6].size())-2];
                                                    char d=a[i+6][(a[i+6].size())-1];
                                                    char e=a[i+7][(a[i+7].size())-1];
                                                    //cout<<b<<c<<d<<e;
                                                    if(!(c=='H'|| c=='h') || !(d=='z'))
                                                    {
                                                        //cout<<"hello";
                                                        if(!(b=='k'|| b=='K'))
                                                        cout<<"frequency prefix error"<<endl;
                                                        else 
                                                        cout<<"frequency unit error"<<endl;
                                                    }
                                                    else if(!(e=='s' || e=='S'))
                                                    {
                                                        cout<<"delay time error"<<endl;
                                                    }
                                                }
                                            }
                                            else 
                                                cout<<"proper comma are not used in voltage or current format"<<endl;
                                        }
                                        else 
                                            cout<<"Error in either current or voltage"<<endl;

                                          
                                          i=i+8;
                                    }
                                    //else cout<<"Sine error"<<endl;
                        }
                        else
                           {
                                cout<<"Net2 error"<<endl;
                                i=a.size();
                            }
                     
                    i++;
                //}
            }

                i++;
                
        }  

}

vector<net_vori> vori;

//find total no. of voltage or current source
int no_of_VS(vector<net_vori> vori)
{
    int no_of_VS;
    for (int i = 0; i < vori.size(); i++)
    {
        if((vori[i].name)[0]=='V'||(vori[i].name)[0]=='v')
            no_of_VS++;
        //cout<<vori[i].name[0]<<endl;
    }
    return no_of_VS;
}
//also need to count current sources
elem_thru_net net_data[100][100];


//find total no. of nodes
int no_of_node(elem_thru_net net_data[][100])
{
    vector<int> net_count;
    for (int i = 0; i < 100; i++)
    {
        bool found = 0;
        for (int j = 0; j <i ; j++)
        {
            if(net_data[j][i].RLC.size()+net_data[j][i].VI.size()!=0)
            {   found = 1;
                break;
            }
        }
        if(found == 0)
        {   for (int j = i; j <100 ; j++)
            {
                if(net_data[i][j].RLC.size()+net_data[i][j].VI.size()!=0!=0)
                {   found = 1;
                    break;
                }
            }
        }
        if(found == 1)
        net_count.push_back(i);
    }
    return (net_count.size()-1);
}


//to conv frequency unit 
double freqUnit(string freq)
{
    double fr=stod(freq);
    char q=freq[freq.size()-3];
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
    return fr;
} 

Complex get_Impedance(net_comp nets,string freq)
{
    char p=(nets.name[0]);
    //cout<<p<<endl;
    double fr=stod(freq.substr(0,freq.size()-3));
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
        string valueR=(nets.mag).substr(0,(nets.mag).size());
        double valR=stod(valueR);
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
            double valR=stod(valueR);
        }
        //cout<<"valR"<<valR<<endl;
    return div(makeComplex(1,0),makeComplex(valR,0));
    }
    else if(p=='C'|| p=='c')
    {
        string valueLC=(nets.mag).substr(0,(nets.mag).size()-2);
        //cout<<valueLC<<endl;
        double valLC=stod(valueLC);
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
        double valLC=stod(valueLC);
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

//make another function to B,C,D matrix
void matrixB(vector<net_vori> source,Complex B1[][20],int a, int b)
{
    int s=a;
    int t=b;
    for (int i = 0; i < source.size(); ++i)
    {
        string name=(source[i].name).substr(1,(source[i].name).size());
        int vN=stod(name);
        //cout<<"h"<<vN<<endl;
        string temp3=(source[i].net1).substr(3,(source[i].net1).size());
        int temp3V=stod(temp3);
        //cout<<"temp3"<<temp3<<vN<<endl;
        B1[temp3V-1][vN-1]=makeComplex(1,0);
        //cout<<B1[temp3V-1][0].real<<","<<B1[temp3V-1][0].img<<endl;
        if((source[i].net2)[0]=='N'||(source[i].net2)[0]=='n')
        {
            //cout<<"loop"<<endl;
            string temp4=(source[i].net2).substr(3,(source[i].net2).size());
            int temp4V=stod(temp4);
            B1[temp4V-1][vN-1]=makeComplex(-1,0);
        }
    }
}


///########solve functiion start #################
void solve(vector<net_comp> net,net_vori source, int a, int b)
{
    //cout<<"hello"<<endl;
    int s= a;
    //cout<<a<<endl;
    int t=b;
    //cout<<b;
    Complex A[s+t][20];
    Complex G[t][t];
    Complex B[t][s];
    Complex B1[t][20];
    Complex C[s][t],D[s][s];
    Complex x[s+t][1];
    Complex z[s+t][1];
    //G matrix
    for (int i = 0; i < net.size(); i++)
    {
        //cout<<"hello"<<endl;
         if(((net[i].net1)[0]=='N'||(net[i].net1)[0]=='n')&&((net[i].net2)[0]=='N'||(net[i].net2)[0]=='n'))
         {
            string temp1=(net[i].net1).substr(3,(net[i].net1).size());
            string temp2 =(net[i].net2).substr(3,(net[i].net2).size());
            //cout<<temp1<<endl<<temp2<<endl;
            int temp1V=stod(temp1);
            int temp2V=stod(temp2);
            //cout<<"hi"<<temp1V<<temp2V<<endl;
            G[temp1V-1][temp1V-1]=Add(G[temp1V-1][temp1V-1],get_Impedance(net[i],source.damp));
            G[temp2V-1][temp2V-1]=Add(G[temp2V-1][temp2V-1],get_Impedance(net[i],source.damp));
            G[temp2V-1][temp1V-1]=Sub(G[temp2V-1][temp1V-1],get_Impedance(net[i],source.damp));
            G[temp1V-1][temp2V-1]=Sub(G[temp1V-1][temp2V-1],get_Impedance(net[i],source.damp));
         }
         else if(((net[i].net1)[0]=='N'||(net[i].net1)[0]=='n')&&((net[i].net2)=="0"))
         {
            //cout<<"hi"<<endl;
            string temp1=(net[i].net1).substr(3,(net[i].net1).size());
            //cout<<temp1<<endl;
            int temp1V=stod(temp1);
            //cout<<"hi"<<temp1V;
            //cout<<source[0].freq<<source[0].dc<<source[0].name<<source[0].delay<<source[0].net1<<source[0].net2<<source[0].damp;
            G[temp1V-1][temp1V-1]=Add(G[temp1V-1][temp1V-1],get_Impedance(net[i],source.damp));
         }
    }
    /*for (int i = 0; i < t; ++i)
    {
        for (int j = 0; j < t; ++j)
        {
            cout<<i<<j<<" "<<G[i][j].real<<" "<<G[i][j].img<<endl;
        }
    }*/

    //B matrix 1 if +ve terminal else -1 or 0 if not connected
    //B n*m need to traverse source structure
    
    matrixB(vori,B1,s,t);

    // for (int i = 0; i < t; ++i)
    // {
    //     for (int j = 0; j < s; ++j)
    //     {
    //         B[i][j]=B1[i][j];
    //         cout<<i<<j<<"B "<<B[i][j].real<<" "<<B[i][j].img<<endl;
    //     }
    // }
    //intially intialized with zero
    //else if(source.net2=="0")
    //{
        //make 
    //}
    //C=transpose(B);
    for (int i = 0; i < s; i++)
    {
        
        for (int j = 0; j <t; j++)
        {
            C[i][j]=B[j][i];
        }
    }
    //find no. of voltage source D m*m
    for (int i = 0; i < s; i++)
    {
        for (int j = 0; j <s; j++)
        {
            D[i][j]=makeComplex(0,0);
        }
    }
    

    //A matrix
    for (int i = 0; i < t; i++)
    {
        for (int j = 0; j < t; j++)
        {
            A[i][j]=G[i][j];
        }
    }
    for (int i = t; i < s+t; i++)
    {
        for (int j = 0; j <t; j++)
        {
            A[i][j]=C[i-t][j];
        }
    }
    for (int i = 0; i <t ; i++)
    {
        for (int j = t; j < s+t; j++)
        {
            A[i][j]=B[i][j-t];
        }
    }
    for (int i = t; i <s+t ; i++)
    {
        for (int j = t; j < s+t; j++)
        {
            A[i][j]=D[i-t][j-t];
        }
    }


    //A[2][2]=D[1][1];
    // cout<<s<<t<<endl;
    // for (int i = 0; i < s+t; i++)
    // {
    //     for (int j = 0; j < s+t; j++)
    //     {
    //         cout<<i<<j<<",A,"<<A[i][j].real<<","<<A[i][j].img<<endl;
    //     }
    // }

    //x is unknown matrix
    //voltage of each net  n*1
    //assuming voltage of ground to be zero
    Complex V[t][1];
    //current through voltage source m*1
    Complex I[s][1];
    for (int i = 0; i < t; ++i)
    {
        for (int j = 0; j < 1; j++)
        {
            x[i][j]=V[i][j];
        }
    }
    for (int i = t; i < s+t; ++i)
    {
        for (int j = 0; j < 1; j++)
        {
            x[i][j]=I[i-t][j];
        }
    }


    /*string net1;
    int inv = 0;
    string net2;
    string name;
    string dc;
    string freq;
    string damp;
    string delay;*/
    //cout<<source.freq<<source.dc<<source.name<<source.delay<<source.net1<<source.net2<<source.damp<<source.net1<<source.net2;
    //##################z matrix (n+m)*1
    //current through current source to particular net
    Complex I_n[t][1];
    Complex V_s[s][1];
    //Known complex voltage of source
    //string s=source.
    //if(source.)
    string ph=(source.delay).substr(1,(source.delay).size());
    char vi=(source.name)[0];
    //cout<<stod(ph)<<"ph";
    //cout<<vi<<"ph";
    if(vi=='I')
    {
        if(source.net1!="0")
        {
            string temp1=(source.net1).substr(3,(source.net1).size());
            int temp1V=stod(temp1);
            double fr=freqUnit(source.damp);
            double omega=2*pi*fr;
            double angle=(stod(source.delay)*omega);
            double magn=stod(source.freq);
            //also have to consider delay and dc offset
            I_n[temp1V][0]=makeComplex((magn)*cos(angle),(magn)*sin(angle));
            for (int i = t; i <t+s ; ++i)
            {
                V_s[i][0]=makeComplex(0,0);
            }
        }
    }
    else if(vi=='V'||vi=='v')
    {
        string temp1=(source.net1).substr(3,(source.net1).size());
        int temp1V=stod(temp1);
        double fr=freqUnit(source.damp);
        double omega=2*pi*fr;
        //cout<<omega<<"om"<<endl;
        double angle=stod(source.delay)*omega;
        double magn=stod(source.freq);
        //cout<<sin(angle)<<"V"<<magn<<endl;
        //also have to consider delay and dc offset
        V_s[temp1V-1][0]=makeComplex((magn)*cos(angle),(magn)*sin(angle));
        //cout<<V_s[temp1V-1][0].real<<" "<<V_s[temp1V-1][0].img<<"val";
        for (int i = 0; i <t ; ++i)
        {
            I_n[i][0]=makeComplex(0,0);
        }
    }

    for (int i = 0; i <t; i++)
    {
        for (int j = 0; j < 1; j++)
        {
            z[i][j]=I_n[i][j];
        }
    }

    for (int i = t; i <t+s; i++)
    {
        for (int j = 0; j < 1; j++)
        {
            //cout<<"loop"<<V_s[i-t][j].real<<V_s[i-t][j].img;
            z[i][j]=V_s[i-t][j];
        }
    }
    
    //cout<<"t"<<t<<"s"<<s;
    // for (int i = 0; i < s+t; ++i)
    // {
    //     for (int j = 0; j < 1; j++)
    //     {
    //         cout<<i<<j<<"z  "<<z[i][j].real<<"+ j"<<z[i][j].img<<endl;
    //     }
    // }
    // cout<<"s+t"<<s+t<<endl;
     int p=s+t;
     Complex det=determinant(A ,p);
    //cout<<"det"<<det.real<<" "<<det.img<<endl;

    Complex adj[s+t][20];
    adjoint(A,adj);
    // for (int i = 0; i < s+t; i++)
    // {
    //     for (int j = 0; j < s+t; j++)
    //     {
    //         cout<<i<<j<<"adj  "<<adj[i][j].real<<" "<<adj[i][j].img<<endl;
    //     }
    // }

    Complex inv[s+t][20];
    inverse(A,inv);
    //inv[2][2].real=0;
    // for (int i = 0; i < s+t; i++)
    // {
    //     for (int j = 0; j < s+t; j++)
    //     {
    //         cout<<i<<j<<"inv(a)*z "<<inv[i][j].real<<" "<<inv[i][j].img<<endl;
    //     }
    // }
    multiply(inv,z,x);
    //printing voltage of each net and current through voltage source
    // for (int i = 0; i < s+t; i++)
    // {
    //     //cout<<"hello"<<endl;
    //     for (int j = 0; j < 1; j++)
    //     {
    //         cout<<i<<j<<"inv(a)*z "<<x[i][j].real<<" "<<x[i][j].img<<endl;
    //     }
    // }

    //print solution and write solution to results.txt

    //print voltage
    fout.open("results.txt");
    fout<<"Voltage"<<endl;
    fout<<source.name<<" ";
    //string temp3=(source.net1).substr(3,(source.net1).size());
    //int temp3V=stoi(temp3);
    //cout<<temp3<<endl;
    //B[temp3V-1][0]=makeComplex(1,0);
    //cout<<B[temp3V-1][1].real<<","<<B[temp3V-1][1].img<<endl;
    string temp3=(source.net1).substr(3,(source.net1).size());
    int temp3V=stod(temp3);
    if((source.net2)[0]=='N'||(source.net2)[0]=='n')
    {
        //cout<<"loop"<<endl;
        string temp4=(source.net2).substr(3,(source.net2).size());
        int temp4V=stod(temp4);
        Complex V=Sub(x[temp3V-1][0],x[temp4V-1][0]);
        //cout<<abs(V)<<" "<<atan(V.img/V.real)<<endl;
        //B[temp3V-1][0]=makeComplex(-1,0);
    }
    else{
        Complex V=Sub(x[temp3V-1][0],makeComplex(0,0));
        fout<<abs(V)<<" "<<atan(V.img/V.real)<<endl;
    }
    
    for (int i = 0; i < net.size(); i++)
    {
        // cout<<"hello"<<endl;
         if(((net[i].net1)[0]=='N'||(net[i].net1)[0]=='n')&&((net[i].net2)[0]=='N'||(net[i].net2)[0]=='n'))
         {
            string temp1=(net[i].net1).substr(3,(net[i].net1).size());
            string temp2 =(net[i].net2).substr(3,(net[i].net2).size());
            int temp1V=stod(temp1);
            int temp2V=stod(temp2);
            //cout<<x[temp1V-1][1].real<<" "<<x[temp2V-1][1].real<<endl;
            Complex res=Sub(x[temp1V-1][0],x[temp2V-1][0]);
            fout<<net[i].name<<" "<<abs(res)<<" "<<atan(res.img/res.real)<<endl;
         }
         else if(((net[i].net1)[0]=='N'||(net[i].net1)[0]=='n')&&((net[i].net2)=="0"))
         {
            //cout<<"hi"<<endl;
            string temp1=(net[i].net1).substr(3,(net[i].net1).size());
            int temp1V=stod(temp1);
            //cout<<temp1V<<endl;
            Complex res=Sub(x[temp1V-1][0],makeComplex(0,0));
            fout<<(net[i].name)<<" "<<abs(res)<<" "<<atan(res.img/res.real)<<endl;
         }
    }

    //print current
    fout<<"Current"<<endl;
    cout<<source.name<<" "<<abs(x[t][0])<<" "<<atan(x[t][0].img/x[t][0].real)<<endl;
    for (int i = 0; i < net.size(); i++)
    {
        // cout<<"hello"<<endl;
         if(((net[i].net1)[0]=='N'||(net[i].net1)[0]=='n')&&((net[i].net2)[0]=='N'||(net[i].net2)[0]=='n'))
         {
            string temp1=(net[i].net1).substr(3,(net[i].net1).size());
            string temp2 =(net[i].net2).substr(3,(net[i].net2).size());
            int temp1V=stod(temp1);
            int temp2V=stod(temp2);
            //cout<<x[temp1V-1][1].real<<" "<<x[temp2V-1][1].real<<endl;
            Complex res=div(Sub(x[temp1V-1][0],x[temp2V-1][0]),get_Impedance(net[i],source.freq));
            cout<<net[i].name<<" "<<abs(res)<<" "<<atan(res.img/res.real)<<endl;
         }
         else if(((net[i].net1)[0]=='N'||(net[i].net1)[0]=='n')&&((net[i].net2)=="0"))
         {
            //cout<<"hi"<<endl;
            string temp1=(net[i].net1).substr(3,(net[i].net1).size());
            int temp1V=stod(temp1);
            //cout<<temp1V<<endl;
            Complex res=div(Sub(x[temp1V-1][0],makeComplex(0,0)),get_Impedance(net[i],source.freq));
            cout<<(net[i].name)<<" "<<abs(res)<<" "<<atan(res.img/res.real)<<endl;
         }
    }
}
///////#############solve function end here#########


int main()
{
    
    std::vector<string> a;
    fin.open("top.cir");
    char s;
    char *t;
    int capacity = 10;
    t = new char[capacity];
    int ssize = 0;
    
    vector<int> netlist;
    
    int netmax = 0;
    avail aval[100];
    int y_cord = 0;
    int x_cord = 0;
    drw draw[100][100];
    int r=0;
    int v=0;
    int k = 0;
    avail dest[100];
    avail star[100];


    while (!fin.eof())
    {


            fin.get(s);
            if(!(s<123&&int(s)>96||int(s)>64&&int(s)<91||int(s)>47&&int(s)<58||int(s)==46))
                {
                   
                    t[ssize]='\0';
                    if(ssize>=1)
                    {
                     a.push_back(t);
                     string t1 = t;
                     if(t1.substr(0, 3) == "Net" || t1.substr(0, 3) == "NET"||t1.substr(0, 3) == "net")
                        {
                            
                            int temp = t1.at(3)-48;
                            //cout<<temp;
                            bool found = 0;
                            for(int i=0; i< netlist.size(); i++)
                            {
                                if(netlist[i] == temp)
                                {
                                    found = 1;
                                }
                                
                            }
                            if(found == 0)
                                    netlist.push_back(temp);
                        }


                    }
                    if(int(s)==40)
                    {
                        
                        a.push_back("(");
                    }
                    else
                    if(int(s)==41)
                    {
                        a.push_back(")");
                    }

                    ssize= 0;
                    capacity=10;
                    t=new char [capacity];
                }
                

            else
                {  
                   
                    if(ssize<capacity)
                    {   
                         t[ssize]=s;
                        ssize++;
                    }
                    else
                    {   char*temp = t;

                    t = new char[capacity*2];
                    capacity = 2*capacity;
                    for(int i=0; i<ssize;i++)
                        t[i] = temp[i];
                    t[ssize] = s;
                    ssize++;
                    delete []temp;
                    }
                }
    }
fin.close();

    



for(int i=0;i<netlist.size();i++)
{
    if(netlist[i]>netmax)
        netmax = netlist[i];
}


// for (int i = 0; i < a.size(); ++i)
// {
//     cout<<a[i]<<endl;
// }

check(a);

int i=0;
while(i<a.size())
    {
        if(a[i].substr(0, 3) == "Net" || a[i].substr(0, 3) == "NET"|| a[i].substr(0, 3) == "net")

            {
                if(a[i+1].substr(0, 3) == "Net" || a[i+1].substr(0, 3) == "NET" || a[i+1].substr(0, 3) == "net" || a[i+1] =="0")

                {
                        if(a[i+1] == "0" && count0 ==0)
                        count0 = 1;

                       if(a[i+2] != "SINE")  // it is RLC
                            {   
                                        net_comp temp;
                                        if(a[i]!="0" && a[i+1]!="0")
                                        {   if(a[i].at(3)<a[i+1].at(3))
                                            {temp.net1 =a[i];
                                            temp.net2 =a[i+1];}
                                            else
                                            {temp.net1 =a[i+1];
                                            temp.net2 =a[i];}
                                        }
                                        else if(a[i]=="0")
                                        {   
                                            temp.net1 =a[i+1];
                                            temp.net2 =a[i];
                                        }
                                        else
                                        {   
                                            temp.net2 =a[i+1];
                                            temp.net1 =a[i];
                                        }
                                        temp.name = a[i-1];
                                        temp.mag = a[i+2];
                                        comp.push_back(temp);
                                        i = i+4;
                            }
                        else
                            {   
                                        net_vori temp;
                                        if(a[i]!="0" && a[i+1]!="0")
                                        {   if(a[i].at(3)<a[i+1].at(3))
                                            {temp.net1 =a[i];
                                            temp.net2 =a[i+1];}
                                            else
                                            {temp.net1 =a[i+1];
                                            temp.net2 =a[i];
                                            temp.inv = 1;}
                                        }
                                        else if(a[i]=="0")
                                        {   
                                            temp.net1 =a[i+1];
                                            temp.net2 =a[i];
                                            temp.inv = 1;
                                        }
                                        else
                                        {   
                                            temp.net2 =a[i+1];
                                            temp.net1 =a[i];
                                        }
                                        temp.name = a[i-1];
                                        temp.dc = a[i+4];
                                        temp.freq = a[i+5];
                                        temp.damp = a[i+6];
                                        temp.delay = a[i+7];
                                        vori.push_back(temp);
                            
                                  
                                  i=i+8;
                            }
                }
                else
                i++;
            }
            else
                i++;
            
    }

vector<net_comp> new_comp;
vector<net_vori> new_vori;
for(int i=0; i< comp.size(); i++)
{
    bool y=0;
    bool er=0; 
    for(int j=i+1; j< comp.size(); j++)
    {
        if(comp[i].name == comp[j].name)
        {   y=1;
            if(comp[i].net1 != comp[j].net1 || comp[i].net2 != comp[j].net2 || comp[i].mag != comp[j].mag)
                er =1;
        }
    }
    if(y == 0 && er == 0)
    {
        new_comp.push_back(comp[i]);
    }
}
for(int i=0; i< vori.size(); i++)
{
    bool y=0;
    bool er=0; 
    for(int j=i+1; j< vori.size(); j++)
    {
        if(vori[i].name == vori[j].name)
        {   y=1;
            if(vori[i].net1 != vori[j].net1 || vori[i].net2 != vori[j].net2 || vori[i].dc != vori[j].dc || vori[i].freq != vori[j].freq || vori[i].damp != vori[j].damp || vori[i].delay != vori[j].delay)
                er =1;
        }
    }
    if(y == 0 && er == 0)
    {
        new_vori.push_back(vori[i]);
    }
    else if(er == 1)
        input_error++;
}

comp = new_comp;
vori = new_vori;


for(int i=0; i< comp.size(); i++)
{
    if(comp[i].net1 != "0" && comp[i].net2 != "0")
    net_data[comp[i].net1.at(3)-48][comp[i].net2.at(3)-48].RLC.push_back(comp[i]);
    else if (comp[i].net1 == "0" && comp[i].net2 != "0")
    net_data[netmax+count0][comp[i].net2.at(3)-48].RLC.push_back(comp[i]);
    else
    net_data[comp[i].net1.at(3)-48][netmax + count0].RLC.push_back(comp[i]);
    
}
for(int i=0; i< vori.size(); i++)
{
    if(vori[i].net1 != "0" && vori[i].net2 != "0")
    net_data[vori[i].net1.at(3)-48][vori[i].net2.at(3)-48].VI.push_back(vori[i]);
    else if (vori[i].net1 == "0" && vori[i].net2 != "0")
    net_data[netmax+count0][vori[i].net2.at(3)-48].VI.push_back(vori[i]);
    else
    net_data[vori[i].net1.at(3)-48][netmax + count0].VI.push_back(vori[i]);
    
}








for(int i=0; i< netmax+count0+1; i++)
{
    
    vector<net_comp> temp_comp;
    vector<net_vori> temp_vori;
    
    for(int j=i; j< netmax+count0+1; j++)
    {
        if(net_data[i][j].VI.size()>0)
        {
            for(int m=0; m<net_data[i][j].VI.size(); m++)
            {
                temp_vori.push_back(net_data[i][j].VI[m]);
            }
        }
        if(net_data[j][i].VI.size()>0)
        {
            for(int m=0; m<net_data[j][i].VI.size(); m++)
            {
                temp_vori.push_back(net_data[j][i].VI[m]);
            }
        }
        if(net_data[i][j].RLC.size()>0)
        {
            for(int m=0; m<net_data[i][j].RLC.size(); m++)
            {
                temp_comp.push_back(net_data[i][j].RLC[m]);
            }
        }
        if(net_data[j][i].RLC.size()>0)
        {
            for(int m=0; m<net_data[j][i].RLC.size(); m++)
            {
                temp_comp.push_back(net_data[j][i].RLC[m]);
            }
        }
    }
    int total_elem = temp_comp.size() + temp_vori.size();
    if(total_elem > 0)
        {
            y_cord = y_cord + k - (total_elem-1)/2;
            if(y_cord < 0)
                y_cord = 0;
            r = 0;
            v = 0;
            k = 0;
            while(r < temp_comp.size() && v < temp_vori.size())
            {   
                //cout<<"yo yo";
                if(temp_comp[r].net2 !="0" && temp_vori[v].net2 != "0")
                {   
                    if(temp_comp[r].net2.at(3) < temp_vori[v].net2.at(3))
                    {   draw[k + y_cord][x_cord + 0].netname = temp_comp[r].net1.at(3)-48;
                        draw[k + y_cord][x_cord + 0].right = temp_comp[r].name;
                        draw[k + y_cord][x_cord + 0].type = 0;
                        draw[k + y_cord][x_cord + 0].degree++;
                        draw[k + y_cord][x_cord + 1].netname = temp_comp[r].net2.at(3)-48;
                        draw[k + y_cord][x_cord + 1].left = temp_comp[r].name;
                        draw[k + y_cord][x_cord + 1].type = 0;
                        draw[k + y_cord][x_cord + 1].degree++;
                        aval[temp_comp[r].net1.at(3)].first.push_back(k + y_cord);
                        aval[temp_comp[r].net1.at(3)].second.push_back(x_cord + 0);
                        aval[temp_comp[r].net2.at(3)].first.push_back(k + y_cord);
                        aval[temp_comp[r].net2.at(3)].second.push_back(x_cord + 1);
                        r++;
                        k++;
                    }
                    else
                    {   draw[k + y_cord][x_cord + 0].netname = temp_vori[v].net1.at(3)-48;
                        draw[k + y_cord][x_cord + 0].right = temp_vori[v].name;
                        draw[k + y_cord][x_cord + 0].type = 1;
                        draw[k + y_cord][x_cord + 0].degree++;
                        draw[k + y_cord][x_cord + 1].netname = temp_vori[v].net2.at(3)-48;
                        draw[k + y_cord][x_cord + 1].left = temp_vori[v].name;
                        draw[k + y_cord][x_cord + 1].type = 1;
                        draw[k + y_cord][x_cord + 1].degree++;
                        aval[temp_vori[v].net1.at(3)].first.push_back(k + y_cord);
                        aval[temp_vori[v].net1.at(3)].second.push_back(x_cord + 0);
                        aval[temp_vori[v].net2.at(3)].first.push_back(k + y_cord);
                        aval[temp_vori[v].net2.at(3)].second.push_back(x_cord + 1);
                        v++;
                        k++;
                    }
                }
                else if(temp_comp[r].net2 =="0")
                {
                        
                        while(v<temp_vori.size())
                        {   draw[k + y_cord][x_cord + 0].netname = temp_vori[v].net1.at(3)-48;
                            draw[k + y_cord][x_cord + 0].right = temp_vori[v].name;
                            draw[k + y_cord][x_cord + 0].type = 1;
                            draw[k + y_cord][x_cord + 0].degree++;

                            if (temp_vori[v].net2 != "0")
                            {   draw[k + y_cord][x_cord + 1].netname = temp_vori[v].net2.at(3)-48;
                                draw[k + y_cord][x_cord + 1].left = temp_vori[v].name;
                                draw[k + y_cord][x_cord + 1].type = 1;
                                draw[k + y_cord][x_cord + 1].degree++;
                                aval[temp_vori[v].net1.at(3)].first.push_back(k + y_cord);
                                aval[temp_vori[v].net1.at(3)].second.push_back(x_cord + 0);
                                aval[temp_vori[v].net2.at(3)].first.push_back(k + y_cord);
                                aval[temp_vori[v].net2.at(3)].second.push_back(x_cord + 1);
                            }
                            else
                            {   draw[k + y_cord][x_cord + 1].netname = -1;
                                draw[k + y_cord][x_cord + 1].left = temp_vori[v].name;
                                draw[k + y_cord][x_cord + 1].type = 1;
                                draw[k + y_cord][x_cord + 1].degree++;
                                aval[temp_vori[v].net1.at(3)].first.push_back(k + y_cord);
                                aval[temp_vori[v].net1.at(3)].second.push_back(x_cord + 0);
                                aval[netmax + count0].first.push_back(k + y_cord);
                                aval[netmax + count0].second.push_back(x_cord + 1);
                            }
                            v++;
                            k++;
                        }
                        while(r<temp_comp.size())
                        {   draw[k + y_cord][x_cord + 0].netname = temp_comp[r].net1.at(3)-48;
                            draw[k + y_cord][x_cord + 0].right = temp_comp[r].name;
                            draw[k + y_cord][x_cord + 0].type = 0;
                            draw[k + y_cord][x_cord + 0].degree++;
                            draw[k + y_cord][x_cord + 1].netname = -1;
                            draw[k + y_cord][x_cord + 1].left = temp_comp[r].name;
                            draw[k + y_cord][x_cord + 1].type = 0;
                            draw[k + y_cord][x_cord + 1].degree++;
                            aval[temp_comp[r].net1.at(3)].first.push_back(k + y_cord);
                            aval[temp_comp[r].net1.at(3)].second.push_back(x_cord + 0);
                            aval[netmax + count0].first.push_back(k + y_cord);
                            aval[netmax + count0].second.push_back(x_cord + 1);
                            r++;
                            k++;
                        }
                }
                else if(temp_vori[v].net2 =="0")
                {
                    //cout<<"helo";
                    while(r<temp_comp.size())
                    {   draw[k + y_cord][x_cord + 0].netname = temp_comp[r].net1.at(3)-48;
                        draw[k + y_cord][x_cord + 0].right = temp_comp[r].name;
                        draw[k + y_cord][x_cord + 0].type = 0;
                        draw[k + y_cord][x_cord + 0].degree++;
                        if (temp_comp[r].net2 != "0")
                        {   draw[k + y_cord][x_cord + 1].netname = temp_comp[r].net2.at(3)-48;
                            draw[k + y_cord][x_cord + 1].left = temp_comp[r].name;
                            draw[k + y_cord][x_cord + 1].type = 0;
                            draw[k + y_cord][x_cord + 1].degree++;
                            aval[temp_comp[r].net1.at(3)].first.push_back(k + y_cord);
                            aval[temp_comp[r].net1.at(3)].second.push_back(x_cord + 0);
                            aval[temp_comp[r].net2.at(3)].first.push_back(k + y_cord);
                            aval[temp_comp[r].net2.at(3)].second.push_back(x_cord + 1);
                        }
                        else
                        {   draw[k + y_cord][x_cord + 1].netname = -1;
                            draw[k + y_cord][x_cord + 1].left = temp_comp[r].name;
                            draw[k + y_cord][x_cord + 1].type = 0;
                            draw[k + y_cord][x_cord + 1].degree++;
                            aval[temp_comp[r].net1.at(3)].first.push_back(k + y_cord);
                            aval[temp_comp[r].net1.at(3)].second.push_back(x_cord + 0);
                            aval[netmax + count0].first.push_back(k + y_cord);
                            aval[netmax + count0].second.push_back(x_cord + 1);
                        }
                        r++;
                        k++;
                    }
                    while(v<temp_vori.size())
                    {   draw[k + y_cord][x_cord + 0].netname = temp_vori[v].net1.at(3)-48;
                        draw[k + y_cord][x_cord + 0].right = temp_vori[v].name;
                        draw[k + y_cord][x_cord + 0].type = 1;
                        draw[k + y_cord][x_cord + 0].degree++;
                        draw[k + y_cord][x_cord + 1].netname = -1;
                        draw[k + y_cord][x_cord + 1].left = temp_vori[v].name;
                        draw[k + y_cord][x_cord + 1].type = 1;
                        draw[k + y_cord][x_cord + 1].degree++;
                        aval[temp_vori[v].net1.at(3)].first.push_back(k + y_cord);
                        aval[temp_vori[v].net1.at(3)].second.push_back(x_cord + 0);
                        aval[netmax + count0].first.push_back(k + y_cord);
                        aval[netmax + count0].second.push_back(x_cord + 1);
                        v++;
                        k++;
                    }       
                }

            }

            if(r >= temp_comp.size())
                {
                    //cout<<"r has fault";
                    while(v < temp_vori.size())
                    {
                        //cout<<"rlc has fault";
                        draw[k + y_cord][x_cord + 0].netname = temp_vori[v].net1.at(3)-48;
                        draw[k + y_cord][x_cord + 0].right = temp_vori[v].name;
                        draw[k + y_cord][x_cord + 0].type = 1;
                        draw[k + y_cord][x_cord + 0].degree++;
                        if (temp_vori[v].net2 != "0")
                        {   draw[k + y_cord][x_cord + 1].netname = temp_vori[v].net2.at(3)-48;
                            draw[k + y_cord][x_cord + 1].left = temp_vori[v].name;
                            draw[k + y_cord][x_cord + 1].type = 1;
                            draw[k + y_cord][x_cord + 1].degree++;
                            aval[temp_vori[v].net1.at(3)].first.push_back(k + y_cord);
                            aval[temp_vori[v].net1.at(3)].second.push_back(x_cord + 0);
                            aval[temp_vori[v].net2.at(3)].first.push_back(k + y_cord);
                            aval[temp_vori[v].net2.at(3)].second.push_back(x_cord + 1);
                        }
                        else
                        {   draw[k + y_cord][x_cord + 1].netname = -1;
                            draw[k + y_cord][x_cord + 1].left = temp_vori[v].name;
                            draw[k + y_cord][x_cord + 1].type = 1;
                            draw[k + y_cord][x_cord + 1].degree++;
                            aval[temp_vori[v].net1.at(3)].first.push_back(k + y_cord);
                            aval[temp_vori[v].net1.at(3)].second.push_back(x_cord + 0);
                            aval[netmax + count0].first.push_back(k + y_cord);
                            aval[netmax + count0].second.push_back(x_cord + 1);
                        }
                        v++;
                        k++;
                    }
                }
            else if (v >= temp_vori.size())
                {
                    //cout<<"v has fault";
                    while(r < temp_comp.size())
                    {
                        //cout<<"vi has fault";
                        draw[k + y_cord][x_cord + 0].netname = temp_comp[r].net1.at(3)-48;
                        draw[k + y_cord][x_cord + 0].right = temp_comp[r].name;
                        draw[k + y_cord][x_cord + 0].type = 0;
                        draw[k + y_cord][x_cord + 0].degree++;
                        if (temp_comp[r].net2 != "0")
                        {   draw[k + y_cord][x_cord + 1].netname = temp_comp[r].net2.at(3)-48;
                            draw[k + y_cord][x_cord + 1].left = temp_comp[r].name;
                            draw[k + y_cord][x_cord + 1].type = 0;
                            draw[k + y_cord][x_cord + 1].degree++;
                            aval[temp_comp[r].net1.at(3)].first.push_back(k + y_cord);
                            aval[temp_comp[r].net1.at(3)].second.push_back(x_cord + 0);
                            aval[temp_comp[r].net2.at(3)].first.push_back(k + y_cord);
                            aval[temp_comp[r].net2.at(3)].second.push_back(x_cord + 1);
                        }
                        else
                        {   draw[k + y_cord][x_cord + 1].netname = -1;
                            draw[k + y_cord][x_cord + 1].left = temp_comp[r].name;
                            draw[k + y_cord][x_cord + 1].type = 0;
                            draw[k + y_cord][x_cord + 1].degree++;
                            aval[temp_comp[r].net1.at(3)].first.push_back(k + y_cord);
                            aval[temp_comp[r].net1.at(3)].second.push_back(x_cord + 0);
                            aval[netmax + count0].first.push_back(k + y_cord);
                            aval[netmax + count0].second.push_back(x_cord + 1);
                        }
                        r++;
                        k++;
                    }
                }
         x_cord = x_cord + 3;
        }
}
int wid = 0;
int hei = 0;
for(int p=0; p<100; p++)
{
    for(int q=0; q<100; q++)
    {
        if(draw[p][q].netname != -2)
        {
            if(p>hei)
                hei = p;
            if(q > wid)
                wid = q;
        }
    }
}

// Making SVG


fout.open("ans.svg");
start((wid+1)*250, (hei+1)*150);
int x=85;
int y=50;
int st = -5;
int en = -5;
int elem = -5;
int st1 = -5;
int en1 = -5;
int elem1 = -5; 
for(int c=0; c<98; c+=3)
{
    st = -5;
    en = -5;
    elem = -5;
    st1 = -5;
    en1 = -5;
    elem1 = -5;
    for(int d=0; d<98; d++)
    {
        
        if(elem == draw[d][c].netname)
        {
            en++;
        }
        else
        {   if(st!=en && elem != -2)
            {   fout<<"<path d=\"M"<<((x)-75)<<" "<<50+((st)*50) <<" L "<<((x)-75)<<" "<<50+((en)*50)<<" \" stroke=\"blue\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" />\n";
                //cout<<elem<<"\t";
                
            }
            if(elem >= -1)
            {
                //cout<<elem<<"\t";
                if(elem != -1)
                {   star[elem].first.push_back(((x)-75));
                    star[elem].second.push_back(((st+en+2)*(50))/2);
                }
                else if(elem == -1)
                {   star[netmax+count0].first.push_back(((x)-75));
                    star[netmax+count0].second.push_back(((st+en+2)*(50))/2);
                }
            }
           
            st = d;
            en = d;
            elem = draw[d][c].netname;
        }
        if(elem1 == draw[d][c+1].netname)
        {
            en1++;
        }
        else
        {   if(st1!=en1 && elem1 != -2)
            {   fout<<"<path d=\"M"<<((x)+75)<<" "<<50+((st1)*50) <<" L "<<((x)+75)<<" "<<50+((en1)*50)<<" \" stroke=\"blue\" stroke-linecap=\"round\" stroke-width=\"1\" fill=\"none\" />\n";
                //cout<<elem1<<"\t";
                
            }
            if(elem1 >= -1)
            {
                //cout<<elem1<<"\t";
                if(elem1 != -1)
                {   dest[elem1].first.push_back(((x)+75));
                    dest[elem1].second.push_back(((st1+en1+2)*(50))/2);
                }
                else if(elem1 == -1)
                {   dest[netmax+count0].first.push_back(((x)+75));
                    dest[netmax+count0].second.push_back(((st1+en1+2)*(50))/2);
                }
            }
            st1 = d;
            en1 = d;
            elem1 = draw[d][c+1].netname;
        }

        if(draw[d][c].netname != -2)
        {
            if(draw[d][c].type == 0 && draw[d][c].right != "null")
            {   if(draw[d][c].right.at(0) == 'r' || draw[d][c].right.at(0) == 'R')
                {
                    resistance(x, y, 0, draw[d][c].right);
                    
                }
                else if(draw[d][c].right.at(0) == 'c' || draw[d][c].right.at(0) == 'C')
                {
                    capacitor(x, y, 0, draw[d][c].right);
                }
                else if(draw[d][c].right.at(0) == 'l' || draw[d][c].right.at(0) == 'L')
                {
                    inductor(x, y, 0, draw[d][c].right);
                }
            }
            else if(draw[d][c].type == 1 && draw[d][c].right != "null")
            {
                if(draw[d][c].right.at(0) == 'v' || draw[d][c].right.at(0) == 'V')
                {
                    int in = 0;
                    for(int y=0; y<vori.size(); y++)
                    {
                        if(draw[d][c].right == vori[y].name)
                        {   in = vori[y].inv;
                            break;
                        }
                    }
                    voltage(x, y, in, draw[d][c].right);
                }
                else if(draw[d][c].right.at(0) == 'i' || draw[d][c].right.at(0) == 'I')
                {
                    int in = 0;
                    for(int y=0; y<vori.size(); y++)
                    {
                        if(draw[d][c].right == vori[y].name)
                        {   in = vori[y].inv;
                            break;
                        }

                    }
                    current(x, y, in, draw[d][c].right);
                }
            }
            
        }
        y+=50;
    }
    x+=250;
    y=50;

}

for(int a=0; a<count0+netmax+1; a++)
{
    if(star[a].first.size()>0 && dest[a].first.size()>0)
    {   for(int g=0; g<dest[a].first.size(); g++)
        {
                connect( dest[a].first[g], dest[a].second[g], star[a].first[0], star[a].second[0], (100/(a+g+1)), g);
            
        }
    }
    else if(star[a].first.size()==0 && dest[a].first.size()>1)
    {
        for(int g=0; g<dest[a].first.size()-1; g++)
        {
                connect_last( dest[a].first[g], dest[a].second[g], dest[a].first[dest[a].first.size()-1], dest[a].second[dest[a].first.size()-1], 100/((a+g+1)), g);
            
        }   
    }
}

if(count0 = 1)
{
    ground(dest[count0+netmax].first[dest[count0+netmax].first.size()-1],dest[count0+netmax].second[dest[count0+netmax].first.size()-1]);
}

end();
fout.close();


if(input_error > 0)
{
    cout<<"Error: Some Error in input file but we have drawn circuit with manipulation so that there is no conflict"<<endl;
}
int net_error = 0;
for(int i; i<count0+netmax+1; i++)
{
    if(star[i].first.size()+dest[i].first.size() == 1)
        net_error++;
}
if(net_error > 1)
{
    cout<<"Error: One singly connected Net"<<endl;
}
// acS= no_of_VS(vori);
// cout<<acS<<endl;
// nodes=no_of_node(net_data);
// cout<<nodes<<endl;

// initializeSize();
for (int i = 0; i < vori.size(); ++i)
{
    //cout<<"hello";
    solve(comp,vori[0], no_of_VS(vori), no_of_node(net_data));
}

//out();
}