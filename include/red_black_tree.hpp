#ifndef __RED_BLACK_TREE_HPP__
#define __RED_BLACK_TREE_HPP__

#include <string>
#include <vector>

using namespace std;

enum class color_t {
    Red, Black
};

template<typename T>
class red_black_tree {
public:
    //Produce red_black_tree
    //It doesn't support empty tree
    red_black_tree(T value, color_t color = color_t::Red);

    
    ~red_black_tree();

    //Search whether value exists
    bool search(T value);

    //Insert value
    void insert(T value);

    //Extract value from a tree
    //If tree has only single node, it extracts self.
    //Check as follows.
    // redred_black_tree<T> t;
    // ..
    // redred_black_tree<T>* e = t.extract(some_value);
    // if(t == *e) {
    //  //it extracted self.  
    // }
    red_black_tree<T>* extract(T value);

    //correct the node when it is inserted
    void correction();

    //Get height of tree
    int height();

    //Convert tree to multiple lines of strings.
    vector<string> toStrings();

    //Convert tree to a single line of string.
    string toString();

    //Get top value.
    T top();

    //get minimum value
    T min();

    //get maximum value
    T max();

    //check whether tree is valid
    bool valid();

private:
    //top data of this tree
    T value;
    color_t color;

    //empty spaces for print function
    const char emptySpace = ' ';
    
    //pointers for relations
    red_black_tree<T>* left = nullptr;
    red_black_tree<T>* right = nullptr;
    red_black_tree<T>* parent = nullptr;

    //rotate tree
    //      b               b
    //  a      z   ==>  x       a
    // x  y                   y   z  (but "a" and "b"'s values have been swapped)
    // Not changed the position of "a" and "b" to prevent root switching problem
    void rotate_r();

    //rotate tree
    //       a                    a
    //  x         b   ==>    b         z
    //          y  z       x   y  (but "a" and "b"'s values have been swapped)
    // Not changed the position of "a" and "b" to prevent root switching problem
    void rotate_l();

    //Actual extract function
    red_black_tree<T>* _extract();

    //Handle a block where extra block is needed.
    //used for extract function
    void handle_extra_black();

    //finder functions
    //it returns nullptr when it can not find.

    //find grandparent
    red_black_tree<T>* grandParent();

    //find uncle
    red_black_tree<T>* uncle();

    //find sibling
    red_black_tree<T>* sibling();

    //find nephew that located close
    red_black_tree<T>* nephew_close();

    //find nephew that located far
    red_black_tree<T>* nephew_far();

    //Swap only value of the root
    void swap(red_black_tree<T>& rhs);
};


//Functions due to the templates

template<typename T>
red_black_tree<T>::red_black_tree(T value, color_t color = color_t::Red) {
    this->value = value;
    this->color = color;
}

template<typename T>
red_black_tree<T>::~red_black_tree() {
    if (left != nullptr) delete left;
    if (right != nullptr) delete right;
}

template<typename T>
bool red_black_tree<T>::search(T value) {
    if (this->value == value) return true;
    if (value < this->value && left != nullptr) return left->search(value);
    if (value > this->value && right != nullptr) return right->search(value);
    return false;
}

template<typename T>
void red_black_tree<T>::insert(T value) {
    if (value <= this->value) {
        if (left != nullptr) {
            left->insert(value);
            return;
        }
        left = new red_black_tree(value);
        left->parent = this;
        left->correction();
    }
    else {//if(value > this->value){
        if (right != nullptr) {
            right->insert(value);
            return;
        }
        right = new red_black_tree(value);
        right->parent = this;
        right->correction();
    }
}

template<typename T>
red_black_tree<T>* red_black_tree<T>::extract(T value) {
    //Search and remove
    if (this->value == value) return _extract();
    if (value < this->value && left != nullptr) return left->extract(value);
    if (value > this->value && right != nullptr) return right->extract(value);
    return nullptr;
}

template<typename T>
void red_black_tree<T>::correction() {
    //correct red node.
    //case 0 : nothing is wrong.
    if (this->color == color_t::Black) return;
    // case 1 : this is root
    if (parent == nullptr) {
        this->color = color_t::Black;
        return;
    }
    //case 2 : No rule violation.
    if (parent->color == color_t::Black) return;
    // it implies that g != nullptr.
    // If g == nullptr, this is the child of root.
    // which means parent->color == color_t::Black since root is black.

    // correction node will change in some cases.
    //u and g only valid in here.
    red_black_tree<T>* u = uncle();
    red_black_tree<T>* g = grandParent();
    // case 3 : both parents are red.(color swap)
    if ((u != nullptr) && (u->color == color_t::Red)) {//parent->color == color_t::Red
        parent->color = color_t::Black;
        u->color = color_t::Black;
        g->color = color_t::Red;
        g->correction();
        return;
    }
    //case 4/5 : one of parent is black and the other is red.
    //case 4 : force slopes for rotate.
    if ((parent->left == this) && (g->right == parent)) {
        parent->rotate_r();
    }
    else if ((parent->right == this) && (g->left == parent)) {
        parent->rotate_l();
    }
    //case 5: rotate to move red.
    parent->color = color_t::Black;
    g->color = color_t::Red;
    if (parent == g->left) {
        g->rotate_r();
    }
    else {
        g->rotate_l();
    }
}

