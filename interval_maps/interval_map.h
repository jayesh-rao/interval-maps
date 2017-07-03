
#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <map>
#include <limits>
#include <climits>

using namespace std;

typedef enum {
    SUCCESS,
    FAILURE
} status_t;

template<class K, class V>
class MyMap {
    
private:
    std::map<K,V> m_;

public:
    // constructor associates whole range of K with val by inserting (K_min, val)
    // into the map
    MyMap( V const& val) {
        m_.insert(m_.begin(),std::make_pair(std::numeric_limits<K>::lowest(),val));
    };

    // Assign value val to interval [keyStart, keyEnd). 
    // Overwrite previous values in this interval. 
    // Do not change values outside this interval.
    // Conforming to the C++ Standard Library conventions, the interval 
    // includes keyStart, but excludes keyEnd.
    // If !( keyStart < keyEnd ), this designates an empty interval, 
    // and assign must do nothing.
    status_t insert( K const& keyStart, K const& keyEnd, const V& val ) {

        if (!( keyStart < keyEnd )) {
            return FAILURE;
        }

        // 1. Just before inserting, gather the previous value at keyEnd.
        //    And then insert/overwrite at keyEnd.
        auto ub_it = m_.upper_bound(keyEnd);
        const V& prev_val = std::prev(ub_it)->second;
        auto last = m_.insert(std::prev(ub_it), std::make_pair(keyEnd, prev_val));
        // If insertion fails, then overwite it with 'prev_val'
        if( !(last->second == prev_val)) {
            m_[keyEnd] = prev_val;
        }

        // 2. See if there are overlapping ranges, if yes coalesce, by
        // a. erasing anything between first and last
        // b. insert at 'keyStart'
        auto first = m_.lower_bound(keyStart);
        //auto last = m_.find(keyEnd);
        m_.erase(first, last);
        auto ret = m_.insert(std::prev(last), std::make_pair(keyStart, val));
        return SUCCESS;
    }

    auto lookup( K const& keyStart, K const& keyEnd ) {
        if( !(keyStart < keyEnd) ) {
            return std::map<K, V>();
        }
        return std::map<K, V>(std::prev(m_.upper_bound(keyStart)), m_.lower_bound(keyEnd));
    }

    // look-up of the value associated with key
    V const& operator[]( K const& key ) const {
        return ( --m_.upper_bound(key) )->second;
    }

    int distance(K const& keyStart, K const& keyEnd) {
        return std::distance(m_.find(keyStart), m_.find(keyEnd));
    }
    
    void display() {
        for (auto &e: m_) {
            std::cout << std::endl << e.first << "===>" << e.second << std::endl;
        }
        cout << endl << "size: " << m_.size();
    }
};

