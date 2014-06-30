#include "stdafx.h"
#include "AutoRange.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>

using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;
using std::ostringstream;


AutoRange::AutoRange(void)
{
}


AutoRange::~AutoRange(void)
{
}


// File IO
bool AutoRange::ReadFromFile(const char* _filename)
{
	ifstream fin(_filename);

	if (!fin)
	{
		cout << "Failed to open "<<_filename<<"!\n";
		return false;
	}

	fin >> num_nodes_;
	cout << "num: " << num_nodes_ << endl;

	matrix_position_.resize(num_nodes_, 2);
	for (int i = 0; i < num_nodes_; i++)
	{
		fin >> matrix_position_(i, 0) >> matrix_position_(i, 1);
//		cout << matrix_position_(i, 0) <<' '<< matrix_position_(i, 1) << endl;
	}

	matrix_index_.resize(num_nodes_, 2);
	for (int i = 0; i < num_nodes_; i++)
	{
		fin >> matrix_index_(i, 0) >> matrix_index_(i, 1);
//		cout << matrix_index_(i, 0) <<' '<< matrix_index_(i, 1)<<endl;
	}

	vector_type_.resize(num_nodes_);
	for (int i = 0; i < num_nodes_; i++)
	{
		fin >> vector_type_(i);
//		cout << vector_type_(i) << endl;
	}
	fin.close();

	Init();
	return true;
}


bool AutoRange::ReadNetworkFromFile(const char* _filename)
{
	ifstream fin(_filename);

	if (!fin)
	{
		cout << "Failed to open "<<_filename<<"!\n";
		return false;
	}

	fin >> num_nodes_;
	cout << "num: " << num_nodes_ << endl;

	matrix_range_net_.resize(num_nodes_, num_nodes_);
	for (int i=0; i<num_nodes_; i++)
	{
		for (int j=0; j<num_nodes_; j++)
		{
			fin >> matrix_range_net_(i, j);
		}
	}

// 	for (int i=0; i<num_nodes_; i++)
// 	{
// 		for (int j=0; j<num_nodes_; j++)
// 		{
// 			cout << matrix_range_net_(i, j) << ' ';
// 		}
// 		cout<<endl;
// 	}

	matrix_position_.resize(num_nodes_, 2);
	for (int i = 0; i < num_nodes_; i++)
	{
		fin >> matrix_position_(i, 0) >> matrix_position_(i, 1);
		cout << matrix_position_(i, 0) <<' '<< matrix_position_(i, 1) << endl;
	}

	matrix_index_.resize(num_nodes_, 2);
	for (int i = 0; i < num_nodes_; i++)
	{
		fin >> matrix_index_(i, 0) >> matrix_index_(i, 1);
		cout << matrix_index_(i, 0) <<' '<< matrix_index_(i, 1)<<endl;
	}

	vector_type_.resize(num_nodes_);
	for (int i = 0; i < num_nodes_; i++)
	{
		fin >> vector_type_(i);
		cout << vector_type_(i) << endl;
	}

	fin.close();

	//Init();
	InitDistance();
	return true;
}


bool AutoRange::WriteNetworkToFile(const char* _filename)
{
	ofstream fout(_filename);

	if (!fout)
	{
		cout << "Failed to open "<<_filename<<"!\n";
		return false;
	}

	fout << num_nodes_ << endl;

	for (int i = 0; i < num_nodes_; i++)
	{
		for (int j = 0; j < num_nodes_; j++)
		{
			fout << matrix_range_net_(i, j) << ' ';
			//		cout << matrix_range_net_(i, j) << ' ';
		}
		fout << endl;
		//	cout << '\n';
	}

	for (int i = 0; i < num_nodes_; i++)
	{
		fout << matrix_position_(i, 0) << ' ' << matrix_position_(i, 1) << endl;
		//	cout << matrix_postion_(i, 0) << ' ' << matrix_postion_(i, 1) << endl;
	}

	for (int i = 0; i < num_nodes_; i++)
	{
		fout << vector_type_(i) << endl;
		//	cout << vector_type_(i) << endl;
	}

	fout << num_max_lines_ << endl;
	cout << "num_max_lines: " << num_max_lines_<<endl;

	fout << distance_inline_ << endl;
	cout << "distance_inline: " << distance_inline_ << endl;

	fout << distance_adjacent_ << endl;
	cout << "distance_adjacent: " << distance_adjacent_ << endl;

	fout << distance_cross_ << endl;
	cout << "distance_cross: " << distance_cross_ << endl;

	fout.close();
	return true;
}


