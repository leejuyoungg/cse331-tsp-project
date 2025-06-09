#include "ap_hk.hpp"        
#include "mst2x.hpp"       
#include <unordered_map>
#include <vector>
#include <climits>
#include <algorithm>

using Mask = std::uint64_t;
using Cost = long long;
const Cost INF = std::numeric_limits<Cost>::max();

std::vector<int> mst2x_2opt_tour(const std::vector<Point>& pts)
{
    int n = (int)pts.size(), root = 0;
    auto mst = mst_prim_linear(pts, root);     
    std::vector<int> walk;  dfs_pre(root, -1, mst, walk);

    std::vector<char> vis(n,0);
    std::vector<int> tour;
    for(int v:walk) if(!vis[v]){ tour.push_back(v); vis[v]=1; }
    tour.push_back(root);                      

    auto dist = [&](int a,int b){return (long long)::dist(pts[a],pts[b]);};
    int m = (int)tour.size()-1;               
    for(int iter=0; iter<50; ++iter){
        bool improved=false;
        for(int i=1;i<m-2;++i)
            for(int j=i+1;j<m-1;++j){
                int a=tour[i-1], b=tour[i],
                    c=tour[j],   d=tour[j+1];
                if(dist(a,b)+dist(c,d) > dist(a,c)+dist(b,d)){
                    std::reverse(tour.begin()+i, tour.begin()+j+1);
                    improved=true;
                }
            }
        if(!improved) break;
    }
    return tour;
}

Cost prim_mst_cost(const std::vector<std::vector<int>>& D,
                   const std::vector<int>& R)
{
    if(R.size()<=1) return 0;
    int n=(int)D.size();
    std::vector<int> key(n,INT_MAX); std::vector<char> in(n,0);
    int root=R[0]; key[root]=0; Cost tot=0;

    for(size_t step=0; step<R.size(); ++step){
        int u=-1,best=INT_MAX;
        for(int v:R) if(!in[v]&&key[v]<best){best=key[v];u=v;}
        in[u]=1; tot+=best;
        for(int v:R) if(!in[v]&&D[u][v]<key[v]) key[v]=D[u][v];
    }
    return tot;
}

Cost lower_bound(const std::vector<std::vector<int>>& D,
                 Mask mask,int last,const std::vector<int>& min0)
{
    int n=(int)D.size(); std::vector<int> R;
    for(int v=1;v<n;++v) if(!(mask&(Mask(1)<<v))) R.push_back(v);
    if(R.empty()) return D[last][0];

    Cost mst=prim_mst_cost(D,R);
    int inEd=INT_MAX,outEd=INT_MAX;
    for(int v:R){
        inEd = std::min(inEd,  D[last][v]);
        outEd= std::min(outEd, min0[v]);
    }
    return mst+inEd+outEd;
}

std::pair<std::vector<int>,Cost>
ap_hk_tour(const std::vector<Point>& pts)
{
    int n=(int)pts.size();
    if(n==0||n>=63) return {{},-1};

    /* 0. 거리행렬 + 초기 UB = MST-2× + 2-opt */
    auto D = build_distance_matrix(pts);
    auto ubTour = mst2x_2opt_tour(pts);
    Cost UB=0; for(size_t i=1;i<ubTour.size();++i) UB+=D[ubTour[i-1]][ubTour[i]];

    std::vector<int> min0(n); for(int v=0;v<n;++v) min0[v]=D[v][0];

    using Vec=std::vector<Cost>; using Layer=std::unordered_map<Mask,Vec>;
    Layer prev,layer;                   
    prev.reserve(1u<<(n/2));            
    prev[1]=Vec(n,INF); prev[1][0]=0;

    std::unordered_map<Mask,std::vector<int>> parent;
    parent.reserve(1u<<(n/2));

    /* 1. 레이어 확장 + LB prune */
    for(int k=2;k<=n;++k){
        layer.clear();
        for(auto &b:prev){
            Mask mask=b.first; const Vec&vec=b.second;
            for(int last=0;last<n;++last){
                Cost base=vec[last];
                if(base==INF) continue;
                for(int v=1;v<n;++v){
                    if(mask&(Mask(1)<<v)) continue;
                    if(base>INF-D[last][v]) continue;      
                    Mask m2=mask|(Mask(1)<<v);
                    Cost cand=base+D[last][v];
                    Cost lb = cand+lower_bound(D,m2,v,min0);
                    if(lb>=UB) continue;                   

                    Vec& dst=layer[m2];
                    if(dst.empty()) dst.assign(n,INF);
                    if(cand<dst[v]){
                        dst[v]=cand;
                        auto &par=parent[m2];
                        par.assign(n, -1);
                        par[v]=last;
                    }
                    if(k==n){                              
                        Cost tour=cand+D[v][0];
                        if(tour<UB) UB=tour;
                    }
                }
            }
        }
        prev.swap(layer);                                  
    }

    /* 2. 마지막 레이어에서 최적 비용·끝점 찾기 */
    Mask FULL=(Mask(1)<<n)-1;
    const Vec& lastV=prev[FULL];
    Cost best=INF; int last=-1;
    for(int v=1;v<n;++v){
        if(lastV[v]==INF) continue;
        Cost cand=lastV[v]+D[v][0];
        if(cand<best){best=cand; last=v;}
    }

    /* 3. parent 테이블로 경로 역추적 */
    std::vector<int> path; path.reserve(n+1);
    Mask sub=FULL; int cur=last;
    while(cur!=-1){
        path.push_back(cur);
        int pred = (parent.count(sub) && cur < (int)parent[sub].size())
           ? parent[sub][cur]
           : -1;
        sub&=~(Mask(1)<<cur); cur=pred;
    }
    std::reverse(path.begin(),path.end());
    path.push_back(0);
    return {path,best};
}
