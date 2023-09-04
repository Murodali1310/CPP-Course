#pragma once

#include "iostream"

template <typename T, typename Allocator = std::allocator<T>>
class List {
 private:
  template <bool IsConst>
  struct CommonIterator;

 public:
  List(const Allocator& allocator = Allocator()) : node_alloc_(allocator) {
    head_ = std::allocator_traits<node_allocator>::allocate(node_alloc_, 1);
    head_->right = head_->left = head_;
    sz_ = 0;
  }
  List(size_t count, const T& val, const Allocator& allocator = Allocator())
      : node_alloc_(allocator) {
    head_ = std::allocator_traits<node_allocator>::allocate(node_alloc_, 1);
    head_->right = head_->left = head_;
    sz_ = 0;
    try {
      for (size_t i = 0; i < count; ++i) {
        push_back(val);
      }
    } catch (...) {
      clear();
      throw;
    }
  }
  explicit List(size_t count, const Allocator& allocator = Allocator())
      : node_alloc_(allocator) {
    head_ = std::allocator_traits<node_allocator>::allocate(node_alloc_, 1);
    head_->right = head_->left = head_;
    sz_ = 0;
    try {
      for (size_t i = 0; i < count; ++i) {
        push_back();
      }
    } catch (...) {
      clear();
      throw;
    }
  }
  List(const List& other)
      : node_alloc_(
            std::allocator_traits<Allocator>::
                select_on_container_copy_construction(other.node_alloc_)) {
    head_ = std::allocator_traits<node_allocator>::allocate(node_alloc_, 1);
    head_->right = head_->left = head_;
    sz_ = 0;
    try {
      for (const auto& elem : other) {
        push_back(elem);
      }
    } catch (...) {
      clear();
      throw;
    }
  }
  List(std::initializer_list<T> initializer,
       const Allocator& allocator = Allocator())
      : node_alloc_(static_cast<node_allocator>(allocator)) {
    head_ = std::allocator_traits<node_allocator>::allocate(node_alloc_, 1);
    head_->right = head_->left = head_;
    sz_ = 0;
    try {
      for (const auto& elem : initializer) {
        push_back(elem);
      }
    } catch (...) {
      clear();
      throw;
    }
  }
  void swap(List& other) {
    std::swap(node_alloc_, other.node_alloc_);
    std::swap(head_, other.head_);
    std::swap(sz_, other.sz_);
  }
  List& operator=(const List& other) {
    if (this == &other) {
      return *this;
    }
    List temp(other);
    if (std::allocator_traits<
            node_allocator>::propagate_on_container_copy_assignment::value and
        temp.node_alloc_ != other.node_alloc_) {
      temp.node_alloc_ = other.node_alloc_;
    }
    swap(temp);
    return *this;
  }
  using iterator = CommonIterator<false>;
  using const_iterator = CommonIterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  template <typename... Args>
  iterator emplace(const const_iterator& pos, Args&&... args) {
    BaseNode* node1 =
        std::allocator_traits<node_allocator>::allocate(node_alloc_, 1);
    std::allocator_traits<node_allocator>::construct(
        node_alloc_, node1, std::forward<Args>(args)...);
    ++sz_;
    link_after(pos.ptr->left, node1);
    iterator ret(node1);
    return ret;
  }

  iterator insert(const const_iterator& iter, const T& cst) {
    BaseNode* node1 = create_new_node(cst);
    ++sz_;
    link_after(iter.ptr->left, node1);
    iterator ret(node1);
    return ret;
  }

  iterator insert(const const_iterator& iter, T&& cst) {
    BaseNode* node1 = create_new_node(std::move(cst));
    ++sz_;
    link_after(iter.ptr->left, node1);
    iterator ret(node1);
    return ret;
  }

  iterator erase_node(const const_iterator& iter) {
    iterator nxt(const_cast<BaseNode*>(iter.ptr));
    ++nxt;
    --sz_;
    cut(iter.ptr);
    std::allocator_traits<node_allocator>::destroy(node_alloc_, iter.ptr);
    std::allocator_traits<node_allocator>::deallocate(
        node_alloc_, const_cast<BaseNode*>(iter.ptr), 1);
    return nxt;
  }

  iterator erase_node(const_iterator first, const_iterator last) {
    while (first != last) {
      first = erase_node(first);
    }
    iterator ans(const_cast<BaseNode*>(first.ptr));
    return ans;
  }

  iterator begin() {
    iterator iter = head_->right;
    return iter;
  }

  iterator end() {
    iterator iter = head_;
    return iter;
  }

  const_iterator begin() const {
    const_iterator iter = head_->right;
    return iter;
  }

  const_iterator end() const {
    const_iterator iter = head_;
    return iter;
  }

  const_iterator cbegin() const {
    const_iterator iter = head_->right;
    return iter;
  }

  const_iterator cend() const {
    const_iterator iter = head_;
    return iter;
  }

  reverse_iterator rbegin() {
    reverse_iterator iter(head_);
    return iter;
  }

  reverse_iterator rend() {
    reverse_iterator iter(head_->right);
    return iter;
  }

