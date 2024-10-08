#include "Node.h"

Node::Node() {
    left=nullptr;
    right=nullptr;
}

Node::Node(int b_code, int pg, int para, int s_no, int off){
    left=nullptr;
    right=nullptr;
    book_code = b_code;
    page = pg;
    paragraph = para;
    sentence_no = s_no;
    offset = off;
}