bool AutoRange::WriteRangeToFile(const char* _filename)
{
	ofstream fout(_filename);

	if (!fout)
	{
		cout << "Failed to open "<<_filename<<"!\n";
		return false;
	}

	fout<<"Launch: "<<endl;
	for (size_t i=0; i<nodes_acoustic_.size(); i++)
	{
		int nindex = nodes_acoustic_[i];
		fout<<nindex<<": "<<nodes_[nindex].launch_time_<<endl;
	}
	
	fout<<endl;
	fout<<"Apeture window: "<<endl;
	for (size_t i=0; i<nodes_.size(); i++)
	{
		fout<<"Node: "<<i<<endl;
		for (size_t j=0; j<nodes_[i].apeturetime_open_.size(); j++)
		{
			fout<<nodes_[i].apeture_index_[j]<<' '<<nodes_[i].apeturetime_open_[j]<<' '<<nodes_[i].apeturetime_close_[j]<<endl;
		}
		fout<<endl;
	}
	return true;
}


bool AutoRange::ReadConfiguration(const char* _filename)
{
	ifstream fin(_filename);

	if (!fin)
	{
		cout << "Failed open " << _filename << "!\n";
		return false;
	}

	fin >> num_max_lines_;
	cout << "max number of lines: " << num_max_lines_ << endl;

	fin >> distance_inline_;
	cout << "distance in line: " << distance_inline_ << endl;

	fin >> distance_adjacent_;
	cout << "distance adjacent: " << distance_adjacent_ << endl;

	fin >> distance_cross_;
	cout << "distance_cross: " << distance_cross_ << endl;
	return true;
}


// Operate
void AutoRange::CreateExamples(const char* _filename)
{
	for (int i=8; i>0; i--)
	{
		num_max_lines_ = i;
		Init();
		CreateNetwork();

		string file(_filename);
		
		ostringstream ost;
		ost << i;
		string fileout = file + "_result" + ost.str() + ".txt";

		WriteNetworkToFile(fileout.c_str());
	}
}


void AutoRange::CreateNetwork()
{
	CreateAcouNetwork();
	CreateVeliNetwork();
	CreateExtraLines();

	ExtractLines();
}


void AutoRange::CreateAcouNetwork()
{
	int prior_index = SearchPriorNode();
//	cout<<"prior_index: "<<prior_index<<endl;
	//	cout<<"degree: "<<nodes_[prior_index].candidate_nodes_index_.size()<<endl;

	while (prior_index>-1)
	{
		nodes_[prior_index].ReduceOneLine();
		prior_index = SearchPriorNode();
//		cout<<"prior_index: "<<prior_index<<endl;

		if (prior_index >=0)
		{
//			cout<<"degree: "<<nodes_[prior_index].candidate_nodes_index_.size()<<endl;
		}

	}
}


void AutoRange::CreateVeliNetwork()
{
	int range = nodes_velo_.size();
	vector<int> index_in_order(range);
	for (int i=0; i<range; i++)
	{
		index_in_order[i] = i;
	}

	for (int i=range-1; i>0; i--)
	{
		int p = rand() % (range);
		int tmp = index_in_order[p];
		index_in_order[p] = index_in_order[i];
		index_in_order[i] = tmp;
	}

	for (size_t i = 0; i < nodes_velo_.size(); i++)
	{
		int index_point = nodes_velo_[index_in_order[i]];

		nodes_[index_point].AddLines();	
	}
}


