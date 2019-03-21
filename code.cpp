#include <iostream>
#include <vector>
using namespace std;

inline bool leq(int a1, int a2, int b1, int b2)
{ 
    return (a1 < b1 || a1 == b1 && a2 <= b2);
} 
inline bool leq(int a1, int a2, int a3, int b1, int b2, int b3)
{
    return (a1 < b1 || a1 == b1 && leq(a2, a3, b2, b3));
}
void c_sort(int *indices, int *SA, int *text, int n, int alphabet_size)
{

    int *counts = new int[alphabet_size + 1];
    for (int i = 0; i <= alphabet_size; i++)
        counts[i] = 0;
    for (int i = 0; i < n; i++)
        counts[text[indices[i]]]++;
    int temp;
    int sum = 0;
    for (int i = 0; i <= alphabet_size; i++)
    {
        temp = counts[i];
        counts[i] = sum;
        sum += temp;
    }
    for (int i = 0; i < n; i++)
        SA[counts[text[indices[i]]]++] = indices[i];
    delete[] counts;
}

// find the suffix array SA of s[0..n-1] in {1..K}^n
// require s[n]=s[n+1]=s[n+2]=0, n>=2
void suffixArray(int *text, int *SA, int n, int alphabet_size)
{

    int n0 = (n + 2) / 3;
    int n1 = (n + 1) / 3;
    int n2 = (n) / 3;
    int n02 = n0 + n2;

    int *text_0 = new int[n0];
    int *SA_0 = new int[n0];

    int *text_12 = new int[n02 + 3];
    int *SA_12 = new int[n02 + 3];

    text_12[n02] = text_12[n02 + 1] = text_12[n02 + 2] = 0;
    SA_12[n02] = SA_12[n02 + 1] = SA_12[n02 + 2] = 0;

    int j;
    j = 0;
    for (int i = 0; i < n + (n0 - n1); i++)
    {
        if (i % 3 != 0)
        {
            text_12[j++] = i;
        }
    }
    c_sort(text_12, SA_12, text + 2, n02, alphabet_size);
    c_sort(SA_12, text_12, text + 1, n02, alphabet_size);
    c_sort(text_12, SA_12, text, n02, alphabet_size);
    // find lexicographic names of triples
    int name = 0;
    int temp0 = -1;
    int temp1 = -1;
    int temp2 = -1;

    for (int i = 0; i < n02; i++)
    {
        if (text[SA_12[i]] != temp0 || text[SA_12[i] + 1] != temp1 || text[SA_12[i] + 2] != temp2)
        {
            name++;
            temp0 = text[SA_12[i]];
            temp1 = text[SA_12[i] + 1];
            temp2 = text[SA_12[i] + 2];
        }
        if (SA_12[i] % 3 == 1)
        {
            text_12[SA_12[i] / 3] = name;
        } // left half
        else
        {
            text_12[SA_12[i] / 3 + n0] = name;
        } // right half
    }
    // recurse if names are not yet unique
    if (name < n02)
    {
        // cout << "Inside IF\n";
        suffixArray(text_12, SA_12, n02, name);
        // store unique names in s12 using the suffix array
        for (int i = 0; i < n02; i++)
            text_12[SA_12[i]] = i + 1;
    }
    else // generate the suffix array of s12 directly
    {
        for (int i = 0; i < n02; i++)
            SA_12[text_12[i] - 1] = i;
    }

    j = 0;
    for (int i = 0; i < n02; i++)
    {
        if (SA_12[i] < n0)
        {
            text_0[j++] = 3 * SA_12[i];
        }
    }

    c_sort(text_0, SA_0, text, n0, alphabet_size);
    // merge sorted SA0 suffixes and sorted SA12 suffixes
    int t = n0 - n1;
    int p = 0;
    for (int k = 0; k < n; k++)
    {

        int i;
        if (SA_12[t] < n0)
        {
            i = SA_12[t] * 3 + 1;
        }
        else
        {
            i = (SA_12[t] - n0) * 3 + 2;
        }
        int j = SA_0[p];
        if (SA_12[t] < n0 ? leq(text[i], text_12[SA_12[t] + n0], text[j], text_12[j / 3]) : leq(text[i], text[i + 1], text_12[SA_12[t] - n0 + 1], text[j], text[j + 1], text_12[j / 3 + n0]))
        {
            SA[k] = i;
            t++;
            if (t == n02)
            {
                for (k++; p < n0; p++, k++)
                {
                    SA[k] = SA_0[p];
                }
            }
        }
        else
        {
            SA[k] = j;
            p++;
            if (p == n0)
            { 
                for (k++; t < n02; t++, k++)
                {
                    if (SA_12[t] < n0)
                    {
                        SA[k] = SA_12[t] * 3 + 1;
                    }
                    else
                    {
                        SA[k] = (SA_12[t] - n0) * 3 + 2;
                    }
                }
            }
        }
    }
    delete[] text_0;
    delete[] SA_0;
    delete[] text_12;
    delete[] SA_12;
}

void make_lcp(int *sa, int *lcp, const vector<int> &text, int n)
{
    int *rank = new int[n];
    for (int i = 0; i < n; i++)
    {
        rank[sa[i]] = i;
    }

    int k = 0;
    int j;
    //k-- to remove last character
    for (int i = 0; i < n; i++, k ? k-- : 0)
    {
        if (rank[i] == n - 1)
        {
            k = 0;
            continue;
        }

        j = sa[rank[i] + 1];

        while (i + k < n && j + k < n && text[i + k] == text[j + k])
        {
            k++;
        }
        lcp[rank[i]] = k;
    }
    delete[] rank;
}

