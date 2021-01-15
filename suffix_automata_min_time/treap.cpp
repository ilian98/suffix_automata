/// Implementation of treap which uses indices in array instead of pointers to nodes array, initialized in init() function
#include<algorithm>
struct node {
    int state;
    int l,r;
    char x,y;
};
node* nodes;
int num_nodes;

inline void merge (int& t, int l, int r) {
    if ((l==-1)||(r==-1)) {
       if (l!=-1) t=l;
       else t=r;
       return ;
    }
    if (nodes[l].y>nodes[r].y) t=l, merge(nodes[t].r,nodes[l].r,r);
    else t=r, merge(nodes[t].l,l,nodes[r].l);
}
inline void split (int t, char x, int& l, int& r) {
    if (t==-1) {
       l=r=-1;
       return ;
    }
    if (nodes[t].x<x) l=t, split(nodes[t].r,x,nodes[l].r,r);
    else r=t, split(nodes[t].l,x,l,nodes[r].l);
}
void insert_treap (int& root, char x, int state) {
    int l,r,ex;
    split(root,x,l,r);
    split(r,x+1,ex,r);
    ex=num_nodes++;
    nodes[ex].x=x; nodes[ex].y=rand()%256;
    nodes[ex].state=state;
    nodes[ex].l=-1; nodes[ex].r=-1;
    merge(root,l,ex);
    merge(root,root,r);
}
void copy_treap (int& curr, int& other) {
    if (other==-1) return ;
    curr=num_nodes++;
    nodes[curr]=nodes[other];
    if (nodes[other].l!=-1) copy_treap(nodes[curr].l,nodes[other].l);
    if (nodes[other].r!=-1) copy_treap(nodes[curr].r,nodes[other].r);
}
inline int find_treap (int root, char x) {
    if (root==-1) return -1;
    if (nodes[root].x==x) return root;
    else if (nodes[root].x<x) return (nodes[root].r==-1)?-1:find_treap(nodes[root].r,x);
    else return (nodes[root].l==-1)?-1:find_treap(nodes[root].l,x);
}