void AutoRange::CreateExtraLines()
{
	for (size_t i=0; i<nodes_acoustic_.size(); i++)
	{
		int index_point = nodes_acoustic_[i];
		if (nodes_[index_point].candidate_nodes_index_.size()<size_t(num_max_lines_))
		{
			list<int>::iterator liter = nodes_[index_point].candidate_nodes_backup_.begin();
			for (; liter != nodes_[index_point].candidate_nodes_backup_.end(); ++liter)
			{
				int index_add = *(liter);
				if (nodes_[index_add].candidate_nodes_index_.size()<size_t(num_max_lines_))
				{
					matrix_range_net_(index_point, index_add) = matrix_range_net_(index_add, index_point) = 1;
				}
			}
		}
	}
}


void AutoRange::CreateLayers()
{
	list<int> nodes_index;
	for (int i=0; i<num_nodes_; i++)
	{
		nodes_index.push_back(i);
	}

	while (nodes_index.size()>0)
	{
		list<int> onelayer;
		for (list<int>::iterator liter = nodes_index.begin();
			liter != nodes_index.end();)
		{
			if (!IsRelated(*liter, onelayer))
			{
				onelayer.push_back(*liter);
				liter = nodes_index.erase(liter);
			}
			else
			{
				liter++;
			}
		}

		layers_.push_back(onelayer);
	}
	
}


void AutoRange::CreateRange()
{
	CalculateDelay();
	CreateLayers();

	for (size_t i=0; i<layers_[0].size(); i++)
	{
		list<int>::iterator liter = layers_[0].begin();
		for (; liter != layers_[0].end(); ++liter)
		{
			nodes_[*liter].launch_time_ = 60;
		}
	}

	vector<list<int>> layers_backup = layers_;
	for (size_t i=1; i<layers_backup.size(); i++)
	{
		int index_last = layers_backup[i].front();
		while (layers_backup[i].size()>0)
		{
			list<int>::iterator liter = layers_backup[i].begin();
			list<int>::iterator liter_next = liter;
			int nindex_next = *(liter);
			++liter;
			for (;liter != layers_backup[i].end(); ++liter)
			{
				int nindex = *liter;
				if (matrix_distance_(index_last, nindex_next)>matrix_distance_(index_last, nindex))
				{
					nindex_next = nindex;
					liter_next = liter;
				}
			}
			cout<<"next: "<<nindex_next<<endl;
			layers_backup[i].erase(liter_next);

			for (size_t j=0; j<i; j++)
			{
				list<int>::iterator liter = layers_[j].begin();
				for (; liter != layers_[j].end(); ++liter)
				{
					int nindex2 = *liter;
					if (matrix_delay_(nindex_next, nindex2)+nodes_[nindex2].launch_time_>nodes_[nindex_next].launch_time_)
					{
						nodes_[nindex_next].launch_time_ = matrix_delay_(nindex_next, nindex2)+nodes_[nindex2].launch_time_ + 20;
					}
				}
			}
		}
	}

	CreateApetureTime();
}


void AutoRange::CreateApetureTime()
{
	for (size_t i=0; i<num_nodes_; i++)
	{
	//	cout<<i<<": "<<nodes_[i].range_index_.size()<<endl;
		for (size_t j=0; j<nodes_[i].range_index_.size(); j++)
		{
			
			int nindex = nodes_[i].range_index_[j];
			double time_point = nodes_[nindex].launch_time_ + matrix_timeflight_(i, nindex);
			double time_open = time_point - matrix_apeturesize_(i, nindex);
			double time_close = time_point + matrix_apeturesize_(i, nindex);

			nodes_[i].apeture_index_.push_back(nindex);
			nodes_[i].apeturetime_open_.push_back(time_open);
			nodes_[i].apeturetime_close_.push_back(time_close);
		}
	}
}