template<typename T>
int red_black_tree<T>::height() {
    int height = 1;
    if (left != nullptr && height < 1 + left->height()) {
        height = 1 + left->height();
    }
    if (right != nullptr && height < 1 + right->height()) {
        height = 1 + right->height();
    }
    return height;
}

template<typename T>
vector<string> red_black_tree<T>::toStrings() {
    vector<string> lstring;
    vector<string> rstring;
    if (left != nullptr) lstring = left->toStrings();
    if (right != nullptr) rstring = right->toStrings();
    int leftRow = lstring.size();
    int leftCol = leftRow == 0 ? 0 : lstring[0].size();
    int rightRow = rstring.size();
    int rightCol = rightRow == 0 ? 0 : rstring[0].size();
    vector<string> result;
    string firstline;
    string val = to_string(value);
    int digitSize = val.size();

    firstline = string(leftCol, emptySpace) + val
        + (color == color_t::Red ? string("R") : string("B"))
        + string(rightCol, emptySpace);
    result.emplace_back(firstline);
    int minRow = leftRow < rightRow ? leftRow : rightRow; 
    for (int i = 0; i < minRow; ++i) {
        string line = lstring[i] + string(digitSize + 1, emptySpace) + rstring[i];
        result.emplace_back(line);
    }

    for (int i = leftRow; i < rightRow; ++i) {
        string line;
        line = string(digitSize + 1 + leftCol, emptySpace) + rstring[i];
        result.emplace_back(line);
    }
    for (int i = rightRow; i < leftRow; ++i) {
        string line;
        line = lstring[i] + string(digitSize + 1 + rightCol, emptySpace);
        result.emplace_back(line);
    }
    return result;
}

template<typename T>
string red_black_tree<T>::toString() {
    string lstring;
    string rstring;
    if (left != nullptr) lstring = left->toString();
    if (right != nullptr) rstring = right->toString();
    return string("(") + lstring
        + to_string(value)
        + (color == color_t::Red ? string("R") : string("B"))
        + rstring + string(")");
}

template<typename T>
T red_black_tree<T>::top() {
    return value;
}

template<typename T>
T red_black_tree<T>::min() {
    if (left != nullptr) return left->min();
    else return value;
}

template<typename T>
T red_black_tree<T>::max() {
    if (right != nullptr) return right->max();
    else return value;
}

template<typename T>
bool red_black_tree<T>::valid() {
    if ((left != nullptr) && ((left->max() > value) || (!left->valid()))) return false;
    if ((right != nullptr) && ((right->min() < value) || (!right->valid()))) return false;
    color_t lc = left == nullptr ?  color_t::Black : left->color;
    color_t rc = right == nullptr ?  color_t::Black : right->color;
    if(color == color_t :: Red && (lc == color_t :: Red || rc == color_t :: Red)) return false;
    return true;
}

template<typename T>
void red_black_tree<T>::rotate_r() {
    //      b               b
    //  a      z   ==>  x       a
    // x  y                   y   z  (but value has been swapped with a and b)
    // Not changed the position to protect root switching problem
    if (left == nullptr) return;
    red_black_tree* a = left;
    red_black_tree* b = this;
    red_black_tree* x = a->left;
    red_black_tree* y = a->right;
    red_black_tree* z = right;

    b->right = a;
    if (x != nullptr) x->parent = b;
    b->left = x;
    a->left = y;
    if (z != nullptr) z->parent = a;
    a->right = z;
    swap(*a);
}

template<typename T>
void red_black_tree<T>::rotate_l() {
    //       a                    a
    //  x         b   ==>    b         z
    //          y  z       x   y  (but value has been swapped with a and b)
    // Not changed the position to protect root switching problem
    if (right == nullptr) return;
    red_black_tree* a = this;
    red_black_tree* b = right;
    red_black_tree* x = left;
    red_black_tree* y = b->left;
    red_black_tree* z = b->right;
    a->left = b;
    if (x != nullptr) x->parent = b;
    b->left = x;
    b->right = y;
    if (z != nullptr) z->parent = a;
    a->right = z;
    swap(*b);
}