void make_lcplr(int *lcp, int *lcplr, int i, int low, int high)
{

    if (low + 1 == high)
    {
        lcplr[i] = lcp[low];
        return;
    }

    int mid = low + (high - low) / 2;
    make_lcplr(lcp, lcplr, 2 * i + 1, low, mid);
    make_lcplr(lcp, lcplr, 2 * i + 2, mid, high);
    lcplr[i] = min(lcplr[2 * i + 1], lcplr[2 * i + 2]);
}

int find_rightposition(string ipstring, string query, int *SA, int *lcplr)
{

    int n = ipstring.size();
    int text_length = n;
    int sa_length = n;

    int l = 0;
    while (ipstring[SA[0] + l] == query[l])
    {
        l++;
        if (l == query.length() || SA[0] + l == sa_length)
            break;
    }

    int r = 0;
    while (ipstring[SA[text_length - 1] + r] == query[r])
    {
        r++;
        if (r == query.length() || SA[text_length - 1] + r == sa_length)
            break;
    }

    if (r == query.length())
    {
        return text_length - 1;
    }

    if (l < query.length() && ipstring[SA[0] + l] > query[l])
    {
        return -1;
    }
    if (r < query.length() && ipstring[SA[text_length - 1] + r] < query[r])
    {
        return -1;
    }

    int L = 0;
    int R = text_length - 1;
    int M = 0;
    int m = 0;

    int index = 0;
    while (R - L > 1)
    {
        M = (R + L) / 2;
        if (l >= r)
        {
            if (lcplr[2 * index + 1] >= l)
            {
                int lcp = 0;
                if (!(l + lcp == query.length() || SA[M] + l + lcp == sa_length))
                {
                    while (ipstring[SA[M] + l + lcp] == query[l + lcp])
                    {
                        lcp++;

                        if (l + lcp == query.length() || SA[M] + l + lcp == sa_length)
                        {
                            break;
                        }
                    }
                }
                m = l + lcp;
            }
            else
            {
                m = lcplr[2 * index + 1];
            }
        }

        else
        {
            if (lcplr[2 * index + 2] >= r)
            {
                int lcp = 0;
                if (!(r + lcp == query.length() || SA[M] + r + lcp == sa_length))
                {
                    while (ipstring[SA[M] + r + lcp] == query[r + lcp])
                    {
                        lcp++;
                        if (r + lcp == query.length() || SA[M] + r + lcp == sa_length)
                        {
                            break;
                        }
                    }
                }
                m = r + lcp;
            }
            else
            {
                m = lcplr[2 * index + 2];
            }
        }
        if (m < query.length() && query[m] < ipstring[SA[M] + m])
        {
            R = M;
            r = m;
            index = index * 2 + 1;
        }
        else
        {
            L = M;
            l = m;
            index = index * 2 + 2;
        }
    }
    // cout << "\nRight is \n";
    // cout << L << " " << R << "\n";
    return L;
}

int find_leftposition(string ipstring, string query, int *SA, int *lcplr)
{

    int n = ipstring.size();
    int text_length = n;
    int sa_length = n;

    int l = 0;
    while (ipstring[SA[0] + l] == query[l])
    {
        l++;
        if (l == query.length() || SA[0] + l == sa_length)
            break;
    }

    int r = 0;
    while (ipstring[SA[text_length - 1] + r] == query[r])
    {
        r++;
        if (r == query.length() || SA[text_length - 1] + r == sa_length)
            break;
    }

    if (l == query.length())
    {
        return 0;
    }

    if (l < query.length() && ipstring[SA[0] + l] > query[l])
    {
        return -1;
    }
    if (r < query.length() && ipstring[SA[text_length - 1] + r] < query[r])
    {
        return -1;
    }

    int L = 0;
    int R = text_length - 1;
    int M = 0;
    int m = 0;

    int index = 0;
    while (R - L > 1)
    {
        M = (R + L) / 2;
        if (l >= r)
        {
            if (lcplr[2 * index + 1] >= l)
            {
                int lcp = 0;
                if (!(l + lcp == query.length() || SA[M] + l + lcp == sa_length))
                {
                    while (ipstring[SA[M] + l + lcp] == query[l + lcp])
                    {
                        // cout<<"Values are "<<SA[M]+l+lcp<<" "<<l+lcp<<"\n";
                        lcp++;

                        if (l + lcp == query.length() || SA[M] + l + lcp == sa_length)
                        {
                            break;
                        }
                    }
                }
                // cout<<"Inside this\n";
                // cout<<"Value of l and lcp are "<<l<<" "<<lcp<<"\n";
                m = l + lcp;
            }
            else
            {
                m = lcplr[2 * index + 1];
            }
        }

        else
        {
            if (lcplr[2 * index + 2] >= r)
            {
                int lcp = 0;
                if (!(r + lcp == query.length() || SA[M] + r + lcp == sa_length))
                {
                    while (ipstring[SA[M] + r + lcp] == query[r + lcp])
                    {
                        lcp++;
                        if (r + lcp == query.length() || SA[M] + r + lcp == sa_length)
                        {
                            break;
                        }
                    }
                }
                m = r + lcp;
            }
            else
            {
                m = lcplr[2 * index + 2];
            }
        }
        if (m == query.length() || query[m] <= ipstring[SA[M] + m])
        {
            R = M;
            r = m;
            index = 2 * index + 1;
        }
        else
        {
            L = M;
            l = m;
            index = 2 * index + 2;
        }
    }
    // cout << "Left is \n";
    // cout << L << " " << R << "\n";
    return R;
}

unsigned long long rdtsc()
{
    unsigned long long int x;
    unsigned a, d;

    __asm__ volatile("rdtsc"
                     : "=a"(a), "=d"(d));

    return ((unsigned long long)a) | (((unsigned long long)d) << 32);
}
