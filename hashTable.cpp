#include <iostream>
#include <string>

// size of the hash table
const int TABLE_SIZE = 4;

  class HashTable
  {
  private:

    // this will the linked list node
    struct Node
    {
      int key;
      std::string value;
      Node *next;

      Node(int k, std::string v) : key(k), value(v), next(nullptr) {}
    };
    Node *table[TABLE_SIZE]; //this is basically the head of the linked list 

    public:

    // constructor nulls out the table
    HashTable(){
      for (int i = 0; i < TABLE_SIZE; i++)
        table[i] = nullptr;
    }
    
    int  hashFunction(int key){
      int a = 3;
      int b = 1;
      int value = (a * key + b);
      return value % TABLE_SIZE;
    } 

    void insert(int key, const std::string& value){
      int index = hashFunction(key);
      Node* newNode = new Node(key, value);

      // in the case the bucket is empty
      if(table[index] == nullptr){
        table[index] = newNode;
      } else {
        // collision, we need to add to the linked list 
        Node* current = table[index];
        while(current->next != nullptr){
          current = current->next;
        }
        current->next = newNode;
      }
    }

    void find(int key){
      int index = hashFunction(key);
      Node* current = table[index];
      // checking the linked list for the key 
      while(current != nullptr){
        if(current->key == key){
          std::cout << "Found: " << current->value << std::endl;
          return;
        }
        current = current->next;
      }
      std::cout << "Key not found" << std::endl;
    }

    void remove(int key){
      int index = hashFunction(key);
      Node* current = table[index];
      Node* prev = nullptr;

      while(current != nullptr){
        if(current->key == key){
          if(prev == nullptr){
            // removing the head of the list
            table[index] = current->next;
          } else {
            prev->next = current->next;
          }
          delete current;
          std::cout << "Key removed" << std::endl;
          return;
        }
        prev = current;
        current = current->next;
      }
      std::cout << "Key not found" << std::endl;
    }

    void erase(int key, const std::string& value){
      int index = hashFunction(key);
      Node* current = table[index];
      Node* prev = nullptr;

      while(current != nullptr){
        if(current->key == key && current->value == value){
          if(prev == nullptr){
            // removing the head of the list
            table[index] = current->next;
          } else {
            prev->next = current->next;
          }
          delete current;
          std::cout << "Key-Value pair removed" << std::endl;
          return;
        }
        prev = current;
        current = current->next;
      }
      std::cout << "Key-Value pair not found" << std::endl;
    }

  };




int main() {
    HashTable ht;

    // Insert some values
    ht.insert(1, "Alice");
    ht.insert(5, "Bob");       // will collide with 1 if TABLE_SIZE = 4
    ht.insert(9, "Charlie");   // another collision

    // Find values
    ht.find(1);   // should find Alice
    ht.find(5);   // should find Bob
    ht.find(9);   // should find Charlie
    ht.find(2);   // not found

    // Remove values
    ht.remove(5); // removes Bob
    ht.find(5);   // should say "Key not found"

    // Erase by key+value
    ht.erase(1, "Alice");  // should remove Alice
    ht.find(1);            // should say "Key not found"

    return 0;
}