  const_reverse_iterator rbegin() const {
    const_reverse_iterator iter(head_);
    return iter;
  }

  const_reverse_iterator rend() const {
    const_reverse_iterator iter(head_->right);
    return iter;
  }

  const_reverse_iterator crbegin() const {
    const_reverse_iterator iter(head_);
    return iter;
  }

  const_reverse_iterator crend() const {
    const_reverse_iterator iter(head_->right);
    return iter;
  }
  auto get_allocator() { return node_alloc_; }

  size_t size() const { return sz_; }

  bool empty() const { return sz_ == 0; }

  void push_back() { link_after(head_->left); }

  void push_back(const T& val) { link_after(head_->left, val); }

  void push_back(T&& val) { link_after(head_->left, std::move(val)); }

  void push_front(T&& val) { link_after(head_, std::move(val)); }

  void push_front(const T& val) { link_after(head_, val); }

  void pop_front() { erase_node(head_->right); }

  void pop_back() { erase_node(head_->left); }

  T& back() { return head_->left->val; }

  const T& back() const { return head_->left->val; }

  T& front() { return head_->right->val; }

  const T& front() const { return head_->right->val; }

  ~List() { clear(); }

 private:
  struct BaseNode {
    T val;
    BaseNode* right;
    BaseNode* left;
    BaseNode(BaseNode* left = nullptr, BaseNode* right = nullptr)
        : val(T()), left(left), right(right) {}
    BaseNode(const T& val, BaseNode* left = nullptr, BaseNode* right = nullptr)
        : val(val), left(left), right(right) {}
  };
  using node_allocator = typename std::allocator_traits<
      Allocator>::template rebind_alloc<BaseNode>;
  BaseNode* head_;
  node_allocator node_alloc_;
  size_t sz_;

  void link_after(BaseNode* node) {
    BaseNode* temp =
        std::allocator_traits<node_allocator>::allocate(node_alloc_, 1);
    try {
      std::allocator_traits<node_allocator>::construct(node_alloc_, temp, node,
                                                       node->right);
    } catch (...) {
      std::allocator_traits<node_allocator>::deallocate(node_alloc_, temp, 1);
      throw;
    }
    node->right = temp;
    temp->right->left = temp;
    ++sz_;
  }

  void link_after(BaseNode* node, const T& val) {
    BaseNode* temp =
        std::allocator_traits<node_allocator>::allocate(node_alloc_, 1);
    try {
      std::allocator_traits<node_allocator>::construct(node_alloc_, temp, val,
                                                       node, node->right);
    } catch (...) {
      std::allocator_traits<node_allocator>::deallocate(node_alloc_, temp, 1);
      throw;
    }
    node->right = temp;
    temp->right->left = temp;
    ++sz_;
  }
  void erase_node(BaseNode* node) {
    if (node == head_) {
      return;
    }
    node->right->left = node->left;
    node->left->right = node->right;
    std::allocator_traits<node_allocator>::destroy(node_alloc_, node);
    std::allocator_traits<node_allocator>::deallocate(node_alloc_, node, 1);
    --sz_;
  }
  void clear() {
    while (!empty()) {
      pop_back();
    }
    std::allocator_traits<node_allocator>::deallocate(node_alloc_, head_, 1);
    sz_ = 0;
  }
  template <bool IsConst>
  class CommonIterator {
   private:
    using t_ref = std::conditional_t<IsConst, const T&, T&>;
    using t_ptr = std::conditional_t<IsConst, const T*, T*>;

   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::conditional_t<IsConst, const T, T>;
    using pointer = t_ptr;
    using reference = t_ref;
    using difference_type = std::ptrdiff_t;

    std::conditional_t<IsConst, const BaseNode*, BaseNode*> ptr = nullptr;

    CommonIterator() = default;
    CommonIterator(const CommonIterator<IsConst>& iter) : ptr(iter.ptr) {}

    operator CommonIterator<true>() {
      CommonIterator<true> iter(ptr);
      return iter;
    }

    CommonIterator(
        std::conditional_t<IsConst, const BaseNode*, BaseNode*> node_ptr)
        : ptr(node_ptr) {}

    CommonIterator& operator++() {
      ptr = ptr->right;
      return *this;
    }

    CommonIterator operator++(int) {
      CommonIterator temp = *this;
      ++(*this);
      return temp;
    }

    CommonIterator& operator--() {
      ptr = ptr->left;
      return *this;
    }

    CommonIterator operator--(int) {
      CommonIterator temp = *this;
      --(*this);
      return temp;
    }
    template <bool Y>
    bool operator==(const CommonIterator<Y>& iter) const {
      return ptr == iter.ptr;
    }

    template <bool Y>
    bool operator!=(const CommonIterator<Y>& iter) const {
      return ptr != iter.ptr;
    }

    reference operator*() const {  /// MAY BE DELETE CONST IN THE END
      return ptr->val;
    }

    pointer operator->() const {
      return &(ptr->val);  ///?????
    }
  };

 public:
  using value_type = T;
  using allocator_type =
      typename std::allocator_traits<Allocator>::allocator_type;
};