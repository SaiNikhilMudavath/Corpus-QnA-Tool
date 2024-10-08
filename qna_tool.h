#pragma once
#include <iostream>
#include <fstream>
#include "Node.h"
#include "search.h"

using namespace std;

//parentanode pointer for top k parentagraphs.

class paranode
{
    public:
    float score;
    int bookn;
    int pagen;
    int paran;
    paranode()
    {
        score=0;
        bookn=0;
        pagen=0;
        paran=0;
    }
    ~paranode()
    {}
};


//avl tree node which has left,right,word and its count in corpus and csv;
class avlnode
{
    public:
    avlnode* left;
    avlnode* right;
    avlnode* parent;
    string word;
    long long count;
    int height;
    avlnode()
    {
        left=nullptr;
        right=nullptr;
        parent=nullptr;
        count=0;word="";
    }
    ~avlnode()
    {
        
    }
    avlnode(long long c,string w)
    {
        left=nullptr;
        right=nullptr;
        parent=nullptr;
        count=c;
        word=w;
    } 

    int get_Height(avlnode* node) {
        if (node == NULL) {
            return -1;
        }
        int leftHeight = get_Height(node->left);
        int rightHeight = get_Height(node->right);
        int result= 1 + std::max(leftHeight, rightHeight);
        return result;

    }

    avlnode* LeftLeftRotation(){
        avlnode* newRoot =this->left;
        if(this->parent!=NULL){
            if(this->parent->word > this->word){
            this->parent->left=this->left; 
            }
            else{
                this->parent->right=this->left;
            }
        }
        newRoot->parent=this->parent;
        this->parent=newRoot;
        this->left = newRoot->right;
        if(this->left!=NULL){
            (this->left)->parent=this;
        }
        newRoot->right = this;

        // Update heights
        int temp_height_1 = get_Height(this);
        this->height=temp_height_1;
        int temp_height_2= get_Height(this->parent);
        this->parent->height = temp_height_2;
        
        return newRoot;
    }

    avlnode* RightRightRotation(){
        avlnode* newRoot =this->right;
        if(this->parent!=NULL){
            if(this->parent->word > this->word){
            this->parent->left=newRoot; 
            }
            else{
                this->parent->right=newRoot;
            }
        }
        newRoot->parent=this->parent;
        this->parent=newRoot;
        this->right = newRoot->left;
        if(this->right!=NULL){
            (this->right)->parent=this;
        }
        newRoot->left = this;

        // Update heights
        int temp_height_1 = get_Height(this);
        this->height=temp_height_1;
        int temp_height_2= get_Height(newRoot);
        this->parent->height=temp_height_2;
        
        return newRoot;
    }

    avlnode* LeftRightRotation(){
        left = left->RightRightRotation();
        avlnode* result = this->LeftLeftRotation();
        return result;
    }

    avlnode* RightLeftRotation(){
        right=right->LeftLeftRotation();
        avlnode* result =this->RightRightRotation();
        return result;
    }

};
class avltree
{
    public:
    avlnode* root;
    avltree()
    {
        root=new avlnode();
        root=nullptr;
    }
    ~avltree()
    {
        postorder(root);
    }
    void postorder(avlnode* node)
    {
        if(node==nullptr)
        {
            return;
        }
        postorder(node->left);
        postorder(node->right);
        delete node;
    }
    int get_height(avlnode* node) {
        if (node == NULL) {
            return -1;
        }
        int leftHeight = get_height(node->left);
        int rightHeight = get_height(node->right);
        int result= 1 + std::max(leftHeight, rightHeight);
        return result;
    }
    int balance_factor(avlnode* node){
        int leftHeight = get_height(node->left);
        int rightHeight = get_height(node->right);
        int ans= leftHeight - rightHeight;
        return ans;
    }

    void insert_avlnode(long long c,string k)
    {
       // cout<<k << c <<endl;
        avlnode* node =new avlnode(c,k);
      //  cout<<root->count <<root->word<<endl;
        if(root==nullptr){
            root= node;
            return;
        }
        //cout<<"sinsert enter dbd"<<endl;
        else{
            avlnode* current=root;
            avlnode* rahul;
            while(current!=nullptr){
                rahul=current;
                if(k>current->word){
                
                    current=current->right;
                }
                else if (k<current->word){
                
                    current=current->left;
                }   
                else{
                    current->count+=c;
                    //cout<<current->word<<endl;
                    return;
                } 
            }
            if(k<rahul->word){
                rahul->left=node;
                node->parent=rahul;
                //cout<<rahul->left->word<<" "<<rahul->left->count<<endl;
            }
            else{
                rahul->right=node;
                node->parent=rahul;
                //cout<<rahul->right->word<<" "<<rahul->right->count<<endl;
            }
        }
    }
    long long get_word_count(string sample_word)
    {
       // cout<< root->word  << root->count <<endl;
        avlnode* current=root;
        while(current!=nullptr)
        {
            if(current->word<sample_word)
            {
                current=current->right;
            }
            else if(current->word>sample_word)
            {
                current=current->left;
            }
            else if(current->word==sample_word)
            {
                return current->count;
            }
        }
        return 0;
    }
};

class QNA_tool {

private:

    // You are free to change the implementation of this function
    void query_llm(string filename, Node* root, int k, string API_word, string question);
    // filename is the python file which will call ChatGPT API
    // root is the head of the linked list of parentagraphs
    // k is the number of parentagraphs (or the number of nodes in linked list)
    // API_word is the API word for ChatGPT
    // question is the question asked by the user

    // You can add attributes/helper functions here

public:

    /* Please do not touch the attributes and
    functions within the guard lines placed below */
    /* ------------------------------------------- */
    
    QNA_tool(); // Constructor
    ~QNA_tool(); // Destructor

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);
    // This function is similar to the functions in dict and search 
    // The corpus will be provided to you via this function
    // It will be called for each sentence in the corpus

    Node* get_top_k_para(string question, int k);
    // This function takes in a question, preprocess it
    // And returns a list of parentagraphs which contain the question
    // In each Node, you must set: book_code, page, parentagraph (other parentameters won't be checked)

    std::string get_paragraph(int book_code, int page, int paragraph);
    // Given the book_code, page number, and the parentagraph number, returns the string parentagraph.
    // Searches through the corpus.

    void query(string question, string filename);
    // This function takes in a question and a filename.
    // It should write the final answer to the specified filename.

    /* -----------------------------------------*/
    /* Please do not touch the code above this line */

    // You can add attributes/helper functions here
    avltree* csv;
    avltree* corpus;
    vector<avltree*> paragraphs;
    vector<int> book_vector;
    vector<int> page_vector;
    vector<int> para_vector;
    
    int prev_book=0;
    int prev_page=0;
    int prev_para=0;

};