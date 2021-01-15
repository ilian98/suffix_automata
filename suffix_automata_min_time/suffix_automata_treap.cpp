#include<iostream>
#include<fstream>
#include "treap.cpp"
#pragma GCC optimize ("O3")

struct state {
    int len,link;
    int first_ind;
    void set_fields (int len1, int link1, int first_ind1) {
        len=len1; link=link1;
        first_ind=first_ind1;
    }
};
state *states;
int *transitions;
int last,num;
inline void add (char letter, int ind) {
    int curr=ind;
    states[curr].set_fields(states[last].len+1,-1,ind);
    transitions[curr]=-1;
    int curr_trans,p;
    p=last;
    while (p!=-1) {
        curr_trans=find_treap(transitions[p],letter);
        if (curr_trans!=-1) break;
        insert_treap(transitions[p],letter,curr);
        p=states[p].link;
    }
    last=curr;
    if (p==-1) {
       states[curr].link=0;
       return ;
    }
    int q=nodes[curr_trans].state;
    if (states[p].len+1==states[q].len) {
       states[curr].link=q;
       return ;
    }

    int clone=num++;
    states[clone].set_fields(states[p].len+1,states[q].link,states[q].first_ind);
    transitions[clone]=-1;
    copy_treap(transitions[clone],transitions[q]);
    while (p!=-1) {
        curr_trans=find_treap(transitions[p],letter);
        if (nodes[curr_trans].state!=q) break;
        nodes[curr_trans].state=clone;
        p=states[p].link;
    }
    states[q].link=states[curr].link=clone;
}

int *prev_edge,cnt_edges;
/// Reusal of old array transitions
#define LAST_EDGE transitions
//int *last_edge;
void add_edge (int from, int to) { /// to is always equal to cnt_edges
    prev_edge[cnt_edges]=LAST_EDGE[from];
    LAST_EDGE[from]=cnt_edges++;
}

int *in,*out;
int t;
void dfs (int vr) {
    in[vr]=t++;
    for (int to=LAST_EDGE[vr]; to!=0; to=prev_edge[to]) {
        dfs(to);
    }
    out[vr]=t++;
}
int cnt_squares;
int* len_state;
void find_squares (int vr) {
    len_state[states[vr].len]=vr;
    int half=len_state[states[vr].len/2];
    if ((half!=-1)&&(states[half].len*2==states[vr].len)) {
       int ind=states[vr].first_ind-states[vr].len/2;
       if ((in[half]<=in[ind])&&(in[ind]<=out[half])) cnt_squares++;
    }
    for (int to=LAST_EDGE[vr]; to!=0; to=prev_edge[to]) {
        find_squares(to);
    }
    len_state[states[vr].len]=-1;
}

#define BUFFER_LENGTH (1<<20)
char buffer[BUFFER_LENGTH];
int buffer_pos=BUFFER_LENGTH-1;
FILE* read;
inline void read_char() {
    if (++buffer_pos==BUFFER_LENGTH) fread(buffer,1,BUFFER_LENGTH,read), buffer_pos=0;
}
void read_string(int n) {
    int string_pos=1;
    for (; buffer[buffer_pos]==' ' || buffer[buffer_pos]=='\n' || buffer[buffer_pos]=='\t' || buffer[buffer_pos]=='\0'; read_char());
    for (; buffer[buffer_pos]!=' ' && buffer[buffer_pos]!='\n' && buffer[buffer_pos]!='\t' && string_pos<=n; read_char()) {
        add(buffer[buffer_pos],string_pos++);
    }
}

void init (int n) {
    states = new state[2*n];
    nodes = new node[3*n]; num_nodes=0;
    transitions = new int[2*n];
}
int main (int argc, char** argv) {
    int n;
    if (argc==1) {
       n=7;
       read=stdin;
    }
    else {
        std::ifstream fin(argv[1],std::ios::binary);
        fin.seekg(-1,std::ios::end);
        char c;
        fin.read(&c,sizeof(char));
        n=fin.tellg();
        if (c=='\n') n--;
        fin.close();
        read=fopen(argv[1],"r");
    }
    init(n);
    states[0].set_fields(0,-1,0);
    transitions[0]=-1;
    num=n+1;
    last=0;

    read_string(n);
    delete[] nodes;
    ///delete[] transitions; - instead of deleting it we use it later

    int i,cnt_final=0,curr=last;
    while (curr!=-1) {
        cnt_final++;
        curr=states[curr].link;
    }

    prev_edge = new int[num];
    ///last_edge = new int[num];
    for (i=0; i<num; i++){
        LAST_EDGE[i]=0;
    }
    cnt_edges=1;
    for (i=1; i<num; i++) {
        add_edge(states[i].link,i);
    }

    in = new int[num];
    out = new int[num];
    t=1;
    dfs(0);

    len_state = new int[n+1];
    for (i=0; i<n; i++) {
        len_state[i]=-1;
    }
    cnt_squares=0;
    find_squares(0);
    std::cout << num << " " << num_nodes << " " << cnt_final << " " << cnt_squares ;
    std::cout << std::endl ;

    delete[] states;
    delete[] prev_edge;
    ///delete[] last_edge;

    delete[] in;
    delete[] out;
    delete[] len_state;

    delete[] transitions;
    return 0;
}
