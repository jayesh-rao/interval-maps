#include <vector>
#include "interval_map.h"

using namespace std;
using namespace std::chrono;

int flip[] = {-1, 1};

template <typename T, typename U>
class Test {

public:
    Test(const U& v) : v_(v), map_(v), test_id_(1) {}

    void start(const T& s, const T& e, const U& v) {
        pvs_ = map_[s-1];
        pve_ = map_[e];
        s_ = s;
        e_ = e;
        v_ = v;
    }

    auto execution() {
        std::cout << std::endl << "Running tc:" << test_id_++ << " start: " << s_ << " end: " << e_ <<" "  << v_ ;
        auto ret = map_.insert(s_, e_, v_);
        auto tmp_map = map_.lookup(s_, e_);
        assert(tmp_map.size() == 1 || tmp_map.size() == 0);
        return ret;
    }

    bool certain_of() {
        map_.display();
        T rand_idx = s_ + (e_-s_)/2;
        if (s_ < 0 && e_ > 0) rand_idx = 0;

        // check the start key (s_) matches with 'v_'
        assert(map_[s_] == v_);
        assert( map_[rand_idx] == v_ );// check the val at any random index [s_, e_)
        assert( map_[e_ - 1] == v_ );  // check the val at the end - 1
        assert( map_[e_] == pve_ );    // check the val at the end, it should match the previous value
        assert( map_[s_ - 1] == pvs_ );// check the val at the start-1, it should match the previous value
        assert( map_.distance(s_, e_) == 1);
        return true;
    }
    
    auto run(const T& s, const T& e, const U& v) {
        start(s, e, v);
        return execution();
    }
    
private:
    T s_, e_;
    U v_;
    U pvs_, pve_;  // Using these fields to primarily assert my understaniding.
    MyMap<T, U> map_;
    int test_id_;
};

#define MAX 1000

void monkey_test() {

    Test<int, char> t('A');
    
    uint32_t i = 0;
    while ( i++ < MAX) {
        auto s = flip[rand()%2] * (rand() % INT_MAX);
        auto e = flip[rand()%2] * (rand() % INT_MAX);
        if (SUCCESS == t.run(s, e, 'A' + rand()%24)) {
            assert(t.certain_of());
        }
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    monkey_test();
    return 0;
}
