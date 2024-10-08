#include <assert.h>
#include <sstream>
#include "qna_tool.h"

using namespace std;

//1.arithmetic functions to change strings of csv to long long numbers

long long power(long long a,int b)
{
    long long h=a;
    if(b==0)
    {
        return 1;
    }
    while(b!=1)
    {
        a=a*h;
        b--;
    }
    return a;
}
long long strtolong(string s)
{
    long long sum=0;
    for(int i=s.length()-1;i>=0;i--)
    {
        long long asc=s[i]-'0';
        int len=s.length()-1;
        sum=sum+asc*power(10,len-i);
    }
    return sum;
}

//2.helper functions to read a csv
vector<string> commasep(string row)
{
    vector<string> answer;
    int c=0;string s1;string s2;
    for(int i=0;i<row.length();i++)
    {
        if(row[i]==',')
        {
            c=i;
            answer.push_back(s1);
            break;
        }
        else
        {
            s1=s1+row[i];
        }
    }
    for(int i=c+1;i<row.length();i++)
    {
        s2=s2+row[i];
    }
    answer.push_back(s2);
    return answer;
}
// to read csv
vector<vector<string>> read_csv(string csv)
{
   vector<vector<string>> data;
    std::ifstream file(csv);
    if(file.is_open())
    {
        string line;
        while(getline(file,line))
        {
            //cout<<"word is "<<line<<endl;
            vector<string> answer=commasep(line);
            //string count =answer[1];
            //what about count??
            //vector<string> p;
            data.push_back(answer);
        }
    }
    file.close();
    return data;
}
//3.functions to change sentence into vector of words.

bool isSeparator(char c)
{
    static const string separators = ".,-:!\"'()?—[]“”‘’˙;@ ";
    return separators.find(c) != string::npos;
}
string to_lower(string& result)
{
    for(char& c:result)
    {
        c=towlower(c);
    }
    return result;
}
vector<string> split_sentence(const string& sentence)
{
    vector<string> words;
    string word;
    for (char c:sentence)
    {
        if (isSeparator(c))
        {
            if (!word.empty())
            {
                words.push_back(to_lower(word)); // Convert to lowercase
            }
            word.clear();
        } 
        else
        {
            word += c;
        }
    }
    // If there's a word at the end of the sentence, add it to the vector
    if (!word.empty())
    {
        words.push_back(to_lower(word)); // Convert to lowercase
    }

    return words;
}
//4. merge sort to sort the scores of paragraphs
void merge(vector<paranode*>& arr,int l,int m,int r)
{
    int n1=m-l+1;
    int n2=r-m;

    vector<paranode*> L(n1);
    vector<paranode*> R(n2);

    for(int i=0;i<n1;i++)
    {
        L[i]=arr[l+i];
    }
    for(int j=0;j<n2;j++)
    {
        R[j]=arr[m+1+j];
    }
    
    int i=0;int j=0;int k=l; 

    while((i<n1)&&(j<n2))
    {
        if(L[i]->score>=R[j]->score)
        {
            arr[k]=L[i];
            i++;
        } 
        else
        {
            arr[k]=R[j];
            j++;
        }
        k++;
    }
    while(i<n1)
    {
        arr[k]=L[i];
        i++;k++;
    }
    while(j<n2)
    {
        arr[k]=R[j];
        j++;k++;
    }
}
void mergesort(vector<paranode*>& arr, int l, int r) {
    if(l<r)
    {
        int m=l+(r-l)/2;
        mergesort(arr,l,m);
        mergesort(arr,m+1,r);
        merge(arr,l,m,r);
    }
}
//5. node function to create a linked list of top k paragraphs
void nodeinsert(Node* &root,Node* &ins)
{
    if(root==nullptr)
    {
        root=ins;
        return;
    }
    Node* current=root;
    while(current->right!=nullptr)
    {
        current=current->right;
    }
    current->right=ins;
    ins->left=current;
}

QNA_tool::QNA_tool()
{
    csv=new avltree();
    corpus=new avltree();
    //cout<<"done"<<endl;
    vector<vector<string>> data=read_csv("unigram_freq.csv");
    //cout<<"seg"<<endl;
    //data.erase(data.begin());
    for(int i=0;i<data.size();i++)
    {
        //cout<<data[i][0]<<" "<<strtolong(data[i][1])<<endl;
        csv->insert_avlnode(strtolong(data[i][1]),(data[i][0]));
        //cout<<i<<endl;
    }
    //cout<<"end"<<endl;
}

QNA_tool::~QNA_tool()
{
    delete csv;
    delete corpus;
    for(int i=0;i<paragraphs.size();i++)
    {
        delete paragraphs[i];
    }
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence)
{
    //inserting to corpus 
    //cout<<"paragraphs size in insert"<<paragraphs.size()<<"  "<<size<<endl;
    //cout<<sentence<<endl;
    vector<string> words=split_sentence(sentence);
    for(int i=0;i<words.size();i++)
    {
        corpus->insert_avlnode(1,words[i]);
    }

    //inserting to paragraphs
    if((book_code==prev_book)&&(page==prev_page)&&(paragraph==prev_para))
    {
        
    }
    else
    {
        //cout<<sentence<<endl;
        //cout<<"book code "<<book_code<<"page is "<<page<<"para "<<paragraph<<endl;
        avltree* new_tree=new avltree();
        paragraphs.push_back(new_tree);
        book_vector.push_back(book_code);
        page_vector.push_back(page);
        para_vector.push_back(paragraph);
    }
    for(int i=0;i<words.size();i++)
    {
        int size_of=paragraphs.size();
        paragraphs[size_of-1]->insert_avlnode(1,words[i]);
    }
    prev_book=book_code;
    prev_page=page;
    prev_para=paragraph;
}
    

