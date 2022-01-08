#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <deque>
#include <set>
#include <climits>
#include <unistd.h>

unsigned int microseconds=100000;

#define ll int
#define mp make_pair
#define pb push_back
#define outarr(arr) for(ll i=0;i<arr.size();i++) {cout<<arr[i];} cout<<endl;
using namespace std;

class tsp
{
	public:
		string s;	
		ll n;
		bool euclidean;	
		vector<pair<double,double>>coords;
		vector<vector<double>>distance;
		vector<vector<pair<ll,double>>>costs;
		vector<pair<double,pair<ll,ll>>>edges;
}data;

deque<ll>tour;
vector<ll>best_tour;
vector<ll>nn_tour;
vector<ll>nnbe_tour;
vector<ll>greedy_tour;
vector<ll>savings_tour;

double best_cost=INT_MAX;

bool cmp(pair<ll,double> a,pair<ll,double> b)
{
	return a.second<b.second;
}
bool cmp2(pair<double,pair<ll,ll>>a,pair<double,pair<ll,ll>> b)
{
	return a.first<b.first;
}
bool cmp3(pair<double,pair<ll,ll>>a,pair<double,pair<ll,ll>> b)
{
	return a.first>b.first;
}
int find(vector<int>&parent, int i)
{
    if (parent[i] != i)
    parent[i] = find(parent, parent[i]);	 
    return parent[i];
}
void Union(vector<int>&parent,vector<int>&rank, int xroot, int yroot)
{	 
    if (rank[xroot] < rank[yroot])
        parent[xroot] = yroot;
    else if (rank[xroot] > rank[yroot])
 		parent[yroot] = xroot;
    else 
    {
        parent[yroot] = xroot;
        rank[xroot]++;
    }
}
//Check if the given tour is valid
bool checkvalidtour(vector<ll>&tour)
{
	
	bool present[data.n]={0};
	bool check=1;
	for(ll i=0;i<tour.size();i++)
	{
		present[tour[i]]=1;
	}
	for(ll i=0;i<data.n;i++)
	{
		if(!present[i])
		check=0;
	}
	if(data.n == tour.size() && check==1)
	return true;
	return false;
}

//Print the tour
void printtour(vector<ll>&tour)
{
	//cout<<"Tour: ";
	for(ll i=0;i<tour.size();i++)
	{
		cout<<tour[i]<<" ";
	}	
	cout<<endl;
}

//Find the cost of a tour
double findcost(vector<ll>&tour)
{
	double cost = 0.0;
    for(ll i=0; i<data.n-1; i++)
    cost += data.distance[tour[i]][tour[i+1]];
    
	cost += data.distance[tour[data.n-1]][tour[0]];
    return cost;
}

double findcost(deque<ll>&tour)
{
	double cost = 0.0;
    for(ll i=0; i<data.n-1; i++)
    cost += data.distance[tour[i]][tour[i+1]];
    
	cost += data.distance[tour[data.n-1]][tour[0]];
    return cost;
}