void AutoRange::Init()
{
	matrix_range_net_ = MatrixXi::Zero(num_nodes_, num_nodes_);
	matrix_distance_ = MatrixXd::Zero(num_nodes_, num_nodes_);
	matrix_topo_distance_ = MatrixXd::Zero(num_nodes_, num_nodes_);

	for (int i = 0; i < num_nodes_; i++)
	{
		for (int j = i; j < num_nodes_; j++)
		{
			Vector2d vtmp(matrix_position_(i, 0) - matrix_position_(j, 0),
				matrix_position_(i, 1) - matrix_position_(j, 1));
			matrix_distance_(i, j) = matrix_distance_(j, i) = vtmp.norm();

			double dis = matrix_distance_(i, j);

			Vector2d vtmp_topo(matrix_index_(i, 0) - matrix_index_(j, 0),
				matrix_index_(i, 1) - matrix_index_(j, 1));
			matrix_topo_distance_(i, j) = matrix_topo_distance_(j, i) = vtmp_topo.norm();
		}
	}

	position_center_x_ = 0.5*(matrix_position_(0, 0)+matrix_position_(num_nodes_-1, 0));
	position_center_y_ = 0.5*(matrix_position_(0, 1)+matrix_position_(num_nodes_-1, 1));

	nodes_.clear();
	nodes_.resize(num_nodes_);
	nodes_acoustic_.clear();
	nodes_velo_.clear();
	
	for (int i = 0; i < num_nodes_; i++)
	{
		nodes_[i].index_ = i;
		nodes_[i].position_x_ = matrix_position_(i, 0);

		nodes_[i].position_y_ = matrix_position_(i, 1);
		nodes_[i].index_line_ = matrix_index_(i, 0);
		nodes_[i].index_inline_ = matrix_index_(i, 1);

		int id_type = vector_type_(i);
		switch (id_type)
		{
		case 0:
			nodes_[i].type_ = kAcoustic;
			nodes_acoustic_.push_back(i);
			break;

		case 1:
			nodes_[i].type_ = kVelocimeter;
			nodes_velo_.push_back(i);
			break;

		default:
			nodes_[i].type_ = kNone;
			break;
		}

		nodes_[i].distances_.resize(num_nodes_);
		nodes_[i].distances_topo_.resize(num_nodes_);
		for (int j=0; j<num_nodes_; j++)
		{
			nodes_[i].distances_[j] = matrix_distance_(i, j);
			nodes_[i].distances_topo_[j] = matrix_topo_distance_(i, j);
		}
	}

	matrix_delay_.resize(num_nodes_, num_nodes_);

	for (int i=0; i<num_nodes_; i++)
	{
		for (int j=0; j<num_nodes_; j++)
		{
			matrix_delay_(i, j) = -9999;
		}
	}

	for (int i=0; i<num_nodes_; i++)
	{
		nodes_[i].InitCandidate(this);
	}
}


int AutoRange::CalculateLineDegree(int lineindex)
{
	int degree = 0;
	for (int i=0; i<num_nodes_; i++)
	{
		if (nodes_[i].index_line_ == lineindex)
		{
			degree += nodes_[i].candidate_nodes_index_.size();
		}
	}

	return degree;
}


int AutoRange::CalculateLineDegree(int lineindex1, int lineindex2)
{
	int degree = 0;
	for (int i=0; i<num_nodes_; i++)
	{
		if (nodes_[i].index_line_ == lineindex1)
		{
			list<int>::iterator liter = nodes_[i].candidate_nodes_index_.begin();
			for (;liter != nodes_[i].candidate_nodes_index_.end(); ++liter)
			{
				int nodesindex2 = *(liter);
				if (nodes_[nodesindex2].index_line_ == lineindex2)
				{
					degree ++;
				}
			}
		}
	}

	return degree;
}


int AutoRange::CalculateInLineDegree(int inlineindex1, int inlineindex2)
{
	int degree = 0;
	for (int i=0; i<num_nodes_; i++)
	{
		if (nodes_[i].index_inline_ == inlineindex1)
		{
			list<int>::iterator liter = nodes_[i].candidate_nodes_index_.begin();
			for (;liter != nodes_[i].candidate_nodes_index_.end(); ++liter)
			{
				int nodesindex2 = *(liter);
				if (nodes_[nodesindex2].index_inline_ == inlineindex2)
				{
					degree ++;
				}
			}
		}
	}

	return degree;
}


