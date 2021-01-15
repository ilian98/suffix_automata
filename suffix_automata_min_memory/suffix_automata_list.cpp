#include<iostream>
#include<fstream>
#pragma GCC optimize ("O3")

/// The arrays are not in struct in order for some of them to be deleted after use in suffix automata and reused
int *len,*link,*first_ind,*last_trans;
int *prev_trans,*trans;
char *letter_trans;
int last,num,num_trans;
inline int find_trans (int state, int letter) {
    for (int curr=last_trans[state]; curr!=0; curr=prev_trans[curr]) {
        if (letter_trans[curr]==letter) return curr;
    }
    return 0;
}
inline void add_transition (int state, int letter, int next_state) {
    prev_trans[num_trans]=last_trans[state];
    trans[num_trans]=next_state;
    letter_trans[num_trans]=letter;
    last_trans[state]=num_trans++;
}
inline void copy_transitions (int new_state, int state) {
    int curr=last_trans[state];
    while (curr!=0) {
        add_transition(new_state,letter_trans[curr],trans[curr]);
        curr=prev_trans[curr];
    }
}
inline void add (char letter, int ind) {
    int cur=ind;
    len[cur]=len[last]+1;
    first_ind[cur]=ind;
    last_trans[cur]=0;
    int curr_trans,p;
    p=last;
    while (p!=-1) {
        curr_trans=find_trans(p,letter);
        if (curr_trans!=0) break;
        add_transition(p,letter,cur);
        p=link[p];
    }
    last=cur;
    if (p==-1) {
       link[cur]=0;
       return ;
    }
    int q=trans[curr_trans];
    if (len[p]+1==len[q]) {
       link[cur]=q;
       return ;
    }

    int clone=num++;
    len[clone]=len[p]+1;
    last_trans[clone]=0;
    copy_transitions(clone,q);
    link[clone]=link[q];
    first_ind[clone]=first_ind[q];
    while (p!=-1) {
        curr_trans=find_trans(p,letter);
        if (trans[curr_trans]!=q) break;
        trans[curr_trans]=clone;
        p=link[p];
    }
    link[q]=link[cur]=clone;
}

/// Here is reusal of old arrays
#define LAST_EDGE last_trans
#define PREV_EDGE prev_trans
///int *prev_edge,*last_edge;
int cnt_edges;
void add_edge (int from, int to) { /// to is always equal to cnt_edges
    PREV_EDGE[cnt_edges]=LAST_EDGE[from];
    LAST_EDGE[from]=cnt_edges++;
}

/// Here is reusal of old arrays
#define IN link
#define OUT trans
///int *in,*out;
int t;
void dfs (int vr) {
    IN[vr]=t++;
    for (int to=LAST_EDGE[vr]; to!=0; to=PREV_EDGE[to]) {
        dfs(to);
    }
    OUT[vr]=t++;
}
int cnt_squares;
int* len_state;
void find_squares (int vr) {
    len_state[len[vr]]=vr;
    int half=len_state[len[vr]/2];
    if ((half!=-1)&&(len[half]*2==len[vr])) {
       int ind=first_ind[vr]-len[vr]/2;
       if ((IN[half]<=IN[ind])&&(IN[ind]<=OUT[half])) cnt_squares++;
    }
    for (int to=LAST_EDGE[vr]; to!=0; to=PREV_EDGE[to]) {
        find_squares(to);
    }
    len_state[len[vr]]=-1;
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
    len = new int[2*n];
    link = new int[2*n];
    first_ind = new int[2*n];
    last_trans = new int[2*n];
    prev_trans = new int[3*n];
    trans = new int[3*n];
    letter_trans = new char[3*n];
}
int main (int argc, char** argv) {
    int n;
    if (argc==1) { /// used for stdin testing until end of input
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
    len[0]=0; link[0]=-1;
    first_ind[0]=0;
    last_trans[0]=0;
    num=n+1;
    last=0;
    num_trans=1;

    read_string(n);
    ///delete[] last_trans; - instead of deleting it we use it later
    ///delete[] prev_trans;
    ///delete[] trans;
    delete[] letter_trans;

    int i,cnt_final=0,cur=last;
    while (cur!=-1) {
        cnt_final++;
        cur=link[cur];
    }

    ///prev_edge = new int[num];
    ///last_edge = new int[num];
    for (i=0; i<num; i++){
        LAST_EDGE[i]=0;
    }
    cnt_edges=1;
    for (i=1; i<num; i++) {
        add_edge(link[i],i);
    }
    ///delete[] link;

    ///in = new int[num];
    ///out = new int[num];
    t=1;
    dfs(0);

    cnt_squares=0;
    len_state = new int[n+1];
    for (i=0; i<n; i++) {
        len_state[i]=-1;
    }
    find_squares(0);
    std::cout << num << " " << num_trans-1 << " " << cnt_final << " " << cnt_squares ;
    std::cout << std::endl ;

    delete[] len;
    delete[] first_ind;
    ///delete[] prev_edge;
    ///delete[] last_edge;

    ///delete[] in;
    ///delete[] out;
    delete[] len_state;

    delete[] last_trans;
    delete[] prev_trans;
    delete[] trans;
    delete[] link;
    return 0;
}