template<typename T>
red_black_tree<T>* red_black_tree<T>::_extract() {
    if (left != nullptr && right != nullptr) {
        red_black_tree<T>* target = left;
        while (target->right != nullptr) {
            target = target->right;
        }
        T temp = value;
        value = target->value;
        target->value = temp;
        return target->_extract();
    }
    red_black_tree<T>* c = (left == nullptr ? right : left);
    if (parent == nullptr) { //root
        if (c == nullptr) {//no child
            return this; //Caution : Extracting self
        }
        T temp = value;
        value = c->value;
        c->value = temp;

        c->parent = nullptr;
        if (left == c) left = nullptr;
        else right = nullptr;
        c->color = color_t::Black;
        return c;
    }
    //nil node to trace the node if there is no child.
    red_black_tree<T>* nil = new red_black_tree<T>(0, color_t::Black);
    if (c == nullptr) c = nil;
    c->parent = parent;
    if (this == parent->left)
        parent->left = c;
    else//if(this == parent->right)
        parent->right = c;
    if (color == color_t::Black) {// if it is red, no problem at all.
        c->handle_extra_black();
    }
    red_black_tree<T>* np = nil->parent;
    if (np != nullptr) {//remove nil if it has been used
        if (np->left == nil) np->left = nullptr;
        else np->right = nullptr;
        nil->parent = nullptr;
    }
    delete nil;
    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;
    color = color_t::Black;
    return this;
}

template<typename T>
void red_black_tree<T>::handle_extra_black() {
    if (parent == nullptr) {
        color = color_t::Black;
        return;
    }
    if (color == color_t::Red) {
        color = color_t::Black;
        return;
    }
    red_black_tree<T>* s = sibling();
    red_black_tree<T>* nf = nephew_far();
    red_black_tree<T>* nc = nephew_close();
    color_t nfc = (nf != nullptr && nf->color == color_t::Red) ? color_t::Red : color_t::Black;
    color_t ncc = (nc != nullptr && nc->color == color_t::Red) ? color_t::Red : color_t::Black;
    if (s->color == color_t::Black) {
        if (nfc == color_t::Red) {
            color_t temp;
            temp = parent->color;
            parent->color = color_t::Black;
            s->color = temp;
            nf->color = color_t::Black;

            if (this == parent->left)
                parent->rotate_l();
            else
                parent->rotate_r();
        }
        else {//if(nfc == color_t::Black){
            if (ncc == color_t::Red) {
                nc->color = color_t::Black;
                s->color = color_t::Red;
                if (this == parent->left)
                    s->rotate_r();
                else
                    s->rotate_l();
                handle_extra_black();
            }
            else {//if(ncc == color_t::Black){
                s->color = color_t::Red;
                parent->handle_extra_black();
            }
        }
    }
    else {//if(s->color == color_t::Red){
        s->color = color_t::Black;
        parent->color = color_t::Red;
        if (this == parent->right) parent->rotate_r();
        else parent->rotate_l(); //if(this == parent->left)
        handle_extra_black();
    }
}


template<typename T>
red_black_tree<T>* red_black_tree<T>::grandParent() {
    if (parent == nullptr) return nullptr;
    return parent->parent;
}

template<typename T>
red_black_tree<T>* red_black_tree<T>::uncle() {
    red_black_tree* g = grandParent();
    if (g == nullptr) return nullptr;
    if (g->left == parent) return g->right;
    return g->left;
}

template<typename T>
red_black_tree<T>* red_black_tree<T>::sibling() {
    if (parent == nullptr) return nullptr;
    if (parent->left == this) return parent->right;
    return parent->left;
}

template<typename T>
red_black_tree<T>* red_black_tree<T>::nephew_close() {
    red_black_tree<T>* s = sibling();
    if (s == nullptr) return nullptr;
    if (parent->left == this) return s->left;
    return s->right;
}

template<typename T>
red_black_tree<T>* red_black_tree<T>::nephew_far() {
    red_black_tree<T>* s = sibling();
    if (s == nullptr) return nullptr;
    if (parent->left == this) return s->right;
    return s->left;
}

template<typename T>
void red_black_tree<T>::swap(red_black_tree<T>& rhs) {
    {
        T temp = value;
        value = rhs.value;
        rhs.value = temp;
    }
    {
        color_t temp = color;
        color = rhs.color;
        rhs.color = temp;
    }
}

#endif __RED_BLACK_TREE_HPP__