//Nearest Neighbour Heuristic
void genbestnearestneighbour()
{
	//cout<<"Nearest Neighbour"<<endl;
	ll i,j,cost,k;
	ll n=data.n;
	ll best_nn_cost=INT_MAX;
	for(i=0;i<n;i++)
	{
		bool visited[n]={0};
		cost=0;
		tour[0]=i;
		visited[i]=1;
		for(j=1;j<n;j++)
		{
			ll curr=tour[j-1];
			vector<pair<ll,double>>neighbours=data.costs[curr];
			sort(neighbours.begin(),neighbours.end(),cmp);
			/*for(k=0;k<n;k++)
			{
				cout<<neighbours[k].first<<" "<<neighbours[k].second<<endl;
			}*/			
			for(k=0;k<n;k++)
			{
				ll vert=neighbours[k].first;
				if(!visited[vert])
				{
					tour[j]=vert;
					visited[vert]=1;
					break;
				}
			}
		}
		/*cout<<"Starting Vertex: "<< i<<" Cost: "<<cost<<endl;
		cout<<"Tour: ";
		for(j=0;j<n;j++)
		cout<<tour[j]<<" ";
		cout<<endl;*/
		//copy(tour.begin(),tour.end(),nn_tour.begin());
		//cout<<"Copied"<<endl;
		cost=findcost(tour);
		if(cost<best_nn_cost)
		{
			if(checkvalidtour(nn_tour))
			{
				best_nn_cost=cost;
				copy(tour.begin(),tour.end(),nn_tour.begin());
			}
			else
			{
				//cout<<"Tour not valid in nearest neighbour with both ends"<<endl;
				printtour(nn_tour);
			}	

		}
	}
	if(best_nn_cost < best_cost)
	{
		best_cost=best_nn_cost;
		copy(nn_tour.begin(),nn_tour.end(),best_tour.begin());
	}	

}
//Nearest Neighbour Heuristic where both ends can be extended
void genbestnearestneighbourbothends()
{
	//cout<<"Nearest Neighbour at both ends"<<endl;
	ll i,j,cost,k1,k2,k;
	ll n=data.n;
	ll best_nnbe_cost=INT_MAX;
	for(i=0;i<n;i++)
	{
		bool visited[n]={0};
		cost=0;
		tour.push_back(i);
		visited[i]=1;
		for(j=1;j<n;j++)
		{
			ll curr1=tour[j-1],curr2=tour[0];
			vector<pair<ll,double>>neighbours1=data.costs[curr1];
			sort(neighbours1.begin(),neighbours1.end(),cmp);
			vector<pair<ll,double>>neighbours2=data.costs[curr2];
			sort(neighbours2.begin(),neighbours2.end(),cmp);
			/*cout<<"Neighbours of last vertex \t";
			for(k=0;k<n;k++)
			{
				cout<<neighbours1[k].first<<" "<<neighbours1[k].second<<"\t";
			}
			cout<<endl;
			cout<<"Neighbours of first vertex \t";
			for(k=0;k<n;k++)
			{
				cout<<neighbours2[k].first<<" "<<neighbours2[k].second<<"\t";
			}
			cout<<endl;*/
			k1=0;k2=0;
			while(k1<n && k2<n)	
			{
				ll vert1=neighbours1[k1].first;
				ll vert2=neighbours2[k2].first;
				if(visited[vert1])
				{
					k1++;
				}
				else if(visited[vert2])
				{
					k2++;
				}
				else if(data.distance[curr1][vert1] <= data.distance[vert2][curr2])
				{
					tour.push_back(vert1);
					visited[vert1]=1;
					cost+=data.distance[curr1][vert1];
					break;
				}
				else
				{
					tour.push_front(vert2);
					visited[vert2]=1;
					cost+=data.distance[vert2][curr2];
					break;
				}				
			}					
		}
		cost=findcost(tour);
		if(cost<best_nnbe_cost)
		{
			best_nnbe_cost=cost;
			copy(tour.begin(),tour.end(),nnbe_tour.begin());
		}	
		/*cout<<"Starting Vertex: "<< i<<" Cost: "<<cost<<endl;
		cout<<"Tour: ";
		for(j=0;j<n;j++)
		cout<<tour[j]<<" ";
		cout<<endl;*/		
		tour.clear();
	}
	if(best_nnbe_cost < best_cost)
	{
		best_cost=best_nnbe_cost;
		copy(nnbe_tour.begin(),nnbe_tour.end(),best_tour.begin());
	}
}

