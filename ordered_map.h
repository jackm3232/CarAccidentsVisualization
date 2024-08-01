#include <iostream>
#include <vector>

//Node in Red Black tree that holds a key-value pair and is either red or black
//Keep track of a node's parent in order to make rotations and violation fixes more straightforward
template <typename Key, typename Value>
class TreeNode {
public:
    Key key;
    Value value;
    std::string color = "red";
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    TreeNode* parent = nullptr;

    TreeNode(Key key, Value value) {
        this->key = key;
        this->value = value;
    }
};

//Ordered map is backed by a red black tree that holds nodes, each containing a key-value pair
template <typename Key, typename Value>
class OrderedMap {
    TreeNode<Key, Value>* root_entire_tree = nullptr;

public:
    //Overload [] operator to allow map to work just like an STL ordered map
    //The value associated with the key is updated/created depending on what we set [] call equal to: "map[key] = value"
    Value& operator[](const Key &key) {
        TreeNode<Key, Value>* result_of_search;
        root_entire_tree = search_for_key(root_entire_tree, key, result_of_search);

        //If key was not found, insert the value specified along with its key into the tree
        if (result_of_search == nullptr) {
            insert(key, Value());
            TreeNode<Key, Value>* newly_inserted_node;
            root_entire_tree = search_for_key(root_entire_tree, key, newly_inserted_node);
            return newly_inserted_node->value;
        }
        return result_of_search->value;
    }

    //Do an inorder traversal of the tree in ascending order of the keys
    void traverse(std::vector<Key> &key_vect, std::vector<Value> &value_vect) {
        traverse_helper(root_entire_tree, key_vect, value_vect);
    }

    //Destructor that deletes all dynamically allocated nodes when the tree falls out of scope
    ~OrderedMap() {
        delete_nodes(root_entire_tree);
        root_entire_tree = nullptr;
    }

private:
    //Insert specified key-value pair into the tree and then fix the potential red black tree violations that arise
    void insert(const Key &key, const Value &value) {
        TreeNode<Key, Value>* newly_inserted_node;
        root_entire_tree = actual_insert(root_entire_tree, key, value, newly_inserted_node);
        fix_potential_violations(newly_inserted_node);
    }

    //Insert specified key-value pair into the tree, update the newly inserted node to the node created
    TreeNode<Key, Value>* actual_insert(TreeNode<Key, Value>* sub_root, const Key &key_to_insert,
                                        const Value &value_to_insert, TreeNode<Key, Value>* &newly_inserted_node) {
        //We have reached the bottom of the tree, so create and insert new node
        if (sub_root == nullptr) {
            newly_inserted_node = new TreeNode<Key, Value>(key_to_insert, value_to_insert);
            return newly_inserted_node;
        }
        else if (key_to_insert < sub_root->key) {
            sub_root->left = actual_insert(sub_root->left, key_to_insert, value_to_insert, newly_inserted_node);
            sub_root->left->parent = sub_root;
        }
        else if (key_to_insert > sub_root->key) {
            sub_root->right = actual_insert(sub_root->right, key_to_insert, value_to_insert, newly_inserted_node);
            sub_root->right->parent = sub_root;
        }
        return sub_root;
    }

    //Search for a node with the passed in key
    //If the node is found, update the result node; if node is not found, update the result node to nullptr
    TreeNode<Key, Value>* search_for_key(TreeNode<Key, Value>* sub_root, const Key &key_to_search,
                                         TreeNode<Key, Value>* &result_of_search) {
        if (sub_root == nullptr) {
            result_of_search = nullptr;
            return sub_root;
        }
        else if (key_to_search < sub_root->key) {
            sub_root->left = search_for_key(sub_root->left, key_to_search, result_of_search);
        }
        else if (key_to_search > sub_root->key) {
            sub_root->right = search_for_key(sub_root->right, key_to_search, result_of_search);
        }
        //If node with the searched key is found
        else {
            result_of_search = sub_root;
            return sub_root;
        }
        return sub_root;
    }

    //Do an inorder traversal of the tree, storing the keys and values within the passed in vectors
    void traverse_helper(TreeNode<Key, Value>* sub_root, std::vector<Key> &key_vect, std::vector<Value> &value_vect) {
        if (sub_root == nullptr) {
            return;
        }
        traverse_helper(sub_root->left, key_vect, value_vect);
        key_vect.push_back(sub_root->key);
        value_vect.push_back(sub_root->value);
        traverse_helper(sub_root->right, key_vect, value_vect);
    }

    //Code for performing rotations inspired by:
    //https://www.geeksforgeeks.org/introduction-to-red-black-tree/?ref=lbp#4-rotation
    //Rotations are performed when a newly inserted node's uncle is black

