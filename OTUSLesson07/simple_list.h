#pragma once

#include <utility>
#include <iostream>
#include <memory>

template <typename T, typename Allocator = std::allocator<T>>
class MyList {
    struct Node {
        Node* next = nullptr;
        T value;
    };
public:
    using Node_ptr = Node*;
    class Iterator{
    public:
        explicit Iterator(Node_ptr node = nullptr) : node_(node) {}
        Iterator operator++() {node_ = node_->next; return *this;}
        const T& operator*() const {return node_->value;}
        friend bool operator==(Iterator lhs, Iterator rhs) { return lhs.node_ == rhs.node_;}
        friend bool operator!=(Iterator lhs, Iterator rhs) { return !(lhs == rhs);}
    private:
        Node_ptr node_ = nullptr; // not owns memory
    };
public:
    MyList() = default;

    MyList(const MyList& other) {
        if (other.is_empty()) return;
        head_ = allocator_.allocate(1u);
        head_->next = nullptr;
        allocator_.construct(&(head_->value), other.get_head()->value);
        auto p = other.get_head()->next;
        auto prev = head_;
        while (p) {
            auto new_p = allocator_.allocate(1u);
            new_p->next = nullptr;
            allocator_.construct(&(new_p->value), p->value);
            prev->next = new_p;
            prev = new_p;
            p = p->next;
        }
    }

    template <typename OtherAlloc>
    MyList(const MyList<T, OtherAlloc>& other) {
        if (other.is_empty()) return;
        head_ = allocator_.allocate(1u);
        head_->next = nullptr;
        allocator_.construct(&(head_->value), other.get_head()->value);
        auto p = other.get_head()->next;
        auto prev = head_;
        while (p) {
            auto new_p = allocator_.allocate(1u);
            new_p->next = nullptr;
            allocator_.construct(&(new_p->value), p->value);
            prev->next = new_p;
            prev = new_p;
            p = p->next;
        }
    }

    MyList(MyList&& other) noexcept : head_(other.head_), allocator_(std::move(other.allocator_)) {
        other.head_ = nullptr;
    }

    template <typename OtherAlloc>
    MyList(MyList<T, OtherAlloc>&& other) {
        if (other.is_empty()) return;
        head_ = allocator_.allocate(1u);
        head_->next = nullptr;
        allocator_.construct(&(head_->value), std::move(other.get_head()->value));
        auto p = other.get_head()->next;
        auto prev = head_;
        while (p) {
            auto new_p = allocator_.allocate(1u);
            new_p->next = nullptr;
            allocator_.construct(&(new_p->value), std::move(p->value));
            prev->next = new_p;
            prev = new_p;
            p = p->next;
        }
    }

    ~MyList() {
        while (head_) {
            auto p = head_;
            head_ = head_->next;
            allocator_.destroy(&(p->value));
            allocator_.deallocate(p, 1u);
        }
    }

    template<typename ... Args>
    void emplace_front(Args&&...args) {
        auto p = allocator_.allocate(1u);
        allocator_.construct(&(p->value), std::forward<Args>(args)...);
        p->next = head_;
        head_ = p;
    }
    const T& front() const { return head_->value;}
    const Node_ptr get_head() const {return head_;}
    Node_ptr get_head() {return head_;}
    Iterator begin() {return Iterator(head_);}
    const Iterator begin() const {return Iterator(head_);}
    Iterator end() {return Iterator();}
    const Iterator end() const {return Iterator();}
    friend std::ostream& operator<<(std::ostream& out, const MyList& lst) {
        bool is_first = true;
        for (const auto& v : lst) {
            if (is_first) {
                is_first = false;
            } else {
                out << '\n';
            }
            out << v;
        }
        return out;
    }
    [[nodiscard]]bool is_empty() const {return head_ == nullptr;}
private:
    Node_ptr head_ = nullptr;
    typename Allocator::template rebind<Node>::other allocator_;
};

template <typename T, typename ...Args>
MyList<T> make_list(Args&&...args) {
    MyList<T> list;
    (list.emplace_front(std::forward<Args>(args)),...);
    return list;
}