#include <iostream>
using namespace std;

template <typename T>
class TreeNode
{
public:
    T data;
    TreeNode *left;
    TreeNode *right;

    TreeNode(T value)
    {
        data = value;
        left = nullptr;
        right = nullptr;
    }
};

template <typename T>
class Tree
{
private:
    TreeNode<T> *root;

    TreeNode<T> *insert(TreeNode<T> *node, T value)
    {
        if (node == nullptr)
        {
            return new TreeNode<T>(value);
        }
        if (value < node->data)
        {
            node->left = insert(node->left, value);
        }
        else
        {
            node->right = insert(node->right, value);
        }
        return node;
    }

    int getSize(TreeNode<T> *node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        return 1 + getSize(node->left) + getSize(node->right);
    }

    void inorder(TreeNode<T> *node)
    {
        if (node != nullptr)
        {
            inorder(node->left);
            cout << node->data << " ";
            inorder(node->right);
        }
    }

    void preorder(TreeNode<T> *node)
    {
        if (node != nullptr)
        {
            cout << node->data << " ";
            preorder(node->left);
            preorder(node->right);
        }
    }

    void postorder(TreeNode<T> *node)
    {
        if (node != nullptr)
        {
            postorder(node->left);
            postorder(node->right);
            cout << node->data << " ";
        }
    }

    int getDepth(TreeNode<T> *node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        int leftDepth = getDepth(node->left);
        int rightDepth = getDepth(node->right);
        return 1 + max(leftDepth, rightDepth);
    }

    TreeNode<T> *getMin(TreeNode<T> *node)
    {
        if (node == nullptr || node->left == nullptr)
        {
            return node;
        }
        return getMin(node->left);
    }

    TreeNode<T> *getMax(TreeNode<T> *node)
    {
        if (node == nullptr || node->right == nullptr)
        {
            return node;
        }
        return getMax(node->right);
    }

    TreeNode<T> *deleteNode(TreeNode<T> *node, T value)
    {
        if (node == nullptr)
        {
            return node;
        }

        if (value < node->data)
        {
            node->left = deleteNode(node->left, value);
        }
        else if (value > node->data)
        {
            node->right = deleteNode(node->right, value);
        }
        else
        {
            if (node->left == nullptr)
            {
                TreeNode<T> *temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr)
            {
                TreeNode<T> *temp = node->left;
                delete node;
                return temp;
            }

            TreeNode<T> *temp = getMin(node->right);
            node->data = temp->data;
            node->right = deleteNode(node->right, temp->data);
        }

        return node;
    }

public:
    Tree()
    {
        root = nullptr;
    }

    void insert(T value)
    {
        root = insert(root, value);
    }

    void deleteNode(T value)
    {
        root = deleteNode(root, value);
    }

    int getSize()
    {
        return getSize(root);
    }

    void printInorder()
    {
        inorder(root);
        cout << endl;
    }

    void printSorted()
    {
        inorder(root);
        cout << endl;
    }

    void printPreorder()
    {
        preorder(root);
        cout << endl;
    }

    void printPostorder()
    {
        postorder(root);
        cout << endl;
    }

    int getDepth()
    {
        return getDepth(root);
    }

    T getMin()
    {
        TreeNode<T> *minNode = getMin(root);
        if (minNode != nullptr)
        {
            return minNode->data;
        }
        throw runtime_error("Tree is empty");
    }

    T getMax()
    {
        TreeNode<T> *maxNode = getMax(root);
        if (maxNode != nullptr)
        {
            return maxNode->data;
        }
        throw runtime_error("Tree is empty");
    }

    ~Tree()
    {
        destroyTree(root);
    }

private:
    void destroyTree(TreeNode<T> *node)
    {
        if (node != nullptr)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
};