// Greedy Heuristic
void greedytour()
{
	//cout<<"Greedy Tour"<<endl;
	ll n=data.n,i,j,k;
	bool outgoing[n]={0},incoming[n]={0};
	vector<int>parent(n,-1);
	vector<int>rank(n,0);
	sort(data.edges.begin(),data.edges.end(),cmp2);
	/*for(i=0;i<data.edges.size();i++)
	cout<<data.edges[i].second.first<<" "<<data.edges[i].second.first<<endl;*/
	for(int i = 0 ;i < n;i++)
			parent[i] = i;
	ll tmp_cost=0;
	vector<pair<double,pair<ll,ll>>>selected_edg;
	ll tot=0;
	i=0;
	//set<pair<ll,ll>>adj_vert;
	while(tot<(n-1))
	{		
		ll dum1=data.edges[i].second.first,dum2=data.edges[i].second.second;		
		while(outgoing[dum1]==1 || incoming[dum2]==1 || (find(parent,dum1) == find(parent,dum2)))
		{
			//cout<<"i: "<<i<<endl;
			//cout<<"tot: "<<tot<<"dum1: "<<dum1<<"dum2: "<<dum2<<endl;			
			i++;
			dum1=data.edges[i].second.first;
			dum2=data.edges[i].second.second;
			//usleep(microseconds);
			ll duq1;
			cin>>duq1;
		}
		if(i>=data.edges.size())
		{
			//cout<<"Warning"<<endl;
			break;
		}
		//cout<<"Chosen dum1: "<<dum1<<"Chosen dum2: "<<dum2<<endl;
		selected_edg.pb(data.edges[i]);
		outgoing[dum1]=1;
		incoming[dum2]=1;
		Union(parent,rank,find(parent,dum1),find(parent,dum2));
		//adj_vert.insert(mp(dum1,dum2));
		tmp_cost+=data.edges[i].first;
		i++;
		tot++;
	}

	/*cout<<selected_edg.size()<<" Edge Selection Complete"<<endl;
	for(i=0;i<selected_edg.size();i++)
	{
		cout<<selected_edg[i].second.first<<" "<<selected_edg[i].second.second<<endl;
	}*/
	vector<int>adj(data.n,0);
	//cout<<"Checkpoint 3"<<endl;
	for(i=0;i<selected_edg.size();i++)
	{
		adj[selected_edg[i].second.first]=selected_edg[i].second.second;
	}
	ll dum=selected_edg.size();
	adj[selected_edg[dum-1].second.second]=selected_edg[0].second.first;
	/*for(i=0;i<adj.size();i++)
	{
		cout<<"i: "<<i<<" adj[i]: "<<adj[i]<<endl;
	}*/
	//cout<<"Checkpoint 2"<<endl;
	j=0;
	tour.clear();
	for(i=0;i<n;i++)
	{
		tour.pb(j);
		j=adj[j];
	}
	//cout<<"Checkpoint 1"<<endl;
	tmp_cost=findcost(tour);
	copy(tour.begin(),tour.end(),greedy_tour.begin());
	if(tmp_cost<best_cost)
	{
		if(checkvalidtour(greedy_tour))
			{
				best_cost=tmp_cost;
				copy(tour.begin(),tour.end(),best_tour.begin());
			}
			else
			{
				//cout<<"Tour not valid in greedy heuristic"<<endl;
				printtour(greedy_tour);
			}		
	}	
}

