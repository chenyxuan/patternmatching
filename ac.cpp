#include <bits/stdc++.h>
const int pattern_maxlen = 1000;

class Node {
private:
    std::unordered_map<char, Node*> next;
    std::vector<int> output;
    Node *fail;

public:
    Node *getNextOrCreateNext(char ch) {
        if(next.count(ch)) {
            return next[ch];
        } else {
            next[ch] = new Node;
            return next[ch];
        }
    }
    Node *getNext(char ch) {
        if(next.count(ch)) {
            return next[ch];
        } else {
            return NULL;
        }
    }
    void addOutput(int idx) {
        output.push_back(idx);
    }
    const std::vector<int> &getOutput() {
        return output;
    }
    void setFail(Node *f) {
        fail = f;
    }
    Node *getFail() {
        return fail;
    }
    const std::unordered_map<char, Node*> &getNextMap() {
        return next;
    }
    void initAcRoot() {
        for(int i = 0; i < 256; i++) {
            if(!next.count(i)) {
                next[i] = this;
            }
        }
        fail = this;
    }
};

void init(const char* pfn, const char* tfn, 
std::vector<std::string> &ps, std::vector<std::string> &ts,
int &pL, int &tL) {
    static char pstr[pattern_maxlen];
    printf("Filename: %s %s\n", pfn, tfn);
    std::string tmp;
    std::fstream pfs (pfn, std::fstream::in);
    while(pfs.getline(pstr, pattern_maxlen)) {
        ps.push_back(std::string(pstr));
        if(ps.back().back() == 13) {
            ps.back().pop_back();
        }
        pL = pfs.tellg();
    }
    pfs.close();
    std::fstream tfs (tfn, std::fstream::in);
    while(tfs >> tmp) {
        ts.push_back(tmp);
        tL = tfs.tellg();
    }
    tfs.close();
}

Node *build(const std::vector<std::string> &patterns) {
    Node *root = new Node;
    for(int i = 0; i < patterns.size(); i++) {
        Node *cur = root;
        for(auto &ch : patterns[i]) {
            cur = cur->getNextOrCreateNext(ch);
        }
        cur->addOutput(i);
    }

    std::queue<Node *> q;
    root->initAcRoot();
    for(auto it : root->getNextMap()) {
        Node *next = it.second;
        if(next != root) {
            next->setFail(root);
            q.push(next);
        }
    }

    while(!q.empty()) {
        Node *cur = q.front();
        q.pop();

        for(auto it : cur->getNextMap()) {
            char ch = it.first;
            Node *next = it.second;
            Node *state = cur->getFail();

            while(state->getNext(ch) == NULL) {
                state = state->getFail();
            }
            next->setFail(state->getNext(ch));
            for(auto out : next->getFail()->getOutput()) {
                next->addOutput(out);
            }
            q.push(next);
        }
    }

    return root;
}

void run(Node *root, const std::vector<std::string> &texts, std::vector<std::vector<int> > &res) {
    for(auto &text : texts) {
        Node *cur = root;
        res.push_back(std::vector<int>());
        for(auto &ch : text) {
            while(cur->getNext(ch) == NULL) {
                cur = cur->getFail();
            }
            cur = cur->getNext(ch);
            for(auto out : cur->getOutput()) {
                res.back().push_back(out);
            }
        }
    }
}

int main(int argc, char **argv) {
    std::vector<std::string> patterns;
    std::vector<std::string> texts;
    int pLength, tLength;

    printf("Algorithm: Aho–Corasick Algorithm\n");

    init(argc > 1 ? argv[1] : "url_2w.txt",
        "url_data.txt", patterns, texts, pLength, tLength);

    printf("Pattern Count: %ld\nText Count: %ld\n", patterns.size(), texts.size());

    auto build_begin = clock();

    Node *ac_root = build(patterns);

    auto build_end = clock();

    std::vector<std::vector<int> > res;

    auto run_begin = clock();

    run(ac_root, texts, res);

    auto run_end = clock();

    auto build_time = (build_end - build_begin) * 1.0 / CLOCKS_PER_SEC;
    auto run_time = (run_end - run_begin) * 1.0 / CLOCKS_PER_SEC;

    printf("Build Time: %f\nRuntime: %f\n", build_time, run_time);

    auto b_perf = pLength * 1.0 / 1024 / 1024 / build_time;
    auto r_perf = tLength * 1.0 / 1024 / 1024 / run_time;
    printf("Build Perf: %f\nRunning Perf: %f\n", b_perf, r_perf);

    freopen("ac_output.txt", "w", stdout);
    for(int i = 0; i <  res.size(); i++) {
        if(!res[i].empty()) {
            printf("%d:", i);
            for(auto &resElem : res[i]) {
                std::cout << ' ' << patterns[resElem];
            }
            printf("\n");
        }
    }
    fclose(stdout);
    return 0;
}
