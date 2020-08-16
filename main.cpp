#include <bits/stdc++.h>
using namespace std;
struct dataset_entry {
    vector<int> items;
};
struct fp_tree_node {
    int item;
    int value;
    vector<fp_tree_node*> child;
    fp_tree_node* next_same;
    fp_tree_node* prev_node;
};
struct fp_subtree_node {
    int item;
    int value;
    int branch_length;
    vector<fp_subtree_node*> child;
    fp_subtree_node* next_node;  //next close to leaf
    fp_subtree_node* prev_node;  //prev close to root
    fp_subtree_node* merge_to;
};
struct merged_nodes_data {
    vector<fp_subtree_node*>::iterator it;
    fp_subtree_node* master;
    int node_item;
};
struct frequent_pattern {
    vector<int> seq;
    double support;
};
struct header_table_entry {
    int item;
    int freq;
    fp_tree_node* first_node;
    fp_tree_node* least_node;
};
struct freq_table_entry {
    int freq;
    header_table_entry* ht_addr;
};
int transaction_num;
class dataset {
   public:
    dataset() {
        root = new fp_tree_node;
    }
    void set_data_entry(int row, int data) {
        data_array[row].items.push_back(data);
    }

    void add_new_entry() {
        dataset_entry tmp;
        data_array.push_back(tmp);
    }

    int get_size() {  //num of entries
        return data_array.size();
    }
    void print_items() {
        vector<dataset_entry>::iterator it1;
        vector<int>::iterator it2;
        for (it1 = data_array.begin(); it1 != data_array.end(); it1++) {
            for (it2 = (*it1).items.begin(); it2 != (*it1).items.end(); it2++) {
                cout << *it2 << " ";
            }
            cout << endl;
        }
        cout << "***********" << endl;
    }
    void count_frequency(freq_table_entry* frequency_table, vector<header_table_entry>& header_table) {
        vector<dataset_entry>::iterator it1;
        vector<int>::iterator it2;
        for (it1 = data_array.begin(); it1 != data_array.end(); it1++) {
            for (it2 = (*it1).items.begin(); it2 != (*it1).items.end(); it2++) {
                // if (frequency_table[*it2].freq == 0) {
                //     header_table_entry tmp;
                //     tmp.item = *it2;
                //     tmp.first_node = NULL;
                //     tmp.least_node = NULL;
                //     header_table.push_back(tmp);
                // }
                frequency_table[*it2].freq += 1;
            }
        }
        // vector<header_table_entry>::iterator it3;
        // for (it3 = header_table.begin(); it3 != header_table.end(); it3++) {  //update frequency in header table and header table addr in freq. table
        //     (*it3).freq = frequency_table[(*it3).item].freq;
        //     frequency_table[(*it3).item].ht_addr = &(*it3);
        // }
    }
    void construct_header_table(int min_support_num, freq_table_entry* frequency_table, vector<header_table_entry>& header_table) {
        for (int i = 0; i < 1000; i++) {
            if (frequency_table[i].freq >= min_support_num) {
                header_table_entry tmp;
                tmp.item = i;
                tmp.first_node = NULL;
                tmp.least_node = NULL;
                tmp.freq = frequency_table[i].freq;
                header_table.push_back(tmp);
                // vector<header_table_entry>::iterator it;
                // it = header_table.end()-1;
                // frequency_table[i].ht_addr = &(*it);
            } else {
                frequency_table[i].ht_addr = NULL;
            }
        }
        vector<header_table_entry>::iterator it;
        for (it = header_table.begin(); it != header_table.end(); it++) {
            frequency_table[(*it).item].ht_addr = &(*it);
        }
        // for (int i = 0; i < 1000; i++) {
        //     if (frequency_table[i].ht_addr != NULL) {
        //         cout << (*(frequency_table[i].ht_addr)).item << "(" << (*(frequency_table[i].ht_addr)).freq << ")" << endl;
        //     }
        // }
        // cout << "----" << endl;
    }
    void prune_under_minsupport(int min_support_num, freq_table_entry* frequency_table) {
        vector<dataset_entry>::iterator it1;
        vector<int>::iterator it2;
        for (it1 = data_array.begin(); it1 != data_array.end(); it1++) {
            for (it2 = (*it1).items.begin(); it2 != (*it1).items.end(); it2++) {
                if (frequency_table[(*it2)].freq < min_support_num) {  //prune
                    (*it1).items.erase(it2, it2 + 1);
                    it2--;
                }
            }
            if ((*it1).items.size() == 0) {
                data_array.erase(it1, it1 + 1);
                it1--;
            } else {
                heap_sort_dataset((*it1).items, (*it1).items.size(), frequency_table);
            }
        }
    }

