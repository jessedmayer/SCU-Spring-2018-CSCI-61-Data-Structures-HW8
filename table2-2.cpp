#include <iostream>
#include <cassert>  // Provides assert
#include <cstdlib>  // Provides size_t
#include "table2.h"
#include "node2.h"
namespace main_savitch_12B {

    template<class RecordType>
    table<RecordType>::table() {
        std::size_t i;

        total_records = 0;
        for (i = 0; i < TABLE_SIZE; ++i)
            data[i] = NULL;
    }

    template<class RecordType>
    table<RecordType>::table(const table& source){
        std::size_t i;

        total_records = source.total_records;
        for (i = 0; i < TABLE_SIZE; ++i)
            data[i] = source.data[i];
    }

    template<class RecordType>
    table<RecordType>::~table( ){
        std::size_t i;

        for (i = 0; i < TABLE_SIZE; ++i) {
            while (data[i] != NULL) {
                main_savitch_6B::node <RecordType> *remove_ptr;

                remove_ptr = data[i];
                data[i] = data[i]->link();
                delete remove_ptr;
            }
        }
    }

    template<class RecordType>
    void table<RecordType>::insert(const RecordType& entry){
        //   void insert(const RecordType& entry)
//     Precondition: entry.key >= 0. Also if entry.key is not already a key in
//     the table, then the Table has space for another record
//     (i.e., size( ) < CAPACITY).
//     Postcondition: If the table already had a record with a key equal to
//     entry.key, then that record is replaced by entry. Otherwise, entry has
//     been added as a new record of the Table.
        assert(entry.key >= 0);
        size_t index = hash(entry.key);
        total_records++;
        main_savitch_6B::node <RecordType> *temp = data[index];

        if(data[index] == NULL) {
            temp = new main_savitch_6B::node(entry,NULL);
        }
        else{
            while(temp != NULL){
                temp = temp->link();
            }
            temp = new main_savitch_6B::node(entry,NULL);
        }
    }

    template<class RecordType>
    void table<RecordType>::remove(int key){
        //   void remove(int key)
//     Postcondition: If a record was in the Table with the specified key, then
//     that record has been removed; otherwise the table is unchanged.
        assert(key >= 0);
        if (is_present(key)) {   // The key was found, so remove this record and reduce used by 1.
            size_t index = hash(key);
            main_savitch_6B::node <RecordType> *temp = data[index];

                while(temp->data().key != key){
                    temp = temp->link();
                }

            main_savitch_6B::node <RecordType> *remove_ptr;
            remove_ptr = temp;
            temp = temp->link();
            delete remove_ptr;
            total_records--;
        }
    }

    template<class RecordType>
    void table<RecordType>::operator =(const table& source){
        std::size_t i;

        total_records = source.total_records;
        for (i = 0; i < TABLE_SIZE; ++i)
            data[i] = source.data[i];
    }


    template<class RecordType>
    void table<RecordType>::find(int key, bool& found, RecordType& result)const{
        //   void find(int key, bool& found, RecordType& result) const
//     Postcondition: If a record is in the Table with the specified key, then
//     found is true and result is set to a copy of the record with that key.
//     Otherwise found is false and the result contains garbage.
        assert(key >= 0);
        if(is_present(key)) {
            size_t index = hash(key);
            main_savitch_6B::node<RecordType> *temp = data[index];

            while (temp->data().key != key) {
                temp = temp->link();
            }
            found = 1;
            result = temp->data();
        }
        else{
            found = 0;
            result = NULL;
        }
    }

    template<class RecordType>
    bool table<RecordType>::is_present(int key) const{
        //   bool is_present(const Item& target) const
//     Postcondition: The return value is true if there is a record in the
//     Table with the specified key. Otherwise, the return value is false.
        assert(key >= 0);
        size_t index = hash(key);
        main_savitch_6B::node<RecordType> *temp = data[index];

        while (temp->data().key != key) {
            if(temp==NULL){
                return 0;
            }
            temp = temp->link();
        }
        return 1;
    }


    template<class RecordType>
    size_t table<RecordType>::hash(int key) const{
        return (key % TABLE_SIZE);
    }
}