//Savings Heuristic
void savingtour()
{
	//cout<<"Saving Heuristic"<<endl;
	ll n=data.n,i,j,k,l;
	ll tmp_cost=0;
	set<pair<ll,ll>>selected_edg;
	set<pair<ll,ll>>final_edg; // Stores the current edges in the tour	
	ll tot=0;
	i=0;
	for(i=0;i<n;i++)
	{
		bool outgoing[n]={0},incoming[n]={0};	
		for(j=0;j<n;j++)
		{
			if(j!=i)
			{
				selected_edg.insert(mp(j,i));
				selected_edg.insert(mp(i,j));
			}
		}
		//cout<<"Selected edge size: "<<selected_edg.size()<<" Final edge size: "<<final_edg.size()<<endl;
		vector<pair<double,pair<ll,ll>>>saving;
		ll dum=0,tmp1;
		ll picked[n]={0};
		for(k=0;k<n;k++)
		{
			for(l=0;l<n;l++)
			{
				if(k!=l && l!=i  && k!=i)
				{
					tmp1=data.distance[i][k]+data.distance[l][i]-data.distance[k][l];
					saving.pb(mp(tmp1,mp(k,l)));
				}
			}
		}
		sort(saving.begin(),saving.end(),cmp3);
		//cout<<saving.size()<<endl;
		/*for(ll p=0;p<saving.size();p++)
		{
			cout<<saving[p].first<<" "<<saving[p].second.first<<" "<<saving[p].second.second<<endl;
		}*/
		ll ctr=0;
		for(j=1;j<n-1 && ctr<saving.size();j++)  //n-2 merge operations
		{
			//cout<<ctr<<" "<<" j: "<<j<<" ";
			k=saving[ctr].second.first;
			l=saving[ctr].second.second;
			//cout<<"k: "<<k<<" l: "<<l<<endl;
			if( (!picked[k] || !picked[l])) /*final_edg.find(mp(k,l))==final_edg.end() && selected_edg.find(mp(i,k))!=selected_edg.end() && selected_edg.find(mp(l,i))!=selected_edg.end() && !outgoing[k] && !incoming[l] &&*/
			{
				selected_edg.erase(mp(i,k));
				selected_edg.erase(mp(l,i));
				final_edg.insert(mp(k,l));
				outgoing[k]=1;
				incoming[l]=1;
				picked[l]=1;
				picked[k]=1;
			}
			else
			j--;

			ctr++;
		}
		if(j!=n-1)
		;
		//cout<<"Not completed"<<endl;
			/* 
			ll sav=-1;ll best1=-1,best2=-1,tmp1,tmp2;
			if()
						{

							tmp1=data.distance[i][k]+data.distance[l][i]-data.distance[k][l];
							if(tmp1>sav)
							{
								sav=tmp1;
								best1=k;
								best2=l;
							}
						}
						if(final_edg.find(mp(l,k))==final_edg.end() && selected_edg.find(mp(k,i))!=selected_edg.end() && selected_edg.find(mp(i,l))!=selected_edg.end() && !outgoing[l] && !incoming[k] && (!picked[k] || !picked[l]))
						{
							tmp2=data.distance[i][l]+data.distance[k][i]-data.distance[l][k];
							if(tmp2>sav)
							{
								sav=tmp2;
								best1=l;
								best2=k;
							}
						}
					}
				}
			}*/		
						


			/*if(best1!=-1 && best2!=-1)
			{
				//cout<<best1<<" "<<best2<<endl;
				
			}
			else
			{
				dum++;
			}	*/		
			//cout<<best1<<" "<<best2<<" deleted "<<endl;
		/*if(dum!=0)
		cout<<"Warning in Savings Heuristic"<<endl;*/
		//cout<<"Dum: "<<dum<<endl;
		//cout<<"Selected edge size: "<<selected_edg.size()<<" Final edge size: "<<final_edg.size()<<endl;
		ll cnt=0;
		for(auto it=selected_edg.begin();it!=selected_edg.end();it++)
		{
			cnt++;			
			ll dum1=(*it).first;
			ll dum2=(*it).second;
			if(!outgoing[dum1] && !incoming[dum2])
			{
				final_edg.insert(mp(dum1,dum2));
			}
			else if(!outgoing[dum2] && !incoming[dum1])
			{
				final_edg.insert(mp(dum2,dum1));
			}
		}
		//cout<<"Number of edges left = "<<cnt<<endl;
		/*cout<<"Final edges for i= "<<i<<endl;
		for(auto it=final_edg.begin();it!=final_edg.end();it++)
		{
			cout<<(*it).first<<" "<<(*it).second<<endl;
		}*/
		cnt=0;
		tmp_cost=0;vector<ll>adj(n,0);
		for(auto it=final_edg.begin();it!=final_edg.end();it++)
		{	
			cnt++;
			ll dum1=(*it).first;
			ll dum2=(*it).second;
			tmp_cost+=data.distance[dum1][dum2];
			//cout<<dum1<<" "<<dum2<<endl;
			adj[dum1]=dum2;
		}	
		//cout<<"Best savings cost: "<<tmp_cost<<" with the following number of edges: "<<cnt<<endl;
		tmp1=0;ll tmp2;
		tour.clear();
		for(tmp2=0;tmp2<n;tmp2++)
		{
			tour.pb(tmp1);
			tmp1=adj[tmp1];
		}
		copy(tour.begin(),tour.end(),savings_tour.begin());
		tmp_cost=findcost(tour);
		if(tmp_cost<best_cost)
		{
			if(checkvalidtour(savings_tour))
			{
				best_cost=tmp_cost;
				copy(tour.begin(),tour.end(),best_tour.begin());
			}
			else
			{
				//cout<<"Tour not valid in savings heuristic"<<endl;
				printtour(savings_tour);
			}		
		}	
		selected_edg.clear();
		final_edg.clear();
		saving.clear();
	}
}
void printbesttour()
{
	cout<<"Current best cost: "<<findcost(best_tour)<<endl;
	for(ll i=0;i<best_tour.size();i++)
	{
		cout<<best_tour[i]<<" ";
	}	
	cout<<endl;
}

