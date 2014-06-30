#pragma once
#include <Eigen/dense>
#include <string>
#include <vector>
#include "Node.h"

using namespace Eigen;
using std::string;
using std::vector;

class AutoRange
{
public:
	AutoRange(void);
	~AutoRange(void);

	// File IO
	bool ReadFromFile(const char* _filename);
	bool ReadNetworkFromFile(const char* _filename);
	bool WriteNetworkToFile(const char* _filename);
	bool WriteRangeToFile(const char* _filename);
	bool ReadConfiguration(const char* _filename);

	// Operate
	void CreateExamples(const char* _filename);
	void CreateNetwork();
	void CreateAcouNetwork();
	void CreateVeliNetwork();
	void CreateExtraLines();
	void CreateLayers();
	void CreateRange();
	void CreateApetureTime();
	void Init();
	int CalculateLineDegree(int lineindex);
	int CalculateLineDegree(int lineindex1, int lineindex2);
	int CalculateInLineDegree(int inlineindex1, int inlineindex2);
	double CalculateApeturesize(int index1, int index2);
	
	void CalculateDelay();
	bool IsRelated(int _index, list<int>& _layer);
	void InitDistance();

	// Check functions
	void CheckConflict();
	bool CheckConflict(double _timeopen1, double _timeclose1, double _timeopen2, double _timeclose2);

	// Test functions
	void TestDelay();
	void TestLayers();
	void TestRange();
	void TestApeture();


private:
	int SearchPriorNode();
	void ExtractLines();

public:
	MatrixXi matrix_range_net_;
	MatrixXd matrix_position_;
	MatrixXd matrix_distance_;
	MatrixXd matrix_topo_distance_;
	MatrixXi matrix_index_;
	MatrixXd matrix_delay_;
	MatrixXd matrix_timeflight_;
	VectorXi vector_type_;

	vector<Node> nodes_;
	vector<int> nodes_acoustic_;
	vector<int> nodes_velo_;

	vector<list<int>> layers_;
	MatrixXd matrix_apeturesize_;

	int num_max_lines_;
	int num_nodes_;
	int distance_inline_;
	int distance_adjacent_;
	int distance_cross_;

	double position_center_x_;
	double position_center_y_;

	string filename_;
};