double AutoRange::CalculateApeturesize(int _index1, int _index2)
{
	if (nodes_[_index1].index_line_ == nodes_[_index2].index_line_)
	{
		return 5;
	}
	else
	{
		double dis = matrix_distance_(_index1, _index2);

		if (dis<=0)
		{
			return -1;
		}
		else if (dis<999)
		{
			return 10;
		}
		else if (dis<2999)
		{
			return 20;
		}
		else
		{
			return 30;
		}
	}
}


void AutoRange::CalculateDelay()
{	
	for (int i=0; i<num_nodes_; i++)
	{
		nodes_[i].CalculateDelay();
	}
}


bool AutoRange::IsRelated(int _index, list<int>& _layer)
{
	list<int>::iterator liter = _layer.begin();
	for (; liter != _layer.end(); ++liter)
	{
		if (matrix_delay_(_index, *liter)>-9998 && matrix_delay_(*liter, _index)>-9998)
		{
			return true;
		}
	}

	return false;
}


void AutoRange::ExtractLines()
{
	for (int i = 0; i < num_nodes_; i++)
	{
		for (list<int>::iterator liter = nodes_[i].candidate_nodes_index_.begin();
			liter != nodes_[i].candidate_nodes_index_.end(); liter++)
		{
			matrix_range_net_(i, *(liter)) = 1;
		}
	}
}


int AutoRange::SearchPriorNode()
{
	int index = nodes_acoustic_.front();
//	cout<<"frontindex: "<<index<<endl;

	for (size_t i = 1; i < nodes_acoustic_.size(); i++)
	{
		int index_candi = nodes_acoustic_[i];
		if (nodes_[index_candi]>nodes_[index])
		{
			index = index_candi;
		}
	}

	if (nodes_[index].candidate_nodes_index_.size()<=size_t(num_max_lines_))
	{
//		cout<<"candidate size: "<<nodes_[index].candidate_nodes_index_.size()<<endl;
		return -1;
	}
	else
	{
		return index;
	}
}


void AutoRange::InitDistance()
{
	matrix_distance_ = MatrixXd::Zero(num_nodes_, num_nodes_);
	matrix_timeflight_ = MatrixXd::Zero(num_nodes_, num_nodes_);

	for (int i = 0; i < num_nodes_; i++)
	{
		for (int j = i; j < num_nodes_; j++)
		{
			Vector2d vtmp(matrix_position_(i, 0) - matrix_position_(j, 0),
				matrix_position_(i, 1) - matrix_position_(j, 1));
			matrix_distance_(i, j) = matrix_distance_(j, i) = vtmp.norm();

			double dis = matrix_distance_(i, j);
		}
	}

	nodes_.clear();
	nodes_.resize(num_nodes_);
	nodes_acoustic_.clear();
	nodes_velo_.clear();

	for (int i = 0; i < num_nodes_; i++)
	{
		nodes_[i].index_ = i;
		nodes_[i].position_x_ = matrix_position_(i, 0);

		nodes_[i].position_y_ = matrix_position_(i, 1);
		nodes_[i].index_line_ = matrix_index_(i, 0);
		nodes_[i].index_inline_ = matrix_index_(i, 1);

		int id_type = vector_type_(i);
		switch (id_type)
		{
		case 0:
			nodes_[i].type_ = kAcoustic;
			nodes_acoustic_.push_back(i);
			break;

		case 1:
			nodes_[i].type_ = kVelocimeter;
			nodes_velo_.push_back(i);
			break;

		default:
			nodes_[i].type_ = kNone;
			break;
		}

		nodes_[i].distances_.resize(num_nodes_);
		for (int j=0; j<num_nodes_; j++)
		{
			nodes_[i].distances_[j] = matrix_distance_(i, j);
		}
	}

	matrix_delay_.resize(num_nodes_, num_nodes_);

	for (int i=0; i<num_nodes_; i++)
	{
		for (int j=0; j<num_nodes_; j++)
		{
			matrix_delay_(i, j) = -9999;
		}
	}

	for (int i=0; i<num_nodes_; i++)
	{
		nodes_[i].InitCandidate(this);
	}

	matrix_apeturesize_.resize(num_nodes_, num_nodes_);
	for (int i=0; i<num_nodes_; i++)
	{
		for (int j=0; j<num_nodes_; j++)
		{
			double apeturesize = CalculateApeturesize(i, j);
			matrix_apeturesize_(i, j) = apeturesize;
			if (matrix_range_net_(i, j) == 1)
			{
				nodes_[i].range_index_.push_back(j);	
			}
		}
		cout<<"range: "<<i<<' '<<nodes_[i].range_index_.size()<<endl;
	}
}

