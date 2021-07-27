void downHeap(Node* n)
{
    Node* tmp = nullptr;

    if(!n) {return;}

    downHeap(n->left);
    downHeap(n->right);

    if(!n->left && !n->right) {return;}

    if(n->left && n->right)
    {
        if (n->left->value < n->right->value)
            tmp = n->left;
        else
            tmp = n->right;
    }
    else
    {
        if (n->left)
            tmp = n->left;
        if (n->right)
            tmp = n->right;
    }

    if (tmp->value < n->value)
    {
        std::swap(n->value, tmp->value);
        downHeap(tmp);
    }
}