    void heap_sort_dataset(vector<int>& arr, int n, freq_table_entry* frequency_table) {
        // Build heap (rearrange array)
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify_dataset(arr, n, i, frequency_table);
        }
        // One by one extract an element from heap
        for (int i = n - 1; i > 0; i--) {
            // Move current root to end
            swap(arr[0], arr[i]);

            // call max heapify on the reduced heap
            heapify_dataset(arr, i, 0, frequency_table);
        }
    }
    void heapify_dataset(vector<int>& arr, int n, int i, freq_table_entry* frequency_table) {
        int largest = i;    // Initialize largest as root
        int l = 2 * i + 1;  // left = 2*i + 1
        int r = 2 * i + 2;  // right = 2*i + 2

        // If left child is larger than root
        if (l < n && frequency_table[arr[l]].freq < frequency_table[arr[largest]].freq) {
            largest = l;
        } else if (l < n && frequency_table[arr[l]].freq == frequency_table[arr[largest]].freq) {
            if (arr[l] > arr[largest]) {
                largest = l;
            }
        }
        // If right child is larger than largest so far
        if (r < n && frequency_table[arr[r]].freq < frequency_table[arr[largest]].freq) {
            largest = r;
        } else if (r < n && frequency_table[arr[r]].freq == frequency_table[arr[largest]].freq) {
            if (arr[r] > arr[largest]) {
                largest = r;
            }
        }
        // If largest is not root
        if (largest != i) {
            swap(arr[i], arr[largest]);

            // Recursively heapify the affected sub-tree
            heapify_dataset(arr, n, largest, frequency_table);
        }
    }
    void heap_sort_path(vector<int>& arr, int n) {
        // Build heap (rearrange array)
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify_path(arr, n, i);
        }
        // One by one extract an element from heap
        for (int i = n - 1; i > 0; i--) {
            // Move current root to end
            swap(arr[0], arr[i]);

            // call max heapify on the reduced heap
            heapify_path(arr, i, 0);
        }
    }
    void heapify_path(vector<int>& arr, int n, int i) {
        int largest = i;    // Initialize largest as root
        int l = 2 * i + 1;  // left = 2*i + 1
        int r = 2 * i + 2;  // right = 2*i + 2

        // If left child is larger than root
        if (l < n && arr[l] > arr[largest])
            largest = l;

        // If right child is larger than largest so far
        if (r < n && arr[r] > arr[largest])
            largest = r;

        // If largest is not root
        if (largest != i) {
            swap(arr[i], arr[largest]);

            // Recursively heapify the affected sub-tree
            heapify_path(arr, n, largest);
        }
    }
    void construct_tree(freq_table_entry* frequency_table, vector<header_table_entry>& header_table) {
        vector<dataset_entry>::iterator it1;
        vector<int>::iterator it2;
        for (it1 = data_array.begin(); it1 != data_array.end(); it1++) {
            fp_tree_node* cur_node = root;
            for (it2 = (*it1).items.begin(); it2 != (*it1).items.end(); it2++) {
                cur_node = insert_child(cur_node, *it2, frequency_table);
            }
        }
        // cout << "???" << endl;
    }
    fp_tree_node* insert_child(fp_tree_node* cur_node, int item, freq_table_entry* frequency_table) {
        if (cur_node->child.size() != 0) {
            vector<fp_tree_node*>::iterator it;
            for (it = cur_node->child.begin(); it != cur_node->child.end(); it++) {
                if ((*it)->item == item) {
                    (*it)->value++;
                    return (*it);
                }
            }
        }
        //if cur_node's child is empty or there is no such child
        fp_tree_node* tmp = new fp_tree_node;
        tmp->item = item;
        tmp->value = 1;
        tmp->next_same = NULL;
        tmp->prev_node = cur_node;
        cur_node->child.push_back(tmp);
        if (frequency_table[item].ht_addr->first_node == NULL) {  //add item's first node in the header table
            frequency_table[item].ht_addr->first_node = tmp;
            frequency_table[item].ht_addr->least_node = tmp;
        } else {
            frequency_table[item].ht_addr->least_node->next_same = tmp;
            frequency_table[item].ht_addr->least_node = tmp;
        }
        return tmp;
    }
    void print_root_child() {
        vector<fp_tree_node*>::iterator it;
        for (it = root->child.begin(); it != root->child.end(); it++) {
            cout << "item=" << (*it)->item << endl;
        }
    }
    void print_traverse_same_item_node(int item, freq_table_entry* frequency_table) {
        fp_tree_node* cur_node = frequency_table[item].ht_addr->first_node;
        while (cur_node->next_same != NULL) {
            cout << "!!!" << endl;
            cur_node = cur_node->next_same;
        }
    }
    void heap_sort_subtree_vector(vector<fp_subtree_node*>& arr, int n, int* hash_table, int item) {
        // Build heap (rearrange array)
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify_subtree_vector(arr, n, i, hash_table, item);
        }
        // One by one extract an element from heap
        for (int i = n - 1; i > 0; i--) {
            // Move current root to end
            swap(arr[0], arr[i]);

            // call max heapify on the reduced heap
            heapify_subtree_vector(arr, i, 0, hash_table, item);
        }
    }
    void heapify_subtree_vector(vector<fp_subtree_node*>& arr, int n, int i, int* hash_table, int item) {
        int largest = i;    // Initialize largest as root
        int l = 2 * i + 1;  // left = 2*i + 1
        int r = 2 * i + 2;  // right = 2*i + 2
        //subtree內的總value越小要排在越後面
        //同value的情況下item越小要排在越前面
        //leaf(suffix)要排在最後面
        // If left child is larger than root
        if (l < n && hash_table[arr[l]->item] < hash_table[arr[largest]->item]) {
            largest = l;
        } else if (l < n && hash_table[arr[l]->item] == hash_table[arr[largest]->item]) {
            if (arr[l]->item > arr[largest]->item) {
                largest = l;
            }
        }
        // If right child is larger than largest so far
        if (r < n && hash_table[arr[r]->item] < hash_table[arr[largest]->item]) {
            largest = r;
        } else if (r < n && hash_table[arr[r]->item] == hash_table[arr[largest]->item]) {
            if (arr[r]->item > arr[largest]->item) {
                largest = r;
            }
        }
        if (l < n && arr[l]->item == item) {
            largest = l;
        } else if (r < n && arr[r]->item == item) {
            largest = r;
        } else if (arr[i]->item == item) {
            largest = i;
        }
        // If largest is not root
        if (largest != i) {
            swap(arr[i], arr[largest]);

            // Recursively heapify the affected sub-tree
            heapify_subtree_vector(arr, n, largest, hash_table, item);
        }
    }
    void add_branch_to_root(fp_subtree_node* sub_tree_root, vector<vector<fp_subtree_node*>>& subtree_vector, int item) {
        for (int i = 0; i < subtree_vector.size(); i++) {
            for (int j = 0; j < subtree_vector[i].size(); j++) {
                if (subtree_vector[i][j]->item == item) {
                    subtree_vector[i][j]->next_node = NULL;
                } else {
                    if (j != subtree_vector[i].size() - 1) {
                        subtree_vector[i][j]->next_node = subtree_vector[i][j + 1];
                        subtree_vector[i][j + 1]->prev_node = subtree_vector[i][j];
                        subtree_vector[i][j]->child.push_back(subtree_vector[i][j + 1]);
                    }
                }
                subtree_vector[i][j]->branch_length = subtree_vector[i].size() - j;
            }
            sub_tree_root->child.push_back(subtree_vector[i][0]);
        }
    }
    vector<frequent_pattern>
    process_sub_tree(vector<header_table_entry>& header_table, int min_support_num, freq_table_entry* frequency_table) {
        vector<header_table_entry>::iterator it;
        vector<frequent_pattern> combination_vector;
        for (int i = header_table.size() - 1; i >= 0; i--) {  //for each item to become leaf node
            // cout << "***************" << endl;
            // cout << header_table[i].item << ":" << endl;
            int hash_table[1000] = {0};
            vector<vector<int>> path_vector;
            fp_subtree_node* sub_tree_root = new fp_subtree_node;
            vector<vector<fp_subtree_node*>> subtree_vector;
            traverse_item_subtree(header_table[i].first_node, hash_table, subtree_vector);
            for (int j = 0; j < subtree_vector.size(); j++) {
                heap_sort_subtree_vector(subtree_vector[j], subtree_vector[j].size(), hash_table, header_table[i].item);
            }
            add_branch_to_root(sub_tree_root, subtree_vector, header_table[i].item);
            merge_subtree(sub_tree_root->child);
            frequent_pattern first_pattern;
            first_pattern.seq.push_back(header_table[i].item);
            first_pattern.support = (double)header_table[i].freq / ((double)transaction_num);
            insert_combination(combination_vector, first_pattern);
            // cout << "*-1*" << endl;
            deal_with_subtree(sub_tree_root, header_table[i].first_node->item, min_support_num, combination_vector, hash_table);
        }
        return combination_vector;
    }
    void traverse_item_subtree(fp_tree_node* leaf_node, int* hash_table, vector<vector<fp_subtree_node*>>& subtree_vector) {
        if (leaf_node != NULL) {
            fp_tree_node* cur_node = leaf_node;
            // fp_subtree_node* next_subtree_node = NULL;
            // int branch_length = 0;
            vector<fp_subtree_node*> subtree_v;
            while (cur_node != root) {
                // branch_length++;
                hash_table[cur_node->item] += leaf_node->value;
                fp_subtree_node* cur_subtree_node = new fp_subtree_node;
                // fp_subtree_node* cur_subtree_node = new fp_subtree_node;
                // cur_subtree_node->item = cur_node->item;
                cur_subtree_node->item = cur_node->item;
                // cur_subtree_node->value = leaf_node->value;
                cur_subtree_node->value = leaf_node->value;
                // cur_subtree_node->next_node = next_subtree_node;
                // if (next_subtree_node != NULL) {
                //     cur_subtree_node->child.push_back(next_subtree_node);
                // }
                // cur_subtree_node->branch_length = branch_length;
                // if (next_subtree_node != NULL) {
                //     next_subtree_node->prev_node = cur_subtree_node;
                // }
                // next_subtree_node = cur_subtree_node;
                subtree_v.push_back(cur_subtree_node);
                cur_node = cur_node->prev_node;
            }

            if (!subtree_v.empty()) {
                subtree_vector.push_back(subtree_v);
            }
            // next_subtree_node->prev_node = sub_tree_root;
            // sub_tree_root->child.push_back(next_subtree_node);
            traverse_item_subtree(leaf_node->next_same, hash_table, subtree_vector);  //traverse all item leaf node
        }
        return;
    }
    void heap_sort_subtree_branch_by_item_and_length(vector<fp_subtree_node*>& arr, int n) {
        // Build heap (rearrange array)
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify_subtree_branch_by_item_and_length(arr, n, i);
        }
        // One by one extract an element from heap
        for (int i = n - 1; i > 0; i--) {
            // Move current root to end
            swap(arr[0], arr[i]);

            // call max heapify on the reduced heap
            heapify_subtree_branch_by_item_and_length(arr, i, 0);
        }
    }
    void heapify_subtree_branch_by_item_and_length(vector<fp_subtree_node*>& arr, int n, int i) {
        int largest = i;    // Initialize largest as root
        int l = 2 * i + 1;  // left = 2*i + 1
        int r = 2 * i + 2;  // right = 2*i + 2
        // If left child is larger than root
        if (l < n && arr[l]->item > arr[largest]->item) {
            largest = l;
        } else if (l < n && arr[l]->item == arr[largest]->item) {
            if (arr[l]->branch_length < arr[largest]->branch_length) {
                largest = l;
            }
        }
        // If right child is larger than largest so far
        if (r < n && arr[r]->item > arr[largest]->item) {
            largest = r;
        } else if (r < n && arr[r]->item == arr[largest]->item) {
            if (arr[r]->branch_length < arr[largest]->branch_length) {
                largest = r;
            }
        }
        // If largest is not root
        if (largest != i) {
            swap(arr[i], arr[largest]);

            // Recursively heapify the affected sub-tree
            heapify_subtree_branch_by_item_and_length(arr, n, largest);
        }
    }
    void heap_sort_subtree_branch_by_value_and_item(vector<fp_subtree_node*>& arr, int n, int item, int* hash_table) {
        // Build heap (rearrange array)
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify_subtree_branch_by_value_and_item(arr, n, i, item, hash_table);
        }
        // One by one extract an element from heap
        for (int i = n - 1; i > 0; i--) {
            // Move current root to end
            swap(arr[0], arr[i]);

            // call max heapify on the reduced heap
            heapify_subtree_branch_by_value_and_item(arr, i, 0, item, hash_table);
        }
    }
    void heapify_subtree_branch_by_value_and_item(vector<fp_subtree_node*>& arr, int n, int i, int item, int* hash_table) {
        int largest = i;    // Initialize largest as root
        int l = 2 * i + 1;  // left = 2*i + 1
        int r = 2 * i + 2;  // right = 2*i + 2
        // If left child is larger than root
        if (l < n && hash_table[arr[l]->item] < hash_table[arr[largest]->item]) {  //higher value,higher priority
            largest = l;
        } else if (l < n && hash_table[arr[l]->item] == hash_table[arr[largest]->item]) {
            if (arr[l]->item > arr[largest]->item) {  //smaller item,higher priority
                largest = l;
            }
        }
        // If right child is larger than largest so far
        if (r < n && hash_table[arr[r]->item] < hash_table[arr[largest]->item]) {
            largest = r;
        } else if (r < n && hash_table[arr[r]->item] == hash_table[arr[largest]->item]) {
            if (arr[r]->item > arr[largest]->item) {
                largest = r;
            }
        }
        if (l < n && arr[l]->item == item) {
            largest = l;
        } else if (r < n && arr[r]->item == item) {
            largest = r;
        } else if (arr[i]->item == item) {
            largest = i;
        }
        // If largest is not root
        if (largest != i) {
            swap(arr[i], arr[largest]);

            // Recursively heapify the affected sub-tree
            heapify_subtree_branch_by_value_and_item(arr, n, largest, item, hash_table);
        }
    }
    void merge(fp_subtree_node* master, fp_subtree_node* fork) {
        master->value += fork->value;
        if (master->value > 10000) {
            cout << "!!!!" << endl;
        }
        // cout << "mI:" << master->item << " mV:" << master->value << endl;
        if (fork->child.empty()) {
            delete fork;
            return;
        }
        int orig_master_child_size = master->child.size();
        for (int j = 0; j < fork->child.size(); j++) {
            bool erased = false;
            for (int i = 0; i < orig_master_child_size; i++) {
                if (fork->child[j]->item == master->child[i]->item) {
                    merge(master->child[i], fork->child[j]);
                    fork->child.erase(fork->child.begin() + j, fork->child.begin() + j + 1);
                    j--;
                    erased = true;
                    break;
                }
            }
            if (!erased) {
                fork->child[j]->prev_node = master;
                master->child.push_back(fork->child[j]);
            }
            // if (fork->next_node == NULL) {
            //     return;
            // }
            // if (master->child[i]->item == fork->next_node->item) {
            //     merge(master->child[i], fork->next_node);
            //     delete fork;
            //     return;
            // }
        }
        delete fork;
    }
    void merge_subtree(vector<fp_subtree_node*>& child) {
        heap_sort_subtree_branch_by_item_and_length(child, child.size());
        for (int i = 0; i < child.size(); i++) {  //for each path under subtree root
            if (i + 1 == child.size()) {          //it is the least item in child
                break;
            } else {
                if (child[i]->item == child[i + 1]->item) {
                    merge(child[i], child[i + 1]);
                    child.erase(child.begin() + i + 1, child.begin() + i + 2);
                    i--;
                }
            }
        }
    }
    void invertiable_merge(fp_subtree_node* master, fp_subtree_node* fork, vector<fp_subtree_node*>& deleted_nodes, vector<merged_nodes_data>& merged_nodes, int& deleted_nodes_num, int& merged_nodes_num) {
        master->value += fork->value;
        fork->merge_to = master;
        // cout << "mI:" << master->item << " mV:" << master->value << endl;
        if (fork->child.empty()) {
            deleted_nodes.push_back(fork);
            deleted_nodes_num++;
            return;
        }
        int orig_master_child_size = master->child.size();
        for (int j = 0; j < fork->child.size(); j++) {
            bool erased = false;
            for (int i = 0; i < orig_master_child_size; i++) {
                if (fork->child[j]->item == master->child[i]->item) {
                    invertiable_merge(master->child[i], fork->child[j], deleted_nodes, merged_nodes, deleted_nodes_num, merged_nodes_num);
                    // fork->child.erase(fork->child.begin() + j, fork->child.begin() + j + 1);
                    erased = true;
                    break;
                }
            }
            if (!erased) {
                master->child.push_back(fork->child[j]);
                merged_nodes_data tmp;
                tmp.it = master->child.end() - 1;
                tmp.master = master;
                tmp.node_item = fork->child[j]->item;
                merged_nodes.push_back(tmp);
                merged_nodes_num++;
            }
        }
        deleted_nodes.push_back(fork);
        deleted_nodes_num++;
        return;
    }
    int invertiable_merge_subtree(vector<fp_subtree_node*>& child, vector<fp_subtree_node*>& deleted_nodes, vector<merged_nodes_data>& merged_nodes, int& deleted_nodes_num, int& merged_nodes_num) {
        heap_sort_subtree_branch_by_item_and_length(child, child.size());

        for (int i = 0; i < child.size(); i++) {  //for each path under subtree root
            if (i + 1 == child.size()) {          //it is the least item in child
                break;
            } else {
                if (child[i]->item == child[i + 1]->item) {
                    invertiable_merge(child[i], child[i + 1], deleted_nodes, merged_nodes, deleted_nodes_num, merged_nodes_num);
                    child.erase(child.begin() + i + 1, child.begin() + i + 2);
                    i--;
                }
            }
        }
        return deleted_nodes_num;
    }
    void inverse_merge(vector<fp_subtree_node*>& deleted_nodes, int deleted_nodes_num, vector<merged_nodes_data>& merged_nodes, int merged_nodes_num) {
        // cout << "??????" << "del:" << deleted_nodes_num << " mer:" << merged_nodes_num << endl;
        // cout << "*5.11.1*" << endl;
        while (!deleted_nodes.empty()) {
            // cout << "*5.11.2*" << endl;
            fp_subtree_node* tmp = deleted_nodes.back();
            // cout << "*5.11.3*" << endl;
            // if (tmp->value > 10000) {
            //     cout << "????" << endl;
            // }
            tmp->merge_to->value -= tmp->value;
            deleted_nodes.pop_back();
            // cout << "*5.11.4*" << endl;
        }
        while (!merged_nodes.empty()) {
            // cout << "*5.11.5*" << endl;
            // cout << "inverting item:" << merged_nodes[i].node_item << "," << (*(merged_nodes[i].it))->item << endl;
            for (int j = 0; j < merged_nodes.back().master->child.size(); j++) {
                if (merged_nodes.back().master->child[j]->item == merged_nodes.back().node_item) {
                    merged_nodes.back().master->child.erase(merged_nodes.back().master->child.begin() + j, merged_nodes.back().master->child.begin() + j + 1);
                    break;
                }
            }
            merged_nodes.pop_back();
            // merged_nodes[i].master->child.erase(merged_nodes[i].it, merged_nodes[i].it + 1);
            // cout << "*5.11.6*" << endl;
        }
    }
    void process_subtree_recursion(fp_subtree_node* cur_node, int min_support_num, vector<frequent_pattern>& combination_vector, frequent_pattern& sub_combination, int item, vector<fp_subtree_node*>& deleted_nodes, vector<merged_nodes_data>& merged_nodes, int* hash_table) {
        // cout << "*5.1*" << endl;
        if (cur_node->value < min_support_num) {
            // cout << "*5.2*" << endl;
            return;
        } else {
            // cout << "*5.3*" << endl;
            vector<fp_subtree_node*> tmp_deleted_nodes;
            vector<merged_nodes_data> tmp_merged_nodes;
            sub_combination.seq.push_back(cur_node->item);
            sub_combination.seq.push_back(item);
            // cout << cur_node->item << "(" << cur_node->value << ")" << endl;
            sub_combination.support = (double)cur_node->value / ((double)transaction_num);
            // cout << "*5.4*" << endl;
            insert_combination(combination_vector, sub_combination);  //sort sub_combination and insert the combination to the correst place
            // cout << "*5.5*" << endl;
            // combination_vector.push_back(sub_combination);
            // vector<int>::iterator it3;
            // for (it3 = sub_combination.seq.begin(); it3 != sub_combination.seq.end(); it3++) {
            //     cout << (*it3) << "->";
            // }
            // cout << endl;
            // cout << "support:" << sub_combination.support << endl;
            sub_combination.seq.pop_back();
            vector<fp_subtree_node*> copy_of_child(cur_node->child);
            // for (int i = 0; i < copy_of_child.size(); i++) {
            //     cout << copy_of_child[i]->item << endl;
            // }
            int deleted_nodes_num = 0;
            int merged_nodes_num = 0;
            // cout << "*5.6*" << endl;
            while (!copy_of_child.empty()) {
                // cout << "*5.7*" << endl;
                heap_sort_subtree_branch_by_value_and_item(copy_of_child, copy_of_child.size(), item, hash_table);
                // cout << "////" << copy_of_child[0]->item << "(" << copy_of_child[0]->value << ")"
                //      << "////";
                for (int j = 0; j < copy_of_child.size(); j++) {
                    // cout << copy_of_child[j]->item << "(" << copy_of_child[j]->value<< ")"
                    //      << ",";
                }
                // cout << endl;
                // cout << "*5.8*" << endl;
                if (copy_of_child[0]->item == item) {
                    // cout << "*5.!!!*" << endl;
                    break;
                }
                // cout << "!!!!!" << endl;
                // cout << "I:" << copy_of_child[0]->item << " V:" << copy_of_child[0]->value << endl;
                // cout << endl;
                // cout << "***" << copy_of_child[0]->item << "(" << copy_of_child[0]->value << ")" << "***" << endl;
                // print_subtree(copy_of_child[0]);
                // cout << "*5.9*" << endl;
                process_subtree_recursion(copy_of_child[0], min_support_num, combination_vector, sub_combination, item, deleted_nodes, merged_nodes, hash_table);
                // cout << "*5.10*" << endl;
                vector<fp_subtree_node*>::iterator it2;
                for (it2 = copy_of_child[0]->child.begin(); it2 != copy_of_child[0]->child.end(); it2++) {
                    copy_of_child.push_back((*it2));
                }
                copy_of_child.erase(copy_of_child.begin(), copy_of_child.begin() + 1);  //pop front
                invertiable_merge_subtree(copy_of_child, tmp_deleted_nodes, tmp_merged_nodes, deleted_nodes_num, merged_nodes_num);
            }
            // cout << "*5.11*" << endl;
            inverse_merge(tmp_deleted_nodes, deleted_nodes_num, tmp_merged_nodes, merged_nodes_num);
            // cout << "*5.12*" << endl;
            sub_combination.seq.pop_back();
            // cout << "*5.13*" << endl;
        }
    }
    void print_subtree(fp_subtree_node* sub_tree_root) {
        for (int i = 0; i < sub_tree_root->child.size(); i++) {
            cout << sub_tree_root->child[i]->item << "(" << sub_tree_root->child[i]->value << ")"
                 << "->";
            print_subtree(sub_tree_root->child[i]);
            cout << endl;
        }
    }
    void deal_with_subtree(fp_subtree_node* sub_tree_root, int item, int min_support_num, vector<frequent_pattern>& combination_vector, int* hash_table) {
        // cout << "*0*" << endl;
        // cout << "item=" << item << "***********************" << endl;
        // cout << "root's child:";
        // for (int i = 0; i < sub_tree_root->child.size(); i++) {
        //     cout << sub_tree_root->child[i]->item;
        // }
        // cout << endl;
        // for (int i = 0; i < combination_vector.size(); i++) {
        //     for (int j = 0; j < combination_vector[i].seq.size() - 1; j++) {
        //         cout << combination_vector[i].seq[j] << " , ";
        //     }
        //     cout << combination_vector[i].seq[combination_vector[i].seq.size() - 1] << endl;
        // }
        while (!sub_tree_root->child.empty()) {
            // print_subtree(sub_tree_root);
            // cout << "*1*" << endl;
            heap_sort_subtree_branch_by_value_and_item(sub_tree_root->child, sub_tree_root->child.size(), item, hash_table);
            // cout << "node:" << sub_tree_root->child[0]->item << "value" << sub_tree_root->child[0]->value << endl;
            // cout << "*2*" << endl;
            if (sub_tree_root->child[0]->item == item) {
                delete sub_tree_root->child[0];
                // cout << "*4*" << endl;
                sub_tree_root->child.erase(sub_tree_root->child.begin(), sub_tree_root->child.begin() + 1);
            } else {
                //start recursion
                frequent_pattern sub_combination;
                vector<fp_subtree_node*> deleted_nodes;  //將master的value減回去
                vector<merged_nodes_data> merged_nodes;  //將master->child裡面被merge進去的刪掉
                // cout << "*5*" << endl;
                process_subtree_recursion(sub_tree_root->child[0], min_support_num, combination_vector, sub_combination, item, deleted_nodes, merged_nodes, hash_table);
                // cout << "*6*" << endl;
                // cout << "end recursion" << endl;
                // print_subtree(sub_tree_root);
                for (int i = 0; i < sub_tree_root->child[0]->child.size(); i++) {
                    sub_tree_root->child[0]->child[i]->prev_node = sub_tree_root;
                    if (sub_tree_root->child[0]->child[i]->item >= 0 && sub_tree_root->child[0]->child[i]->item < 1000) {
                        sub_tree_root->child.push_back(sub_tree_root->child[0]->child[i]);  //
                    } else {
                        cout << "!!!!" << endl;
                    }
                }
                // cout << "*7*" << endl;
                delete sub_tree_root->child[0];
                sub_tree_root->child.erase(sub_tree_root->child.begin(), sub_tree_root->child.begin() + 1);
                merge_subtree(sub_tree_root->child);
                // cout << "*8*" << endl;
            }
            // cout << "*3*" << endl;
        }
    }
    int binary_search(vector<int>& v, int item, int idx) {
        if (v[idx] > item) {
            if (idx == 0) {
                return 0;
            }
            if (v[idx - 1] < item) {
                return idx;
            }
            return binary_search(v, item, idx / 2);
        } else {
            if (idx == v.size() - 1 || v[idx + 1] > item) {
                return idx + 1;
            }
            return binary_search(v, item, (idx + idx * 2) / 2);
        }
    }
    bool compare_comb(frequent_pattern comb, frequent_pattern sub) {  //return true means sub is bigger than comb
        bool bigger = false;
        if (comb.seq.size() < sub.seq.size()) {
            bigger = true;
        } else if (comb.seq.size() > sub.seq.size()) {
            bigger = false;
        } else {
            for (int i = 0; i < comb.seq.size(); i++) {
                if (comb.seq[i] < sub.seq[i]) {
                    bigger = true;
                    break;
                } else if (comb.seq[i] > sub.seq[i]) {
                    bigger = false;
                    break;
                }
            }
        }
        return bigger;
    }
    int binary_search_comb_vector(vector<frequent_pattern>& combination_vector, frequent_pattern& sub_combination, int start_idx, int end_idx) {
        int idx = (start_idx + end_idx) / 2;
        // cout <<start_idx<<","<< end_idx << endl;
        if (!combination_vector.empty()) {
            if (compare_comb(combination_vector[idx], sub_combination)) {  //sub_combination is bigger than comb
                if (idx == combination_vector.size() - 1 || compare_comb(combination_vector[idx + 1], sub_combination) == false) {
                    return idx + 1;
                }
                return binary_search_comb_vector(combination_vector, sub_combination, idx, end_idx);
            } else {
                if (idx == 0) {
                    return 0;
                }
                if (compare_comb(combination_vector[idx - 1], sub_combination)) {
                    return idx;
                }
                return binary_search_comb_vector(combination_vector, sub_combination, start_idx, idx);
            }
        } else {
            return 0;
        }
    }
    void heap_sort_subcombination(vector<int>& arr, int n) {
        // Build heap (rearrange array)
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify_subcombination(arr, n, i);
        }
        // One by one extract an element from heap
        for (int i = n - 1; i > 0; i--) {
            // Move current root to end
            swap(arr[0], arr[i]);

            // call max heapify on the reduced heap
            heapify_subcombination(arr, i, 0);
        }
    }
    void heapify_subcombination(vector<int>& arr, int n, int i) {
        int largest = i;    // Initialize largest as root
        int l = 2 * i + 1;  // left = 2*i + 1
        int r = 2 * i + 2;  // right = 2*i + 2

        // If left child is larger than root
        if (l < n && arr[l] > arr[largest])
            largest = l;

        // If right child is larger than largest so far
        if (r < n && arr[r] > arr[largest])
            largest = r;

        // If largest is not root
        if (largest != i) {
            swap(arr[i], arr[largest]);

            // Recursively heapify the affected sub-tree
            heapify_subcombination(arr, n, largest);
        }
    }
    void insert_combination(vector<frequent_pattern>& combination_vector, frequent_pattern sub_combination) {  //add item into the comb. and add comb. into the comb_vector
        heap_sort_subcombination(sub_combination.seq, sub_combination.seq.size());
        int idx = binary_search_comb_vector(combination_vector, sub_combination, 0, combination_vector.size());
        combination_vector.insert(combination_vector.begin() + idx, sub_combination);
    }

   private:
    vector<dataset_entry> data_array;
    fp_tree_node* root;
};

