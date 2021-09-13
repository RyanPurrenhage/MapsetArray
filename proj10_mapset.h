/* proj09_mapset By: Ryan Purrenhage

Brief rundown:
   After 5 painful hours and then sleeping it off. I eventually realized that I made the destructor incorrectly. So I basically wasted those three hours ;(. Oh well.After that the program was easy, if only I had done that sooner.
   
Coding traits:
I use i as a variable in practially every single for loop. It was just how i was initially taught when I learned Java. If that is an issue, can you please let me know with a comment.
I use tabs. Bill Gates also uses tabs. Make of that information what you will. No but seriously, if this is an issue I can modify the space of my tabs.

There are individual explanations of the funcitons above each one.
*/


#ifndef MAP_SET
#define MAP_SET

#include<iostream>
using std::ostream; using std::endl; using std::cout;
#include<string>
using std::string;
#include<vector>
using std::vector;
#include<utility>
using std::pair;
#include<initializer_list>
using std::initializer_list;
#include<algorithm>
using std::sort; using std::lower_bound;
#include<sstream>
using std::ostringstream;
#include <iterator>
using std::begin; using std::end; 
using std::swap;


//
// Node
//
template<typename K, typename V>
struct Node {
  K first;
  V second;
  Node() = default;
  Node(K,V);
  bool operator<(const Node&) const;
  bool operator==(const Node&) const;
  friend ostream& operator<<(ostream &out, const Node &n){

    // WRITE YOUR CODE HERE!!!
    out << n.first << ":" << n.second;
    return out;
  }
};
//Creats a node
template<typename K, typename V>
Node<K,V>::Node(K key, V value){
    first = key;
    second = value;
}
// Compare's the two nodes based on the "key".
template<typename K, typename V>
bool Node<K,V>::operator<(const Node &n) const{
    if(first < n.first){
        return true;
    }else{
        return false;
    }
}
// Determines if the two Pairs are equal and returns a boolean. Primarily used for logic.
template<typename K, typename V>
bool Node<K,V>::operator==(const Node &n) const{
    if(first == n.first){
        return true;
    }else{
        return false;
    }
}

//
// MapSet
// 
template<typename K, typename V>
class MapSet{
 private:
  Node<K,V>* ary_;
  size_t last_;
  size_t capacity_;
  Node<K,V>* find_key(K);
  void grow ();
 public:
  MapSet(int sz = 2);
  MapSet(initializer_list< Node<K,V> >);
  MapSet (const MapSet&);
  MapSet operator=(MapSet);
  ~MapSet();
  size_t size();
  bool remove (K);  
  bool add(Node<K,V>);
  Node<K,V> get(K);
  bool update(K,V);  
  int compare(MapSet&);
  MapSet mapset_union (MapSet&);
  MapSet mapset_intersection(MapSet&);

  friend ostream& operator<<(ostream &out, const MapSet &ms){
    unsigned int j = 0;
    for(auto i = ms.ary_; i != ms.ary_ + ms.last_; i++){
        out << *i;
        if(j != (ms.last_ - 1 )){
            out << ", ";    
        }
        j++;
    }
    
    return out;
  }  
};
// Makes a MapSet with only the capacity specified.
template<typename K, typename V>
MapSet<K,V>::MapSet(int capacity){
    capacity_ = capacity;
    last_ = 0;
    ary_ = new Node<K,V>[capacity];
}
// Makes a MapSet from an initializer list.
template<typename K, typename V>
MapSet<K,V>::MapSet(initializer_list< Node<K,V> > il){
    capacity_ = il.size();
    last_ = il.size();
    ary_ = new Node<K,V>[il.size()];
    copy(il.begin(), il.end(), ary_);
    for(unsigned int j = 0; j < last_ - 1; j++){
            for(unsigned int i = 0; i < last_ - 1; i++){
                if(ary_[i+1] < ary_[i]){
                    auto temp = ary_[i];
                    ary_[i] = ary_[i+1];
                    ary_[i+1] = temp;
                }
            }
            
        }
}
// Makes a MapSet equal to another map
template<typename K, typename V>
MapSet<K,V>::MapSet(const MapSet &ms){
    capacity_ = ms.capacity_;
    last_ = ms.last_;
    ary_ = new Node<K,V>[ms.capacity_];
    copy(ms.ary_, ms.ary_ + ms.last_, ary_);
}

