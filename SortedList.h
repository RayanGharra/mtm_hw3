#pragma once

#include <iostream>
#include <stdexcept>

namespace mtm {

    template <typename T>
    class SortedList {
        struct Node {
            T data;
            Node* next;
            Node(const T& data) : data(data), next(nullptr) {}
        };

        Node* head;
        int cur_length;
    public:
        class ConstIterator;

        SortedList();
        SortedList(const SortedList&);
        SortedList& operator=(const SortedList&);
        ~SortedList();

        void insert(const T& element);
        void remove(ConstIterator it);
        int length() const;

        template <typename Predicate>
        SortedList<T> filter(Predicate pred) const;
        template <typename operation>
        SortedList<T> apply(operation op) const;

        ConstIterator begin() const;
        ConstIterator end() const;

    };

    template <typename T>
    SortedList<T>::SortedList() : head(nullptr), cur_length(0) {}

    template <typename T>
    SortedList<T>::SortedList(const SortedList& other) : head(nullptr), cur_length(0) {
        if (other.head == nullptr) {
            return;
        }

        Node* head = new Node(other.head->data);
        cur_length++;

        Node* cur_old = other.head->next;
        Node* cur_new = head;
        while (cur_old != nullptr) {
            cur_new->next = new Node(cur_old->data);

            cur_new = cur_new->next;
            cur_old = cur_old->next;

            cur_length++;
        }
    }

    template <typename T>
    SortedList<T>& SortedList<T>::operator=(const SortedList& other) {

        if (this == &other) {
            return *this;
        }

        while (head != nullptr) {
            Node* node_to_delete = head;
            head = head->next;
            delete node_to_delete;
        }
        cur_length = 0;
        if (other.head == nullptr) {
            return *this;
        }

        head = new Node(other.head->data);
        cur_length++;

        Node* current_old = other.head->next;
        Node* current_new = head;

        while (current_old != nullptr) {
            current_new->next = new Node(current_old->data);
            current_new = current_new->next;
            current_old = current_old->next;
            cur_length++;
        }
        return *this;
    }

    template <class T>
    void SortedList<T>::insert(const T& element) {
        Node* new_node = new Node(element);
        if (head == nullptr || element > head->data) {
            new_node->next = head;
            head = new_node;
            cur_length++;
            return;
        }
        Node* cur = head;
        while (cur->next != nullptr && !(element > cur->next->data)) {
            cur = cur->next;
        }
        new_node->next = cur->next;
        cur->next = new_node;
        cur_length++;

    }

    template <class T>
    class SortedList<T>::ConstIterator {
        const Node* cur_node;
        ConstIterator(const Node* node) : cur_node(node) {}

        friend class SortedList<T>;

    public:
        ConstIterator() : cur_node(nullptr) {}

        const T& operator*() const {
            return cur_node->data;
        }
        ConstIterator& operator++() {
            if (cur_node != nullptr) {
                cur_node = cur_node->next;
            }
            return *this;
        }
        bool operator==(const ConstIterator& other) const {
            return cur_node == other.cur_node;
        }

        bool operator!=(const ConstIterator& other) const {
            return cur_node != other.cur_node;
        }
    };

    template <typename T>
void SortedList<T>::remove(ConstIterator it) {
        if (it.cur_node == nullptr || head == nullptr) {
            return;
        }


        if (head == it.cur_node) {
            Node* node_to_delete = head;
            head = head->next;
            delete node_to_delete;
            cur_length--;
            return;
        }

        Node* cur = head;

        while (cur->next != nullptr && cur->next != it.cur_node) {
            cur = cur->next;
        }

        if (cur->next == it.cur_node) {
            Node* node_to_delete = cur->next;
            cur->next = cur->next->next;
            delete node_to_delete;
            cur_length--;
        }
    }
    template <typename T>
    int SortedList<T>::length() const {
        return cur_length;
    }

    template <typename T>
    template <typename Predicate>
    SortedList<T> SortedList<T>::filter(Predicate pred) const {
        SortedList<T> filtered_list;

        for (ConstIterator it = begin(); it != end(); ++it) {
            if (pred(*it)) {
                filtered_list.insert(*it);
            }
        }
        return filtered_list;
    }

    template <typename T>
    template <typename Operation>
    SortedList<T> SortedList<T>::apply(Operation op) const {
        SortedList<T> new_list;
        for (ConstIterator it = begin(); it != end(); ++it) {
            new_list.insert(op(*it));
        }

        return new_list;
    }

    template <typename T>
    typename SortedList<T>::ConstIterator SortedList<T>::begin() const {
        return ConstIterator(head);
    }

    template <typename T>
    typename SortedList<T>::ConstIterator SortedList<T>::end() const {
        return ConstIterator(nullptr);
    }

    template <typename T>
SortedList<T>::~SortedList() {
        Node* current = head;

        while (current != nullptr) {
            Node* next_node = current->next;
            delete current;
            current = next_node;
        }

        head = nullptr;
        cur_length = 0;
    }




}



