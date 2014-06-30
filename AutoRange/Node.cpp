#include "stdafx.h"
#include "Node.h"
#include <iostream>
#include <stdlib.h>
#include "AutoRange.h"

using std::cout;
using std::endl;

Node::Node(void)
{
	launch_time_ = 0;
}


Node::~Node(void)
{
}


// Operate
bool Node::Compare(int _index1, int _index2)
{
	int linetype1 = 0;
	int linetype2 = 0;

	if (index_line_ != ptr_autorange_->nodes_[_index1].index_line_)
	{
		linetype1 += 2;
	}

	if (index_line_ != ptr_autorange_->nodes_[_index2].index_line_)
	{
		linetype2 += 2;
	}

	if (index_inline_ != ptr_autorange_->nodes_[_index1].index_inline_)
	{
		linetype1 += 1;
	}

	if (index_inline_ != ptr_autorange_->nodes_[_index2].index_inline_)
	{
		linetype2 += 1;
	}

	if (linetype1 != linetype2)
	{
		return linetype1<linetype2;	
	}
	else
	{
		double dis1 = distances_topo_[_index1];
		double dis2 = distances_topo_[_index2];	

		if (fabs(dis1-dis2)>0.01)
		{
			return dis1>dis2;
		}
		else
		{
			int degree1 = ptr_autorange_->nodes_[_index1].candidate_nodes_index_.size();
			int degree2 = ptr_autorange_->nodes_[_index2].candidate_nodes_index_.size();

			if (degree1!=degree2)
			{
				return degree1>degree2;
			}
			else
			{
				int lineid1 = ptr_autorange_->nodes_[_index1].index_line_;
				int lineid2 = ptr_autorange_->nodes_[_index2].index_line_;

				int linedegree1 = ptr_autorange_->CalculateLineDegree(index_line_, lineid1);
				int linedegree2 = ptr_autorange_->CalculateLineDegree(index_line_, lineid2);

				int inlineid1 = ptr_autorange_->nodes_[_index1].index_inline_;
				int inlineid2 = ptr_autorange_->nodes_[_index2].index_inline_;

				int inlinedegree1 = ptr_autorange_->CalculateInLineDegree(index_inline_, inlineid1);
				int inlinedegree2 = ptr_autorange_->CalculateInLineDegree(index_inline_, inlineid2);

				int finaldegree1 = linedegree1 + inlinedegree1;
				int finaldegree2 = linedegree2 + inlinedegree2;

				if (finaldegree1 != finaldegree2)
				{
					return finaldegree1>finaldegree2;
				}
				else
				{
					int N = rand() % 2;
		//			cout<<"N: "<<N<<endl;

					return bool(N);	
				}
			}
		}	
	}
}


void Node::ReduceOneLine()
{
	if (candidate_nodes_index_.size()==0)
	{
		return;
	}

	list<int>::iterator liter = candidate_nodes_index_.begin();
	if (candidate_nodes_index_.size()==1)
	{
		candidate_nodes_index_.erase(liter);
		return;
	}

	int reduceline_index = *(liter);
	list<int>::iterator liter_backup = liter;
	++liter;
	for (; liter != candidate_nodes_index_.end(); ++liter)
	{
		if (!Compare(reduceline_index, *(liter)))
		{
			reduceline_index = *(liter);
			liter_backup = liter;
		}
	}

//	cout<<"reduced: "<<reduceline_index<<endl;
	ptr_autorange_->nodes_[reduceline_index].ReduceNeighbor(index_);
	candidate_nodes_index_.erase(liter_backup);
}


void Node::AddLines()
{
	if (candidate_nodes_index_.size()<=size_t(ptr_autorange_->num_max_lines_))
	{
		return;
	}

	while (candidate_nodes_index_.size()>size_t(ptr_autorange_->num_max_lines_))
	{
		list<int>::iterator liter = candidate_nodes_index_.begin();
		list<int>::iterator liter_backup = liter;
		int index_backup = *(liter_backup);
		++liter;

		for (; liter != candidate_nodes_index_.end(); ++liter)
		{
			int index = *(liter);
	
			if (!Compare(index_backup, index))
			{
				index_backup = index;
				liter_backup = liter;
			}
		}

		candidate_nodes_index_.erase(liter_backup);
	}
}


