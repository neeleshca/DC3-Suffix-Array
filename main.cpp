#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <iomanip>

#define FREQ 2.60e9

using namespace std;

void suffixArray(int *s, int *SA, int n, int K);
void make_lcp(int *sa, int *lcp, const vector<int> &text, int n);
void make_lcplr(int *lcp, int *lcplr, int i, int low, int high);
int find_rightposition(string ipstring, string query, int *SA, int *lcplr);
int find_leftposition(string ipstring, string query, int *SA, int *lcplr);
unsigned long long rdtsc();

int main()
{

    ifstream file1("data.txt");
    ofstream file2("output.txt");
    // ifstream file3("query.txt");
    vector<int> int_string;
    vector<string> arr;

    string temp1;

    string ipstring;


    getline(file1, ipstring);
    std::stringstream delim(ipstring);
    string temp2;
    while (getline(delim, temp2, '@'))
    {
        arr.push_back(temp2);
    }

    for (int i = 0; i < ipstring.length(); i++)
    {
        if (ipstring[i] == '@')
        {
            ipstring[i] = char(1);
        }
    }

    int t = arr.size();

    int k = 1;
    for (int i = 0; i < t; i++)
    {
        for (int j = 0; j < arr[i].length(); j++)
        {
            int_string.push_back(arr[i][j] + t + 1);
        }
        int_string.push_back(k++);
    }

    int alphabet = 256 + t;

    int n = int_string.size();
    int *s = new int[n + 3];
    int *SA = new int[n + 3];
    for (int i = 0; i < n + 3; i++)
    {
        s[i] = int_string[i];
        SA[i] = 1;
    }
    s[n] = s[n + 1] = s[n + 2] = SA[n] = SA[n + 1] = SA[n + 2] = 0;
    unsigned long long int t0sa, t1sa, t0esa, t1esa;

    t0sa = rdtsc(); //start timer

    suffixArray(s, SA, n, alphabet);
    t1sa = rdtsc(); //stop timer

    file2 << std::setprecision(9) << std::showpoint << std::fixed;
    // file2 << "Time to construct suffix array is: " << (t1sa - t0sa) / FREQ << "\n";

    int *lcp = new int[n];

    int temp = pow(2, ceil(log2(n - 1)) + 1);
    if (temp < 100)
    {
        temp = 100;
    }
    int *lcplr = new int[temp];
    for (int i = 0; i < temp; i++)
    {
        lcplr[i] = 0;
    }

    t0esa = rdtsc(); //start timer

    make_lcp(SA, lcp, int_string, n);
    make_lcplr(lcp, lcplr, 0, 0, n - 1);

    t1esa = rdtsc(); //stop timer

    file2 << std::setprecision(9) << std::showpoint << std::fixed;
    // file2 << "Time to construct enhancements for suffix array is: " << (t1esa - t0esa) / FREQ << "\n";
    file2 << ((t1esa-t0esa)+(t1sa-t0sa))/FREQ<<"\n";
    int t1;
    cin >> t1;
    string query;
    getline(cin,query);

    

    
    for (int i3 = 0; i3 < t1; i3++)
    {
        while (getline(cin, query))
        {
            file2<<"Query is "<<query<<"\n";
            unsigned long long int t0, t1;
            t0 = rdtsc(); //start timer

            int r = find_rightposition(ipstring, query, SA, lcplr);
            int l = find_leftposition(ipstring, query, SA, lcplr);
            t1 = rdtsc(); //stop timer

            double time1 = ((t1 - t0) / FREQ);

            int val1 = r - l + 1;

            if (r == -1 || l == -1 || r < l)
            {
                val1 = 1;
            }
            vector<int> pos(val1);
            if (r == -1 || l == -1 || r < l)
            {
                pos[0] = -1;
            }
            else
            {
                int k = 0;
                for (int i = l; i <= r; i++)
                {
                    pos[k++] = SA[i];
                }
            }
            sort(pos.begin(), pos.end());

            file2 << pos.size()<<"   ";
            for (int i = 0; i < pos.size(); i++)
            {
                file2 << pos[i] << ", ";
            }
            file2 << std::setprecision(9) << std::showpoint << std::fixed;

            file2 << time1;
            file2 << "\n";
        }
    }

    delete [] s;
    delete [] SA;
    delete [] lcp;
    delete [] lcplr;

    
    return 0;
}
