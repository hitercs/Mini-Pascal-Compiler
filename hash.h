#ifndef HASH_H
#define HASH_H
#define BUCKET_SIZE 400
template <class T>
class Hash{
public:
    Hash();
    void insert_element(T element);
    T* search_element(char* key);
    int delete_element(const char* key);
    int xpos;
    int ypos;
private:
    T* Array[BUCKET_SIZE];
};
#endif // HASH_H