// Check functions
void AutoRange::CheckConflict()
{
	for (int i=0; i<num_nodes_; i++)
	{
		for (size_t j=0; j<nodes_[i].apeturetime_open_.size(); j++)
		{
			list<int>::iterator liter = nodes_[i].candidate_nodes_index_.begin();
			for (;liter != nodes_[i].candidate_nodes_index_.end(); ++liter)
			{
				int nindex = *liter;

				if (matrix_range_net_(i, nindex) == 1)
				{
					continue;
				}

				double timepoint = nodes_[nindex].launch_time_ + matrix_timeflight_(nindex, i);
				double timeopen2 = timepoint - nodes_[i].apeture_size_;
				double timeclose2 = timepoint + nodes_[i].apeture_size_;

				bool isok = CheckConflict(nodes_[i].apeturetime_open_[j], nodes_[i].apeturetime_close_[j], timeopen2, timeclose2);

				if (isok)
				{
					//			cout<<i<<' '<<nindex<<"true"<<endl;
				}
				else
				{
					cout<<i<<' '<<nindex<<"false"<<endl;
					cout<<"apeture: "<<nodes_[i].apeturetime_open_[j]<<' '<<nodes_[i].apeturetime_close_[j]<<endl;
					cout<<"conflict: "<<timeopen2<<' '<<timeclose2<<endl;
				}
			}
		}
	}
}


// False means has conflict, true means everything ok
bool AutoRange::CheckConflict(double _timeopen1, double _timeclose1, double _timeopen2, double _timeclose2)
{
	if (_timeclose1<_timeopen2 || _timeclose2<_timeopen1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Test functions
void AutoRange::TestDelay()
{
	for (int i=0; i<num_nodes_; i++)
	{
		for (int j = 0; j < num_nodes_; j++)
		{
			if (matrix_delay_(i, j)>-9998)
			{
				cout<<i<<' '<<j<<": "<<matrix_delay_(i, j)<<endl;
			}
			
		}
	}
}


void AutoRange::TestLayers()
{
	cout<<"layerssize: "<<layers_.size()<<endl;

	for (size_t i=0; i<layers_.size(); i++)
	{
		cout<<"layer "<<i<<": "<<endl;

		list<int>::iterator liter = layers_[i].begin();
		for (; liter != layers_[i].end(); ++liter)
		{
			cout<<*liter<<endl;
		}
	}
}


void AutoRange::TestRange()
{
	for (size_t i=0; i<nodes_acoustic_.size(); i++)
	{
		int index = nodes_acoustic_[i];
		cout<<"range: "<<index<<' '<<nodes_[index].launch_time_<<endl;
	}
}


void AutoRange::TestApeture()
{
	for (int i=0; i<num_nodes_; i++)
	{
		cout<<i<<endl;
		
		for (size_t j=0; j<nodes_[i].apeturetime_open_.size(); j++)
		{
			cout<<"open: "<<nodes_[i].apeturetime_open_[j]<<endl;
			cout<<"close: "<<nodes_[i].apeturetime_close_[j]<<endl;
		}
	}
}
