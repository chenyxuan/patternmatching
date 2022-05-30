#include <bits/stdc++.h>

const int pattern_maxlen = 1000;

void init(const char* pfn, const char* tfn, 
std::vector<std::string> &ps, std::vector<std::string> &ts,
int &pL, int &tL, int &pMinLen) {
    static char pstr[pattern_maxlen];
    printf("Filename: %s %s\n", pfn, tfn);
    std::string tmp;
    std::fstream pfs (pfn, std::fstream::in);
    pMinLen = pattern_maxlen;
    while(pfs.getline(pstr, pattern_maxlen)) {
        ps.push_back(std::string(pstr));
        if(ps.back().back() == 13) {
            ps.back().pop_back();
        }
        pMinLen = std::min(pMinLen, (int)ps.back().length());
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

const int B = 2;
int shift[256][256];
std::unordered_map<unsigned int, std::vector<int> > hash[256][256];

void build(const std::vector<std::string> &patterns, int pml) {
    for(int i = 0; i < 256; i++) {
        for(int j = 0; j < 256; j++) {
            shift[i][j] = pml - B + 1;
        }
    }
    for(int pid = 0; pid < patterns.size(); pid++) {
        auto &pattern = patterns[pid];
        for(int i = 0; i + 1 < pml; i++) {
            int &sft = shift[(unsigned char)pattern[i]][(unsigned char)pattern[i + 1]];
            sft = std::min(sft, (pml - 1) - (i + 1));
        }
        unsigned int prefix = (unsigned int) pattern[0] * 277 * 277 + pattern[1] * 277 + pattern[2];
        hash[(unsigned char)pattern[pml - 2]][(unsigned char)pattern[pml - 1]][prefix].push_back(pid);
    }
}


void run(int pml, std::vector<std::string> &ts, std::vector<std::string> &ps, std::vector<std::vector<int> > &res) {
    for(auto &text : ts) {
        int cur = 0;
        
        res.push_back(std::vector<int>());
        while(cur + 1 < text.length()) {
            int sft = shift[(unsigned char)text[cur]][(unsigned char)text[cur + 1]];
            if(sft == 0) {
                int left = cur + 1 - pml + 1;
                if(left >= 0) {
                    unsigned int prefix = (unsigned int) text[left] * 277 * 277 + text[left + 1] * 277 + text[left + 2];
                    auto &hashmap = hash[(unsigned char)text[cur]][(unsigned char)text[cur + 1]];
                    if(hashmap.count(prefix)) {
                        for(auto pid : hashmap[prefix]) {
                            if(ps[pid].compare(text.substr(left, ps[pid].length())) == 0) {
                                res.back().push_back(pid);
                            } 
                        }
                    }
                }
                cur++;
            } else {
                cur += sft;
            }
        }
    }
}


int main(int argc, char **argv) {
    std::vector<std::string> patterns;
    std::vector<std::string> texts;
    std::vector<std::vector<int> > res;
    int pLength, tLength, pml;

    printf("Algorithm: Wu-Manber Algorithm\n");

    init(argc > 1 ? argv[1] : "url_2w.txt",
        "url_data.txt", patterns, texts, pLength, tLength, pml);

    printf("Pattern Count: %ld\nText Count: %ld\n", patterns.size(), texts.size());
    printf("Pattern MinLength: %d\n", pml);

    auto build_begin = clock();
    build(patterns, pml);
    auto build_end = clock();

    auto run_begin = clock();
    run(pml, texts, patterns, res);
    auto run_end = clock();

    
    auto build_time = (build_end - build_begin) * 1.0 / CLOCKS_PER_SEC;
    auto run_time = (run_end - run_begin) * 1.0 / CLOCKS_PER_SEC;

    printf("Build Time: %f\nRuntime: %f\n", build_time, run_time);

    auto b_perf = pLength * 1.0 / 1024 / 1024 / build_time;
    auto r_perf = tLength * 1.0 / 1024 / 1024 / run_time;

    printf("Build Perf: %f\nRunning Perf: %f\n", b_perf, r_perf);

    freopen("wm_output.txt", "w", stdout);
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