void read_data(ifstream& file, dataset& transactions) {
    string line;
    int num_of_row = 0;
    while (getline(file, line)) {
        stringstream s(line);
        for (int i; s >> i;) {
            if (transactions.get_size() == num_of_row) {  //add a row
                transactions.add_new_entry();
            }
            transactions.set_data_entry(num_of_row, i);
            if (s.peek() == ',' || s.peek() == ' ') {
                s.ignore();
            }
        }
        num_of_row++;
    }
}

void init_freq_table(freq_table_entry* frequency_table) {
    for (int i = 0; i < 1000; i++) {
        frequency_table[i].freq = 0;
    }
}

void heapify_datatable(vector<header_table_entry>& arr, int n, int i, freq_table_entry* frequency_table) {
    int largest = i;    // Initialize largest as root
    int l = 2 * i + 1;  // left = 2*i + 1
    int r = 2 * i + 2;  // right = 2*i + 2

    // If left child is larger than root
    if (l < n && arr[l].freq < arr[largest].freq) {
        largest = l;
    } else if (l < n && arr[l].freq == arr[largest].freq) {
        if (arr[l].item > arr[largest].item) {
            largest = l;
        }
    }

    // If right child is larger than largest so far
    if (r < n && arr[r].freq < arr[largest].freq) {
        largest = r;
    } else if (r < n && arr[r].freq == arr[largest].freq) {
        if (arr[r].item > arr[largest].item) {
            largest = r;
        }
    }

    // If largest is not root
    if (largest != i) {
        header_table_entry* tmp;
        tmp = frequency_table[arr[i].item].ht_addr;
        frequency_table[arr[i].item].ht_addr = frequency_table[arr[largest].item].ht_addr;
        frequency_table[arr[largest].item].ht_addr = tmp;
        swap(arr[i], arr[largest]);

        // Recursively heapify the affected sub-tree
        heapify_datatable(arr, n, largest, frequency_table);
    }
}
void heap_sort_datatable(vector<header_table_entry>& arr, int n, freq_table_entry* frequency_table) {
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify_datatable(arr, n, i, frequency_table);
    }
    // One by one extract an element from heap
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end
        header_table_entry* tmp;
        tmp = frequency_table[arr[0].item].ht_addr;
        frequency_table[arr[0].item].ht_addr = frequency_table[arr[i].item].ht_addr;
        frequency_table[arr[i].item].ht_addr = tmp;
        swap(arr[0], arr[i]);

        // call max heapify on the reduced heap
        heapify_datatable(arr, i, 0, frequency_table);
    }
}
int main(int argc, char* argv[]) {
    string input_file, output_file;
    double min_support;
    freq_table_entry frequency_table[1000];
    vector<header_table_entry> header_table;

    init_freq_table(frequency_table);
    min_support = atof(argv[1]);
    input_file = argv[2];
    output_file = argv[3];
    ifstream file(input_file);
    dataset transactions;
    read_data(file, transactions);  //read data in "file" and add data into "transactions"
    int min_support_num;
    transaction_num = transactions.get_size();
    // cout << "transaction num:" << transaction_num << endl;
    min_support_num = (int)ceil((min_support * (double)transaction_num));
    // cout << "min support num=" << min_support_num << endl;
    transactions.count_frequency(frequency_table, header_table);
    transactions.construct_header_table(min_support_num, frequency_table, header_table);
    heap_sort_datatable(header_table, header_table.size(), frequency_table);
    transactions.prune_under_minsupport(min_support_num, frequency_table);
    // transactions.print_items();
    transactions.construct_tree(frequency_table, header_table);
    // transactions.print_root_child();
    // transactions.print_traverse_same_item_node(7, frequency_table);
    vector<frequent_pattern> combination_vector;
    // cout << "!!!";
    combination_vector = transactions.process_sub_tree(header_table, min_support_num, frequency_table);
    setlocale(LC_ALL, "en_US.UTF-8");
    FILE* f = fopen(output_file.c_str(), "w");
    if (f) {
        for (int i = 0; i < combination_vector.size(); i++) {
            for (int j = 0; j < combination_vector[i].seq.size() - 1; j++) {
                fprintf(f, "%d,", combination_vector[i].seq[j]);
            }
            fprintf(f, "%d:", combination_vector[i].seq[combination_vector[i].seq.size() - 1]);
            // cout << combination_vector[i].support << endl;
            fprintf(f, "%.4f\n", combination_vector[i].support);
        }
        fclose(f);
    }
}
/*
TODO:484行 invert merge
*/
