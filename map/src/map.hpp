/**
 * implement a container like std::map
 */
#include <map>
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP
// #include <type_traits>
#include <bits/stdc++.h>
// std::swap()
// only for std::less<T>
#include <functional>
#include <cstddef>
#include <iostream>
#include <regex>
#include <sys/types.h>
#include "utility.hpp"
#include "exceptions.hpp"
extern std::map<int,int>mp;
extern std::vector<int>re;
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
    size_t size()const
    {
        if(root==nullptr)
            return 0;
        return root->size;
    }
    bool empty()const
    {
        return size()==0;
    }
    void clear()
    {
        destruct_tree(root);
        root=nullptr;
    }
    Node * search(const Key &key)const
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
	const Node * constsearch(const Key &key)const
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
	static const Node* find_prev(const Node * node)
    {
        if(node==nullptr)
        {
//            std::cerr<<"!!!";exit(0);
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
    static Node* find_prev(Node * node)
    {
        if(node==nullptr)
        {
//            std::cerr<<"!!!";exit(0);
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
	static Node* find_next(Node * node)
    {
        if(node==nullptr)
        {
//			exit(0);
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
    static const Node* find_next(const Node * node)
    {
        if(node==nullptr)
        {
//			exit(0);
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


    Node* find_first_element()const
    {
        Node *node=root;
        if(node==nullptr)
            return nullptr;
        while(node->left!=nullptr)
            node=node->left;
        return node;
    }
    Node* find_last_element()const
    {
        Node *node=root;
        if(node==nullptr)
            return nullptr;
        while(node->right!=nullptr)
            node=node->right;
        return node;
    }
	bool is_left_(Node *node)
	{
		if(node==nullptr){
//			exit(0);
            throw invalid_operator("is_left: node is nullptr");

		}
		if(node->parent==nullptr){
			return false;
		}
		return node->parent->left==node;
	}
	void update(Node *node)
	{
		if(node==nullptr)
			// std::ce";exit(0);throw invalid_operator("update: node is nullptr");
			return;
		if(node->parent==nullptr){
			root=node;
			node->color=BLACK;
//            std::cerr<<")))))";
		}
		node->size=(node->left==nullptr?0:node->left->size)+(node->right==nullptr?0:node->right->size)+1;
	}
    void rotate_left(Node *node){
		if(node==nullptr){
//			exit(0);
            throw invalid_operator("rotate_left: node is nullptr");
		}
		if(node->right==nullptr){
//			exit(0);
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
//			exit(0);
            throw invalid_operator("rotate_right: node is nullptr");
		}
		if(node->left==nullptr){
//			exit(0);
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
        update_up(node);
        //siz
		insert_fixup_(node);
		update_up(node);
	}
	void insert_fixup_(Node *node)
	{
		if(node==nullptr||node->parent==nullptr||node->parent->color==BLACK)
			return;
		Node * father=node->parent,*grandfather=father->parent,*uncle; 
		//father must be exist and his color is red
		bool is_left_father=is_left_(father);
		if(is_left_father){
			uncle=father->parent->right;
			if(uncle!=nullptr&&uncle->color==RED){
				father->color=BLACK;
				uncle->color=BLACK;
				grandfather->color=RED;
				insert_fixup_(grandfather);
				return;
			}
			bool is_left=is_left_(node);
			if(is_left_(node)){
				father->color=BLACK;
				grandfather->color=RED;
				rotate_right(grandfather);
				return;
			}else{
				rotate_left(father);
				node->color=BLACK;
				grandfather->color=RED;

				rotate_right(grandfather);
				return;
			}
		}else{
			uncle=father->parent->left;
			if(uncle!=nullptr&&uncle->color==RED){
				father->color=BLACK;
				uncle->color=BLACK;
				grandfather->color=RED;
				insert_fixup_(grandfather);
				return;
			}
			bool is_left=is_left_(node);
			if(is_left_(node)){
				rotate_right(father);
				node->color=BLACK;
				grandfather->color=RED;
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
	class iterator;
	pair<iterator,bool> insert(const value_type &val)
	{
		Node *node=insert_bykey(val);
		if(node==nullptr)
			return pair(end(),false);
		else
			return pair(iterator(node),true);
	}
	void erase(const Key &key)
	{
		if(root==nullptr)
			return ;
		if(root->left==nullptr&&root->right==nullptr)
		{
			if(!Compare()(root->val.first,key)&&!Compare()(key,root->val.first))
			{
				delete root;
				root=nullptr;
			}
			return ;
		}
		Node *node=root;
		while(node!=nullptr){
			// getchar();
			// std::cerr<<node->val.first<<std::endl;
			erase_fixdown(node,key);
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
                node=next->right;
				// std::cerr<<"JJJJ";
				continue;
			}
			if(node->left!=nullptr){
				if(node==root){
					root=node->left;
					node->left->parent=nullptr;
				}
				else if(is_left_(node)){
					node->parent->left=node->left;
					node->left->parent=node->parent;
				}else{
					node->parent->right=node->left;
					node->left->parent=node->parent;
				}
				update_up(node->left);
				delete node;
				return;
			}else if(node->right!=nullptr){
				if(node==root){
					root=node->right;
					node->right->parent=nullptr;
				}
				else if(is_left_(node)){
					node->parent->left=node->right;
					node->right->parent=node->parent;
				}else{
					node->parent->right=node->right;
					node->right->parent=node->parent;
				}
				update_up(node->right);
				delete node;
				return;
			}else{
				if(node==root){
					delete node;
					root=nullptr;
					return;
				}
				// if(node->color==BLACK)
				// 	erase_fixdown(node,key);
				if(is_left_(node))
					node->parent->left=nullptr;
				else
					node->parent->right=nullptr;
				update_up(node->parent);
                delete node;
				return;
			}
			
			
		}
        update_up(root);
	}
	void node_swap(Node *lhs,Node *rhs)
	{
		if(lhs==nullptr||rhs==nullptr){
//			exit(-1);
            throw invalid_operator("node_swap: lhs or rhs is nullptr");

		}
		Node*tmpl=lhs->parent,*tmpr=rhs->parent;
		std::swap(lhs->size,rhs->size);
		std::swap(lhs->color,rhs->color);
		//or the size would be wrong
		if(tmpl==rhs){
			std::swap(lhs,rhs);
			std::swap(tmpl,tmpr);
		}
		if(tmpr==lhs){
			//lhs is the parent of rhs
			if(is_left_(rhs)){
				bool tmp=is_left_(lhs);
				rhs->parent=lhs->parent;
				lhs->parent=rhs;
				std::swap(lhs->right,rhs->right);
				lhs->left=rhs->left;
				rhs->left=lhs;
				if(lhs==root)
					root=rhs;
				else if(tmp)
					rhs->parent->left=rhs;
				else
					rhs->parent->right=rhs;

            }else{
				bool tmp=is_left_(lhs);
				rhs->parent=lhs->parent;
				lhs->parent=rhs;
				std::swap(lhs->left,rhs->left);
				lhs->right=rhs->right;
				rhs->right=lhs;
				if(lhs==root)
					root=rhs;
				else if(tmp)
					rhs->parent->left=rhs;
				else
					rhs->parent->right=rhs;
			}
            if(lhs->left)lhs->left->parent=lhs;
            if(rhs->left)rhs->left->parent=rhs;

            if(lhs->right)lhs->right->parent=lhs;
            if(rhs->right)rhs->right->parent=rhs;

            return ;
		}
		bool lp=is_left_(lhs),rp=is_left_(rhs);
		std::swap(lhs->parent,rhs->parent);
		std::swap(lhs->left,rhs->left);
		std::swap(lhs->right,rhs->right);
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
	bool is_two_blackchild(Node* node)
	{
		if(node==nullptr){
//			exit(-1);
            throw invalid_operator("is_two_blackchild: node is nullptr");

		}
		if(node->left&&node->left->color==RED)
			return false;
		if(node->right&&node->right->color==RED)
			return false;
		return true;
	
	}
	void erase_fixdown(Node *node,const Key key)
	{
		if(node==nullptr)
			return ;
		if(node->color==RED)
			return ;	
		
		// if(node->parent==nullptr)
		// {
		// 	//root
		// 	if(node->left&&node->right&&node->left->color==node->right->color&&node->left->color==BLACK)
		// 	{
		// 		node->color=RED;
		// 		node->left->color=BLACK;
		// 		node->right->color=BLACK;
		// 		return ;
		// 	}


		// 	//to do
		// }
		Node *father=node->parent;
		Node *brother=father==nullptr?nullptr:(is_left_(node)?father->right:father->left);
		//we assume that parent is red
		//and now we try to change node into red

		//if brother exist!!!!!!!
		if(is_two_blackchild(node)){
			if(!brother||is_two_blackchild(brother))
			{
				if(father)father->color=BLACK;
				if(brother)brother->color=RED;
				node->color=RED;
				return ;
			}
			if(is_left_(node)){

				if(brother->left&&brother->left->color==RED)
				{
					rotate_right(brother);
					rotate_left(father);
					node->color=RED;
					father->color=BLACK;
				}else{
					rotate_left(father);
					node->color=RED;
					father->color=BLACK;
					brother->color=RED;
					brother->right->color=BLACK;
					//brother's right is still itself
				}
			}else{

				if(brother->right&&brother->right->color==RED)
				{
					rotate_left(brother);
					rotate_right(father);
					node->color=RED;
					father->color=BLACK;
				}else{
					rotate_right(father);
					node->color=RED;
					father->color=BLACK;
					brother->color=RED;
					brother->left->color=BLACK;
					//brother's left is still itself
				}
			}
			return;
		}
		if(Compare()(key,node->val.first)){
			if(node->left&&node->left->color==BLACK)
			{
				rotate_left(node);
				std::swap(node->color,node->parent->color);
			}
		}else if(Compare()(node->val.first,key)){
			if(node->right&&node->right->color==BLACK)
			{
				rotate_right(node);
				std::swap(node->color,node->parent->color);
			}
		}else{
			//delete node
			if(!node->left){
				rotate_left(node);
				std::swap(node->color,node->parent->color);
			}else if(!node->right){
				rotate_right(node);
				std::swap(node->color,node->parent->color);
			}else{
				if(node->right->color==BLACK)
				{
					rotate_right(node);
					std::swap(node->color,node->parent->color);
				}
			}
		}	
		
	}
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong std::ce";exit(0);throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
	 public:
	class const_iterator;
	class iterator {
		
		friend class map;
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		 Node * ptr;
         bool is_end=true;
	public:
		iterator() {
			// TODO
            ptr=nullptr;
			is_end=true;
		}
		iterator(const iterator &other) {
			// TODO
			ptr=other.ptr;
			is_end=other.is_end;
		}
		iterator(Node *node,bool is_end=false):ptr(node),is_end(is_end){}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			if(is_end){
				throw invalid_iterator("iterator++: is_end");
			}
			iterator tmp(*this);
			ptr=find_next(ptr);
			if(ptr)
				*this=iterator(ptr);
			else
				*this= iterator(tmp.ptr,true);
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {
			if(is_end){
				throw invalid_iterator("++iterator: is_end");
			}
			Node* tmp=ptr;
			ptr=find_next(ptr);
			if(ptr)
				return *this=iterator(ptr);
			else
				return *this=iterator(tmp,true);
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			if(!ptr)
				throw invalid_iterator("iterator--: is_begin");
			if(is_end){
				return iterator(ptr);
			}
			ptr=find_prev(ptr);
			if(!ptr)
				throw invalid_iterator("iterator--: is_end");
			else
				return iterator(ptr);
		}
		/**
		 * TODO --iter
		 */
		iterator & operator--() {
			if(!ptr)
				throw invalid_iterator("--iterator: is_begin");
			if(is_end){
				return *this=iterator(ptr);
			}
			ptr=find_prev(ptr);
			if(!ptr)
				throw invalid_iterator("--iterator: is_begin");
			else
				return *this=iterator(ptr);
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {
			if(is_end||!ptr){
				throw invalid_iterator("iterator*: is_end");
			}
			return ptr->val;
		}
		bool operator==(const iterator &rhs) const {
			return ptr==rhs.ptr&&is_end==rhs.is_end;
		}
		bool operator==(const const_iterator &rhs) const {
			return ptr==rhs.ptr&&is_end==rhs.is_end;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return ptr!=rhs.ptr||is_end!=rhs.is_end;
		}
		bool operator!=(const const_iterator &rhs) const {
			return ptr!=rhs.ptr||is_end!=rhs.is_end;
		}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {
			if(is_end||!ptr){
				throw invalid_iterator("iterator->: is_end");
			}
			return &ptr->val;
		}
	};
	class const_iterator {
		friend class map;
		private:
			const Node * ptr;
			bool is_end;
		public:
			const_iterator():ptr(nullptr),is_end(true) {}
			const_iterator(const const_iterator &other) {
				ptr=other.ptr;
				is_end=other.is_end;
			}
			const_iterator(const iterator &other) {
				ptr=other.ptr;
				is_end=other.is_end;
			}
			const_iterator(const Node *node,bool is_end=false):ptr(node),is_end(is_end){}
			const_iterator& operator=(const const_iterator &other) {
				ptr=other.ptr;
				is_end=other.is_end;
				return *this;
			}
			const_iterator& operator=(const iterator &other) {
				ptr=other.ptr;
				is_end=other.is_end;
				return *this;
			}
			const_iterator& operator++() {
				if(is_end){
					throw invalid_iterator("++iterator: is_end");
				}
				ptr=find_next(ptr);
				if(ptr)
					return *this;
				else
					return *this=const_iterator(ptr,true);
			}
			const_iterator operator++(int) {
				if(is_end){
					throw invalid_iterator("++iterator: is_end");
				}
				const_iterator tmp(*this);
				ptr=find_next(ptr);
				if(ptr)
					return tmp;
				else{
					ptr=tmp.ptr;
					is_end=true;
					return tmp;
				}
			}
			const_iterator& operator--() {
				if(!ptr)
					throw invalid_iterator("--iterator: is_begin");
				if(is_end){
					return *this=const_iterator(ptr);
				}
				ptr=find_prev(ptr);
				if(!ptr)
					throw invalid_iterator("--iterator: is_begin");
				else
					return *this;
			}
			const_iterator operator--(int) {
				if(!ptr)
					throw invalid_iterator("iterator--: is_begin");
				if(is_end){
					return const_iterator(ptr);
				}
				const_iterator tmp(*this);
				ptr=find_prev(ptr);
				if(!ptr)
					throw invalid_iterator("iterator--: is_begin");
				else
					return tmp;
			}
			const value_type& operator*() const {
				if(is_end||!ptr){
					throw invalid_iterator("iterator*: is_end");
				}
				return ptr->val;
			}
			bool operator==(const const_iterator &rhs) const {
				return ptr==rhs.ptr&&is_end==rhs.is_end;
			}
			bool operator==(const iterator &rhs) const {
				return ptr==rhs.ptr&&is_end==rhs.is_end;
			}
			bool operator!=(const const_iterator &rhs) const {
				return ptr!=rhs.ptr||is_end!=rhs.is_end;
			}
			bool operator!=(const iterator &rhs) const {
				return ptr!=rhs.ptr||is_end!=rhs.is_end;
			}
			const value_type* operator->() const noexcept {
				if(is_end||!ptr){
					throw invalid_iterator("iterator->: is_end");
				}
				return &ptr->val;
			}
	};
	
	// class const_iterator {
	// 	// it should has similar member method as iterator.
	// 	//  and it should be able to construct from an iterator.
	// 	private:
	// 		// data members.
	// 		const Node * ptr;
	// 		bool is_end;
	// 	public:
	// 		const_iterator():ptr(nullptr),is_end(true) {}
	// 			// TODO
			
	// 		const_iterator(const const_iterator &other) {
	// 			// TODO
	// 			ptr=other.ptr;
	// 			is_end=other.is_end;
	// 		}
	// 		const_iterator(const iterator &other) {
	// 			// TODO
	// 			ptr=other.ptr;
	// 			is_end=other.is_end;
	// 		}
			
	// 		// And other methods in iterator.
	// 		// And other methods in iterator.
	// 		// And other methods in iterator.
	// };
	/**
	 * TODO two constructors
	 */
	map(const map &other) {
		// TODO
		root=nullptr;
		for(const_iterator it=other.cbegin();it!=other.cend();it++)
		{
			insert(*it);
		}
	}
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other) {
		// TODO
		if(this==&other)
			return *this;
		clear();
		for(const_iterator it=other.cbegin();it!=other.cend();it++)
		{
			insert(*it);
		}
		return *this;
	}
	/**
	 * TODO Destructors
	 */
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {
		Node *node=search(key);
		if(node==nullptr)
			throw index_out_of_bound("at: no such element");
		return node->val.second;
	}
	const T & at(const Key &key) const {
		Node *node=search(key);
		if(node==nullptr)
			throw index_out_of_bound("at: no such element");
		return node->val.second;
	}
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {
		Node *node=search(key);
		if(node==nullptr)
		{
			node=insert_bykey(value_type(key,T()));
		}
		return node->val.second;
	}
	/**
	 * behave like at() std::ce";exit(0);throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
		Node *node=search(key);
		if(node==nullptr)
			throw index_out_of_bound("operator[]: no such element");
		return node->val.second;
	}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {
		Node *node=find_first_element();
		if(node)
			return iterator(node);
		else
			return iterator();
	}
	const_iterator cbegin() const {
		const Node *node=find_first_element();
		if(node)
			return const_iterator(node);
		else
			return const_iterator();
	}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {
		Node * node=find_last_element();
		if(node)
			return iterator(node,true);
		else
			return iterator(nullptr,true);
	}
	const_iterator cend() const {
		const Node * node=find_last_element();
		if(node)
			return const_iterator(node,true);
		else
			return const_iterator(nullptr,true);
	}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	// bool empty() const {}
	/**
	 * returns the number of elements.
	 */
	// size_t size() const {}
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
	 * std::ce";exit(0);throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) {
		if(pos.is_end)
			throw invalid_iterator("erase: pos==end()");
		if(pos.ptr==nullptr)
			throw invalid_iterator("erase: pos.ptr==nullptr");
		erase(pos->first);
	}
	// void erase(const Key &key)
	// {
	// 	erase()(key);
	// }
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const {
		Node *node=search(key);
		if(node==nullptr)
			return 0;
		return 1;
	}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	auto find(const Key &key)->iterator  {
		Node *node=search(key);
		if(node==nullptr)
			return end();
		return iterator(node);
	}
	auto find(const Key &key)const ->const_iterator  {
		Node *node=search(key);
		if(node==nullptr)
			return cend();
		return const_iterator(node);
	}
	
	// const_iterator find(const Key &key) const {
	// 	Node *node=search(key);
	// 	if(node==nullptr)
	// 		return cend();
	// 	return const_iterator(node);
	// }

	//this is debug
#ifdef SJTU_MAP_DEBUG 
	void print_treenode(Node *node,int f=0,int mi=0,int mx=100000)
	{
		
		if(node==nullptr)
			return ;
        if(f>=2)goto P;
//		if(f==1)exit(0);
		std::cout<<node->val.first<<" ";
		std::cout<<(node->color==RED?"RED":"BLACK")<<std::endl;
		std::cout<<"left"<<((node->left)?node->left->val.first:0)<<' '<<"right"<<((node->right)?node->right->val.first:0)<<std::endl;
        if(node->left&&node->left->parent!=node){
            std::cerr<<"erro";
            int p;std::cin>>p;

        }
        if(node->right&&node->right->parent!=node){
            std::cerr<<"erro";
            int p;std::cin>>p;

        }
        P:
        if(f==2){
            if(node->val.first<mi||node->val.first>mx){
                std::cerr<<"hhhhh"<<mi<<mx;            int p;std::cin>>p;

            }
            if(mp[node->val.first]!=node->val.second){
                std::cerr<<"nooooo";            int p;std::cin>>p;

            }
        }
        print_treenode(node->left,f,mi,node->val.first);
		re.push_back(node->val.first);
        print_treenode(node->right,f,node->val.first,mx);
	}
	void print_tree(int f=0)
	{
		if(f<2)std::cout<<"-------------------\n";
		print_treenode(root,f);
		if(f<2)std::cout<<"-------------------\n";
	}
    int chk(Node*node,int f=0)
    {
        if(!node)return 1;
        if(!node->parent) {
            if (node->color == RED)return f==-2?-2:-110;
        }else{
            if(node->color==RED&&node->parent->color==RED)
                return f==-2?-2:-110;
        }
        if(node->left&&node->left->parent!=node)return f==-3?-3:-110;
        if(node->right&&node->right->parent!=node)return f==-3?-3:-110;

        int nl=chk(node->left,f);
        int nr=chk(node->right,f);
        if(nl<0||nr<0||nl!=nr)return f==nl?f:(f==nr?f:-110);
        return nl+(node->color==BLACK?1:0);
    }
#endif

};

}

#endif