void Node::InitCandidate(AutoRange* _ptr_autorange)
{
	ptr_autorange_ = _ptr_autorange;

	for (size_t i=0; i<distances_.size(); i++)
	{
		if (ptr_autorange_->nodes_[i].type_ == kVelocimeter)
		{
			continue;
		}

		double dis = distances_[i];

		if ((dis>0.001)&&(dis<600))
		{
			if (index_line_ == _ptr_autorange->nodes_[i].index_line_)
			{
				if (dis<_ptr_autorange->distance_inline_)
				{
					candidate_nodes_index_.push_back(i);
				}
			}
			else if (index_inline_ == _ptr_autorange->nodes_[i].index_inline_)
			{
				if (dis<_ptr_autorange->distance_adjacent_)
				{
					candidate_nodes_index_.push_back(i);
				}
			}
			else if (dis<_ptr_autorange->distance_cross_)
			{
				candidate_nodes_index_.push_back(i);
			}

		}
	}

	candidate_nodes_backup_ = candidate_nodes_index_;
}


bool Node::operator>(const Node& _node_right)
{
	int degree = candidate_nodes_index_.size();
	int degree_right = _node_right.candidate_nodes_index_.size();

	if (degree == degree_right)
	{
		Vector2d v1(position_x_-ptr_autorange_->position_center_x_,
			position_y_-ptr_autorange_->position_center_y_);
		double dis1 = v1.norm();

		Vector2d v2(_node_right.position_x_-ptr_autorange_->position_center_x_,
			_node_right.position_y_-ptr_autorange_->position_center_y_);
		double dis2 = v2.norm();

		return dis1<dis2;
	}
	else
	{
		return degree>degree_right;
	}
}


void Node::ReduceNeighbor(int neiindex)
{
	for (list<int>::iterator liter = candidate_nodes_index_.begin();
		liter != candidate_nodes_index_.end();)
	{
		if (*liter == neiindex)
		{
			liter = candidate_nodes_index_.erase(liter);
		}
		else
		{
			liter++;
		}
	}
}


void Node::CalculateDelay()
{
	for (list<int>::iterator liter_first = candidate_nodes_backup_.begin();
		liter_first != candidate_nodes_backup_.end(); ++liter_first)
	{
		int index1 = *liter_first;
		list<int>::iterator liter_second = liter_first;
		++liter_second;

		for (;liter_second != candidate_nodes_backup_.end(); ++liter_second)
		{
			int index2 = *liter_second;
			double time1 = distances_[index1]/1.5;
			double time2 = distances_[index2]/1.5;

			ptr_autorange_->matrix_timeflight_(index_, index1) = time1;
			ptr_autorange_->matrix_timeflight_(index1, index_) = time1;
			ptr_autorange_->matrix_timeflight_(index_, index2) = time2;
			ptr_autorange_->matrix_timeflight_(index2, index_) = time2;

			double a1 = time1 - ptr_autorange_->matrix_apeturesize_(index_, index1);
			double b1 = time1 + ptr_autorange_->matrix_apeturesize_(index_, index1);

			double a2 = time2 - ptr_autorange_->matrix_apeturesize_(index_, index2);
			double b2 = time2 + ptr_autorange_->matrix_apeturesize_(index_, index2);

			if (ptr_autorange_->matrix_delay_(index1, index2)<b2-a1)
			{
				ptr_autorange_->matrix_delay_(index1, index2) = b2-a1;
			}

			if (ptr_autorange_->matrix_delay_(index2, index1)<b1-a2)
			{
				ptr_autorange_->matrix_delay_(index2, index1) = b1-a2;
			}
		}
	}
}