//Actually sets a MapSet equal to the other map.
template<typename K, typename V>
MapSet<K,V> MapSet<K,V>::operator=(MapSet<K,V> ms){
    swap(this->capacity_, ms.capacity_);
    swap(this->last_, ms.last_);
    swap(this->ary_, ms.ary_);
    return *this;
}
// The Deconstructor. 
template<typename K, typename V>
MapSet<K,V>::~MapSet(){
    delete [] ary_;
}
//Returns last_ or the Size
template<typename K, typename V>
size_t MapSet<K,V>::size(){
    return last_;
}
// Determines whether the array needs to grow then allocates more data to make it a larger array.
template<typename K, typename V>
void MapSet<K,V>::grow(){
    if(size() == capacity_){
        auto new_data = new Node<K,V>[capacity_ * 2];
        copy(ary_, ary_ + last_ ,new_data);
        swap(ary_, new_data);
        //delete new_data;
        capacity_ = capacity_ * 2;
    }
}
// Finds the key in the MapSet and return's a pointer to it.
template<typename K, typename V>
Node<K,V>* MapSet<K,V>::find_key(K key){
    for(auto i = ary_; i != ary_ + last_; i++){
        if(i->first == key){
            return i;
        }
    }
    return nullptr;
}
// Adds the node to the back of the list, which then sorts it so the node is in the correct spot.
template<typename K, typename V>
bool MapSet<K,V>::add(Node<K,V> n){
    if(find_key(n.first) == nullptr){
        if(last_ == capacity_){
            grow();
        }
        ary_[last_].first = n.first;
        ary_[last_].second = n.second;
        last_++;
        for(unsigned int j = 0; j < last_ - 1; j++){
            for(unsigned int i = 0; i < last_ - 1; i++){
                if(ary_[i+1] < ary_[i]){
                    auto temp = ary_[i];
                    ary_[i] = ary_[i+1];
                    ary_[i+1] = temp;
                }
            }
            
        }
        return true;
    }else{
        return false;    
    }
}
//This one is fun, I found the spot where the key we want to remove is, and simply wrote over it for each consecutive node in the MapSet.
template<typename K, typename V>
bool MapSet<K,V>::remove(K key){
    if(find_key(key) != nullptr){
        //remove the Node
            bool found = false;
            for(int i = 0; i < last_; i++){
                 if(ary_[i].first == key){
                    found = true;
                    continue;
                 }
                 if(found == true){
                     ary_[i-1] = ary_[i];
                 }
            }
            last_--;
        return true;
    }else{
        return false;    
    }
}
// Does the same thing as find_key but actually returns the node. Kinda pointless really.
template<typename K, typename V>
Node<K,V> MapSet<K,V>::get(K key){
    if(find_key(key) != nullptr){
        return *find_key(key);
    }else{
        return Node<K,V>("",0);    
    }
}
// updates a node in the MapSet.
template<typename K, typename V>
bool MapSet<K,V>::update(K key, V value){
    if(find_key(key) != nullptr){
        find_key(key)->second = value;
        return true;
    }else{
        return false;
    }
}
//Compares two seperate MapSet's and returns either a 1,-1 and 0.
template<typename K, typename V>
int MapSet<K,V>::compare(MapSet &ms){
    if(last_ >= ms.last_){
        auto j = ary_;
        for(auto i = ms.ary_; i != ms.ary_ + ms.last_; i++){
            if(i->first > j->first){
                return -1;
            }else if(i->first < j->first){
                return 1;
            }else{
                j++;
            }
        }
    }else{
        auto j = ms.ary_;
        for(auto i = ary_; i != ary_ + last_; i++){
            if(i->first > j->first){
                return 1;
            }else if(i->first < j->first){
                return -1;
            }else{
                j++;
            }
        }
    }
    if(size() > ms.size()){
        return 1;
    }
    else if(size() < ms.size()){
        return -1;
    }
    return 0;
}
// Combines two different MapSet's together into one.
template<typename K, typename V>
MapSet<K,V> MapSet<K,V>::mapset_union(MapSet<K,V> &ms){
    MapSet result;
    for(auto i = ary_; i != ary_ + last_; i++){
        result.add(*i);
    }
    for(auto i = ms.ary_; i != ms.ary_ + ms.last_; i++){
        result.add(*i);
    }
    return result;
}
// Combines two different MapSet's but only takes the nodes that are in both.
template<typename K, typename V>
MapSet<K,V> MapSet<K,V>::mapset_intersection(MapSet<K,V> &ms){
    MapSet result;
    for(auto i = ary_; i != ary_ + last_; i++ ){
        if(ms.find_key(i->first) != nullptr){
            result.add(*i);
        }
        
    }
    for(auto i = ms.ary_; i != ms.ary_ + last_; i++){
         if(find_key(i->first) != nullptr){
            result.add(*i);
        }
    }
    return result;
}

#endif
  