void genetictour()
{
	if(checkvalidtour(nn_tour))
	printtour(nn_tour);
	if(checkvalidtour(nnbe_tour))
	printtour(nnbe_tour);
	if(checkvalidtour(greedy_tour))
	printtour(greedy_tour);
	
}
void generatetour()
{
	//Generate dummy tours
	for(ll i=0;i<data.n;i++)
	tour.push_back(i);
	for(ll i=0;i<data.n;i++)
	best_tour.push_back(i);
	for(ll i=0;i<data.n;i++)
	nn_tour.push_back(i);
	for(ll i=0;i<data.n;i++)
	nnbe_tour.push_back(i);
	for(ll i=0;i<data.n;i++)
	greedy_tour.push_back(i);
	for(ll i=0;i<data.n;i++)
	savings_tour.push_back(i);

	//Generate the best nearest neighbour tour	
	genbestnearestneighbour();

	// Print the current optimal tour
	printbesttour();

	//Variation of best nearest neighour tour
	genbestnearestneighbourbothends();

	// Print the current optimal tour
	printbesttour();

	//Generate tour based on greedy heuristic
	greedytour();

	// Print the current optimal tour
	printbesttour();

	/*if(data.n<=500)
	{
		//Generate tour based on savings heuristic
		savingtour();

		// Print the current optimal tour
		printbesttour();
	}*/
	

	genetictour();

}

int main()
{
	getline(cin,data.s);
	if(data.s=="euclidean")
	{
		data.euclidean=true;
	}
	else if(data.s=="non euclidean")
	{
		data.euclidean=false;	
	}
	cin>>data.n;
	ll i,j;double a,b;
	for(i=0;i<data.n;i++)
	{
		cin>>a>>b;
		data.coords.push_back(mp(a,b));
	}
	for(i=0;i<data.n;i++)
	{
		vector<double>dum;
		vector<pair<ll,double>>dum2;
		for(j=0;j<data.n;j++)
		{
			cin>>a;
			dum.push_back(a);
			dum2.push_back(mp(j,a));
			if(i!=j)
			data.edges.pb(mp(a,mp(i,j)));
		}
		data.distance.push_back(dum);
		data.costs.pb(dum2);
	}
	/*cout<<"Edges: \t";
	for(i=0;i<data.edges.size();i++)
	{
		cout<<data.edges[i].first<<" "<<data.edges[i].second.first<<" "<<data.edges[i].second.second<<"\t";
	}
	cout<<endl;*/
	/*cout<<"End of data input"<<endl;
	cout<<"Euclidean: "<<data.euclidean<<endl;
	cout<<"Number of cities: "<<data.n<<endl;
	for(i=0;i<data.n;i++)
	{
		cout<<data.coords[i].first<<" "<<data.coords[i].second<<endl;
	}
	for(i=0;i<data.n;i++)
	{
		for(j=0;j<data.n;j++)
		{
			cout<<data.distance[i][j]<<" ";
		}
		cout<<endl;
	}*/
	generatetour();
	return 0;
}