Node* QNA_tool::get_top_k_para(string question, int k)
{
    Node* head=nullptr;
    vector<paranode*> scores;
    scores.resize(paragraphs.size());
    //cout<<"paragraphs size is "<<paragraphs.size()<<" "<<scores.size()<<endl;
    // for(int i=0;i<page_vector.size();i++)
    // {
    //     cout<<"book is "<<book_vector[i]<<" page is "<<page_vector[i]<<" para_vector "<<para_vector[i]<<endl;
    // }
    /*created a vector named scores with paranodes 
    to store the scores with parameters like book,page,para number
    and initializing the pointers below*/

    for(int i=0;i<scores.size();i++)
    {
        scores[i]=new paranode();
    }
    vector<string> words=split_sentence(question);

    //calcultaing the score of each word of given question:

    for(int i=0;i<words.size();i++)
    {
        long long c1=corpus->get_word_count(words[i]);
        
        //cout<<"c1: "<<c1<<endl;
        long long c2=csv->get_word_count(words[i]);
        //cout<<"word:"<< words[i] <<" c1: "<< c1 <<" c2:" << c2 <<endl;
        long double score_of_word=(static_cast<long double>(c1+1))/(static_cast<long double>(c2+1));
        //cout<<"score of word: "<<score_of_word<<endl;

        //iterating over paragraphs to calculate score of paragraphs
        for(int j=0;j<paragraphs.size();j++)
        {
            long long word_freq=paragraphs[j]->get_word_count(words[i]);
            
            scores[j]->bookn=book_vector[j];
            scores[j]->pagen=page_vector[j];
            scores[j]->paran=para_vector[j];
            //cout<<scores[j]->bookn<<endl;cout<<book_vector[j]<<endl;
            //cout<<scores[j]->pagen<<endl;cout<<page_vector[j]<<endl;
            //cout<<scores[j]->paran<<endl;cout<<para_vector[j]<<endl;
            scores[j]->score=scores[j]->score+(word_freq*score_of_word);
            //love+=word_freq;
        }
        //cout<<"iterating over word "<<words[i]<<" "<<love<<endl;
    }
    /*vector reduced so that only non zero 
    terms are present and zero score terms are deleted*/
    vector<paranode*> final_scores;
    //cout<<"scores size is"<<paragraphs.size()<<" "<<scores.size()<<endl;
    for(int i=0;i<scores.size();i++)
    {
        if(scores[i]->score!=0)
        {
            final_scores.push_back(scores[i]);
        }
    }
    // for(int i=0;i<scores.size();i++)
    // {
    //       cout<<final_scores[i]->paran<<" ";
    // }
    //cout<<"computed final scores"<<endl;

    mergesort(final_scores,0,final_scores.size()-1);
    // for(int i=0;i<final_scores.size();i++)
    // {
    //     cout<<final_scores[i]->score<<" ";
    // }
    // cout<<"displaying final scores"<<final_scores.size()<<endl;
    // for(int i=0;i<final_scores.size();i++)
    // {
    //    cout<<final_scores[i]->bookn<<" "<<final_scores[i]->pagen<<" "<<final_scores[i]->paran<<" "<<final_scores[i]->score<<endl;
    // }

    //cout<<"done sorting"<<endl;
    // cout<<"size of paras is:"<<sizeof(paragraphs)<<endl;
    // cout<<"size of csv is:"<<sizeof(corpus)<<endl;
    // cout<<"size of corpus is:"<<sizeof(csv)<<endl;

    for(int i=0;i<k;i++)
    //for(int i=final_scores.size()-1;i>=final_scores.size())
    {
        Node* ins=new Node();
        ins->book_code=final_scores[i]->bookn;
        ins->page=final_scores[i]->pagen;
        ins->paragraph=final_scores[i]->paran;
        //cout<<"scores para"<<final_scores[i]->paran<<endl;
        //cout<<"ins para"<<ins->paragraph<<endl;
        nodeinsert(head,ins);
    }
   // cout<<"head is"<<head<<endl;
    // for(int i=0;i<scores.size();i++)
    // {
    //     delete scores[i];
    // }
    // for(int i=0;i<final_scores.size();i++)
    // {
    //     delete final_scores[i];
    // }
    return head;
}

void QNA_tool::query(string question, string filename){
    // Implement your function here 

    Node* head=get_top_k_para(question,5);
    std::cout << "Q: " << question << std::endl;
    query_llm(filename,head,5,"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",question);
    // while(head!=nullptr)
    // {
    //     string answer=get_paragraph(head->book_code,head->page,head->paragraph);
    //     cout<<answer<<endl;
    //     head=head->right;
    // }
    // cout<<endl;
    

    //std::cout << "A: " << "Studying COL106 :)" << std::endl;

    return;
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){

    //cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;
    
    std::string filename = "mahatma-gandhi/mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph)
        ){
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY, string question){

    // first write the k paragraphs into different files

    Node* traverse = root;
    int num_paragraph = 0;

    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
    outfile << question;
    outfile << "dont use data other than the above experts.if the answer doesnt lie in the above experts then dont give me answer from internet.";
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();
 
    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}