    void left_rotate(TreeNode<Key, Value>* &rotation_origin_node) {
        TreeNode<Key, Value>* origin_right_node = rotation_origin_node->right;

        //If rotation origin node was the root of the whole tree
        if (rotation_origin_node->parent == nullptr) {
            root_entire_tree = origin_right_node;
        }
        //If rotation origin node was the left child of its parent
        else if (rotation_origin_node == rotation_origin_node->parent->left) {
            rotation_origin_node->parent->left = origin_right_node;
        }
        //If rotation origin node was the right child of its parent
        else {
            rotation_origin_node->parent->right = origin_right_node;
        }
        origin_right_node->parent = rotation_origin_node->parent;

        rotation_origin_node->right = origin_right_node->left;
        if (rotation_origin_node->right != nullptr) {
            rotation_origin_node->right->parent = rotation_origin_node;
        }

        origin_right_node->left = rotation_origin_node;
        rotation_origin_node->parent = origin_right_node;
    }

    void right_rotate(TreeNode<Key, Value>* &rotation_origin_node) {
        TreeNode<Key, Value>* origin_left_node = rotation_origin_node->left;

        if (rotation_origin_node->parent == nullptr) {
            root_entire_tree = origin_left_node;
        }
        else if (rotation_origin_node == rotation_origin_node->parent->left) {
            rotation_origin_node->parent->left = origin_left_node;
        }
        else {
            rotation_origin_node->parent->right = origin_left_node;
        }
        origin_left_node->parent = rotation_origin_node->parent;

        rotation_origin_node->left = origin_left_node->right;
        if (rotation_origin_node->left != nullptr) {
            rotation_origin_node->left->parent = rotation_origin_node;
        }

        origin_left_node->right = rotation_origin_node;
        rotation_origin_node->parent = origin_left_node;
    }

    //Code for fixing red black tree violations inspired by:
    //https://www.geeksforgeeks.org/introduction-to-red-black-tree/?ref=lbp#implementation-of-redblack-tree

    //Fix the potential red black tree structure violations that arise when a new node is inserted
    void fix_potential_violations(TreeNode<Key, Value>* &curr_child_node) {

        //Continuously fix violations and propagate upwards through the tree until we get two non-red nodes in a row
        //We could potentially go all the way up to the root of the entire tree
        while (curr_child_node != root_entire_tree and curr_child_node->color == "red"
               and curr_child_node->parent->color == "red") {

            TreeNode<Key, Value>* parent_curr_child = curr_child_node->parent;
            TreeNode<Key, Value>* grandparent_curr_child = curr_child_node->parent->parent;

            //If uncle of the current node is the right child of the current node's grandparent
            if (parent_curr_child == grandparent_curr_child->left) {
                TreeNode<Key, Value>* uncle_curr_child = grandparent_curr_child->right;

                //If the uncle is nullptr (black) or exists but is black, perform the appropriate rotations/re-colorings
                if (uncle_curr_child == nullptr or uncle_curr_child->color == "black") {
                    //If there is a right right imbalance, fix it with a left rotation
                    if (curr_child_node == parent_curr_child->right) {
                        left_rotate(parent_curr_child);
                        curr_child_node = parent_curr_child;
                        parent_curr_child = curr_child_node->parent;
                    }

                    right_rotate(grandparent_curr_child);
                    //Swap colors of the parent and grandparent nodes of the current child to account for right rotation
                    std::string temp_parent_color = parent_curr_child->color;
                    parent_curr_child->color = grandparent_curr_child->color;
                    grandparent_curr_child->color = temp_parent_color;
                    curr_child_node = parent_curr_child;
                }
                //If the uncle is red, we just have to recolor the uncle, parent, and grandparent nodes
                else {
                    uncle_curr_child->color = "black";
                    parent_curr_child->color = "black";
                    grandparent_curr_child->color = "red";
                    curr_child_node = grandparent_curr_child;
                }
            }

            //The uncle of the current node is the left child of the current node's grandparent
            else {
                TreeNode<Key, Value>* uncle_curr_child = grandparent_curr_child->left;

                //Rotations and re-colorings are needed if uncle is black (or nullptr)
                if (uncle_curr_child == nullptr or uncle_curr_child->color == "black") {
                    if (curr_child_node == parent_curr_child->left) {
                        right_rotate(parent_curr_child);
                        curr_child_node = parent_curr_child;
                        parent_curr_child = curr_child_node->parent;
                    }

                    left_rotate(grandparent_curr_child);
                    std::string temp_parent_color = parent_curr_child->color;
                    parent_curr_child->color = grandparent_curr_child->color;
                    grandparent_curr_child->color = temp_parent_color;
                    curr_child_node = parent_curr_child;
                }
                //Only re-coloring is needed if uncle is red
                else {
                    uncle_curr_child->color = "black";
                    parent_curr_child->color = "black";
                    grandparent_curr_child->color = "red";
                    curr_child_node = grandparent_curr_child;
                }
            }
        }
        //Root node of the entire tree must always be black
        root_entire_tree->color = "black";
    }

    //Deletes all nodes in the tree
    void delete_nodes(TreeNode<Key, Value>* sub_root) {
        if (sub_root != nullptr) {
            delete_nodes(sub_root->left);
            delete_nodes(sub_root->right);
            delete sub_root;
        }
    }
};
