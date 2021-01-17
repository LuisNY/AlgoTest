#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <queue>

struct Node 
{
    int val;
    Node *left;
    Node *right;
    Node(int x) : val(x), left(nullptr), right(nullptr) 
    {
    }
};

//level order traversal of the tree to print nodes level by level
void printNodes(Node* root)
{
    if (root == NULL) 
        return;  
  
    std::queue<Node *> q;    
    q.push(root);  
  
    while (q.empty() == false)  
    {  
        int count = q.size();  
  
        while (count > 0) 
        {  
            Node* node = q.front();  
            std::cout << node->val << " ";  
            q.pop();  
            if (node->left != NULL)  
                q.push(node->left);  
            if (node->right != NULL)  
                q.push(node->right);  
            count--;  
        }  
    }  
    std::cout << std::endl;
}  

//function to build the tree and decide if tree is good or not on the fly
bool buildTree(Node** root, Node* parent, Node* curr, std::vector<std::vector<std::pair<int,bool>>>& vect, int level, int occurrance, int index, bool isRightChild)
{
    if (level >= vect.size()) //reached the leaf, go no further
        return true;

    while (isRightChild && vect[occurrance][index].second) //pick next number of current occurrence that has not been used yet
        ++index;

    if (isRightChild && parent && parent->val >= vect[occurrance][index].first) //parent node is greater than current node, cannot build the tree
        return false;

    curr = new Node(vect[occurrance][index].first);

    if (!parent) 
        *root = curr; //set root if this is the first node
    else if (isRightChild) 
        parent->right = curr;    
    else
        parent->left = curr;
           
    vect[occurrance][index].second = true; //mark current element as already seen

    if (!buildTree(root, curr, curr->left, vect, level+1, occurrance, index, false))
        return false;
        
    if (!buildTree(root, curr, curr->right, vect, level+1, level+1, index, true))
        return false;

    return true;    
}

//function to make sure that each number in the input array appears the right number of times to build the tree
bool validateHelperContainer(const std::vector<std::vector<std::pair<int,bool>>>& helperVect)
{
    int i=0; 
    
    if(helperVect[0].size()!=1)
          return false;
      
    int exp=0;
    int level=1;
    while(level<helperVect.size())
    {
        if(helperVect[level].size() != pow(2,exp))
            return false;
        
        ++exp;
        ++level;
    }

    return true;
}

//function to print out the helper container... only needed to debug
void printHelperContainer(const std::vector<std::vector<std::pair<int,bool>>>& helperVect)
{
    for(int i=0; i<helperVect.size(); ++i)
    {
        for(int j=0; j<helperVect[i].size(); ++j)
        {
            std::cout << helperVect[i][j].first << " " << helperVect[i][j].second << "; ";
        }
        std::cout << std::endl;
    }
}

//function that takes the input array and attempts to build the tree if possible
bool analizeVector(std::vector<int> vect, Node** root)
{
    std::map<int, int> myMap; //use map to keep track of how many distinct elements in the input array and how many times each of them repeats

    for(auto iter = vect.begin(); iter!=vect.end(); ++iter)
    {
        if(myMap.find(*iter) == myMap.end())
            myMap.insert(std::make_pair(*iter, 1));
        else
            ++myMap[*iter];
    }

    if(myMap.size() != (vect.size()+1)/2) //number of distinct elements is not correct, cannot build tree
        return false;

    int n = log2(vect.size())+1;
    
    std::vector<std::vector<std::pair<int,bool>>> helperCont; //helper container that keeps track of elements appearance    
    for(int i=0; i<n; ++i)
        helperCont.push_back(std::vector<std::pair<int,bool>>()); //n levels total
    
    for(auto elem = myMap.begin(); elem != myMap.end(); ++elem)
        helperCont[n-(elem->second)].push_back(std::make_pair(elem->first,false)); //populate helper container with elements in the map

    //printHelperContainer(helperCont);
    
    if(!validateHelperContainer(helperCont)) // make sure each number appear the right number of time to build the tree
        return false;

    Node* curr = nullptr;
    Node* parent = nullptr;

    return buildTree(root, parent, curr, helperCont, 0, 0, 0, false);
}

int main()
{
    std::vector<int> inVect = {13,11,1,1,2,2,2,3,3,5,6,11,12,1,1};

    //{1,1,1,1,2,2,2,3,3,8,4,5,6,7,7};//{2,2,1,1,3,1,4};//{1,1,1,7,2,11,12,13,3,3,14,2,35,7,1,1,5,4,10,24,4,4,20,5,6,6,4,20,7,9,3};//{1,1,1,1,2,2,2,8,3,4,4,5,6,7,8};//{1,1,2,2,3,2,4};//{1,1,1,1,2,2,2,8,3,4,4,5,6,7,8};
    //{1,1,1,2,2,11,12,13,3,3,14,15,2,2,1,1,5,8,10,16,4,4,7,5,6,6,4,7,8,9,3};
    //{1,1,1,1,10,10,10,3,3,11,11,5,6,12,13};//{1,1,1,2,2,11,12,13,3,3,14,15,2,2,1,1,5,8,10,16,4,4,7,5,6,6,4,7,8,9,3};//,1,2,3,4,5,6,7,8};
    
    Node* root = nullptr;

    bool res = analizeVector(inVect, &root);

    if (res)
        printNodes(root);
    else 
        std::cout << "Impossible" << std::endl;

    return 0;
}
