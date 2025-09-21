#include <cassert>
#include <vector>
#include <list>
#include <utility>
#include <string>
#include <iostream>
//  figure out how std hash works
// how many buckets when u construct the hash table
// no memory leakage

template <typename K, typename V, typename Hash = std::hash<K>>

class HashMap
{
private:
  std::vector<std::list<std::pair<K, V>>> table;
  size_t bucket_count;
  size_t element_number = 0;
  const double load_factor = 0.75;
  Hash hasher;

  size_t get_index(const K &key) const
  {
    return hasher(key) % bucket_count;
  }

  public:
  size_t get_bucket_count() const
  {
    return bucket_count;
  }
  size_t size() const
  {
    return element_number;
  }
  
  
  HashMap(size_t buckets, Hash h = Hash()) : bucket_count(buckets), table(buckets), hasher(h)
  {
    if (buckets == 0)
    {
      throw std::invalid_argument("Number of buckets must be greater than 0");
    }
    else
    {
      bucket_count = buckets;
    }
    table.resize(bucket_count);
  }

  // if 1 is returned we added a new {key, value}
  // if 0 is returned we update the value
  int insert(const K &key, const V &value)
  {
    size_t index = get_index(key);
    auto &bucket = table[index];
    for (auto &pair : bucket)
    {
      // since we cannot have the same key twice, we chekcing if the key already exists and updating the value
      if (pair.first == key)
      {
        pair.second = value;
        return 0;
      }
    }
    // this would add the element at the end of the bucket
    bucket.emplace_back(key, value);
    element_number++;

    // rehashing logic, i think this works
    // static_cast<double> temporarily converts element_number to double, this should help
    if (static_cast<double>(element_number) / bucket_count > load_factor)
    {
      // rehashing
      size_t new_bucket_count = bucket_count * 2;
      std::vector<std::list<std::pair<K, V>>> new_table(new_bucket_count);

      for (const auto &b : table) // need to go one more in i think
      {
        for (const auto &pair : b)
        {
          size_t new_index = hasher(pair.first) % new_bucket_count;
          new_table[new_index].emplace_back(pair);
        }
      }
      // apparently std::move is much faster than just assigning
      table = std::move(new_table);
      bucket_count = new_bucket_count;
    }
    return 1;
  };

  // const so they don't change anything
  const V *find(const K &key) const
  {
    size_t index = get_index(key);
    auto &bucket = table[index];
    for (auto &pair : bucket)
    {
      if (pair.first == key)
      {
        return &pair.second;
      }
    }
    return nullptr;
  };

  // 1 -> success, 0 -> key not found
  size_t erase(const K &key)
  {
    size_t index = get_index(key);
    auto &bucket = table[index];
    for (auto it = bucket.begin(); it != bucket.end(); ++it)
    {
      if (it->first == key)
      {
        bucket.erase(it);
        // forgot to add this here 
        element_number--;
        return 1;
      }
    }
    return 0;
  };

  // this is redundant since key is always unique
  size_t erase(const K &key, const V &value)
  {
    size_t index = get_index(key);
    auto &bucket = table[index];
    for (auto it = bucket.begin(); it != bucket.end(); ++it)
    {
      if (it->first == key && it->second == value)
      {
        bucket.erase(it);
        element_number--;
        return 1;
      }
    }
    return 0;
  };
};

int main(){
  return 0;
};