//
//  Tree.h
//  huffman
//
//  Created by 陈亮 on 2019/6/3.
//  Copyright © 2019 陈亮. All rights reserved.
//

#ifndef Tree_h
#define Tree_h


typedef struct node{
    node * left;
    node * right;
    char a;
    int quote;
}Node;


bool compare(Node *a,Node*b){
    return a->quote<=b->quote;
}









#endif /* Tree_h */
