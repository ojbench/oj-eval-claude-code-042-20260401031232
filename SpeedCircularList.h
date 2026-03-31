#ifndef SPEEDCIRCULARLIST_H
#define SPEEDCIRCULARLIST_H
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

constexpr int s_prime = 31;

inline int log2(int x) {
	int ans = 0;
	while (x != 1) {
		x /= 2;
		++ans;
	}
	return ans;
}

template<typename T, int b_prime>
class SpeedCircularLinkedList {
public:
    struct Node {
    	// 你可以为 Node 结构体添加更多的成员变量，但是不能修改已有的成员变量
    	std::map<std::string, T> kv_map;
        int bound = 0;
        Node* next = nullptr;
        Node** fast_search_list = nullptr;

    	Node(int bound, int fast_search_list_size) {
            this->bound = bound;
            if (fast_search_list_size > 0) {
                fast_search_list = new Node*[fast_search_list_size];
                for (int i = 0; i < fast_search_list_size; ++i) {
                    fast_search_list[i] = nullptr;
                }
            }
        }

    	~Node() {
            if (fast_search_list != nullptr) {
                delete[] fast_search_list;
            }
        }
    };

private:
	// 你不能添加更多的成员变量
	Node* head = nullptr;
	int fast_search_list_size = 0;
	int list_size = 0;

	/* 这是将字符串映射到整数的哈希函数，用于决定键值对的存储位置。
	 * 你无需关注这个函数的实现细节，只需要在 put 和 get 函数调用它即可。
	 */
	static int GetHashCode(std::string str) {
		long long ans = 0;
		for (auto& ch : str) {
			ans = (ans * s_prime + ch) % b_prime;
		}
		return static_cast<int>((ans + b_prime) % b_prime);
	}


	/* TODO : Build your fast search list.
	 * 题面描述中给出了一种快速查找表的构建方式，你也可以按照自己的方式构造快速查找表。
	 * 我们只会对快速查找的正确性和时间性能进行测试，不会测试你的具体实现。
	 * 如果你暂时没有实现快速查找功能，请留空这个函数。
	 */
	void BuildFastSearchList() {
        if (fast_search_list_size <= 0 || list_size <= 0) {
            return;
        }

        // For each node, build its fast_search_list
        Node* curr = head;
        for (int i = 0; i < list_size; ++i) {
            // For the k-th entry (0-based), store the 2^k-th successor
            Node* successor = curr;
            for (int k = 0; k < fast_search_list_size; ++k) {
                int jump_distance = (1 << k); // 2^k
                // Jump to 2^k-th successor
                for (int j = 0; j < jump_distance; ++j) {
                    successor = successor->next;
                }
                curr->fast_search_list[k] = successor;
            }
            curr = curr->next;
        }
    }

public:

	/* TODO : Constructor
	 * 在这里实现你的加速循环链表类的构造函数。
	 * node_bounds 存储了加速循环链表各节点的存储上界。
	 * 题目保证 node_bounds 严格升序，并且其最后一项等于 b_prime。
	 * 构造函数的最后一行应当是 BuildFastSearchList()，如果没有实现可以将其注释掉。
	 */
    explicit SpeedCircularLinkedList(std::vector<int> node_bounds) {
        list_size = node_bounds.size();

        // Calculate fast_search_list_size
        if (list_size > 1) {
            fast_search_list_size = log2(list_size);
        } else {
            fast_search_list_size = 0;
        }

        // Create nodes
        if (list_size == 0) {
            return;
        }

        head = new Node(node_bounds[0], fast_search_list_size);
        Node* prev = head;

        for (int i = 1; i < list_size; ++i) {
            Node* new_node = new Node(node_bounds[i], fast_search_list_size);
            prev->next = new_node;
            prev = new_node;
        }

        // Make it circular
        prev->next = head;

    	BuildFastSearchList();
    }

	/* TODO : Destructor
	 * 在这里实现你的析构函数
	 */
    ~SpeedCircularLinkedList() {
        if (head == nullptr) {
            return;
        }

        // Break the circular link first
        Node* curr = head;
        for (int i = 0; i < list_size - 1; ++i) {
            curr = curr->next;
        }
        curr->next = nullptr; // Break the circular link

        // Delete all nodes
        curr = head;
        while (curr != nullptr) {
            Node* next_node = curr->next;
            delete curr;
            curr = next_node;
        }

        head = nullptr;
    }

	/* TODO : Put key-value pair
	 * 函数功能：插入一个键值对，其中 str 代表键，value 代表值
	 * 在函数的开始，需要通过调用 GetHashCode() 来获取 str 映射到的整数 code
	 * 根据 code 和各个节点 bound 的关系找到存储该键值对的节点
	 */
    void put(std::string str, T value) {
	    int code = GetHashCode(str);
    	// 请不要修改上面这行代码

    	if (head == nullptr) {
            return;
        }

        // If code <= head->bound, store in head
        if (code <= head->bound) {
            head->kv_map[str] = value;
            return;
        }

        // Find the correct node for this code
        Node* curr = head;
        for (int i = 0; i < list_size; ++i) {
            if (code <= curr->bound) {
                curr->kv_map[str] = value;
                return;
            }
            curr = curr->next;
        }
    }

	/* TODO : Get key-value pair
	 * 函数功能：查询 str 对应的 value
	 * 在函数的开始，需要通过调用 GetHashCode() 来获取 str 映射到的整数 code
	 */
	T get(std::string str) {
    	int code = GetHashCode(str);
    	// 请不要修改上面这行代码

    	if (head == nullptr) {
            return T();
        }

        // If code <= head->bound, search in head
        if (code <= head->bound) {
            auto it = head->kv_map.find(str);
            if (it != head->kv_map.end()) {
                return it->second;
            }
            return T();
        }

        // Find the correct node for this code
        Node* curr = head;
        for (int i = 0; i < list_size; ++i) {
            if (code <= curr->bound) {
                auto it = curr->kv_map.find(str);
                if (it != curr->kv_map.end()) {
                    return it->second;
                }
                return T();
            }
            curr = curr->next;
        }

    	return T(); // 返回T类型默认构造函数产生的值
    }

    /* TODO : Print SpeedCircularList
     * 遍历链表，按照 bound 从小到大输出值
     * 单行输出格式：[Node] Bound = {bound}, kv_map_size = {size}
     * 单行输出示例：[Node] Bound = 104, kv_map_size = 1
     * 行与行之间以及最后一行的结尾都需要输出换行符
     */
    void print() {
        if (head == nullptr) {
            return;
        }

        Node* curr = head;
        for (int i = 0; i < list_size; ++i) {
            std::cout << "[Node] Bound = " << curr->bound
                      << ", kv_map_size = " << curr->kv_map.size() << std::endl;
            curr = curr->next;
        }
    }

    /* TODO Size Method
     * 获取链表大小
     */
    int size() const {
        return list_size;
    }
};
#endif //SPEEDCIRCULARLIST_H
