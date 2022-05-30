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

void build(std::unordered_map<unsigned long long, std::vector<int> > &krmap,
const std::vector<std::string> &patterns, int pml) {
    for(int i = 0; i < patterns.size(); i++) {
        unsigned long long hash = 0;
        const std::string &pattern = patterns[i];
        for(int j = 0; j < pml; j++) {
            hash = hash * 277 + pattern[j];
        }
        krmap[hash].push_back(i);
    }
}

void run(std::unordered_map<unsigned long long, std::vector<int> > &krmap, int pml,
std::vector<std::string> &ts, std::vector<std::string> &ps, std::vector<std::vector<int> > &res) {
    for(auto &text : ts) {
        res.push_back(std::vector<int>());
        unsigned long long base = 1;
        unsigned long long hash = 0;

        for(int i = 0; i < pml; i++) {
            base = base * 277;
            hash = hash * 277 + text[i];
        }
        if(krmap.count(hash)) {
            for(auto idx : krmap[hash]) {
                const auto &pattern = ps[idx];
                if(text.length() >= pattern.length()) {
                    if(pattern.compare(text.substr(0, pattern.length())) == 0) {
                        res.back().push_back(idx);
                    } 
                }
            }
        }

        for(int i = pml; i < text.length(); i++) {
            hash = hash * 277 + text[i] - text[i - pml] * base;        
            
            if(krmap.count(hash)) {
                for(auto idx : krmap[hash]) {
                    const auto &pattern = ps[idx];
                    if(text.length() >= i - pml + 1 + pattern.length()) {
                        if(pattern.compare(text.substr(i - pml + 1, pattern.length())) == 0) {
                            res.back().push_back(idx);
                        } 
                    }
                }
            }
        }
     }
}

int main(int argc, char **argv) {
    std::vector<std::string> patterns;
    std::vector<std::string> texts;
    std::vector<std::vector<int> > res;
    int pLength, tLength, pml;

    printf("Algorithm: Karp-Rabin Algorithm\n");

    init(argc > 1 ? argv[1] : "url_2w.txt",
        "url_data.txt", patterns, texts, pLength, tLength, pml);

    printf("Pattern Count: %ld\nText Count: %ld\n", patterns.size(), texts.size());
    printf("Pattern MinLength: %d\n", pml);

    std::unordered_map<unsigned long long, std::vector<int> > krmap;
    auto build_begin = clock();
    build(krmap, patterns, pml);
    auto build_end = clock();

    
    auto run_begin = clock();
    run(krmap, pml, texts, patterns, res);
    auto run_end = clock();

    
    auto build_time = (build_end - build_begin) * 1.0 / CLOCKS_PER_SEC;
    auto run_time = (run_end - run_begin) * 1.0 / CLOCKS_PER_SEC;

    printf("Build Time: %f\nRuntime: %f\n", build_time, run_time);

    auto b_perf = pLength * 1.0 / 1024 / 1024 / build_time;
    auto r_perf = tLength * 1.0 / 1024 / 1024 / run_time;
    printf("Build Perf: %f\nRunning Perf: %f\n", b_perf, r_perf);

    freopen("kr_output.txt", "w", stdout);
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
