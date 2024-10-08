// // Do NOT add any other includes
 #include "search.h"

// int hashfn(string s)
// {
//     int result=0;
//     for(int i=0;i<s.size();i++)
//     {
//         result=result+(s[i]*(i+1));
//     }
//     return result;
// }
// void setter(string& s)
// {
//     for(int i=0;i<s.length();i++)
//     {
//         if((s[i]>=65)&&(s[i]<=90))
//         {
//             s[i]=s[i]+32;
//         }
//     }
// }
// void insert(Node* &root,Node* &ins)
// {
//     if(root==nullptr)
//     {
//         root=ins;
//     }
//     else if(root->right==nullptr)
//     {
//         root->right=ins;
//         ins->left=root;
//     }
//     else
//     {
//         Node* ri=root->right;
//         root->right=ins;
//         ins->left=root;
//         ri->left=ins;
//         ins->right=ri;
//     }
// }
// SearchEngine::SearchEngine()
// {
//     // Implement your function here 
// }

// SearchEngine::~SearchEngine()
// {
//     // Implement your function here
//     for(int i=0;i<properties.size();i++)
//     {
//         delete properties[i];
//     }  
// }

// void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
// {
//     // Implement your function here
    
//     // sentences.push_back(sentence);
//     // bookcode.push_back(book_code);
//     // pages.push_back(page);
//     // paragraphs.push_back(paragraph);
//     // sentno.push_back(sentence_no);
//     setter(sentence);
//     sentences.push_back(sentence);
//     point* p=new point();
//     p->bookcode=book_code;
//     p->pageno=page;
//     p->para=paragraph;
//     p->sent=sentence_no;
//     properties.push_back(p);
// }

// Node* SearchEngine::search(string pattern, int& n_matches) //modified int& n_matches to int n_matches
// {
//     // Implement your function here 
//     Node* root=nullptr;
//     for(int p=0;p<sentences.size();p++)
//     {
        
//         int pl=pattern.length();  //check for capitals BRINDISI or brindisi
//         int sl=sentences[p].length();
//         int hash_pat=hashfn(pattern);
//         int ohashsen=hashfn(sentences[p].substr(0,pl));
//         bool is;
//         for(int i=0;i<sl-pl+1;i++)
//         {
//             is=false;int hash_sen;
//             if(i==0)
//             {
//                 hash_sen=ohashsen;
//             }
//             else
//             {
//                 int sum=0;
//                 for(int l=i-1;l<i+pl-1;l++)
//                 {
//                     sum=sum+sentences[p][l];
//                 }
//                 hash_sen=(hash_sen-sum)+((sentences[p][i+pl-1])*pl);
//             }
//             if(hash_sen==hash_pat)
//             {
//                 //cout<<"entered hash sen is hash pat"<<endl;
//                 for(int j=0;j<pl;j++)
//                 {
//                     is=true;
//                     if(sentences[p][i+j]!=pattern[j])
//                     {
//                         is=false;
//                         break;
//                     }
//                 }
//             }
//             //cout<<"is:"<<is<<endl;
//             if(is)
//             {
//                 n_matches+=1;
//                 Node* ne;
//                 int w=properties[p]->bookcode;
//                 int x=properties[p]->pageno;
//                 int y=properties[p]->para;
//                 int z=properties[p]->sent; 
                
//                 ne=new Node(w,x,y,z,i);
//                // cout<<"stuck"<<endl;
//                 insert(root,ne);
//                 //cout<<"stuck"<<endl;
//             }
//         }
//         //cout<<n_matches<<endl;
//     } 
//     //cout<<"temp is"<<root<<endl;
//     if(root)
//     {
//         return root;
//     }
//     return nullptr;
// }