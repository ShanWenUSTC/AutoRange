#pragma once
#include <list>
#include <vector>

using std::vector;
using std::list;

enum typebird {kAcoustic, kVelocimeter, kNone} ;
class AutoRange;
class Node
{
public:
	Node(void);
	~Node(void);

	// Operate
	bool Compare(int _index1, int _index2);
	void ReduceOneLine();
	void AddLines();
	void InitCandidate(AutoRange* _ptr_autorange);
	bool operator>(const Node& _node_right);
	void ReduceNeighbor(int neiindex);
	void CalculateDelay();

	typebird type_;
	double position_x_;
	double position_y_;
	int index_;
	int index_line_;
	int index_inline_;
	list<int> candidate_nodes_index_;
	list<int> candidate_nodes_backup_;
	vector<double> distances_;
	vector<double> distances_topo_;
	double apeture_size_;
	double launch_time_;

	vector<int> range_index_;
	vector<int> apeture_index_;
	vector<double> apeturetime_open_;
	vector<double> apeturetime_close_;

	AutoRange* ptr_autorange_;
};

