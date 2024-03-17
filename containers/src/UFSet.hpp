#pragma once

#include <iostream>
#include <iomanip>
#include <utility>
#include <vector>

template <typename T> class UFSet;
template <typename T> std::ostream& operator<<(std::ostream&, const UFSet<T>&);

template <typename T>
class UFSet{
protected:
    std::vector<std::pair<int, T>> set;
public:
    UFSet()=default;
    UFSet(T[], int);
    virtual ~UFSet()=default;
    int getNum(const T&) const;
    int find(const T&) const;
    void unite(const T& a, const T& b);
friend std::ostream& operator<< <>(std::ostream&, const UFSet&);
};

template <typename T>
UFSet<T>::UFSet(T element[], int n)
{
    set.resize(n);
    for(int i=0; i<n; i++){
        set[i].first=-1;
        set[i].second=element[i];
    }
}

template <typename T>
auto UFSet<T>::getNum(const T& value) const -> int
{
    int p=0;
    while(p<set.size() && set[p].second!=value)
        p++;
    if(p==set.size())
        return -1;
    return p;
}

template <typename T>
auto UFSet<T>::find(const T& value) const -> int
{
  	int p = 0;
    int size=set.size();
  	while (p<size && set[p].second!=value)
   		p++;
	if (p==size)
		return -1;
	while(set[p].first>-1)
		p=set[p].first;
	return p;
}

template <typename T>
auto UFSet<T>::unite(const T& a, const T& b) -> void
{
	int r1 = find(a);
	int r2 = find(b);
	if (r1!=r2 && r1!=-1) {
        if(set[r1].first==set[r2].first){
            set[r1].first--;
            set[r2].first=r1;
        }else{
            if(r1>r2)
                std::swap(r1, r2);
            set[r1].first=std::min(set[r1].first, set[r2].first-1);
            set[r2].first=r1;
        }
	}
}

template <typename T>
auto operator<<(std::ostream& out, const UFSet<T>& rhs) -> std::ostream&
{
    for(int i=0; i<rhs.set.size(); i++)
        out<<std::setw(3)<<i;
    out<<'\n';
    for(int i=0; i<rhs.set.size(); i++)
        out<<std::setw(3)<<rhs.set[i].first;
    out<<'\n';
    for(int i=0; i<rhs.set.size(); i++)
        out<<std::setw(3)<<rhs.set[i].second;
    out<<'\n';
    return out;
}

#endif