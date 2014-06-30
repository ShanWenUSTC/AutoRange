// AutoRange.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "AutoRange.h"
#include <iostream>
#include <string>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	AutoRange n;

// 	string filename_in;
// 	string filename_config;
// 	string filename;
// 	cout << "Please input filename of nodes: \n";
// 	cin >> filename;
// 	filename_config = filename + "_config.ini";
// 	n.ReadConfiguration(filename_config.c_str());
// 
// 	filename_in = filename + ".txt";
// 	n.ReadFromFile(filename_in.c_str());
// 
// 	n.CreateExamples(filename.c_str());
// //	n.CreateNetwork();
// 
// // 	filename_in = filename + "_result.txt";
//  //	n.WriteToFile(filename_in.c_str());
// 
// 	n.CalculateDelay();
// //	n.TestDelay();
// //	n.TestLayers();
// 	n.TestRange();
// 	string filename_range = filename + "_range.txt";
// 	n.WriteRangeToFile(filename_range.c_str());

	string filename_range;
	string filename_net;
	string filename_config;
	string filename;
	

	cout << "Please input filename of network: \n";
	cin >> filename;
	filename_config = filename + "_config.ini";
	n.ReadConfiguration(filename_config.c_str());

	filename_net = filename + "_net.txt";
	n.ReadNetworkFromFile(filename_net.c_str());
	
	

//	n.TestDelay();
//	n.CalculateDelay();
	n.CreateRange();
	//n.TestLayers();
	n.TestRange();
 	n.TestApeture();
	n.CheckConflict();
	filename_range = filename + "_range.txt";
	n.WriteRangeToFile(filename_range.c_str());

	return 0;
}

