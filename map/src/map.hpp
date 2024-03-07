/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type;
	
	enum Color{BLACK,RED};
    struct Node{
        Node *left,*right,*parent;
        value_type val;
        int size;
		Color color;
        Node():left(nullptr),size(0),right(nullptr),parent(nullptr){}
        Node(const value_type &val,const Color color):left(nullptr),size(1),right(nullptr),parent(nullptr),val(val),color(color){}
    };
    Node *root;
    map():root(nullptr){}
    ~map(){destruct_tree(root);}
    void destruct_tree(Node *node){
        if(node==nullptr)return;
        destruct_tree(node->left);
        destruct_tree(node->right);
        delete node;
    }
    int size()
    {
        if(root==nullptr)
            return 0;
        return root->size;
    }
    bool empty()
    {
        return size()==0;
    }
    void clear()
    {
        destruct_tree(root);
        root=nullptr;
    }
    Node * search(const key_t &key)
    {
        Node *node=root;
        while(node!=nullptr)
        {
            if(Compare()(key,node->val.first))
                node=node->left;
            else if(Compare()(node->val.first,key))
                node=node->right;
            else
                return node;
        }
        return nullptr;
    }
    Node* find_prev(Node * node)
    {
        if(node==nullptr)
        {
            throw index_out_of_bound("find_prev: node is nullptr");
        }
        if(node->left!=nullptr)
        {
            node=node->left;
            while(node->right!=nullptr)
                node=node->right;
            return node;
        }
        else
        {
            while(node->parent!=nullptr&&node->parent->left==node)
                node=node->parent;
            return node->parent;
        }
    }
    Node* find_next(Node * node)
    {
        if(node==nullptr)
        {
			throw index_out_of_bound("find_next: node is nullptr");
        }
        if(node->right!=nullptr)
        {
            node=node->right;
            while(node->left!=nullptr)
                node=node->left;
            return node;
        }
        else
        {
            while(node->parent!=nullptr&&node->parent->right==node)
                node=node->parent;
            return node->parent;
        }
    }


    Node* find_first_element()
    {
        Node *node=root;
        if(node==nullptr)
            return nullptr;
        while(node->left!=nullptr)
            node=node->left;
        return node;
    }
    Node* find_last_element()
    {
        Node *node=root;
        if(node==nullptr)
            return nullptr;
        while(node->right!=nullptr)
            node=node->right;
        return node;
    }
	bool is_left(Node *node)
	{
		if(node==nullptr)
			throw invalid_operator("is_left: node is nullptr");
		if(node->parent==nullptr){
			return false;
		}
		return node->parent->left==node;
	}
	void update(Node *node)
	{
		if(node==nullptr)
			// throw invalid_operator("update: node is nullptr");
			return;
		if(node->parent==nullptr){
			root=node;
			node->color=BLACK;
		}
		node->size=(node->left==nullptr?0:node->left->size)+(node->right==nullptr?0:node->right->size)+1;
	}
    void rotate_left(Node *node){
		if(node==nullptr){
			throw invalid_operator("rotate_left: node is nullptr");
		}
		if(node->right==nullptr){
			throw invalid_operator("rotate_left: node->right is nullptr");
		}
		Node *right=node->right;
		node->right=right->left;
		if(right->left!=nullptr)
			right->left->parent=node;
		right->left=node;

		right->parent=node->parent;
		if(node->parent==nullptr)
			root=right;
		else if(node->parent->left==node)
			node->parent->left=right;
		else
			node->parent->right=right;
		node->parent=right;
		right->size=node->size;
		update(node);
	}
	void rotate_right(Node *node){
		if(node==nullptr){
			throw invalid_operator("rotate_right: node is nullptr");
		}
		if(node->left==nullptr){
			throw invalid_operator("rotate_right: node->left is nullptr");
		}
		Node *left=node->left;
		node->left=left->right;
		if(left->right!=nullptr)
			left->right->parent=node;
		left->right=node;
		if(node->parent==nullptr)
			root=left;
		else if(node->parent->left==node)
			node->parent->left=left;
		else
			node->parent->right=left;
		left->parent=node->parent;
		node->parent=left;
		left->size=node->size;
		update(node);
	}
	void insert_fixup(Node *node)
	{
		insert_fixup_(node);
		update_up(node);
	}
	void insert_fixup_(Node *node)
	{
		if(node==nullptr||node->parent==nullptr||node->parent->color==BLACK)
			return;
		Node * father=node->parent,*grandfather=father->parent,*uncle; 
		//father must be exist and his color is red
		bool is_left_father=is_left(father);
		if(is_left_father){
			uncle=father->parent->right;
			if(uncle!=nullptr&&uncle->color==RED){
				father->color=BLACK;
				uncle->color=BLACK;
				grandfather->color=RED;
				insert_fixup(grandfather);
				return;
			}
			bool is_left=is_left(node);
			if(is_left(node)){
				father->color=BLACK;
				grandfather->color=RED;
				rotate_right(grandfather);
				return;
			}else{
				rotate_left(father);
				node->color=BLACK;
				father->color=RED;
				rotate_right(grandfather);
				return;
			}
		}else{
			uncle=father->parent->left;
			if(uncle!=nullptr&&uncle->color==RED){
				father->color=BLACK;
				uncle->color=BLACK;
				grandfather->color=RED;
				insert_fixup(grandfather);
				return;
			}
			bool is_left=is_left(node);
			if(is_left(node)){
				rotate_right(father);
				node->color=BLACK;
				father->color=RED;
				rotate_left(grandfather);
				return;
			}else{
				father->color=BLACK;
				grandfather->color=RED;
				rotate_left(grandfather);
				return;
			}

			
		}
	}

    Node* insert_bykey(const value_type &val)
    {
        Node *node=root;
        Node *parent=nullptr;
		bool is_left;
        while(node!=nullptr)
        {
            parent=node;
            if(Compare()(val.first,node->val.first))
                node=node->left,is_left=true;
            else if(Compare()(node->val.first,val.first))
                node=node->right,is_left=false;
            else
                return nullptr;
        }
       	node=new Node(val,RED);
        node->parent=parent;
        if(parent==nullptr)
        {
            root=node;
			node->color=BLACK;
			return node;
        }
        else if(is_left)
        {
            parent->left=node;
        }
        else
        {
            parent->right=node;
        }
        insert_fixup(node);
        return node;
    }
	void update_up(Node *node)
	{
		if(node==nullptr)
			return ;
		update(node);
		update_up(node->parent);
	}
	void insert(const value_type &val)
	{
		Node *node=insert_bykey(val);
		update_up(node);
	}
	void erase(const key_t &key)
	{
		if(root==nullptr)
			return ;
		if(root->left==nullptr&&root->right==nullptr)
		{
			if(root->val.first==key)
			{
				delete root;
				root=nullptr;
			}
			return ;
		}
		Node *node=root,*parent=nullptr,*brother=nullptr;
		while(node!=nullptr){
			erase_fixdown(node);
			if(Compare()(key,node->val.first)){
				node=node->left;
				continue;
			}else if(Compare()(node->val.first,key)){
				node=node->right;
				continue;
			}
			if(node->left!=nullptr&&node->right!=nullptr){
				Node *next=find_next(node);
				node_swap(node,next);
				continue;
			}
			if(node->left!=nullptr){
				if(node==root){
					root=node->left;
					node->left->parent=nullptr;
				}
				else if(is_left(node)){
					node->parent->left=node->left;
					node->left->parent=node->parent;
				}else{
					node->parent->right=node->left;
					node->left->parent=node->parent;
				}
				update_up(node->left);
				delete node;
				return;
			}else{
				if(node==root){
					root=node->right;
					node->right->parent=nullptr;
				}
				else if(is_left(node)){
					node->parent->left=node->right;
					node->right->parent=node->parent;
				}else{
					node->parent->right=node->right;
					node->right->parent=node->parent;
				}
				update_up(node->right);
				delete node;
				return;
			}
			
			
		}
	}
	void node_swap(Node *lhs,Node *rhs)
	{
		if(lhs==nullptr||rhs==nullptr)
			throw invalid_operator("node_swap: lhs or rhs is nullptr");
		Node*tmpl=lhs->parent,*tmpr=rhs->parent;
		swap(lhs->size,rhs->size);
		//or the size would be wrong
		if(tmpl==rhs){
			swap(lhs,rhs);
			swap(tmpl,tmpr);
		}
		if(tmpr==lhs){
			//lhs is the parent of rhs
			if(is_left(rhs)){
				bool tmp=is_left(lhs);
				rhs->parent=lhs->parent;
				lhs->parent=rhs;
				swap(lhs->right,rhs->right);
				lhs->left=rhs->left;
				rhs->left=lhs;
				if(lhs==root)
					root=rhs;
				else if(tmp)
					rhs->parent->left=rhs;
				else
					rhs->parent->right=rhs;
			}else{
				bool tmp=is_left(lhs);
				rhs->parent=lhs->parent;
				lhs->parent=rhs;
				swap(lhs->left,rhs->left);
				lhs->right=rhs->right;
				rhs->right=lhs;
				if(lhs==root)
					root=rhs;
				else if(tmp)
					rhs->parent->left=rhs;
				else
					rhs->parent->right=rhs;
			}
			return ;
		}
		bool lp=is_left(lhs),rp=is_left(rhs);
		swap(lhs->parent,rhs->parent);
		swap(lhs->left,rhs->left);
		swap(lhs->right,rhs->right);
		if(lhs->parent==nullptr)
			root=lhs;
		else if(rp)
			lhs->parent->left=lhs;
		else
			lhs->parent->right=lhs;
		if(rhs->parent==nullptr)
			root=rhs;
		else if(lp)
			rhs->parent->left=rhs;
		else
			rhs->parent->right=rhs;
		
		if(lhs->left!=nullptr)
			lhs->left->parent=lhs;
		if(lhs->right!=nullptr)
			lhs->right->parent=lhs;
		if(rhs->left!=nullptr)
			rhs->left->parent=rhs;
		if(rhs->right!=nullptr)
			rhs->right->parent=rhs;
	}
	void erase_fixdown(Node *node)
	{
		if(node==nullptr)
			return ;
		if(node->color==RED)
			return ;	
		
		if(node->parent==nullptr)
		{
			//root
			if(node->left&&node->right&&node->left->color==node->right->color&&node->left->color==BLACK)
			{
				node->color=RED;
				node->left->color=BLACK;
				node->right->color=BLACK;
				return ;
			}
		}
		Node *father=node->parent;
		Node *brother=is_left(node)?father->right:father->left;
		//we assume that parent is red
		//and now we try to change node into red

	}
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
	class const_iterator;
	class iterator {
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:
		iterator() {
			// TODO
		}
		iterator(const iterator &other) {
			// TODO
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {}
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {}
		/**
		 * TODO --iter
		 */
		iterator & operator--() {}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {}
		bool operator==(const iterator &rhs) const {}
		bool operator==(const const_iterator &rhs) const {}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {}
		bool operator!=(const const_iterator &rhs) const {}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {}
	};
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		private:
			// data members.
		public:
			const_iterator() {
				// TODO
			}
			const_iterator(const const_iterator &other) {
				// TODO
			}
			const_iterator(const iterator &other) {
				// TODO
			}
			// And other methods in iterator.
			// And other methods in iterator.
			// And other methods in iterator.
	};
	/**
	 * TODO two constructors
	 */
	map(const map &other) {}
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other) {}
	/**
	 * TODO Destructors
	 */
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {}
	const T & at(const Key &key) const {}
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {}
	const_iterator cbegin() const {}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {}
	const_iterator cend() const {}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {}
	/**
	 * returns the number of elements.
	 */
	size_t size() const {}
	/**
	 * clears the contents
	 */
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) {}
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const {}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {}
	const_iterator find(const Key &key) const {}
};

}

#endif
